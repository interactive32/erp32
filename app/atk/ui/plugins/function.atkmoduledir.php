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
   * @version $Revision$
   * $Id: function.atkmoduledir.php 6354 2009-04-15 02:41:21Z mvdam $
   */

  /**
   * Implements the {atkmoduledir} plugin for use in templates.
   *
   * The atkmoduledir plugin return module path
   * Useful for including custom templates  into other teplate.
   *
   * Params:
   * modulename   module name
   *
   * Example:
   * {atkmoduledir modulename="project"}
   *
   * @author Yury Golovnya <yury@achievo.org>
   *
   */
  function smarty_function_atkmoduledir($params, &$smarty)
  {
    return $smarty->assign("atkmoduledir", moduleDir($params['modulename']));
  }
?>