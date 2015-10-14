<?php


class export_data extends ia32Node
{

	function export_data()
	{
		$this->atkNode("export_data");

		$this->add(new atkListAttribute("options",	array(),""));
		$this->getAttribute("options")->addOption(atktext("export_all"),"export_all");
		//$this->getAttribute("options")->addOption(atktext("export_maindata"),"export_maindata");
		$this->getAttribute("options")->addFlag(AF_LIST_NO_NULL_ITEM);

	}

	function actionTitle($action, $record="")
	{
		return atktext($this->m_type);
	}


	/*
	 * fix buttons
	 */
	function getFormButtons($mode, $record)
	{
		// ia32.js for print and confirm window
		$page = &$this->getPage();
		$page->register_script(atkconfig("atkroot")."atk/javascript/ia32.js");
			
		if ($mode=='add')
		{
			// export button
			$url = dispatch_url($this->atknodetype(), "download");
			$buttons .= '<input type="button" class="btn_save atkdefaultbutton" name="button" value="'.atktext('download_CSV').'" onClick="simplePopup(\''.$url.'\')">';

			return $buttons;
		}

		// bad action, return to home
		$this->redirect("index.php");
		die;
	}


	/*
	 * download file
	 */
	function action_download()
	{

		// Query the database
		$usernode = &atkGetNode("pricelist.price");
		$priceRecords = $usernode->selectDb();
		foreach($priceRecords AS $key=>&$value){
			$value["pricelist_name"] = $value["pricelist_id"]["name"];
			$value["pricelist_id"] = $value["pricelist_id"]["id"];
			$value["product_name"] = $value["product_id"]["name"];
			$value["product_id"] = $value["product_id"]["id"];
		}


		$usernode = &atkGetNode("products.product");
		$productRecords = $usernode->selectDb();
		foreach($productRecords AS $key=>&$value){
			$value["unit_id"] = $value["unit_id"]["symbol"];
			$value["category_id"] = $value["category_id"]["name"];
			$value["tax_id"] = $value["tax_id"]["name"];

			$value["track_stock"] = ($value["track_stock"]=="1") ? atktext("yes") : atktext("no");
			$value["hidden"] = ($value["hidden"]=="1") ? atktext("yes") : atktext("no");

			// match prices from pricelist table from main pricelist (1000)
			foreach($priceRecords AS $pricevalue){
				if($value["id"]==$pricevalue["product_id"] && $pricevalue["pricelist_id"]=="1000")
				{
					$value["pricesell"]=$pricevalue["pricesell"];
					$value["pricebuy"]=$pricevalue["pricebuy"];
				}
			}
		}

		$usernode = &atkGetNode("customers.customer");
		$customerRecords = $usernode->selectDb();
		foreach($customerRecords AS $key=>&$value){
			$value["type_id"] = $value["type_id"]["name"];
			$value["credit_status_id"] = $value["credit_status_id"]["name"];
		}

		$usernode = &atkGetNode("finance.finance_all");
		$financeRecords = $usernode->selectDb();
		foreach($financeRecords AS $key=>&$value){
			$value["type_id"] = $value["type_id"]["name"];
			$value["date"] = dateutil::arr2str($value["date"],atktext('date_format_pdfreport'));
			$value["ticket_id"] = $value["ticket_id"]["ticket_number"];
			$value["customer_id"] = $value["customer_id"]["name"];
		}

		$usernode = &atkGetNode("stocklist.stockcurrent");
		$stockRecords = $usernode->selectDb();
		foreach($stockRecords AS $key=>&$value){
			$value["location_id"] = $value["location_id"]["name"];
			$value["product_id"] = $value["product_id"]["name"];
		}

		$usernode = &atkGetNode("tickets.ticket_all");
		$ticketRecords = $usernode->selectDb();
		foreach($ticketRecords AS $key=>&$value){
			$value["type_id"] = $value["type_id"]["name"];
			$value["status_id"] = $value["status_id"]["name"];
			$value["customer_id"] = $value["customer_id"]["name"];
			$value["date_created"] = dateutil::arr2str($value["date_created"],atktext('date_format_pdfreport'));
		}

		$usernode = &atkGetNode("tickets.ticket_lines");
		$ticketlinesRecords = $usernode->selectDb();
		foreach($ticketlinesRecords AS $key=>&$value){
			$value["ticket_number"] = $value["ticket_id"]["ticket_number"];
			$value["ticket_id"] = $value["ticket_id"]["id"];
		}


		$allRecords = array();
		$allRecords[] = $productRecords;
		$allRecords[] = $customerRecords;
		$allRecords[] = $financeRecords;
		$allRecords[] = $priceRecords;
		$allRecords[] = $stockRecords;
		$allRecords[] = $ticketRecords;
		$allRecords[] = $ticketlinesRecords;

		$output .= "\n--- ".atktext("products")." ---\n\n";
		$productRecords_column["name"] = atktext("product");
		$productRecords_column["unit_id"] = atktext("unit_id");
		$productRecords_column["tax_id"] = atktext("tax_id");
		$productRecords_column["pricesell"] = atktext("pricesell")." (".atktext("doctype_main").")";
		$productRecords_column["pricebuy"] = atktext("pricebuy")." (".atktext("doctype_main").")";
		$productRecords_column["category_id"] = atktext("category_id");
		$productRecords_column["track_stock"] = atktext("track_stock");
		$productRecords_column["hidden"] = atktext("hidden");
		$productRecords_column["barcode"] = atktext("barcode");

		$output .= $this->buildCSV($productRecords_column, $productRecords);

		$output .= "\n\n\n--- ".atktext("price_list")." ---\n\n";
		$priceRecords_column["pricelist_name"] = atktext("pricelist_id");
		$priceRecords_column["product_name"] = atktext("product_name");
		$priceRecords_column["pricesell"] = atktext("pricesell");
		$priceRecords_column["pricebuy"] = atktext("pricebuy");

		$output .= $this->buildCSV($priceRecords_column, $priceRecords);

		$output .= "\n\n\n--- ".atktext("ticket_stock")." ---\n\n";
		$stockRecords_column["location_id"] = atktext("location_id");
		$stockRecords_column["product_id"] = atktext("product_id");
		$stockRecords_column["amount"] = atktext("amount");
		$stockRecords_column["reservation"] = atktext("reservation");
		$stockRecords_column["on_order"] = atktext("on_order");
		$stockRecords_column["min_stock"] = atktext("min_stock");
		$stockRecords_column["max_stock"] = atktext("max_stock");

		$output .= $this->buildCSV($stockRecords_column, $stockRecords);

		$output .= "\n\n\n--- ".atktext("customers")." ---\n\n";
		$customerRecords_column["name"] = atktext("customers");
		$customerRecords_column["taxid"] = atktext("taxid");
		$customerRecords_column["type_id"] = atktext("customer_type");
		$customerRecords_column["discount"] = atktext("discount");
		$customerRecords_column["finance_sum"] = atktext("finance_sum");
		$customerRecords_column["credit_limit"] = atktext("credit_limit");
		$customerRecords_column["credit_status_id"] = atktext("credit_status_id");
		$customerRecords_column["city"] = atktext("city");
		$customerRecords_column["zip"] = atktext("zip");
		$customerRecords_column["state"] = atktext("state");
		$customerRecords_column["address"] = atktext("address");
		$customerRecords_column["telephone"] = atktext("telephone");
		$customerRecords_column["email"] = atktext("email");
		$customerRecords_column["notes"] = atktext("notes");

		$output .= $this->buildCSV($customerRecords_column, $customerRecords);

		$output .= "\n\n\n--- ".atktext("finance")." ---\n\n";
		$financeRecords_column["type_id"] = atktext("type_id");
		$financeRecords_column["date"] = atktext("date");
		$financeRecords_column["amount"] = atktext("money_amount");
		$financeRecords_column["ticket_id"] = atktext("document_link");
		$financeRecords_column["customer_id"] = atktext("customer");
		$financeRecords_column["notes"] = atktext("notes");

		$output .= $this->buildCSV($financeRecords_column, $financeRecords);

		$output .= "\n\n\n--- ".atktext("report_tickets")." ---\n\n";
		$ticketRecords_column["type_id"] = atktext("document_type");
		$ticketRecords_column["ticket_number"] = atktext("ticket_number");
		$ticketRecords_column["status_id"] = atktext("ticket_status");		
		$ticketRecords_column["customer_id"] = atktext("customer");
		$ticketRecords_column["date_created"] = atktext("date_created");
		$ticketRecords_column["ticket_sum"] = atktext("grand_total");
		$ticketRecords_column["ticket_payment"] = atktext("ticket_payment");
		$ticketRecords_column["notes"] = atktext("notes");


		$output .= $this->buildCSV($ticketRecords_column, $ticketRecords);
		
		$output .= "\n\n\n--- ".atktext("ticket")." / ".atktext("products")." ---\n\n";
		$ticketlinesRecords_column["ticket_number"] = atktext("ticket_number");
		$ticketlinesRecords_column["line"] = atktext("line_number");		
		$ticketlinesRecords_column["fix_productname"] = atktext("product_id");
		$ticketlinesRecords_column["units"] = atktext("units");
		$ticketlinesRecords_column["fix_unitsymbol"] = atktext("unit_id");
		$ticketlinesRecords_column["fix_price"] = atktext("pricesell");
		$ticketlinesRecords_column["fix_pricebuy"] = atktext("pricebuy");		
		$ticketlinesRecords_column["fix_discount"] = atktext("discount");
		$ticketlinesRecords_column["fix_taxname"] = atktext("tax_rate");

		$output .= $this->buildCSV($ticketlinesRecords_column, $ticketlinesRecords);		

		$filename = "export";
		// @param String $type The type (csv / excel / xml)
		$type = "csv";
		$ext = "csv";

		$exporter = &atknew("atk.utils.atkfileexport");
		$exporter->export($output, $filename, $type, $ext, $compression);
			
		die;

	}




}

?>
