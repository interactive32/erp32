<?php


class mod_finance extends atkModule
{

	function getNodes()
	{
		registerNode("finance.finance_all", array("admin"));

		registerNode("finance.payment_from_customer", array("add"));
		registerNode("finance.payment_to_supplier", array("add"));
		registerNode("finance.payment_free", array("add"));
		registerNode("finance.charge_free", array("add"));
		
	}


}

