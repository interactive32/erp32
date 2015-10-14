<?php


class mod_locations extends atkModule
{

	function getNodes()
	{
		registerNode("locations.location", array("admin", "add", "edit", "delete", "change"));	
	}

    
}

