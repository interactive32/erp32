<?php

class log extends ia32Node
{

	function log()
	{
		$this->atkNode("log", NF_ADD_LINK);


		$this->add(new atkAttribute("id", AF_PRIMARY|AF_AUTOINCREMENT|AF_READONLY));
		$this->add(new atkAttribute("timestamp",AF_READONLY));
		$this->add(new atkAttribute("action",AF_READONLY|AF_SEARCHABLE));

		$this->setOrder("timestamp DESC");
		$this->setTable(atkconfig('auth_logtable'));

	}

	function allowed($action, $record="")
	{

		if ($action=="delete" || $action=="edit" ) return false;
		
		return true;
		
	}

	function action_confirmdeletelog ()
	{

		$goto = dispatch_url("admin_tools.log", "deletelog");
		$content = '<center><br>Do you really wish to delete log?<br><br>
		<FORM METHOD="POST" ACTION="'.$goto.'">
		<INPUT TYPE="submit" VALUE="Delete Log!">
		';

		$ui = &$this->getUi();
		$theme = &atkTheme::getInstance();
		$page = &$this->getPage();
		$page->register_style($theme->stylePath("style.css"));

		$page->head(atktext("Delete log"));
		$page->body();
		$box = $ui->renderBox(array("title"=>"Delete log","content"=>$content));
		$actionpage = $this->renderActionPage("view", array($box));
		$page->addContent($actionpage);
		
	}

	function action_deletelog ()
	{

		$db = &atkGetDb();
		$delete_log = "DELETE FROM ".atkconfig('auth_logtable')." where 1=1";
		$db->query($delete_log);
		$db->commit();
			
		
		
		$content = '<center><br>Notice: Log deleted!<br>';

		$ui = &$this->getUi();
		$theme = &atkTheme::getInstance();
		$page = &$this->getPage();
		$page->register_style($theme->stylePath("style.css"));

		$page->head(atktext("Delete log"));
		$page->body();
		$box = $ui->renderBox(array("title"=>"Delete log","content"=>$content));
		$actionpage = $this->renderActionPage("view", array($box));
		$page->addContent($actionpage);
		
		
	}

}

?>