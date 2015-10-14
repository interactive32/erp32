<?php


class mod_tickets extends atkModule
{

	function getNodes()
	{
	
		registerNode("tickets.ticket_all", array("admin"));
		registerNode("tickets.ticket_in", array("admin", "delete")); // purchaseorder can be deleted
		registerNode("tickets.ticket_out", array("admin", "delete")); // quote can be deleted
		registerNode("tickets.ticket_stock", array("admin"));
		registerNode("tickets.ticket_transfer", array("admin"));
		registerNode("tickets.ticket_search", array("admin"));

		registerNode("tickets.ticket_quote", array("view", "add", "edit", "delete", "create_invoice"));
		registerNode("tickets.ticket_invoice", array("view", "add", "return_invoice"));
		registerNode("tickets.ticket_purchaseorder", array("view", "add", "edit", "delete", "create_receipt"));
		registerNode("tickets.ticket_receipt", array("view", "add", "edit"));
		registerNode("tickets.ticket_creditnote", array("view", "add"));
		registerNode("tickets.ticket_inventory", array("view", "add"));
		registerNode("tickets.ticket_internal_transfer", array("view", "add", "close_document"));
		
		// used for export only
		registerNode("tickets.ticket_lines", array("export"));
		
	}

	function search($expression)
	{
		$res = array();
		$custnode = &atkGetNode("tickets.ticket_search");
		$recs = $custnode->searchDb($expression);

		$res["ticket"] = $this->recLinks($recs, "tickets.ticket_search");


		return $res;
	}

	function recLinks($recordset, $nodetype)
	{
		$res = array();

		$node = &atkGetNode($nodetype);
		//  $allowedaction = $node->allowed("edit") ? "edit" : ($node->allowed("view") ? "view" : "");
		// view only
		$allowedaction =  "view";
		if (!empty($allowedaction))
		{
			for($i=0, $_i=count($recordset); $i<$_i; $i++)
			{
				$item["title"] = $node->descriptor($recordset[$i]);

				$ticket_node = $this->ticketAction($recordset[$i]['type_id']['id']);
				$item["url"] = dispatch_url($ticket_node, $allowedaction, array("atkselector"=>$node->primaryKey($recordset[$i])));
				$res[] = $item;
			}
		}

		return $res;
	}
	
	
	/*
	 * recordview actions
	 */
	function ticketAction($type)
	{

		// replace original nodes with type-specific nodes
		switch ($type)
		{
			case 1001:
				$run = 'ticket_quote';
				break;

			case 1002:
				$run = 'ticket_receipt';
				break;

			case 1003:
				$run = 'ticket_creditnote';
				break;

			case 1004:
				$run = 'ticket_invoice';
				break;

			case 1005:
				$run = 'ticket_purchaseorder';
				break;

			case 1006:
				$run = 'ticket_inventory';
				break;

			case 1007:
				$run = 'ticket_internal_transfer';
				break;

			case 1008:
				$run = 'ticket_internal_transfer';
				break;
		}

		return 'tickets.'.$run;
	}
	

}

