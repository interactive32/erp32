<?php

userelation("atkmanytoonerelation");


class customer_types extends ia32Node
{

	function customer_types()
	{
		$this->atkNode("customer_types", NF_ADD_LINK);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE|AF_MULTILANGUAGE));

		$this->setOrder("name");
		$this->setTable("customer_types");
	}



	function descriptor_def()
	{
		return "[name]";
	}


}

?>
