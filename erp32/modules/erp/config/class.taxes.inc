<?php


class taxes extends ia32Node
{

	function taxes()
	{
		$this->atkNode("taxes", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE, array(6,6,6)));

		$this->add(new atkNumberAttribute("rate", AF_OBLIGATORY|AF_SEARCHABLE, array(2,2,2), 2));
		$this->getAttribute("rate")->setLabel(atktext('tax_rate_percent'));

		$this->setOrder("id");
		$this->setTable("taxes");
	}


	function descriptor_def()
	{
		return "[name]";
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
		if($this->checkTaxIsUsed($record['id']))
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
			//$actions["view"] = '';
			//$actions["edit"] = '';
			$actions["delete"] = '';
		}
	}



	/**
	 * check tax is used
	 *
	 */
	function checkTaxIsUsed($tax_id)
	{
		$sql =  "SELECT * FROM ia32_products WHERE tax_id = '".stripquotes($tax_id)."'";
		$recs = $this->db32select($sql);

		if ($recs[0]) return true;

		return false;
	}

	/**
	 * multiply by 100
	 *
	 */
	function edit_values($record)
	{
		$record['rate'] = 100 * $record['rate'];

		return $record;

	}

	/**
	 * divide by 100
	 *
	 */
	function preAdd(&$record)
	{
		$record['rate'] = $record['rate'] / 100;
		$this->getAttribute("rate")->setDecimals(4);		

		return true;
	}

	/**
	 * divide by 100
	 *
	 */
	function preUpdate(&$record)
	{

		$record['rate'] = $record['rate'] / 100;
		$this->getAttribute("rate")->setDecimals(4);
		
		return true;

	}


	function rate_display(&$record, $mode)
	{
		$record['rate'] = 100 * $record['rate'];
		
		$attribute = &$this->getAttribute("rate");
		$original = $attribute->display($record, $mode);

		return $original;
	}



}

?>
