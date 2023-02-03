<?php


class mod_reports extends atkModule
{

	function getNodes()
	{

		registerNode("reports.report_tickets", array("add", "pdfprint"));
		registerNode("reports.report_product_list", array("add", "pdfprint"));
		registerNode("reports.report_product_stockinfo", array("add", "pdfprint"));
		registerNode("reports.report_product_card", array("add", "pdfprint"));
		registerNode("reports.report_customer_list", array("add", "pdfprint"));
		registerNode("reports.report_customer_credits", array("add", "pdfprint"));
		registerNode("reports.report_pricelist", array("add", "pdfprint"));
		registerNode("reports.report_customer_card", array("add", "pdfprint"));
		registerNode("reports.report_product_sell", array("add", "pdfprint"));
		registerNode("reports.report_product_buy", array("add", "pdfprint"));
		registerNode("reports.report_inventory_list", array("add", "pdfprint"));
		registerNode("reports.report_memo", array("add", "pdfprint"));
		registerNode("reports.report_finance_list", array("add", "pdfprint"));
	
	}

}

