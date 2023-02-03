<?php

userelation("atkmanytoonerelation");


class customer_status extends ia32Node
{

	function customer_status()
	{
		$this->atkNode("customer_status", NF_ADD_LINK);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE|AF_MULTILANGUAGE));

		$this->setOrder("name");
		$this->setTable("customer_status");
	}



	function descriptor_def()
	{
		return "[name]";
	}


}

?>
