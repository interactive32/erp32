<?php


class ticket_status extends ia32Node
{

	function ticket_status()
	{
		$this->atkNode("ticket_status", NF_ADD_LINK);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE|AF_MULTILANGUAGE));

		
		$this->setOrder("name");
		$this->setTable("ticket_status");
	}


	function descriptor_def()
	{
		return "[name]";
	}


}

?>
