<?php


class mod_pricelist extends atkModule
{

	function getNodes()
	{
		registerNode("pricelist.price", array("admin", "add", "edit"));
		registerNode("pricelist.price_list", array("admin", "add", "edit", "delete"));
		registerNode("pricelist.batch_copy", array("add", "copyPricelist"));
		registerNode("pricelist.batch_update", array("add", "updatePricelist"));
	
	}


	
}

