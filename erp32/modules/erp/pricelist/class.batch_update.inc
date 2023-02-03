<?php


class batch_update extends ia32Node
{

	function batch_update()
	{
		$this->atkNode("batch_update", NF_READONLY);

		$this->add(new atkManyToOneRelation("pricelist", "pricelist.price_list", AF_SEARCHABLE|AF_PRIMARY|AF_READONLY_EDIT));
		$this->getAttribute("pricelist")->setloadType(NOLOAD);
		$this->getAttribute("pricelist")->setstorageType(NOSTORE);

		$this->add(new atkShuttleRelation("groups", "products.category", "products.category", AF_SEARCHABLE));
		$this->getAttribute("groups")->setloadType(NOLOAD);
		$this->getAttribute("groups")->setstorageType(NOSTORE);

		$this->add(new atkNumberAttribute("pricesell_update_percent", AF_SEARCHABLE, 3, 2));
		$this->getAttribute("pricesell_update_percent")->setloadType(NOLOAD);
		$this->getAttribute("pricesell_update_percent")->setstorageType(NOSTORE);
		$this->getAttribute("pricesell_update_percent")->addDependency(array($this, "refresh_numbers"));

		$this->add(new atkNumberAttribute("pricebuy_update_percent", AF_SEARCHABLE, 3, 2));
		$this->getAttribute("pricebuy_update_percent")->setloadType(NOLOAD);
		$this->getAttribute("pricebuy_update_percent")->setstorageType(NOSTORE);
		$this->getAttribute("pricebuy_update_percent")->addDependency(array($this, "refresh_numbers"));

		$this->addFieldSet('selection_field',$this->drawSelection(),AF_HIDE_LIST|AF_NOLABEL);

	}


	function actionTitle($action, $record="")
	{
		return atktext('batch_update_menu');
	}


	function initial_values()
	{
		return array("pricesell_update_percent"=>"0",
					"pricebuy_update_percent"=>"0",
					"pricelist"=>array('id'=>'1000'));
	}


	/*
	 * refresh numbers to look nicer
	 */
	function refresh_numbers($modifier)
	{
		$record = &$modifier->getRecord();

		if (!$record['pricebuy_update_percent']) $record['pricebuy_update_percent'] = 0;
		if (!$record['pricesell_update_percent']) $record['pricesell_update_percent'] = 0;

		$modifier->refreshAttribute('selection_field');
	}


	function drawSelection()
	{
		$ret = '<center>'
		.atktext("update_pricelist_info").'<br><br>'
		.atktext("select_pricelist").':<br><br>
		[pricelist.field]<br><br>
		'.atktext("select_product_groups").':<br><br>
		[groups.field]<br><br>

		<table border="0">
		<tr>
			<td align="right">
			[pricesell_update_percent.label] [pricesell_update_percent.field] %
			</td>
		</tr>
		<tr>
			<td align="right">
			[pricebuy_update_percent.label] [pricebuy_update_percent.field]	%
			</td>
		</tr>
		</table>


				
		</center>';

		return $ret;
	}


	function action_updatePricelist()
	{
		global $g_sessionManager;

		$filter = $g_sessionManager->stackVar('pricelist');
		$pair = explode("=",$filter);
		$pricelist = stripQuotes($pair[1]);

		// drop if pricelist is null
		if (!$pricelist)
		{
			$this->display_notification(atktext('unknown_error').': '.$filter);
			die;
		}

		$filter = $g_sessionManager->stackVar('groups');

		// drop if no category is selected
		if (!$filter)
		{
			$this->display_notification(atktext('feedback_pricelist_no_category_selected'));
			die;
		}

		// transform category selection to db friendly filter
		$category_filter_array = array();
		foreach ($filter as $selection)
		{
			$category_filter_array[] = "'".stripQuotes($selection['category'])."'";
		}
		$category_filter = implode(", ",$category_filter_array);

		// get pricesell update percent
		$filter = $g_sessionManager->stackVar('pricesell_update_percent');
		$pricesell_update_percent = $this->removeSeparators($filter);
		// division by zerro catch
		if ($pricesell_update_percent && $pricesell_update_percent!=='0') $pricesell_update_percent = $pricesell_update_percent / 100; else $pricesell_update_percent = '0';

		// get pricebuy update percent
		$filter = $g_sessionManager->stackVar('pricebuy_update_percent');
		$pricebuy_update_percent = $this->removeSeparators($filter);
		// division by zerro catch
		if ($pricebuy_update_percent && $pricebuy_update_percent!=='0') $pricebuy_update_percent = $pricebuy_update_percent / 100; else $pricebuy_update_percent = '0';


		// update prices from source pricelist
		$sql = "
			UPDATE ia32_prices 
			JOIN ia32_products ON ia32_products.id = ia32_prices.product_id
			SET pricebuy = pricebuy * (1 + $pricebuy_update_percent), pricesell = pricesell * (1 + $pricesell_update_percent)
			WHERE pricelist_id = '$pricelist' 
			AND ia32_products.category_id IN ($category_filter)			
		";

		$this->db32update($sql);

		$this->display_notification('<br>'.atktext('update_pricelist_success').'<br><br>');

	}


	function getFormButtons($mode, $record)
	{
		$destination_action = "updatePricelist";

		return '
		<form id="entryform_copy" name="entryform_copy" action="index.php" method="post">
		<input type="hidden" name="atkaction" value="'.$destination_action.'">
		<input type="hidden" name="atknodetype" value="'.$this->m_postvars["atknodetype"].'">
		<input type="submit" class="btn_save" value="'.atktext('start_update_prices').'">
		</form>
		';
	}
}

?>
