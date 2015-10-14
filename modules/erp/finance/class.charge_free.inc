<?php

include_once ("class.payment_base.inc");

class charge_free extends payment_base
{

	function charge_free()
	{
		parent::payment_base("charge_free");

		$this->getAttribute("customer_id")->addFlag(AF_OBLIGATORY);

	}


	function initial_values()
	{
		$ret = parent::initial_values();

		$ret["type_id"]["id"] = 1007;


		return $ret;
	}


	function preAdd(&$record)
	{
		// reverse amount
		$record['amount'] = $record['amount'] * -1;

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
		// reverse amount back to normal +
		$record['amount'] = $record['amount'] * -1;

		$amount = $record['amount'];
		$customer_id = $record['customer_id']['id'];

		// update finance on customer table
		$sql =  "UPDATE ia32_customers SET finance_sum = finance_sum - $amount
					WHERE id = '$customer_id' ";
		$this->db32update($sql);

		$sess = array('atklevel'=>'0');
		$this->redirect(dispatch_url("finance.finance_all", "admin", $sess));

		$this->db32commit(); // commit before die;
		die;
	}


}
?>
