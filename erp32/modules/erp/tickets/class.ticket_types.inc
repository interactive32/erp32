<?php


class ticket_types extends ia32Node
{

	function ticket_types()
	{
		$this->atkNode("ticket_types", NF_ADD_LINK);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE|AF_MULTILANGUAGE));

		
		$this->setOrder("name");
		$this->setTable("ticket_types");
	}


	function descriptor_def()
	{
		return "[name]";
	}


}

?>
