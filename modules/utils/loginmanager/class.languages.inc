<?php


class languages extends atkNode
{

	function languages()
	{
		$this->atkNode("languages", NF_ADD_LINK);

		$this->add(new atkAttribute("autoinc", AF_HIDE_ADD));
		$this->add(new atkAttribute("id", AF_PRIMARY|AF_UNIQUE));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE));

		$this->setTable("admin_languages");
		
		$this->setOrder("autoinc");
	}

	function descriptor_def()
	{
		return "[name]";
	}

	
	
}

?>