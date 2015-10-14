<?php
/**
 * This file is part of the Achievo distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package achievo
 * @subpackage search
 *
 * @copyright (c)2008 Ibuildings B.V.
 * @license http://www.achievo.org/licensing Achievo Open Source License
 *
 * @version $Revision: 5049 $
 * $Id: class.search.inc 5049 2008-06-23 20:56:48Z sandy $
 */

/**
 * Global search functionality
 *
 * @package achievo
 * @subpackage search
 */
class search extends ia32Node
{
	/**
	 * Constructor
	 * Anyone may perform global search. (security is enforced per
	 * search result)
	 */
	function search()
	{
		$this->atkNode("search", NF_NO_SECURITY);
	}

	/**
	 * Search action
	 *
	 * @param object $handler Handler
	 * @return boolean
	 */
	function action_search(&$handler)
	{
		global $g_sessionManager;

		// Searchstring is a stackvar. We get it through the post_vars,
		// but we need to store it in the session stack, so the system
		// remembers the last searchstring when coming back from a
		// view action.
		$searchstring = strip_tags($g_sessionManager->stackVar("searchstring"));

		$min_length = atkconfig('top_search_minchars');

		if ($searchstring!="" && strlen($searchstring)>=$min_length)
		{
			$results = atkHarvestModules("search", $searchstring, true);
			$firsturl="";
			$res = "";

			foreach ($results as $module=>$moduleresults)
			{
				foreach ($moduleresults as $category=>$items)
				{
					$cnt = count($items);
					if ($cnt>0)
					{
						$hasresults += $cnt;
						$res .= '<br><b>'.atktext($category, $module).'</b><br>';
						for ($i=0;$i<$cnt;$i++)
						{
							// $res.= '<br>&nbsp; &nbsp;'.href($items[$i]["url"], $items[$i]["title"], SESSION_NESTED);
							$res.= '<br>&nbsp; &nbsp;'.href($items[$i]["url"], $items[$i]["title"], SESSION_NESTED);
							if ($firsturl=="") $firsturl = $items[$i]["url"];
						}
						$res.='<br>';
					}
				}
			}

			if ($hasresults==0)
			{
				$res = '<br>'.sprintf(atktext("search_noresults"), $searchstring);
			}
			else if ($hasresults==1)
			{
				// autoview first url
				return $this->redirect(session_url($firsturl, SESSION_REPLACE));
			}
		}
		elseif ($searchstring!="" && strlen($searchstring)<$min_length)
		{
			$res = '<br>'.sprintf($this->text('search_minimum_chars'), $min_length);
		}else
		{
			$res = '<br>'.atktext("search_emptysearchstring");
		}

		// disabled search again
		// $res.='<br><br>'.atktext('search_again').':<br>';
		// $res.=$this->simpleSearchForm($searchstring, "", SESSION_DEFAULT).'<br><br>';

		$ui = &$this->getUi();
		$page = &$this->getPage();
		$this->addStyle("style.css");
		$box = $ui->renderBox(array("title"=>atkText("search","search"),"content"=>$res));
		$actionpage = $this->renderActionPage("search", $box);
		$page->addContent($actionpage);
		return true;
	}

	/**
	 * Render simple search form
	 *
	 * @param string $searchstring Search string
	 * @param string $targetframe Target frame
	 * @param int $sessionstatus Session status
	 * @return string
	 */
	function simpleSearchForm($searchstring, $targetframe="", $sessionstatus=SESSION_NESTED)
	{
		$ui = &$this->getUi();
		$tplvars = array( 'targetframe'=>$targetframe,
                        'sessionform'=>session_form($sessionstatus),
                        'searchstring'=>$searchstring,
                        'dispatch'=>getDispatchFile(),
		);
		return $ui->render($ui->templatePath('simplesearch.tpl'),$tplvars);
	}
}

?>