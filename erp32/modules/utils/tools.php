<?php

require_once "utility_scripts/dateutil.php";
require_once "utility_scripts/error_window.php";

/*
 * browser title
 */
function getTitle()
{
	$title = atktext("app_title"); //." ".atkconfig("version");
	return $title;
}

/*
 * alcal: set left bars (ads and stuff on left bars)
 */
function set_leftbars()
{
	global $ATK_VARS;

	// simple local gif ad, amazon etc
	include atkconfig("atkroot").'../banner/banner.php';

	if (atkGetUser('name')=='administrator') return array('m_leftbar1' => 'Hi Administrator',
					'm_leftbar2' => $banner,
					'm_leftbar_bottom' => $terms,
					'm_leftbar_footer' => $footer);


	$firm_id = atkGetUser('firm_id');
	$firm_data = getFirmData($firm_id);

	$date = date("d.m.Y");



	$leftbar1 = '<center>
	  <b>'.$firm_data['firm_name'].'</b><br><br>
	  <a href="index.php?atknodetype=config.main_config&atkaction=edit&atklevel=0&atkprevlevel=0&atktab='.atktext('picture_logo_tab').'">
	  <img width="'.atkconfig("left_sidebar_width").'" src="./images/users/logo/'.$firm_data['logo_picture'].'" alt="" border="0"></a>
	  <br><br>
	  '.$date;

	$footer = '';
	
	$ret = array('m_leftbar1' => $leftbar1,
					'm_leftbar2' => $banner,
					'm_leftbar_bottom' => '',
					'm_leftbar_footer' => $footer,
					'left_sidebar_width' => atkconfig("left_sidebar_width"));

	return $ret;
}

/*
 * search and link to node
 */
function getSearchPiece()
{
	$theme = &atkinstance("atk.ui.atktheme");
	if($theme->getAttribute('useframes',true)) { $target='main'; } else { $target=""; }
	$node = atkconfig("top_search_node") ? atkconfig("top_search_node") : "search.search";
	$searchnode = &atkGetNode($node);
	$searchpiece = $searchnode->simpleSearchForm("", $target, SESSION_NEW);
	return $searchpiece;
}

/*
 * center piece links
 */
function getCenterPiece(&$centerpiece,&$centerpiecelinks)
{
	$user = &atkGetUser();

	$theme = &atkinstance("atk.ui.atktheme");

	// Set the dispatchfile for this menu based on the theme setting, or to the default if not set.
	// This makes sure that all calls to dispatch_url will generate a url for the main frame and not
	// within the menu itself.
	$dispatcher = $theme->getAttribute('dispatcher', atkconfig("dispatcher", "index.php")); // do not use atkSelf here!
	$c = &atkinstance("atk.atkcontroller");
	$c->setPhpFile($dispatcher);

	if($theme->getAttribute('useframes',true)) { $target='target="main"'; } else { $target=""; }

	//$centerpiece = $centerpiecelinks['pim'] = href(dispatch_url("dashboard.mainboard", "start"), atktext("pim"), SESSION_NEW, false,$target);

	// change location link - if location_name is null there is only one location!
	if ($user['location_id'] !== null)
	{
		$current_location = atktext('current_location').': '.$user['location_name'];
		$atktarget = "index.php?atknodetype=locations.location&atkaction=change&atklevel=0&atkprevlevel=0&atkselect=[atkprimkey]";
		$params = array("atkfilter"=>$filter,"atktarget"=>$atktarget);
		$centerpiece = $centerpiecelinks['change_location'] = href(dispatch_url("locations.location", "select", $params), $current_location, SESSION_NEW, false,$target);
	}

	// if user settings is allowed put link to it
	if (is_allowed("loginmanager.settings", "edit") && substr($user['name'],0,4)!='demo')
	{
		$centerpiece.= '&nbsp; &nbsp; &nbsp;';
		$centerpiece.= $centerpiecelinks['userprefs'] =
		href(dispatch_url("loginmanager.settings", "edit"), atktext("userprefs"), SESSION_NEW, false,$target);

	}

}

/*
 * global, etc. for debugger output
 */
function getFullUsername()
{
	$user = &atkGetUser();
	$username = $user["name"];
	atkdebug("tools::getFullUsername() -> ".$username);
	return $username;
}


/*
 * get firm data
 */
function getFirmData($id)
{
	$sql = "SELECT * FROM ia32_config";

	$conn = atkGetUser(atkconfig('auth_dbconfigfield'));
	$db = &atkGetDb($conn);

	$user_prefix = atkGetUser(atkconfig('auth_tblprefixfield'));

	$sql = str_replace('ia32_', $user_prefix, $sql);

	$ret = $db->getrows($sql);

	return $ret[0];
}

?>