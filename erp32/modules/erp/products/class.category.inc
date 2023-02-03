<?php

userelation("atkmanytoonerelation");


class category extends ia32Node
{

	function category()
	{
		$this->atkNode("category", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE|AF_MULTILANGUAGE, $this->std_att_size));

		$this->setOrder("name");
		$this->setTable("category");
	}



	function descriptor_def()
	{
		return "[name]";
	}

	
	/*
	 * don't delete if category is used or main record
	 */
	function preDelete($record)
	{
		if (is_numeric($record['id']) && $record['id']==1000)
		{
			return false; // cannot delete main record
		}

		// return false if type is used
		if($this->checkCategoryIsUsed($record['id']))
		{
			$this->display_notification(atktext("feedback_delete_constrain_error"));
			return false;
		}

		return true;
	}
	
	
	/*
	 * recordview actions on admin mode grid
	 */
	function recordActions(&$record, &$actions, &$mraactions)
	{
		// remove all actions for main initial record
		if (is_numeric($record['id']) && $record['id']<=1000)
		{
			$actions["view"] = '';	 // because of doctype_main
			$actions["edit"] = '';	 // because of doctype_main	
			$actions["delete"] = '';
		}
	}
	
	
	/**
	 * check product category is used
	 *
	 */
	function checkCategoryIsUsed($category_id)
	{
		// check parent category
		//$sql =  "SELECT * FROM ia32_category WHERE parent_id = '".stripquotes($category_id)."'";
		//$recs = $this->db32select($sql);
		//if ($recs[0]) return true;

		// check products
		$sql =  "SELECT * FROM ".$this->getUserTablePrefix()."products WHERE category_id = '".stripquotes($category_id)."'";
		$recs = $this->db32select($sql);

		if ($recs[0]) return true;

		return false;

	}

}

?>
