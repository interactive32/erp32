<?php

/**
 * This file is part of the Achievo ATK distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package atk
 * @subpackage ui
 *
 * @copyright (c)2004 Ivo Jansch
 * @license http://www.achievo.org/atk/licensing ATK Open Source License
 *
 * @version $Revision: 4599 $
 * $Id: function.atkconfig.php 6354 2009-04-15 02:41:21Z mvdam $
 */

/**
 * Function for getting a configuration variable and set this in a variable in smarty.
 * 
 * @author Lineke Kerckhoffs-Willems <lineke@ibuildings.nl>
 */
function smarty_function_atkconfig($params, &$smarty)
{
  $smarty->assign($params["smartyvar"], atkconfig($params["var"]));
}

?>