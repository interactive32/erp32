<?php

include_once ("class.ticket_base.inc");

class ticket_all extends ticket_base
{

	function ticket_all()
	{
		parent::ticket_base("ticket_all", NF_NO_ADD|NF_SPECIFIC_5|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY|AF_HIDE, array(255,10,10)));

		$this->add(new atkManyToOneRelation("type_id", "tickets.ticket_types", AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkManyToOneRelation("status_id", "tickets.ticket_status", AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkAttribute("ticket_number", AF_LARGE|AF_SEARCHABLE|AF_READONLY|AF_UNIQUE));

		$this->add(new atkManyToOneRelation("customer_id", "customers.customer", AF_LARGE|AF_OBLIGATORY|AF_RELATION_AUTOCOMPLETE|AF_RELATION_AUTOLINK|AF_SEARCHABLE));
		$this->getAttribute("customer_id")->setLabel(atktext("customer")); // for errors
		$this->getAttribute("customer_id")->setAttribSize(array(255,15,15));
		$this->getAttribute("customer_id")->addDependency(array($this, "refresh_top"));
	
		$this->add(new atkDateAttribute("date_created", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY|AF_DATE_NO_CALENDAR));

		$this->add(new atkNumberAttribute("ticket_sum", AF_SEARCHABLE, 10, 2));
		
		// added for export
		$this->add(new atkTextAttribute("notes", array("cols"=>120,"rows"=>5), AF_HIDE_LIST));
				
		$this->setOrder("date_created DESC");
		$this->setTable("tickets");
		
	}

}

?>
