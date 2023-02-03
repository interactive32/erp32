<?php

/*
 * IP black list, separate by comma
 */
$black_ip_list_array = "

255.255.255.255,
0.0.0.0

";

$black_ip_list = explode (",", $black_ip_list_array);

foreach ($black_ip_list as $blackip) // search IP match
{
	if (trim($blackip) == $_SERVER[REMOTE_ADDR])
	{
		$content = '<center><b>'.atktext('ip_blacklisted_ml');
		$content .= '<br><br>IP: '.$_SERVER[REMOTE_ADDR].'</center>';
		show_error_window($content, false);
		die;
	}
}

?>