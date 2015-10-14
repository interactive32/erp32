<?php


class mod_products extends atkModule
{

	function getNodes()
	{
		registerNode("products.product", array("admin", "add", "edit", "delete"));
		registerNode("products.products_mat", array("admin", "add", "edit", "delete"));
		registerNode("products.category", array("admin", "add", "edit", "delete"));
	}


	function search($expression)
	{
		$res = array();
		$custnode = &atkGetNode("products.product");
		$recs = $custnode->searchDb($expression);

		$res["product"] = $this->recLinks($recs, "products.product");

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

