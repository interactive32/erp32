<?php

include_once ("class.ticket_base.inc");

class ticket_search extends ticket_base
{

	function ticket_search()
	{
		$this->atkNode("ticket_search", NF_ADD_LINK);

		$this->add(new atkAttribute("id", AF_AUTOKEY|AF_HIDE, array(255,10,10)));

		$this->add(new atkManyToOneRelation("type_id", "tickets.ticket_types", AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkManyToOneRelation("status_id", "tickets.ticket_status", AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkAttribute("ticket_number", AF_LARGE|AF_SEARCHABLE|AF_READONLY|AF_UNIQUE));

		// main filter at the top of every other - current location_id
		$location_id = $this->getCurrentLocationId();
		$this->addFilter("location_id = '$location_id'");

		$this->setOrder("date_created DESC");
		$this->setTable("tickets");
	}

}

?>
