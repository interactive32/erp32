<?php

/*
 * USED ONLY FOR DATA EXPORT
 */


class ticket_lines extends ticket_base
{

	function ticket_lines()
	{
		$this->atkNode("ticket_lines");
			
		$this->add(new atkManyToOneRelation("ticket_id", "tickets.ticket_all"));
		$this->add(new atkAttribute("line"));
		//$this->add(new atkManyToOneRelation("product_id", "products.product"));
		$this->add(new atkAttribute("units"));
		$this->add(new atkAttribute("fix_productname"));
		$this->add(new atkAttribute("fix_price"));
		$this->add(new atkAttribute("fix_pricebuy"));
		$this->add(new atkAttribute("fix_discount"));
		$this->add(new atkAttribute("fix_taxname"));
		$this->add(new atkAttribute("fix_unitsymbol"));

		$this->setOrder("ticket_id, line");	
		$this->setTable("ticket_lines");

	}

}

?>
