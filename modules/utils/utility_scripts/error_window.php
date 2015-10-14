<?php

/*
 * uses smarty error.tpl
 */
function show_error_window($content, $closebutton=true,  $windowtitle="")
{

	if (!$windowtitle) $windowtitle = atktext('notice');

	$content .= '<br>';

	if ($closebutton==true)
	{
		$buttonlink = 'true';
	}


	$page = &atknew("atk.ui.atkpage");
	$ui = &atkinstance("atk.ui.atkui");
	$output = &atkOutput::getInstance();
	$page->register_style($ui->stylePath("style.css"));
	$res = $ui->renderBox(array("closebutton"=>$buttonlink,
								"content"=>$content),"error");

	$page->addContent($res);
	$output->output($page->render($windowtitle));
	$output->outputFlush();
}

function show_notification_window($content, $closebutton=true,  $windowtitle="")
{

	if (!$windowtitle) $windowtitle = atktext('notice');

	$content .= '<br>';

	if ($closebutton==true)
	{
		$buttonlink = 'true';
	}


	$page = &atknew("atk.ui.atkpage");
	$ui = &atkinstance("atk.ui.atkui");
	$output = &atkOutput::getInstance();
	$page->register_style($ui->stylePath("style.css"));
	$res = $ui->renderBox(array("closebutton"=>$buttonlink,
								"content"=>$content),"error");

	$page->addContent($res);
	$output->output($page->render($windowtitle));
	$output->outputFlush();
}



?>