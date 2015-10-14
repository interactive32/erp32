<?php


class customer extends ia32Node
{

	function customer()
	{
		$this->atkNode("customer", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_LARGE|AF_OBLIGATORY|AF_SEARCHABLE, $this->std_att_size));
		$this->add(new atkAttribute("taxid", AF_OBLIGATORY|AF_UNIQUE|AF_SEARCHABLE, $this->std_att_size));

		$this->add(new atkManyToOneRelation("type_id", "customers.customer_types", AF_HIDE_LIST|AF_OBLIGATORY));
		$this->getAttribute("type_id")->setLabel(atktext('customer_type'));

		$this->add(new atkNumberAttribute("discount", AF_HIDE_LIST|AF_OBLIGATORY ,10, 2));
		$this->getAttribute("discount")->addDependency(array($this, "refresh_numbers"));
		$this->getAttribute("discount")->setLabel(atktext('discount_percent'));

		$this->add(new atkNumberAttribute("finance_sum", AF_SPECIFIC_5 ,10, 2)); // AF_SPECIFIC_5
		$this->getAttribute("finance_sum")->setstorageType(NOSTORE);

		$this->add(new atkNumberAttribute("credit_limit", "" ,10, 2));
		$this->getAttribute("credit_limit")->addDependency(array($this, "refresh_numbers"));

		$this->add(new atkManyToOneRelation("credit_status_id", "customers.customer_status", AF_HIDE_LIST|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkAttribute("city", AF_HIDE_LIST|AF_SEARCHABLE, $this->std_att_size) , atktext('customer_info'));
		$this->add(new atkAttribute("zip", AF_HIDE_LIST|AF_SEARCHABLE, $this->std_att_size) , atktext('customer_info'));

		$this->add(new atkAttribute("state", AF_HIDE_LIST|AF_SEARCHABLE, $this->std_att_size) , atktext('customer_info'));
		$this->add(new atkAttribute("address", AF_HIDE_LIST|AF_SEARCHABLE, $this->std_att_size), atktext('customer_info'));
		$this->add(new atkAttribute("telephone",  AF_HIDE_LIST, $this->std_att_size), atktext('customer_info'));
		$this->add(new atkEmailAttribute("email", false, AF_HIDE_LIST|AF_SEARCHABLE, $this->std_att_size), atktext('customer_info'));

		$this->add(new atkTextAttribute("notes", array("cols"=>60,"rows"=>10), AF_HIDE_LIST), atktext('notes'));

		$this->add(new atkAttribute("creator", AF_READONLY|AF_HIDE_LIST, $this->std_att_size), atktext("system_tab"));
		$this->add(new atkAttribute("lastmod", AF_READONLY|AF_HIDE_LIST, $this->std_att_size), atktext("system_tab"));

		//$this->add(new atkBoolAttribute("hidden"), atktext("system_tab"));

		$this->setOrder("name");
		$this->setTable("customers");
	}


	function descriptor_def()
	{
		return "[name]";
	}

	function initial_values()
	{
		return array("discount"=>"0",
					"finance_sum"=>"0.00",	
					"credit_status_id"=>array("id"=>"1000"));
	}


	function preDelete(&$record)
	{
		// return false if customer is used
		if($this->checkCustomerIsUsed($record['id']))
		{
			$this->display_error(atktext("feedback_delete_constrain_error"));
			return false;
		}

		// return false if customer has finance_sum <> 0
		if($record['finance_sum']<>0)
		{
			$this->display_error(atktext("feedback_delete_customer_with_finance"));
			return false;
		}

		return true;
	}


	function preAdd(&$record)
	{
		// created by
		$this->getAttribute('creator')->removeFlag(AF_READONLY);
		$record['creator'] = $this->getUserTrack();
		return true;
	}


	function preUpdate(&$record)
	{
		// lastmod by
		$this->getAttribute('lastmod')->removeFlag(AF_READONLY);
		$record['lastmod'] = $this->getUserTrack();

		return true;
	}



	/*
	 * fix buttons
	 */
	function getFormButtons($mode, $record)
	{
		// if add, but not add from other forms
		if ($mode=='add' && !$this->m_postvars['atkpkret'])
		{
			$buttons.= '<input type="submit" class="btn_saveandclose atkdefaultbutton" name="atksaveandclose"  value="'.atktext('saveandclose').'">';

			// cancel button
			$url = dispatch_url($this->atknodetype(), "admin");
			$buttons.= '&nbsp;&nbsp;<input type="button" class="btn_cancel" name="atkcancel"  value="'.atktext('cancel_and_return').'" onClick="location.href=\''.$url.'\'">';

			return $buttons;
		}
			
		$controller = &atkcontroller::getInstance();
		$controller->setNode($this);

		$buttons = $controller->getFormButtons($mode, $record);

		return $buttons;
	}


	/*
	 * post add
	 */
	function postAdd(&$record)
	{
		// if not distant selection - return to admin
		if (!$this->m_postvars['atkpkret'])
		{
			$sess = array('atklevel'=>'0');
			$this->redirect(dispatch_url("customers.customer", "admin", $sess));
				
			$this->db32commit(); // commit before die;
			die;
		}

		return true;
	}



	/**
	 * check if customer is used documents
	 *
	 */
	function checkCustomerIsUsed($id)
	{
		$sql =  "SELECT * FROM ia32_tickets WHERE customer_id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		if ($recs[0]) return true;

		return false;
	}


	/*
	 * refresh numbers to look nicer
	 */
	function refresh_numbers($modifier)
	{
		$modifier->refreshAttribute('discount');
		$modifier->refreshAttribute('credit_limit');
	}

}

?>
