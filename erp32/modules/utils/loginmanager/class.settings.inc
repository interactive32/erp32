<?php

/*
 * USERS ACCESS HERE FOR SETTINGS
 */

useattrib("atkpasswordattribute");
useattrib("atkemailattribute");

class settings extends atkNode
{

	function settings()
	{
		$this->atkNode("settings", NF_ADD_LINK);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		//$this->add(new atkAttribute("user", AF_READONLY|AF_HIDE));
		//$system_data = atktext("id").": ".atkGetUser('firm_id')." / ".atktext("database_size")." = ".atkGetUser('db_size')."MB";
		//$this->addFieldSet('system_info',"$system_data",AF_HIDE_LIST,NULL, 1);

		$this->add(new atkAttribute("full_name", AF_OBLIGATORY|AF_SEARCHABLE, array(40,40,40)));
				
		$this->add(new atkEmailAttribute("email", false, AF_OBLIGATORY|AF_UNIQUE, 40));
	
		$this->add(new atkManyToOneRelation("language","loginmanager.languages", AF_OBLIGATORY));
			
		$this->addPasswordAttribute();

		// current user filter
		$user_id = atkGetUserId();
		$this->addFilter("admin_users.id = '$user_id'");

		$this->setTable("admin_users");
	}

	function addPasswordAttribute()
	{
		$passwordrestrictions = array(
      "minsize" => atkconfig("password_minsize", 4),
      "minupperchars" => atkconfig("password_minupperchars", 0),
      "minlowerchars" => atkconfig("password_minlowerchars", 0),
      "minalphabeticchars" => atkconfig("password_minalphabeticchars", 0),
      "minnumbers" => atkconfig("password_minnumbers", 0),
      "minspecialchars" => atkconfig("password_minspecialchars", 0)
		);
		$this->add(new atkPasswordAttribute("password", false, AF_HIDE_LIST|AF_PASSWORD_NOVALIDATE, 40, $passwordrestrictions));
		$this->add(new atkDummyAttribute("passwordexplanation", atktext("password_leaveblankifunchanged"), AF_HIDE_ADD|AF_HIDE_LIST|AF_HIDE_SEARCH|AF_HIDE_VIEW));
	}


	function descriptor_def()
	{
		return "[user]";
	}


	function preUpdate(&$record)
	{
		// security check if user is updating himself
		if ($record['id'] != atkGetUser('id'))
		{
			echo atktext('error_attributeedit_update');
			session_regenerate_id(true);
			die();
		}

	}


	function postUpdate($record)
	{
		session_regenerate_id(true);
		$this->redirect("index.php?atklogout=1");
		die();
	}


	/*
	 * fix buttons
	 */
	function getFormButtons($mode, $record)
	{
			
		$homepage = "dashboard.mainboard";

		if ($mode=='edit')
		{
			// cancel button
			$url = dispatch_url($homepage, "start");
			$buttons.= '<input type="button" class="btn_cancel atkdefaultbutton" name="atkback"  value="'.atktext('back_button').'" onClick="location.href=\''.$url.'\'">';


			// confirm
			$buttons.= '&nbsp;&nbsp;<input type="submit" class="btn_saveandclose" name="atksaveandclose"  value="'.atktext('saveandclose').'" onClick="return confirm(\''.atktext('confirm_userdata_change').'\')">';
			;

			return $buttons;
		}

		// bad action, return to admin
		$this->redirect(dispatch_url($homepage, "start"));
		die;
	}

}

?>