<?php
// disable error reporting
error_reporting(NULL);

/**
 * @internal includes
 */
$config_atkroot = "./";

require_once("atk.inc");

require_once("atk/atkbrowsertools.inc");

/* load ia32node overloader */
require_once($config_atkroot.'../modules/utils/class.ia32node.inc');

require_once("../modules/utils/tools.php");
require_once("ip_blacklist.php");
atksession();
atksecure();

include "theme.inc";

$theme = &atkinstance('atk.ui.atktheme');

$user = &atkGetUser();
$user['name'] = strtolower($user['name']);


/*
 * if not administrator perform security checks...
 */
if ($user['name']!='administrator')
{
	/*
	 * check if user is suspended or already logged in
	 */

	// read data directly from database because session is cached
	// secutity class.auth_db.inc->selectUser writes user info to db
	$usertable = atkconfig("auth_usertable");
	$userfield = atkconfig("auth_userfield");
	$userid = $user['id'];
	$query = "SELECT allowed_ip, is_suspended, last_seen_on, tbl_prefix FROM $usertable WHERE id ='$userid'";
	$db = &atkGetDb(atkconfig("auth_database"));
	$recs = $db->getrows($query);
	$db_allowed_ip = $recs[0]['allowed_ip'];
	$db_is_suspended = $recs[0]['is_suspended'];
	$db_last_seen_on = $recs[0]['last_seen_on'];
	$db_tbl_prefix = $recs[0]['tbl_prefix'];
	$exp = explode(';',$db_last_seen_on);
	$db_last_seen_ip = $exp[0];
	$last_seen_on = $_SERVER[REMOTE_ADDR].'; '.atkconfig('identifier').'='.htmlspecialchars($_COOKIE[atkconfig('identifier')]);
	

	// check if IP is allowed
	if ($db_allowed_ip && $db_allowed_ip!='0' && $db_allowed_ip!=';')
	{

		$pass_ip = false;
		$exp = explode(';',$db_allowed_ip);
		foreach ($exp as $ip) // search IP match
		{
			if (trim($ip) == $_SERVER[REMOTE_ADDR])
			{
				$pass_ip = true;
				break;
			}
		}

		if (!$pass_ip) // if no IP match is found
		{
			$content = atktext('ip_not_allowed_ml');
			$content .= '<br><br><center>IP: '.$_SERVER[REMOTE_ADDR].'</center>';
			show_error_window($content, false);
			die;
		}

	}

	// check if user is suspended
	if ($db_is_suspended==1)
	{

		$content = atktext('account_suspended_ml');
		$content .= '<br><br>';
		$content .= '<a href="index.php?atklogout=1">'.atktext('login_here_ml').'</a>';
		show_error_window($content, false);
    	die;
	}

}

/*
 * run...
 */
$indexpage = &atknew('atk.ui.atkindexpage');
$indexpage->setUsername(getFullUsername());
$indexpage->setTitle(getTitle());
$indexpage->setTopSearchPiece(getSearchPiece());
$centerpiece="";
$centerpiecelinks=array();
getCenterPiece($centerpiece,$centerpiecelinks);
$indexpage->setTopCenterPieceLinks($centerpiecelinks);

$destination = array("atknodetype"=>"dashboard.mainboard","atkaction"=>"start");

if (atkGetUser('name')=='administrator')
{
	$destination = array("atknodetype"=>"loginmanager.users","atkaction"=>"admin");
}

$indexpage->setDefaultDestination($destination);
$indexpage->generate();

?>