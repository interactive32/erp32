<?php


// tabela sastavnica (recepata)

class products_mat extends ia32Node
{

	function products_mat()
	{
		$this->atkNode("products_mat");

		$this->add(new atkManyToOneRelation("product_id", "products.product", AF_PRIMARY));
		$this->add(new atkManyToOneRelation("material_id", "products.product", AF_PRIMARY|AF_MANYTOONE_AUTOCOMPLETE));
		$this->add(new atkAttribute("amount",AF_OBLIGATORY));

		$this->setTable("products_mat");
	}

	function descriptor_def()
	{
		return "Sastavnica";
	}

	function initial_values()
	{

		return array("amount"=>"1");
	}


}

?>
