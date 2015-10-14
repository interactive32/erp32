<?php

class finance_all extends ia32Node
{

	function finance_all()
	{
		$this->atkNode("finance_all", NF_NO_ADD|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));

		$this->add(new atkManyToOneRelation("type_id", "finance.finance_types", AF_OBLIGATORY|AF_SEARCHABLE));
		$this->getAttribute("type_id")->setAttribSize(array(255,35,15));
			
		$this->add(new atkDateAttribute("date", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY));

		$this->add(new atkManyToOneRelation("ticket_id", "tickets.ticket_all", AF_HIDE_ADD|AF_LARGE|AF_RELATION_AUTOCOMPLETE|AF_SEARCHABLE));
		$this->getAttribute("ticket_id")->setLabel(atktext("document_link"));
		$this->getAttribute("ticket_id")->setAttribSize(array(255,35,15));
	
		$this->add(new atkManyToOneRelation("customer_id", "customers.customer", AF_OBLIGATORY|AF_RELATION_AUTOCOMPLETE|AF_SEARCHABLE));
		$this->getAttribute("customer_id")->setAttribSize(array(255,35,15));

		$this->add(new atkNumberAttribute("amount", AF_OBLIGATORY, 10, 2));
		$this->getAttribute("amount")->setLabel(atktext('money_amount'));
		
		$this->add(new atkTextAttribute("notes", array("cols"=>48,"rows"=>5), AF_HIDE_LIST));

		$this->add(new atkAttribute("creator", AF_READONLY|AF_HIDE_LIST|AF_HIDE_ADD, $this->std_att_size), atktext("system_tab"));

		$this->setOrder("date DESC");
		$this->setTable("finance");
	}

	function descriptor_def()
	{
		return "[type_id.name]";
	}


	/*
	 * recordview actions on admin mode grid
	 */
	function recordActions(&$record, &$actions, &$mraactions)
	{
		// remove edit & delete action
		$actions["edit"] = '';
		$actions["delete"] = '';

	}

}
?>
