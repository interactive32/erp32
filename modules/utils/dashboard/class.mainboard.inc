<?php

class mainboard extends ia32Node
{

	function mainboard()
	{
		$this->atkNode("mainboard");
	}


	function actionTitle($action, $record="")
	{
		return atktext(atktext('mainboard'));
	}


	function action_start(&$handler)
	{

		$randval = mt_rand();
		
		include atkconfig("atkroot").'../banner/banner.php';

		$ui = &$this->getUi();
		$theme = &atkTheme::getInstance();
		$page = &$this->getPage();
		$page->register_style($theme->stylePath("style.css"));

		$page->head(atktext("Delete log"));
		$page->body();
		$box = $ui->renderBox(array("title"=>$this->actionTitle($this->m_action),"content"=>$homepage));
		$actionpage = $this->renderActionPage("view", array($box));
		$page->addContent($actionpage);

	}
}

?>