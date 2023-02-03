<?php


class mod_stocklist extends atkModule
{

	function getNodes()
	{
		registerNode("stocklist.stockcurrent", array("admin", "edit"));	
	}


	
}

