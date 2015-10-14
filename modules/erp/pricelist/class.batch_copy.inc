<?php


class batch_copy extends ia32Node
{

	function batch_copy()
	{
		$this->atkNode("batch_copy", NF_READONLY);

		$this->add(new atkManyToOneRelation("pricelist_from", "pricelist.price_list", AF_SEARCHABLE|AF_PRIMARY|AF_READONLY_EDIT));
		$this->getAttribute("pricelist_from")->setloadType(NOLOAD);
		$this->getAttribute("pricelist_from")->setstorageType(NOSTORE);

		$this->add(new atkManyToOneRelation("pricelist_to", "pricelist.price_list", AF_SEARCHABLE|AF_PRIMARY|AF_READONLY_EDIT));
		$this->getAttribute("pricelist_to")->setloadType(NOLOAD);
		$this->getAttribute("pricelist_to")->setstorageType(NOSTORE);

		$this->add(new atkShuttleRelation("groups", "products.category", "products.category", AF_SEARCHABLE));
		$this->getAttribute("groups")->setloadType(NOLOAD);
		$this->getAttribute("groups")->setstorageType(NOSTORE);

		$this->addFieldSet('selection_field',$this->drawSelection(),AF_HIDE_LIST|AF_NOLABEL);

	}


	function initial_values()
	{
		return array("pricelist_from"=>array('id'=>'1000'));
	}
	
	
	function actionTitle($action, $record="")
	{
		return atktext('batch_copy_menu');
	}


	function drawSelection()
	{
		$ret = '<center>'
		.atktext("copy_pricelist_info").'<br><br>'	
		.atktext("select_pricelists").':<br><br>
		[pricelist_from.field] >> [pricelist_to.field] <br><br><br>
		'.atktext("select_product_groups").':<br><br>
		[groups.field]
		</center>';

		return $ret;
	}


	function action_copyPricelist()
	{
		global $g_sessionManager;

		$filter = $g_sessionManager->stackVar('pricelist_from');
		$pair = explode("=",$filter);
		$pricelist_from = stripQuotes($pair[1]);

		$filter = $g_sessionManager->stackVar('pricelist_to');
		$pair = explode("=",$filter);
		$pricelist_to = stripQuotes($pair[1]);

		$filter = $g_sessionManager->stackVar('groups');

		// drop if no category is selected
		if (!$filter)
		{
			$this->display_error(atktext('feedback_pricelist_no_category_selected'));
			die;
		}

		// transform category selection to db friendly filter
		$category_filter_array = array();
		foreach ($filter as $selection)
		{
			$category_filter_array[] = "'".stripQuotes($selection['category'])."'";
		}
		$category_filter = implode(", ",$category_filter_array);


		if ($pricelist_from == $pricelist_to)
		{
			$this->display_error(atktext('feedback_pricelist_copy_same_records'));
			die;
		}

		// insert any missing product's
		$sql = "
		INSERT INTO ia32_prices 
			SELECT '$pricelist_to' AS pricelist_id, product_id, pricebuy, pricesell 
				FROM ia32_prices 
				JOIN ia32_products ON ia32_products.id = ia32_prices.product_id
				WHERE pricelist_id = '$pricelist_from' 
				AND product_id NOT IN (SELECT product_id FROM ia32_prices WHERE pricelist_id = '$pricelist_to' ) 
				AND ia32_products.category_id IN ($category_filter)
		";

		$this->db32insert($sql);

		// update prices from source pricelist
		$sql = "
			UPDATE ia32_prices dest 
			JOIN ia32_prices src ON dest.product_id = src.product_id AND src.pricelist_id = '$pricelist_from'
			JOIN ia32_products ON ia32_products.id = src.product_id
			SET dest.pricebuy = src.pricebuy, dest.pricesell = src.pricesell 
			WHERE dest.pricelist_id = '$pricelist_to' 
			AND ia32_products.category_id IN ($category_filter)			
		";

		$this->db32update($sql);

		$this->display_notification(atktext('copy_pricelist_success').'<br><br>'.atktext($this->getPricelistName($pricelist_from)).' >> '.atktext($this->getPricelistName($pricelist_to)));
		die; // must die - double display bug
	}


	function getFormButtons($mode, $record)
	{
		$destination_action = "copyPricelist";

		return '
		<form id="entryform_copy" name="entryform_copy" action="index.php" method="post">
		<input type="hidden" name="atkaction" value="'.$destination_action.'">
		<input type="hidden" name="atknodetype" value="'.$this->m_postvars["atknodetype"].'">
		<input type="submit" class="btn_save" value="'.atktext('start_copy_prices').'">
		</form>
		';
	}
}

?>
