<?php

class payment_base extends ia32Node
{

	function payment_base($name, $flags=0)
	{
		$this->atkNode($name, $flags);

		$this->add(new atkAttribute("id", AF_AUTOKEY), 10);

		$this->add(new atkManyToOneRelation("type_id", "finance.finance_types", AF_READONLY), 20);
		$this->getAttribute("type_id")->setAttribSize(array(255,35,15));
			
		$this->add(new atkManyToOneRelation("customer_id", "customers.customer", AF_LARGE|AF_RELATION_AUTOCOMPLETE), 40);
		$this->getAttribute("customer_id")->setAttribSize(array(255,45,45));
		$this->getAttribute("customer_id")->setDescriptorTemplate("[name]  (".atktext('account_t').": [finance_sum])");

		$this->add(new atkDateAttribute("date", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY), 30);

		$this->add(new atkNumberAttribute("amount", AF_OBLIGATORY, 10, 2), 50);
		$this->getAttribute("amount")->setLabel(atktext('money_amount'));
		$this->getAttribute("amount")->addDependency(array($this, "refresh_amount"));

		$this->add(new atkTextAttribute("notes", array("cols"=>48,"rows"=>5), AF_HIDE_LIST), 60);

		$this->add(new atkAttribute("creator", AF_READONLY|AF_HIDE_LIST|AF_HIDE_ADD, $this->std_att_size), atktext("system_tab"));

		$this->setOrder("date DESC");
		$this->setTable("finance");
	}


	function initial_values()
	{
		$ret["amount"] = 0;

		$today  = mktime(0, 0, 0, date("m")  , date("d"), date("Y"));
		$today_array = dateutil::stamp2arr($today);
		$ret["date"] = $today_array;

		return $ret;
	}

	
	function descriptor_def()
	{
		return "[type_id.name]";
	}


	/*
	 * fix buttons
	 */
	function getFormButtons($mode, $record)
	{

		$controller = &atkcontroller::getInstance();
		$controller->setNode($this);


		if ($mode=='add')
		{
			$buttons.= '<input type="submit" class="btn_saveandclose atkdefaultbutton" name="atksaveandclose"  value="'.atktext('saveandclose').'">';
			// cancel button
			$url = dispatch_url("finance.finance_all", "admin");
			$buttons.= '&nbsp;&nbsp;<input type="button" class="btn_cancel" name="atkcancel"  value="'.atktext('cancel_and_return').'" onClick="location.href=\''.$url.'\'">';

			return $buttons;
		}

		return $buttons;
	}


	/*
	 * refresh related fields
	 */
	function refresh_relatedDocumentLoad(atkEditFormModifier $modifier)
	{
		$record = &$modifier->getRecord();

		if ($record['ticket_id'])
		{
			// find leftover sum
			$sql =  "SELECT ticket_sum - ticket_payment AS leftover, customer_id FROM ia32_tickets WHERE id = '".$record['ticket_id']['id']."'";
			$recs = $this->db32select($sql);
			$record['amount'] = $recs[0]['leftover'];

			$record['customer_id']['id'] = $recs[0]['customer_id'];
		}
		
		

		if (!$record['amount']) $record['amount'] = 0;

		$modifier->refreshAttribute('customer_id');
		$modifier->refreshAttribute('amount');

	}


	/*
	 * refresh amount
	 */
	function refresh_amount(atkEditFormModifier $modifier)
	{
		$record = &$modifier->getRecord();

		if (!$record['amount']) $record['amount'] = 0;

		if ($record['amount'] < 0)
		{
			$record['amount'] = 0;

			$page = &$this->getPage();
			$page->register_loadscript("alert('".atktext('error_negativ_number')."');");
		}

		if ($record['ticket_id'])
		{
			// find leftover sum
			$ticket_id = $record['ticket_id']['id'];
			$sql =  "SELECT ticket_sum - ticket_payment AS leftover FROM ia32_tickets WHERE id = '$ticket_id'";
			$recs = $this->db32select($sql);

			// amount_exceeds_document_leftover
			if ($record['amount'] > $recs[0]['leftover'])
			{
				$record['amount'] = $recs[0]['leftover'];

				$page = &$this->getPage();
				$page->register_loadscript("alert('".atktext('amount_exceeds_document_leftover')."');");

			}
		}

		$modifier->refreshAttribute('amount');
		return;
	}


}
?>
