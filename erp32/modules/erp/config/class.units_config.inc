<?php


class units_config extends ia32Node
{

	function units_config()
	{
		$this->atkNode("units_config", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE|AF_MULTILANGUAGE));
		$this->add(new atkAttribute("symbol", AF_OBLIGATORY|AF_SEARCHABLE|AF_MULTILANGUAGE, array(3,3,3)));

		$this->setOrder("id");
		$this->setTable("units");
	}


	function descriptor_def()
	{
		return "[symbol]";
	}


	/*
	 * don't delete main 3 initial records
	 */
	function preDelete($record)
	{
		if (is_numeric($record['id']) && $record['id']<=1002)
		{
			return false;
		}

		// return false if type is used
		if($this->checkUnitIsUsed($record['id']))
		{
			$this->display_error(atktext("feedback_delete_constrain_error"));
			return false;
		}

		return true;
	}

	/*
	 * recordview actions on admin mode grid
	 */
	function recordActions(&$record, &$actions, &$mraactions)
	{
		// remove all actions for main 3 initial records
		if (is_numeric($record['id']) && $record['id']<=1002)
		{
			$actions["view"] = '';
			$actions["edit"] = '';
			$actions["delete"] = '';
		}
	}


	/**
	 * check unit is used
	 *
	 */
	function checkUnitIsUsed($unit_id)
	{
		$sql =  "SELECT * FROM ia32_products WHERE unit_id = '".stripquotes($unit_id)."'";
		$recs = $this->db32select($sql);

		if ($recs[0]) return true;

		return false;

	}
}

?>
