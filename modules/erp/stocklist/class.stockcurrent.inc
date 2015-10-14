<?php


class stockcurrent extends ia32Node
{

	function stockcurrent()
	{
		$this->atkNode("stockcurrent", NF_NO_ADD|NF_NO_DELETE|NF_NO_VIEW|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkManyToOneRelation("location_id", "locations.location", AF_READONLY|AF_SEARCHABLE|AF_PRIMARY));

		$this->add(new atkManyToOneRelation("product_id", "products.product", AF_READONLY|AF_LARGE|AF_SEARCHABLE|AF_PRIMARY));

		$this->add(new atkNumberAttribute("on_stock_sum",AF_NOSORT|AF_SPECIFIC_5, 10, 2));
		$this->getAttribute("on_stock_sum")->setloadType(NOLOAD);
		$this->getAttribute("on_stock_sum")->setstorageType(NOSTORE);

		$this->add(new atkNumberAttribute("amount", AF_SPECIFIC_5, 10, 2));
		$this->getAttribute("amount")->setLabel(atktext("amount_available")); // for errors
		$this->getAttribute("amount")->setstorageType(NOSTORE);
		
		$this->add(new atkNumberAttribute("reservation", AF_SPECIFIC_5, 10, 2));
		$this->getAttribute("reservation")->setstorageType(NOSTORE);
		
		$this->add(new atkNumberAttribute("on_order", AF_SPECIFIC_5, 10, 2));
		$this->getAttribute("on_order")->setstorageType(NOSTORE);
		
		$this->add(new atkDummyAttribute("signal_stock_info", atktext("signal_stock_info"), AF_HIDE_LIST));
		
		$this->add(new atkNumberAttribute("min_stock", "", 10, 2));
		$this->add(new atkNumberAttribute("max_stock", "", 10, 2));
		
		$this->setTable("stockcurrent");

	}

	function descriptor_def()
	{
		return "[product_id][location_id]";
	}

	
	function actionTitle($action, $record="")
	{
		return atktext($this->m_type);
	}
	

	function on_stock_sum_display($record, $mode="")
	{
		$ret  = '<div style="text-align: right;">'.
		$this->formatNumber($record['amount'] + $record['reservation'])
		.'</div>';

		return $ret;
	}

	
	function on_stock_sum_edit($record, $mode="")
	{
		$ret  = '<input disabled="disabled" style="text-align: right;" type="text" class="atkNumberAttribute" name="on_stock_sum" 
		value="'.$this->formatNumber($record['amount'] + $record['reservation']).'" 
		size="13">';

		return $ret;
	}

}

?>
