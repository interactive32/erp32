<?php


class price_list extends ia32Node
{

	function price_list()
	{
		$this->atkNode("price_list", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE|AF_MULTILANGUAGE, $this->std_att_size));

		$this->setOrder("id");
		$this->setTable("price_list");
	}


	function descriptor_def()
	{
		return "[name]";
	}


	/*
	 * check for max pricelist limit
	 */
	function preAdd($record)
	{
		$sql = 'SELECT count(*) AS count_pricelists FROM ia32_price_list';
		$rec = $this->db32select($sql);

		if ($rec[0]['count_pricelists'] >= atkconfig('pricelists_limit'))
		{
			$this->display_error(atktext('max_pricelist_count_reached').atkconfig('pricelists_limit'));
			die;
		}
	}


	/*
	 * don't delete main initial record
	 */
	function preDelete($record)
	{
		if (is_numeric($record['id']) && $record['id']!=1000)
		{
			return false;
		}

		// return error on used
		$feedback = $this->checkPricelistIsUsed($record['id']);
		if($feedback)
		{
			$this->display_error($feedback);
			die;
		}

		return true;
	}


	/*
	 * remove all prices
	 */
	function postDelete($record)
	{
		$sql = "DELETE from ia32_prices WHERE pricelist_id = '".stripquotes($record['id'])."'";
		$record_count = $this->db32delete($sql);

		return true;
	}


	/*
	 * recordview actions on admin mode grid
	 */
	function recordActions(&$record, &$actions, &$mraactions)
	{
		// remove all actions for main initial record
		if (is_numeric($record['id']) && $record['id']<=1000)
		{
			$actions["view"] = '';	 // because of doctype_main
			$actions["edit"] = '';	 // because of doctype_main
			$actions["delete"] = '';
		}
	}


	/**
	 * check finance type is used
	 *
	 */
	function checkPricelistIsUsed($id)
	{
		$sql =  "SELECT * FROM ia32_locations WHERE pricelist_id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		if ($recs[0]) return atktext("pricelist_used_on_location").$recs[0]['name'];

		return false;

	}

}

?>
