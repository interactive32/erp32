<?php

include_once ("class.payment_base.inc");

class payment_from_customer extends payment_base
{

	function payment_from_customer()
	{
		parent::payment_base("payment_from_customer");
			
		$prefix = $this->getUserTablePrefix();

		$this->add(new atkManyToOneRelation("ticket_id", "tickets.ticket_out", AF_OBLIGATORY|AF_LARGE|AF_RELATION_AUTOCOMPLETE), 35);
		$this->getAttribute("ticket_id")->setLabel(atktext('document_link'));
		$this->getAttribute("ticket_id")->m_destinationFilter = $prefix."tickets.ticket_sum - ".$prefix."tickets.ticket_payment > 0"; // filter = not fully paid
		$this->getAttribute("ticket_id")->setAttribSize(array(255,45,15));
		$this->getAttribute("ticket_id")->addDependency(array($this, "refresh_relatedDocumentLoad"));
		$this->getAttribute("ticket_id")->setDescriptorTemplate("[ticket_number]  (".atktext('ticket_payment').": [ticket_payment] / [ticket_sum])");

		$this->getAttribute("customer_id")->addFlag(AF_HIDE);

	}


	function initial_values()
	{
		$ret = parent::initial_values();

		$ret["type_id"]["id"] = 1002;

		return $ret;
	}


	function preAdd(&$record)
	{
		// reverse amount on credit note
		$ticket_type = $this->getTicketType($record['ticket_id']['id']);
		if ($ticket_type['id'] == 1003) $record['amount'] = $record['amount'] * -1;

		if ($record['ticket_id'])
		{
			$customer = $this->getCustomerFromTicket($record['ticket_id']['id']);
			$record['customer_id']['id'] = $customer['id'];
		}

		// created by
		$this->getAttribute('creator')->removeFlag(AF_READONLY);
		$record['creator'] = $this->getUserTrack();

		return true;
	}


	/*
	 * post add - update ticket & customer finance
	 */
	function postAdd(&$record)
	{
		$customer = $this->getCustomerFromTicket($record['ticket_id']['id']);
		$customer_id = $customer['id'];

		$ticket_id = $record['ticket_id']['id'];
		$amount = $record['amount'];

		// update finance on customer table
		$sql =  "UPDATE ia32_customers SET finance_sum = finance_sum + $amount
					WHERE id = '$customer_id' ";
		$this->db32update($sql);

		// reverse amount on credit note for corrent ticket_payment close
		$ticket_type = $this->getTicketType($record['ticket_id']['id']);
		if ($ticket_type['id'] == 1003) $amount = $amount * -1;

		// add amount to ticket's ticket_payment
		$sql =  "UPDATE ia32_tickets SET ticket_payment = ticket_payment + $amount
					WHERE id = '$ticket_id' ";
		$this->db32update($sql);

		$sess = array('atklevel'=>'0');
		$this->redirect(dispatch_url("finance.finance_all", "admin", $sess));

		$this->db32commit(); // commit before die;
		die;
	}

}
?>
