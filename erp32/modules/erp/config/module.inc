<?php


class mod_config extends atkModule
{

	function getNodes()
	{
		registerNode("config.main_config", array("edit"));
		registerNode("config.currency", array("admin", "add", "edit", "delete"));
		registerNode("config.taxes", array("admin", "add", "edit", "delete"));
		registerNode("config.units_config", array("admin", "add", "edit", "delete"));
		registerNode("config.text_templates", array("admin", "add", "edit", "delete"));
		registerNode("config.export_data", array("add", "download"));
			
		
	}



}

