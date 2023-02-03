<?php


class finance_types extends ia32Node
{

	function finance_types()
	{
		$this->atkNode("finance_types", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE|AF_MULTILANGUAGE));

		$this->setOrder("id");
		$this->setTable("finance_types");
	}


	function descriptor_def()
	{
		return "[name]";
	}

	
	/*
	 * don't delete main initial records
	 */
	function preDelete($record)
	{
		if (is_numeric($record['id']) && $record['id']<=1999)
		{
			return false;
		}

		// return false if type is used
		if($this->checkFinanceTypeIsUsed($record['id']))
		{
			$this->display_error(atktext("feedback_delete_constrain_error"));
			die;
		}
		
		return true;
	}
	
	
	/*
	 * recordview actions on admin mode grid
	 */
	function recordActions(&$record, &$actions, &$mraactions)
	{
		// remove all actions for main 3 initial records
		if (is_numeric($record['id']) && $record['id']<=1999)
		{
			$actions["view"] = '';
			$actions["edit"] = '';
			$actions["delete"] = '';
		}
	}
	

	/**
	 * check finance type is used
	 *
	 */
	function checkFinanceTypeIsUsed($type_id)
	{
		$sql =  "SELECT * FROM ia32_finance WHERE type_id = '".stripquotes($type_id)."'";
		$recs = $this->db32select($sql);

		if ($recs[0]) return true;

		return false;

	}
}

?>
