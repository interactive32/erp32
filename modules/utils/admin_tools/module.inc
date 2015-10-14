<?php


class mod_admin_tools extends atkModule
{

	function getNodes()
	{
		registerNode("admin_tools.log", array("admin"));
	}

	function getMenuItems()
	{
		if (atkGetUser('name')=='administrator')
		{
			$this->menuitem("Login manager","","",array("main.loginmanager", "admin"));
			$this->menuitem("User accounts", dispatch_url("loginmanager.users", "admin"), "Login manager", array("main.loginmanager", "admin"));
			$this->menuitem("Add user", dispatch_url("loginmanager.users", "add"), "Login manager",array("main.loginmanager", "admin"));
			$this->menuitem("Permissions", dispatch_url("loginmanager.user_groups", "admin"), "Login manager",array("main.loginmanager", "admin"));
	
			$this->menuitem("Languages","","",array("main.loginmanager", "admin"));
			$this->menuitem("Available Languages", dispatch_url("loginmanager.languages", "admin"), "Languages",array("main.loginmanager", "admin"));
				
			$this->menuitem("Log","","",array("main.loginmanager", "admin"));
			$this->menuitem("Log file", dispatch_url("admin_tools.log", "admin"), "Log",array("main.loginmanager", "admin"));
			$this->menuitem("Delete log", dispatch_url("admin_tools.log", "confirmdeletelog"), "Log",array("main.loginmanager", "admin"));

		}

	}

}

?>