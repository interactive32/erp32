<?php


class mod_loginmanager extends atkModule
{

	function getNodes()
	{
		registerNode("loginmanager.erp_menu_admin", array("show"));

		registerNode("loginmanager.settings", array("edit"));
		registerNode("loginmanager.customlang", array("edit"));
	}


}
?>