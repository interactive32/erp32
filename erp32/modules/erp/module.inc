<?php

class mod_erp extends atkModule
{

	// menu Encode in utf-8 i
	function getMenuItems()
	{
		
		if (atkGetUser('name')!='administrator')
		{

			$sess = array('atklevel'=>'0');

			$this->menuitem(atktext('maindata'),"","",array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('products'), dispatch_url("products.product", "admin", $sess), atktext('maindata'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('add_product'), dispatch_url("products.product", "add", $sess), atktext('maindata'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('product_categories'), dispatch_url("products.category", "admin", $sess), atktext('maindata'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem("-", "", atktext('maindata'));
			$this->menuitem(atktext('customers'), dispatch_url("customers.customer", "admin", $sess), atktext('maindata'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('add_customer'), dispatch_url("customers.customer", "add", $sess), atktext('maindata'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem("-", "", atktext('maindata'));
			$this->menuitem(atktext('prices'), dispatch_url("pricelist.price", "admin", $sess), atktext('maindata'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('pricelist'), dispatch_url("pricelist.price_list", "admin", $sess), atktext('prices'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('batch_copy_menu'), dispatch_url("pricelist.batch_copy", "add", $sess), atktext('prices'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('batch_update_menu'), dispatch_url("pricelist.batch_update", "add", $sess), atktext('prices'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('location_config'), dispatch_url("locations.location", "admin", $sess), atktext('maindata'),array("loginmanager.erp_menu_admin", "show"));

			$this->menuitem(atktext('documents_out'),"","",array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('documents_out_list'), dispatch_url("tickets.ticket_out", "admin", $sess), atktext('documents_out'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('documents_new_quote'), dispatch_url("tickets.ticket_quote", "add", $sess), atktext('documents_out'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('documents_new_invoice'), dispatch_url("tickets.ticket_invoice", "add", $sess), atktext('documents_out'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('documents_new_creditnote'), dispatch_url("tickets.ticket_creditnote", "add", $sess), atktext('documents_out'),array("loginmanager.erp_menu_admin", "show"));

			$this->menuitem(atktext('documents_in'),"","",array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('documents_in_list'), dispatch_url("tickets.ticket_in", "admin", $sess), atktext('documents_in'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('documents_new_purchaseorder'), dispatch_url("tickets.ticket_purchaseorder", "add", $sess), atktext('documents_in'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('documents_new_receipt'), dispatch_url("tickets.ticket_receipt", "add", $sess), atktext('documents_in'),array("loginmanager.erp_menu_admin", "show"));

			$this->menuitem(atktext('stock'),"","",array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('stockcurrent'), dispatch_url("stocklist.stockcurrent", "admin", $sess), atktext('stock'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem("-", "", atktext('stock'));
			$this->menuitem(atktext('documents_new_inventory'), dispatch_url("tickets.ticket_inventory", "add", $sess), atktext('stock'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('documents_stock_list'), dispatch_url("tickets.ticket_stock", "admin", $sess), atktext('stock'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem("-", "", atktext('stock'));
			$this->menuitem(atktext('documents_new_internal_transfer'), dispatch_url("tickets.ticket_internal_transfer", "add", $sess), atktext('stock'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('documents_internal_transfer_list'), dispatch_url("tickets.ticket_transfer", "admin", $sess), atktext('stock'),array("loginmanager.erp_menu_admin", "show"));


			$this->menuitem(atktext('finance'),"","",array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('finance_all'), dispatch_url("finance.finance_all", "admin", $sess), atktext('finance'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem("-", "", atktext('finance'));
			$this->menuitem(atktext('payment_from_customer'), dispatch_url("finance.payment_from_customer", "add", $sess), atktext('finance'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('payment_to_supplier'), dispatch_url("finance.payment_to_supplier", "add", $sess), atktext('finance'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem("-", "", atktext('finance'));
			$this->menuitem(atktext('payment_free'), dispatch_url("finance.payment_free", "add", $sess), atktext('finance'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('charge_free'), dispatch_url("finance.charge_free", "add", $sess), atktext('finance'),array("loginmanager.erp_menu_admin", "show"));

			$this->menuitem(atktext('reports'),"","",array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_tickets'), dispatch_url("reports.report_tickets", "add", $sess), atktext('reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_products'), "", atktext('reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_product_list'), dispatch_url("reports.report_product_list", "add", $sess), atktext('report_products'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_product_card'), dispatch_url("reports.report_product_card", "add", $sess), atktext('report_products'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_customers'), "", atktext('reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_customer_list'), dispatch_url("reports.report_customer_list", "add", $sess), atktext('report_customers'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_customer_card'), dispatch_url("reports.report_customer_card", "add", $sess), atktext('report_customers'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_pricelist'), dispatch_url("reports.report_pricelist", "add", $sess), atktext('reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_product_stockinfo'), dispatch_url("reports.report_product_stockinfo", "add", $sess), atktext('reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('sell_reports'), "", atktext('reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_product_sell'), dispatch_url("reports.report_product_sell", "add", $sess), atktext('sell_reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('buy_reports'), "", atktext('reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_product_buy'), dispatch_url("reports.report_product_buy", "add", $sess), atktext('buy_reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('finance_reports'), "", atktext('reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_finance_list'), dispatch_url("reports.report_finance_list", "add", $sess), atktext('finance_reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_customer_credits'), dispatch_url("reports.report_customer_credits", "add", $sess), atktext('finance_reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('utility_reports'), "", atktext('reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_inventory_list'), dispatch_url("reports.report_inventory_list", "add", $sess), atktext('utility_reports'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('report_memo'), dispatch_url("reports.report_memo", "add", $sess), atktext('utility_reports'),array("loginmanager.erp_menu_admin", "show"));

			$this->menuitem(atktext('config'),"","",array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('main_config'), dispatch_url("config.main_config", "edit", $sess), atktext('config'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('customlang'), dispatch_url("loginmanager.customlang", "edit", $sess), atktext('config'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('taxes'), dispatch_url("config.taxes", "admin", $sess), atktext('config'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('units_config'), dispatch_url("config.units_config", "admin", $sess), atktext('config'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('currency_config'), dispatch_url("config.currency", "admin", $sess), atktext('config'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('text_templates'), dispatch_url("config.text_templates", "admin", $sess), atktext('config'),array("loginmanager.erp_menu_admin", "show"));
			$this->menuitem(atktext('export_data'), dispatch_url("config.export_data", "add", $sess), atktext('config'),array("loginmanager.erp_menu_admin", "show"));
				
			
			$this->menuitem(atktext('help'),atkconfig("atkroot")."../documentation/users_manual");
		}

	}

}

?>