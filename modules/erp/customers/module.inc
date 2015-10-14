<?php


class mod_customers extends atkModule
{

	function getNodes()
	{
		registerNode("customers.customer", array("admin", "add", "edit", "delete"));
	}

	function search($expression)
	{
		$res = array();
		$custnode = &atkGetNode("customers.customer");
		$recs = $custnode->searchDb($expression);

		$res["customer"] = $this->recLinks($recs, "customers.customer");


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
				$item["url"] = dispatch_url($nodetype, $allowedaction, array("atkselector"=>$node->primaryKey($recordset[$i])));
				$res[] = $item;
			}
		}

		return $res;
	}

}

