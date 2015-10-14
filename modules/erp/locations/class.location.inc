<?php


class location extends ia32Node
{

	function location()
	{
		$this->atkNode("location", NF_ADD_LINK|NF_NO_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));

		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE, array(255,60,60)));
			
		$this->add(new atkAttribute("address", AF_OBLIGATORY|AF_SEARCHABLE, array(255,60,60)));
		$this->getAttribute("address")->setLabel(atktext('location_address'));

		$this->add(new atkManyToOneRelation("pricelist_id", "pricelist.price_list", AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkAttribute("prefix", AF_HIDE_LIST|AF_SEARCHABLE, array(5,5,5)));
		$this->getAttribute("prefix")->setLabel(atktext('document_prefix'));

		$this->add(new atkTextAttribute("description", array("cols"=>60,"rows"=>4), AF_HIDE_LIST));
		$this->getAttribute("description")->setLabel(atktext('location_description'));
		
		$this->setOrder("id");
		$this->setTable("locations");
	}


	function descriptor_def()
	{
		return "[name]";
	}


	function actionTitle($action, $record="")
	{
		return atktext(atktext('location_config'));
	}


	function recordActions(&$record, &$actions, &$mraactions)
	{
		// remove delete actions for main record
		if (is_numeric($record['id']) && $record['id']<=1000)
		{
			$actions["delete"] = '';
		}
	}

	/*
	 * change location handler from tools.php
	 * 
	 * (login to another location)
	 */
	function action_change(&$handler)
	{

		$location_filter = explode("=",$handler->m_postvars['atkselect']);
		$location_id = stripQuotes($location_filter[1]);

		$location = $this->getLocationData($location_id);
		$location_name = $location['name'];


		$coredb = $this->getCoreDB();
		$coredb_table = $coredb.'_users';

		$user_id = atkGetUser('id');

		$sql = "UPDATE $coredb_table
		SET location_id = '$location_id', location_name = '$location_name'
		WHERE id = '$user_id'";

		$db = &atkGetDb();
		$db->query($sql);
		$db->commit();

		$this->actionRequireLogout();

		return;
	}


	function postDelete($record)
	{
		$this->removeLocationFromStock($record['id']);

		$sql = "SELECT count(*) AS count FROM ia32_locations";

		$record_count = $this->db32select($sql);

		// if this was last location - turn off location changer by removing location_id/name
		if ($record_count[0]['count'] <= 1)
		{
			$coredb = $this->getCoreDB();
			$coredb_table = $coredb.'_users';

			$firm_id = atkGetUser('firm_id');

			$sql = "UPDATE $coredb_table
			SET location_id = null, location_name = null
			WHERE firm_id = '$firm_id'";

			$db = &atkGetDb();
			$db->query($sql);
			$db->commit();
		}

		return true;
	}


	/*
	 * check for max locations limit
	 */
	function preAdd($record)
	{
		$sql = 'SELECT count(*) AS count_locations FROM ia32_locations';
		$rec = $this->db32select($sql);

		if ($rec[0]['count_locations'] >= atkconfig('locations_limit'))
		{
			$this->display_error(atktext('max_locations_count_reached').atkconfig('locations_limit'));
			die;
		}
	}
	
	
	function postAdd($record, $mode)
	{
		$sql = "SELECT count(*) AS count FROM ia32_locations";

		$this->fillStockWithZerros($record['id']);

		$record_count = $this->db32select($sql);

		// if this is second location, turn on location changer by filling user's location_id/name
		if ($record_count[0]['count'] == 2)
		{
			// find default location name
			$sql = "SELECT * FROM ia32_locations WHERE id = '1000'";
			$locations = $this->db32select($sql);
			$default_location_name = $locations[0]['name'];

			$coredb = $this->getCoreDB();
			$coredb_table = $coredb.'_users';

			$firm_id = atkGetUser('firm_id');

			// users can change location now, so, set current location to default location
			$sql = "UPDATE $coredb_table
			SET location_id = 1000, location_name = '$default_location_name'
			WHERE firm_id = '$firm_id'";

			$db = &atkGetDb();
			$db->query($sql);
			$db->commit();

			$this->display_notification(atktext("data_change_require_relogin"));

			return true;
		}

	}


	/*
	 * fill stockcurrent with zerros
	 */
	function fillStockWithZerros($id)
	{
		$sql =  "
		INSERT INTO ia32_stockcurrent 
		SELECT product_id, '".stripQuotes($id)."', 0, 0, 0, null, null FROM ia32_stockcurrent WHERE location_id = '1000'
		";

		$this->db32insert($sql);
	}


	/*
	 * remove data from stockcurrent
	 */
	function removeLocationFromStock($id)
	{
		$sql =  "DELETE FROM ia32_stockcurrent
		WHERE location_id = '".stripQuotes($id)."'";

		$this->db32delete($sql);
	}


	/*
	 * check for delete errors
	 */
	function preDelete(&$record)
	{
		// return false if customer is used
		$feedback = $this->checkLocationIsUsed($record['id']);
		if($feedback)
		{
			$this->display_error($feedback);
			die;
		}

		return true;
	}


	/**
	 * check if location is used documents
	 *
	 */
	function checkLocationIsUsed($id)
	{
		// check if users use this location now
		$coredb = $this->getCoreDB();
		$coredb_table = $coredb.'_users';

		$firm_id = atkGetUser('firm_id');

		$sql = "SELECT user
				FROM $coredb_table
				WHERE firm_id = '$firm_id' AND location_id = '$id'";

		$db = &atkGetDb();
		$recs = $db->getrows($sql);
		if ($recs[0]['user']) return atktext("feedback_location_in_use_by_user").$recs[0]['user'];

		// check for stockcurrent usage
		$sql =  "SELECT * FROM ia32_stockcurrent
		WHERE (amount <> 0 OR reservation <> 0 OR on_order<> 0)
		AND location_id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);
		if ($recs[0]) return atktext("feedback_delete_location_has_stock");

		// check for ticket usage
		$sql =  "SELECT * FROM ia32_tickets WHERE location_id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);
		if ($recs[0]) return atktext("feedback_delete_constrain_error");

			
		return false;
	}
}

?>
