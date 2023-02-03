-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.5.16 - MySQL Community Server (GPL)
-- Server OS:                    Win32
-- HeidiSQL version:             7.0.0.4123
-- Date/time:                    2012-10-05 10:45:03
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table erp32_database.admin_accessrights
DROP TABLE IF EXISTS `admin_accessrights`;
CREATE TABLE IF NOT EXISTS `admin_accessrights` (
  `node` varchar(255) NOT NULL DEFAULT '',
  `action` varchar(25) NOT NULL DEFAULT '',
  `group_id` varchar(10) NOT NULL DEFAULT '',
  PRIMARY KEY (`node`,`action`,`group_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Dumping data for table erp32_database.admin_accessrights: 532 rows
DELETE FROM `admin_accessrights`;
/*!40000 ALTER TABLE `admin_accessrights` DISABLE KEYS */;
INSERT INTO `admin_accessrights` (`node`, `action`, `group_id`) VALUES
	('config.contact', 'add', '1'),
	('config.contact', 'add', '3'),
	('config.contact', 'add', '4'),
	('config.contact', 'add', '6'),
	('config.contact', 'add', '7'),
	('config.currency', 'add', '1'),
	('config.currency', 'add', '3'),
	('config.currency', 'add', '4'),
	('config.currency', 'add', '6'),
	('config.currency', 'admin', '1'),
	('config.currency', 'admin', '3'),
	('config.currency', 'admin', '4'),
	('config.currency', 'admin', '6'),
	('config.currency', 'admin', '7'),
	('config.currency', 'delete', '1'),
	('config.currency', 'delete', '3'),
	('config.currency', 'delete', '4'),
	('config.currency', 'delete', '6'),
	('config.currency', 'edit', '1'),
	('config.currency', 'edit', '3'),
	('config.currency', 'edit', '4'),
	('config.currency', 'edit', '6'),
	('config.export_data', 'add', '1'),
	('config.export_data', 'add', '3'),
	('config.export_data', 'add', '4'),
	('config.export_data', 'add', '6'),
	('config.export_data', 'download', '1'),
	('config.export_data', 'download', '3'),
	('config.export_data', 'download', '4'),
	('config.export_data', 'download', '6'),
	('config.main_config', 'edit', '1'),
	('config.main_config', 'edit', '3'),
	('config.main_config', 'edit', '7'),
	('config.taxes', 'add', '1'),
	('config.taxes', 'add', '3'),
	('config.taxes', 'add', '4'),
	('config.taxes', 'add', '6'),
	('config.taxes', 'admin', '1'),
	('config.taxes', 'admin', '3'),
	('config.taxes', 'admin', '4'),
	('config.taxes', 'admin', '6'),
	('config.taxes', 'admin', '7'),
	('config.taxes', 'delete', '1'),
	('config.taxes', 'delete', '3'),
	('config.taxes', 'delete', '4'),
	('config.taxes', 'delete', '6'),
	('config.taxes', 'edit', '1'),
	('config.taxes', 'edit', '3'),
	('config.taxes', 'edit', '4'),
	('config.taxes', 'edit', '6'),
	('config.text_templates', 'add', '1'),
	('config.text_templates', 'add', '3'),
	('config.text_templates', 'add', '4'),
	('config.text_templates', 'add', '6'),
	('config.text_templates', 'admin', '1'),
	('config.text_templates', 'admin', '3'),
	('config.text_templates', 'admin', '4'),
	('config.text_templates', 'admin', '6'),
	('config.text_templates', 'admin', '7'),
	('config.text_templates', 'delete', '1'),
	('config.text_templates', 'delete', '3'),
	('config.text_templates', 'delete', '4'),
	('config.text_templates', 'delete', '6'),
	('config.text_templates', 'edit', '1'),
	('config.text_templates', 'edit', '3'),
	('config.text_templates', 'edit', '4'),
	('config.text_templates', 'edit', '6'),
	('config.units_config', 'add', '1'),
	('config.units_config', 'add', '3'),
	('config.units_config', 'add', '4'),
	('config.units_config', 'add', '6'),
	('config.units_config', 'admin', '1'),
	('config.units_config', 'admin', '3'),
	('config.units_config', 'admin', '4'),
	('config.units_config', 'admin', '6'),
	('config.units_config', 'admin', '7'),
	('config.units_config', 'delete', '1'),
	('config.units_config', 'delete', '3'),
	('config.units_config', 'delete', '4'),
	('config.units_config', 'delete', '6'),
	('config.units_config', 'edit', '1'),
	('config.units_config', 'edit', '3'),
	('config.units_config', 'edit', '4'),
	('config.units_config', 'edit', '6'),
	('customers.customer', 'add', '1'),
	('customers.customer', 'add', '3'),
	('customers.customer', 'add', '4'),
	('customers.customer', 'add', '6'),
	('customers.customer', 'admin', '1'),
	('customers.customer', 'admin', '3'),
	('customers.customer', 'admin', '4'),
	('customers.customer', 'admin', '6'),
	('customers.customer', 'admin', '7'),
	('customers.customer', 'delete', '1'),
	('customers.customer', 'delete', '3'),
	('customers.customer', 'delete', '4'),
	('customers.customer', 'delete', '6'),
	('customers.customer', 'edit', '1'),
	('customers.customer', 'edit', '3'),
	('customers.customer', 'edit', '4'),
	('customers.customer', 'edit', '6'),
	('dashboard.mainboard', 'start', '1'),
	('dashboard.mainboard', 'start', '110-e5c12819dc'),
	('dashboard.mainboard', 'start', '118-0c1e7a1765'),
	('dashboard.mainboard', 'start', '2'),
	('dashboard.mainboard', 'start', '3'),
	('dashboard.mainboard', 'start', '4'),
	('dashboard.mainboard', 'start', '6'),
	('dashboard.mainboard', 'start', '7'),
	('finance.charge_free', 'add', '1'),
	('finance.charge_free', 'add', '3'),
	('finance.charge_free', 'add', '4'),
	('finance.charge_free', 'add', '6'),
	('finance.finance_all', 'admin', '1'),
	('finance.finance_all', 'admin', '3'),
	('finance.finance_all', 'admin', '4'),
	('finance.finance_all', 'admin', '6'),
	('finance.finance_all', 'admin', '7'),
	('finance.payment_free', 'add', '1'),
	('finance.payment_free', 'add', '3'),
	('finance.payment_free', 'add', '4'),
	('finance.payment_free', 'add', '6'),
	('finance.payment_from_customer', 'add', '1'),
	('finance.payment_from_customer', 'add', '3'),
	('finance.payment_from_customer', 'add', '4'),
	('finance.payment_from_customer', 'add', '6'),
	('finance.payment_to_supplier', 'add', '1'),
	('finance.payment_to_supplier', 'add', '3'),
	('finance.payment_to_supplier', 'add', '4'),
	('finance.payment_to_supplier', 'add', '6'),
	('locations.location', 'add', '1'),
	('locations.location', 'add', '3'),
	('locations.location', 'admin', '1'),
	('locations.location', 'admin', '3'),
	('locations.location', 'admin', '4'),
	('locations.location', 'admin', '6'),
	('locations.location', 'admin', '7'),
	('locations.location', 'change', '1'),
	('locations.location', 'change', '3'),
	('locations.location', 'change', '6'),
	('locations.location', 'change', '7'),
	('locations.location', 'delete', '1'),
	('locations.location', 'delete', '3'),
	('locations.location', 'edit', '1'),
	('locations.location', 'edit', '3'),
	('locations.location', 'edit', '4'),
	('loginmanager.customlang', 'edit', '1'),
	('loginmanager.customlang', 'edit', '3'),
	('loginmanager.customlang', 'edit', '4'),
	('loginmanager.customlang', 'edit', '6'),
	('loginmanager.erp_menu_admin', 'show', '1'),
	('loginmanager.erp_menu_admin', 'show', '110-e5c12819dc'),
	('loginmanager.erp_menu_admin', 'show', '118-0c1e7a1765'),
	('loginmanager.erp_menu_admin', 'show', '3'),
	('loginmanager.erp_menu_admin', 'show', '4'),
	('loginmanager.erp_menu_admin', 'show', '6'),
	('loginmanager.erp_menu_admin', 'show', '7'),
	('loginmanager.referrals', 'admin', '2'),
	('loginmanager.referrals', 'view', '2'),
	('loginmanager.settings', 'edit', '1'),
	('loginmanager.settings', 'edit', '110-e5c12819dc'),
	('loginmanager.settings', 'edit', '118-0c1e7a1765'),
	('loginmanager.settings', 'edit', '2'),
	('loginmanager.settings', 'edit', '3'),
	('loginmanager.settings', 'edit', '6'),
	('pricelist.batch_copy', 'add', '1'),
	('pricelist.batch_copy', 'add', '3'),
	('pricelist.batch_copy', 'add', '4'),
	('pricelist.batch_copy', 'add', '6'),
	('pricelist.batch_copy', 'copyPricelist', '1'),
	('pricelist.batch_copy', 'copyPricelist', '3'),
	('pricelist.batch_copy', 'copyPricelist', '4'),
	('pricelist.batch_copy', 'copyPricelist', '6'),
	('pricelist.batch_update', 'add', '1'),
	('pricelist.batch_update', 'add', '3'),
	('pricelist.batch_update', 'add', '4'),
	('pricelist.batch_update', 'add', '6'),
	('pricelist.batch_update', 'updatePricelist', '1'),
	('pricelist.batch_update', 'updatePricelist', '3'),
	('pricelist.batch_update', 'updatePricelist', '4'),
	('pricelist.batch_update', 'updatePricelist', '6'),
	('pricelist.price', 'add', '1'),
	('pricelist.price', 'add', '3'),
	('pricelist.price', 'add', '4'),
	('pricelist.price', 'add', '6'),
	('pricelist.price', 'admin', '1'),
	('pricelist.price', 'admin', '3'),
	('pricelist.price', 'admin', '4'),
	('pricelist.price', 'admin', '6'),
	('pricelist.price', 'admin', '7'),
	('pricelist.price', 'edit', '1'),
	('pricelist.price', 'edit', '3'),
	('pricelist.price', 'edit', '4'),
	('pricelist.price', 'edit', '6'),
	('pricelist.price_list', 'add', '1'),
	('pricelist.price_list', 'add', '3'),
	('pricelist.price_list', 'add', '4'),
	('pricelist.price_list', 'add', '6'),
	('pricelist.price_list', 'admin', '1'),
	('pricelist.price_list', 'admin', '3'),
	('pricelist.price_list', 'admin', '4'),
	('pricelist.price_list', 'admin', '6'),
	('pricelist.price_list', 'admin', '7'),
	('pricelist.price_list', 'delete', '1'),
	('pricelist.price_list', 'delete', '3'),
	('pricelist.price_list', 'delete', '4'),
	('pricelist.price_list', 'delete', '6'),
	('pricelist.price_list', 'edit', '1'),
	('pricelist.price_list', 'edit', '3'),
	('pricelist.price_list', 'edit', '4'),
	('pricelist.price_list', 'edit', '6'),
	('products.category', 'add', '1'),
	('products.category', 'add', '3'),
	('products.category', 'add', '4'),
	('products.category', 'add', '6'),
	('products.category', 'admin', '1'),
	('products.category', 'admin', '3'),
	('products.category', 'admin', '4'),
	('products.category', 'admin', '6'),
	('products.category', 'admin', '7'),
	('products.category', 'delete', '1'),
	('products.category', 'delete', '3'),
	('products.category', 'delete', '4'),
	('products.category', 'delete', '6'),
	('products.category', 'edit', '1'),
	('products.category', 'edit', '3'),
	('products.category', 'edit', '4'),
	('products.category', 'edit', '6'),
	('products.product', 'add', '1'),
	('products.product', 'add', '3'),
	('products.product', 'add', '4'),
	('products.product', 'add', '6'),
	('products.product', 'admin', '1'),
	('products.product', 'admin', '3'),
	('products.product', 'admin', '4'),
	('products.product', 'admin', '6'),
	('products.product', 'admin', '7'),
	('products.product', 'delete', '1'),
	('products.product', 'delete', '3'),
	('products.product', 'delete', '4'),
	('products.product', 'delete', '6'),
	('products.product', 'edit', '1'),
	('products.product', 'edit', '3'),
	('products.product', 'edit', '4'),
	('products.product', 'edit', '6'),
	('products.products_mat', 'add', '1'),
	('products.products_mat', 'add', '3'),
	('products.products_mat', 'add', '4'),
	('products.products_mat', 'add', '6'),
	('products.products_mat', 'admin', '1'),
	('products.products_mat', 'admin', '3'),
	('products.products_mat', 'admin', '4'),
	('products.products_mat', 'admin', '6'),
	('products.products_mat', 'admin', '7'),
	('products.products_mat', 'delete', '1'),
	('products.products_mat', 'delete', '3'),
	('products.products_mat', 'delete', '4'),
	('products.products_mat', 'delete', '6'),
	('products.products_mat', 'edit', '1'),
	('products.products_mat', 'edit', '3'),
	('products.products_mat', 'edit', '4'),
	('products.products_mat', 'edit', '6'),
	('reports.report_customer_card', 'add', '1'),
	('reports.report_customer_card', 'add', '3'),
	('reports.report_customer_card', 'add', '4'),
	('reports.report_customer_card', 'add', '6'),
	('reports.report_customer_card', 'add', '7'),
	('reports.report_customer_card', 'pdfprint', '1'),
	('reports.report_customer_card', 'pdfprint', '3'),
	('reports.report_customer_card', 'pdfprint', '4'),
	('reports.report_customer_card', 'pdfprint', '6'),
	('reports.report_customer_card', 'pdfprint', '7'),
	('reports.report_customer_credits', 'add', '1'),
	('reports.report_customer_credits', 'add', '3'),
	('reports.report_customer_credits', 'add', '4'),
	('reports.report_customer_credits', 'add', '6'),
	('reports.report_customer_credits', 'add', '7'),
	('reports.report_customer_credits', 'pdfprint', '1'),
	('reports.report_customer_credits', 'pdfprint', '3'),
	('reports.report_customer_credits', 'pdfprint', '4'),
	('reports.report_customer_credits', 'pdfprint', '6'),
	('reports.report_customer_credits', 'pdfprint', '7'),
	('reports.report_customer_list', 'add', '1'),
	('reports.report_customer_list', 'add', '3'),
	('reports.report_customer_list', 'add', '4'),
	('reports.report_customer_list', 'add', '6'),
	('reports.report_customer_list', 'add', '7'),
	('reports.report_customer_list', 'pdfprint', '1'),
	('reports.report_customer_list', 'pdfprint', '3'),
	('reports.report_customer_list', 'pdfprint', '4'),
	('reports.report_customer_list', 'pdfprint', '6'),
	('reports.report_customer_list', 'pdfprint', '7'),
	('reports.report_finance_list', 'add', '1'),
	('reports.report_finance_list', 'add', '3'),
	('reports.report_finance_list', 'add', '4'),
	('reports.report_finance_list', 'add', '6'),
	('reports.report_finance_list', 'add', '7'),
	('reports.report_finance_list', 'pdfprint', '1'),
	('reports.report_finance_list', 'pdfprint', '3'),
	('reports.report_finance_list', 'pdfprint', '4'),
	('reports.report_finance_list', 'pdfprint', '6'),
	('reports.report_finance_list', 'pdfprint', '7'),
	('reports.report_inventory_list', 'add', '1'),
	('reports.report_inventory_list', 'add', '3'),
	('reports.report_inventory_list', 'add', '4'),
	('reports.report_inventory_list', 'add', '6'),
	('reports.report_inventory_list', 'add', '7'),
	('reports.report_inventory_list', 'pdfprint', '1'),
	('reports.report_inventory_list', 'pdfprint', '3'),
	('reports.report_inventory_list', 'pdfprint', '4'),
	('reports.report_inventory_list', 'pdfprint', '6'),
	('reports.report_inventory_list', 'pdfprint', '7'),
	('reports.report_memo', 'add', '1'),
	('reports.report_memo', 'add', '3'),
	('reports.report_memo', 'add', '4'),
	('reports.report_memo', 'add', '6'),
	('reports.report_memo', 'add', '7'),
	('reports.report_memo', 'pdfprint', '1'),
	('reports.report_memo', 'pdfprint', '3'),
	('reports.report_memo', 'pdfprint', '4'),
	('reports.report_memo', 'pdfprint', '6'),
	('reports.report_memo', 'pdfprint', '7'),
	('reports.report_pricelist', 'add', '1'),
	('reports.report_pricelist', 'add', '3'),
	('reports.report_pricelist', 'add', '4'),
	('reports.report_pricelist', 'add', '6'),
	('reports.report_pricelist', 'add', '7'),
	('reports.report_pricelist', 'pdfprint', '1'),
	('reports.report_pricelist', 'pdfprint', '3'),
	('reports.report_pricelist', 'pdfprint', '4'),
	('reports.report_pricelist', 'pdfprint', '6'),
	('reports.report_pricelist', 'pdfprint', '7'),
	('reports.report_product_buy', 'add', '1'),
	('reports.report_product_buy', 'add', '3'),
	('reports.report_product_buy', 'add', '4'),
	('reports.report_product_buy', 'add', '6'),
	('reports.report_product_buy', 'add', '7'),
	('reports.report_product_buy', 'pdfprint', '1'),
	('reports.report_product_buy', 'pdfprint', '3'),
	('reports.report_product_buy', 'pdfprint', '4'),
	('reports.report_product_buy', 'pdfprint', '6'),
	('reports.report_product_buy', 'pdfprint', '7'),
	('reports.report_product_card', 'add', '1'),
	('reports.report_product_card', 'add', '3'),
	('reports.report_product_card', 'add', '4'),
	('reports.report_product_card', 'add', '6'),
	('reports.report_product_card', 'add', '7'),
	('reports.report_product_card', 'pdfprint', '1'),
	('reports.report_product_card', 'pdfprint', '3'),
	('reports.report_product_card', 'pdfprint', '4'),
	('reports.report_product_card', 'pdfprint', '6'),
	('reports.report_product_card', 'pdfprint', '7'),
	('reports.report_product_list', 'add', '1'),
	('reports.report_product_list', 'add', '3'),
	('reports.report_product_list', 'add', '4'),
	('reports.report_product_list', 'add', '6'),
	('reports.report_product_list', 'add', '7'),
	('reports.report_product_list', 'pdfprint', '1'),
	('reports.report_product_list', 'pdfprint', '3'),
	('reports.report_product_list', 'pdfprint', '4'),
	('reports.report_product_list', 'pdfprint', '6'),
	('reports.report_product_list', 'pdfprint', '7'),
	('reports.report_product_sell', 'add', '1'),
	('reports.report_product_sell', 'add', '3'),
	('reports.report_product_sell', 'add', '4'),
	('reports.report_product_sell', 'add', '6'),
	('reports.report_product_sell', 'add', '7'),
	('reports.report_product_sell', 'pdfprint', '1'),
	('reports.report_product_sell', 'pdfprint', '3'),
	('reports.report_product_sell', 'pdfprint', '4'),
	('reports.report_product_sell', 'pdfprint', '6'),
	('reports.report_product_sell', 'pdfprint', '7'),
	('reports.report_product_stockinfo', 'add', '1'),
	('reports.report_product_stockinfo', 'add', '3'),
	('reports.report_product_stockinfo', 'add', '4'),
	('reports.report_product_stockinfo', 'add', '6'),
	('reports.report_product_stockinfo', 'add', '7'),
	('reports.report_product_stockinfo', 'pdfprint', '1'),
	('reports.report_product_stockinfo', 'pdfprint', '3'),
	('reports.report_product_stockinfo', 'pdfprint', '4'),
	('reports.report_product_stockinfo', 'pdfprint', '6'),
	('reports.report_product_stockinfo', 'pdfprint', '7'),
	('reports.report_tickets', 'add', '1'),
	('reports.report_tickets', 'add', '3'),
	('reports.report_tickets', 'add', '4'),
	('reports.report_tickets', 'add', '6'),
	('reports.report_tickets', 'add', '7'),
	('reports.report_tickets', 'pdfprint', '1'),
	('reports.report_tickets', 'pdfprint', '3'),
	('reports.report_tickets', 'pdfprint', '4'),
	('reports.report_tickets', 'pdfprint', '6'),
	('reports.report_tickets', 'pdfprint', '7'),
	('stocklist.stockcurrent', 'admin', '1'),
	('stocklist.stockcurrent', 'admin', '3'),
	('stocklist.stockcurrent', 'admin', '4'),
	('stocklist.stockcurrent', 'admin', '6'),
	('stocklist.stockcurrent', 'admin', '7'),
	('stocklist.stockcurrent', 'edit', '1'),
	('stocklist.stockcurrent', 'edit', '3'),
	('stocklist.stockcurrent', 'edit', '4'),
	('stocklist.stockcurrent', 'edit', '6'),
	('tickets.ticket_all', 'admin', '1'),
	('tickets.ticket_all', 'admin', '3'),
	('tickets.ticket_all', 'admin', '4'),
	('tickets.ticket_all', 'admin', '6'),
	('tickets.ticket_all', 'admin', '7'),
	('tickets.ticket_creditnote', 'add', '1'),
	('tickets.ticket_creditnote', 'add', '3'),
	('tickets.ticket_creditnote', 'add', '4'),
	('tickets.ticket_creditnote', 'add', '6'),
	('tickets.ticket_creditnote', 'view', '1'),
	('tickets.ticket_creditnote', 'view', '3'),
	('tickets.ticket_creditnote', 'view', '4'),
	('tickets.ticket_creditnote', 'view', '6'),
	('tickets.ticket_creditnote', 'view', '7'),
	('tickets.ticket_in', 'admin', '1'),
	('tickets.ticket_in', 'admin', '3'),
	('tickets.ticket_in', 'admin', '4'),
	('tickets.ticket_in', 'admin', '6'),
	('tickets.ticket_in', 'admin', '7'),
	('tickets.ticket_in', 'delete', '1'),
	('tickets.ticket_in', 'delete', '3'),
	('tickets.ticket_in', 'delete', '4'),
	('tickets.ticket_in', 'delete', '6'),
	('tickets.ticket_internal_transfer', 'add', '1'),
	('tickets.ticket_internal_transfer', 'add', '3'),
	('tickets.ticket_internal_transfer', 'add', '4'),
	('tickets.ticket_internal_transfer', 'add', '6'),
	('tickets.ticket_internal_transfer', 'close_document', '1'),
	('tickets.ticket_internal_transfer', 'close_document', '3'),
	('tickets.ticket_internal_transfer', 'close_document', '4'),
	('tickets.ticket_internal_transfer', 'close_document', '6'),
	('tickets.ticket_internal_transfer', 'view', '1'),
	('tickets.ticket_internal_transfer', 'view', '3'),
	('tickets.ticket_internal_transfer', 'view', '4'),
	('tickets.ticket_internal_transfer', 'view', '6'),
	('tickets.ticket_internal_transfer', 'view', '7'),
	('tickets.ticket_inventory', 'add', '1'),
	('tickets.ticket_inventory', 'add', '3'),
	('tickets.ticket_inventory', 'add', '4'),
	('tickets.ticket_inventory', 'add', '6'),
	('tickets.ticket_inventory', 'view', '1'),
	('tickets.ticket_inventory', 'view', '3'),
	('tickets.ticket_inventory', 'view', '4'),
	('tickets.ticket_inventory', 'view', '6'),
	('tickets.ticket_inventory', 'view', '7'),
	('tickets.ticket_invoice', 'add', '1'),
	('tickets.ticket_invoice', 'add', '3'),
	('tickets.ticket_invoice', 'add', '4'),
	('tickets.ticket_invoice', 'add', '6'),
	('tickets.ticket_invoice', 'return_invoice', '1'),
	('tickets.ticket_invoice', 'return_invoice', '3'),
	('tickets.ticket_invoice', 'return_invoice', '4'),
	('tickets.ticket_invoice', 'return_invoice', '6'),
	('tickets.ticket_invoice', 'view', '1'),
	('tickets.ticket_invoice', 'view', '3'),
	('tickets.ticket_invoice', 'view', '4'),
	('tickets.ticket_invoice', 'view', '6'),
	('tickets.ticket_invoice', 'view', '7'),
	('tickets.ticket_out', 'admin', '1'),
	('tickets.ticket_out', 'admin', '3'),
	('tickets.ticket_out', 'admin', '4'),
	('tickets.ticket_out', 'admin', '6'),
	('tickets.ticket_out', 'admin', '7'),
	('tickets.ticket_out', 'delete', '1'),
	('tickets.ticket_out', 'delete', '3'),
	('tickets.ticket_out', 'delete', '4'),
	('tickets.ticket_out', 'delete', '6'),
	('tickets.ticket_purchaseorder', 'add', '1'),
	('tickets.ticket_purchaseorder', 'add', '3'),
	('tickets.ticket_purchaseorder', 'add', '4'),
	('tickets.ticket_purchaseorder', 'add', '6'),
	('tickets.ticket_purchaseorder', 'create_receipt', '1'),
	('tickets.ticket_purchaseorder', 'create_receipt', '3'),
	('tickets.ticket_purchaseorder', 'create_receipt', '4'),
	('tickets.ticket_purchaseorder', 'create_receipt', '6'),
	('tickets.ticket_purchaseorder', 'delete', '1'),
	('tickets.ticket_purchaseorder', 'delete', '3'),
	('tickets.ticket_purchaseorder', 'delete', '4'),
	('tickets.ticket_purchaseorder', 'delete', '6'),
	('tickets.ticket_purchaseorder', 'edit', '1'),
	('tickets.ticket_purchaseorder', 'edit', '3'),
	('tickets.ticket_purchaseorder', 'edit', '4'),
	('tickets.ticket_purchaseorder', 'edit', '6'),
	('tickets.ticket_purchaseorder', 'view', '1'),
	('tickets.ticket_purchaseorder', 'view', '3'),
	('tickets.ticket_purchaseorder', 'view', '4'),
	('tickets.ticket_purchaseorder', 'view', '6'),
	('tickets.ticket_purchaseorder', 'view', '7'),
	('tickets.ticket_quote', 'add', '1'),
	('tickets.ticket_quote', 'add', '3'),
	('tickets.ticket_quote', 'add', '4'),
	('tickets.ticket_quote', 'add', '6'),
	('tickets.ticket_quote', 'create_invoice', '1'),
	('tickets.ticket_quote', 'create_invoice', '3'),
	('tickets.ticket_quote', 'create_invoice', '4'),
	('tickets.ticket_quote', 'create_invoice', '6'),
	('tickets.ticket_quote', 'delete', '1'),
	('tickets.ticket_quote', 'delete', '3'),
	('tickets.ticket_quote', 'delete', '4'),
	('tickets.ticket_quote', 'delete', '6'),
	('tickets.ticket_quote', 'edit', '1'),
	('tickets.ticket_quote', 'edit', '3'),
	('tickets.ticket_quote', 'edit', '4'),
	('tickets.ticket_quote', 'edit', '6'),
	('tickets.ticket_quote', 'view', '1'),
	('tickets.ticket_quote', 'view', '3'),
	('tickets.ticket_quote', 'view', '4'),
	('tickets.ticket_quote', 'view', '6'),
	('tickets.ticket_quote', 'view', '7'),
	('tickets.ticket_receipt', 'add', '1'),
	('tickets.ticket_receipt', 'add', '3'),
	('tickets.ticket_receipt', 'add', '4'),
	('tickets.ticket_receipt', 'add', '6'),
	('tickets.ticket_receipt', 'edit', '1'),
	('tickets.ticket_receipt', 'edit', '3'),
	('tickets.ticket_receipt', 'edit', '4'),
	('tickets.ticket_receipt', 'edit', '6'),
	('tickets.ticket_receipt', 'view', '1'),
	('tickets.ticket_receipt', 'view', '3'),
	('tickets.ticket_receipt', 'view', '4'),
	('tickets.ticket_receipt', 'view', '6'),
	('tickets.ticket_receipt', 'view', '7'),
	('tickets.ticket_search', 'admin', '1'),
	('tickets.ticket_search', 'admin', '3'),
	('tickets.ticket_search', 'admin', '4'),
	('tickets.ticket_search', 'admin', '6'),
	('tickets.ticket_search', 'admin', '7'),
	('tickets.ticket_stock', 'admin', '1'),
	('tickets.ticket_stock', 'admin', '3'),
	('tickets.ticket_stock', 'admin', '4'),
	('tickets.ticket_stock', 'admin', '6'),
	('tickets.ticket_stock', 'admin', '7'),
	('tickets.ticket_transfer', 'admin', '1'),
	('tickets.ticket_transfer', 'admin', '3'),
	('tickets.ticket_transfer', 'admin', '4'),
	('tickets.ticket_transfer', 'admin', '6'),
	('tickets.ticket_transfer', 'admin', '7');
/*!40000 ALTER TABLE `admin_accessrights` ENABLE KEYS */;


-- Dumping structure for table erp32_database.admin_groups
DROP TABLE IF EXISTS `admin_groups`;
CREATE TABLE IF NOT EXISTS `admin_groups` (
  `id` int(10) NOT NULL DEFAULT '0',
  `name` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `id_2` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Dumping data for table erp32_database.admin_groups: 5 rows
DELETE FROM `admin_groups`;
/*!40000 ALTER TABLE `admin_groups` DISABLE KEYS */;
INSERT INTO `admin_groups` (`id`, `name`) VALUES
	('-1', '.disabled'),
	('3', 'Users_master'),
	('4', 'demo'),
	('6', 'Users_readwrite'),
	('7', 'Users_readonly');
/*!40000 ALTER TABLE `admin_groups` ENABLE KEYS */;


-- Dumping structure for table erp32_database.admin_languages
DROP TABLE IF EXISTS `admin_languages`;
CREATE TABLE IF NOT EXISTS `admin_languages` (
  `AUTOINC` int(10) NOT NULL AUTO_INCREMENT,
  `ID` varchar(10) NOT NULL DEFAULT '',
  `NAME` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `AUTOINC` (`AUTOINC`)
) ENGINE=MyISAM AUTO_INCREMENT=17 DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.admin_languages: 7 rows
DELETE FROM `admin_languages`;
/*!40000 ALTER TABLE `admin_languages` DISABLE KEYS */;
INSERT INTO `admin_languages` (`AUTOINC`, `ID`, `NAME`) VALUES
	(15, 'cp', 'Српски (ћирилица)'),
	(11, 'de', 'Default + German doc.'),
	(10, 'en', 'English'),
	(12, 'fr', 'Default + Français doc.'),
	(16, 'hr', 'Hrvatski'),
	(13, 'mk', 'Default + Mакедонски док.'),
	(14, 'sr', 'Srpski');
/*!40000 ALTER TABLE `admin_languages` ENABLE KEYS */;


-- Dumping structure for table erp32_database.admin_log
DROP TABLE IF EXISTS `admin_log`;
CREATE TABLE IF NOT EXISTS `admin_log` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `TIMESTAMP` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ACTION` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM AUTO_INCREMENT=5264 DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.admin_log: 0 rows
DELETE FROM `admin_log`;
/*!40000 ALTER TABLE `admin_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `admin_log` ENABLE KEYS */;


-- Dumping structure for table erp32_database.admin_users
DROP TABLE IF EXISTS `admin_users`;
CREATE TABLE IF NOT EXISTS `admin_users` (
  `id` varchar(255) NOT NULL DEFAULT '0',
  `license` varchar(255) DEFAULT NULL,
  `firm_id` varchar(255) NOT NULL DEFAULT '',
  `user` varchar(255) DEFAULT NULL,
  `password` varchar(255) DEFAULT NULL,
  `email` varchar(255) DEFAULT NULL,
  `full_name` varchar(100) DEFAULT NULL,
  `group_id` int(11) DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `registerdate` date DEFAULT NULL,
  `expirydate` date DEFAULT NULL,
  `lastlogindate` date DEFAULT NULL,
  `activationkey` varchar(255) DEFAULT NULL,
  `db_config` varchar(255) DEFAULT 'default',
  `tbl_prefix` varchar(255) DEFAULT NULL,
  `db_size` decimal(11,2) DEFAULT NULL,
  `db_version` int(11) DEFAULT NULL,
  `language` varchar(255) DEFAULT NULL,
  `is_debug` varchar(1) DEFAULT '',
  `is_suspended` varchar(1) DEFAULT NULL,
  `last_seen_on` varchar(255) DEFAULT '',
  `allowed_ip` varchar(255) DEFAULT NULL,
  `location_id` varchar(255) DEFAULT NULL,
  `location_name` varchar(255) DEFAULT NULL,
  `notes` varchar(1000) DEFAULT NULL,
  `customlang` blob,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Dumping data for table erp32_database.admin_users: 1 rows
DELETE FROM `admin_users`;
/*!40000 ALTER TABLE `admin_users` DISABLE KEYS */;
INSERT INTO `admin_users` (`id`, `license`, `firm_id`, `user`, `password`, `email`, `full_name`, `group_id`, `status`, `registerdate`, `expirydate`, `lastlogindate`, `activationkey`, `db_config`, `tbl_prefix`, `db_size`, `db_version`, `language`, `is_debug`, `is_suspended`, `last_seen_on`, `allowed_ip`, `location_id`, `location_name`, `notes`, `customlang`) VALUES
	('8fdc824654', 'pro', '1000', 'user1', '24c9e15e52afc47c225b757e7bee1f9d', 'user@example.com', 'Demo User', 3, NULL, '2012-02-23', NULL, '2012-10-03', 'activated: iyz8lw5krc2evd7i', 'default', NULL, 0.00, 4, 'en', '0', '0', '::1; ia32=8e74mssaill23tjeiu50qhgkr1', NULL, '1000', 'Office', 'This is Demo user', NULL),
	('3-eb9054947a', NULL, '1000', 'user2', '7e58d63b60197ceb55a1c487989a3720', NULL, NULL, 3, NULL, NULL, NULL, '2012-10-03', NULL, 'default', NULL, 0.00, NULL, 'en', '', '0', '::1; ia32=8e74mssaill23tjeiu50qhgkr1', NULL, '1000', 'Office', '', NULL);
/*!40000 ALTER TABLE `admin_users` ENABLE KEYS */;


-- Dumping structure for table erp32_database.category
DROP TABLE IF EXISTS `category`;
CREATE TABLE IF NOT EXISTS `category` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.category: 5 rows
DELETE FROM `category`;
/*!40000 ALTER TABLE `category` DISABLE KEYS */;
INSERT INTO `category` (`id`, `name`) VALUES
	('1000', 'doctype_main2');
/*!40000 ALTER TABLE `category` ENABLE KEYS */;


-- Dumping structure for table erp32_database.config
DROP TABLE IF EXISTS `config`;
CREATE TABLE IF NOT EXISTS `config` (
  `firm_id` varchar(255) NOT NULL DEFAULT '',
  `firm_name` varchar(255) NOT NULL DEFAULT '',
  `description` varchar(1000) NOT NULL DEFAULT '',
  `prefix_quote` varchar(255) DEFAULT NULL,
  `prefix_receipt` varchar(255) DEFAULT NULL,
  `prefix_creditnote` varchar(255) DEFAULT NULL,
  `prefix_invoice` varchar(255) DEFAULT NULL,
  `prefix_purchaseorder` varchar(255) DEFAULT NULL,
  `prefix_inventory` varchar(255) DEFAULT NULL,
  `prefix_internal_transfer_out` varchar(255) DEFAULT NULL,
  `prefix_internal_transfer_in` varchar(255) DEFAULT NULL,
  `logo_picture` varchar(255) DEFAULT NULL,
  `logo_width` int(11) NOT NULL DEFAULT '40',
  `strict_stock` varchar(1) DEFAULT '1',
  `display_currency` varchar(255) NOT NULL DEFAULT '1000',
  PRIMARY KEY (`firm_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.config: 1 rows
DELETE FROM `config`;
/*!40000 ALTER TABLE `config` DISABLE KEYS */;
INSERT INTO `config` (`firm_id`, `firm_name`, `description`, `prefix_quote`, `prefix_receipt`, `prefix_creditnote`, `prefix_invoice`, `prefix_purchaseorder`, `prefix_inventory`, `prefix_internal_transfer_out`, `prefix_internal_transfer_in`, `logo_picture`, `logo_width`, `strict_stock`, `display_currency`) VALUES
	('1365', 'My Company', 'Address:\r\nPhone:', 'QUO-', 'IN-', 'RET-', 'OUT-', 'ORD-', 'INV-', 'TRO-', 'TRI-', '111x.jpg', 40, '1', '1000');
/*!40000 ALTER TABLE `config` ENABLE KEYS */;


-- Dumping structure for table erp32_database.currency
DROP TABLE IF EXISTS `currency`;
CREATE TABLE IF NOT EXISTS `currency` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  `symbol` varchar(10) NOT NULL DEFAULT '',
  `rate` decimal(10,5) NOT NULL DEFAULT '0.00000',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.currency: 2 rows
DELETE FROM `currency`;
/*!40000 ALTER TABLE `currency` DISABLE KEYS */;
INSERT INTO `currency` (`id`, `name`, `symbol`, `rate`) VALUES
	('1000', 'Euro', 'EUR', 1.00000),
	('1001', 'United States dollar', 'USD', 0.77490);
/*!40000 ALTER TABLE `currency` ENABLE KEYS */;


-- Dumping structure for table erp32_database.customers
DROP TABLE IF EXISTS `customers`;
CREATE TABLE IF NOT EXISTS `customers` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `taxid` varchar(255) DEFAULT NULL,
  `name` varchar(255) NOT NULL DEFAULT '',
  `type_id` varchar(255) NOT NULL DEFAULT '',
  `city` varchar(255) DEFAULT NULL,
  `zip` varchar(255) NOT NULL DEFAULT '0',
  `state` varchar(255) DEFAULT NULL,
  `address` varchar(255) DEFAULT NULL,
  `telephone` varchar(255) DEFAULT NULL,
  `email` varchar(255) DEFAULT NULL,
  `notes` varchar(1000) DEFAULT NULL,
  `discount` decimal(15,2) DEFAULT NULL,
  `credit_limit` decimal(15,2) DEFAULT NULL,
  `finance_sum` decimal(15,2) DEFAULT '0.00',
  `credit_status_id` varchar(255) DEFAULT '',
  `hidden` int(1) NOT NULL DEFAULT '0',
  `creator` varchar(255) DEFAULT NULL,
  `lastmod` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `type_id` (`type_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.customers: 4 rows
DELETE FROM `customers`;
/*!40000 ALTER TABLE `customers` DISABLE KEYS */;
/*!40000 ALTER TABLE `customers` ENABLE KEYS */;


-- Dumping structure for table erp32_database.customer_status
DROP TABLE IF EXISTS `customer_status`;
CREATE TABLE IF NOT EXISTS `customer_status` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.customer_status: 3 rows
DELETE FROM `customer_status`;
/*!40000 ALTER TABLE `customer_status` DISABLE KEYS */;
INSERT INTO `customer_status` (`id`, `name`) VALUES
	('1000', 'doctype_status_good'),
	('1001', 'doctype_status_bad'),
	('1002', 'doctype_status_stop');
/*!40000 ALTER TABLE `customer_status` ENABLE KEYS */;


-- Dumping structure for table erp32_database.customer_types
DROP TABLE IF EXISTS `customer_types`;
CREATE TABLE IF NOT EXISTS `customer_types` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.customer_types: 4 rows
DELETE FROM `customer_types`;
/*!40000 ALTER TABLE `customer_types` DISABLE KEYS */;
INSERT INTO `customer_types` (`id`, `name`) VALUES
	('1', 'doctype_inactive'),
	('1000', 'doctype_customer'),
	('1001', 'doctype_vendor'),
	('1002', 'doctype_customervendor');
/*!40000 ALTER TABLE `customer_types` ENABLE KEYS */;


-- Dumping structure for table erp32_database.db_lock
DROP TABLE IF EXISTS `db_lock`;
CREATE TABLE IF NOT EXISTS `db_lock` (
  `lock_id` bigint(20) NOT NULL,
  `lock_table` varchar(100) NOT NULL,
  `lock_record` varchar(100) NOT NULL,
  `lock_stamp` datetime NOT NULL,
  `lock_lease` datetime NOT NULL,
  `lock_lease_count` bigint(20) NOT NULL,
  `user_id` varchar(50) NOT NULL,
  `user_ip` varchar(50) NOT NULL,
  `session_id` varchar(32) NOT NULL,
  PRIMARY KEY (`lock_id`,`lock_table`,`lock_record`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.db_lock: 0 rows
DELETE FROM `db_lock`;
/*!40000 ALTER TABLE `db_lock` DISABLE KEYS */;
/*!40000 ALTER TABLE `db_lock` ENABLE KEYS */;


-- Dumping structure for table erp32_database.db_sequence
DROP TABLE IF EXISTS `db_sequence`;
CREATE TABLE IF NOT EXISTS `db_sequence` (
  `seq_name` varchar(255) NOT NULL DEFAULT '',
  `nextid` int(10) NOT NULL,
  PRIMARY KEY (`seq_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- Dumping data for table erp32_database.db_sequence: 8 rows
DELETE FROM `db_sequence`;
/*!40000 ALTER TABLE `db_sequence` DISABLE KEYS */;
INSERT INTO `db_sequence` (`seq_name`, `nextid`) VALUES
	('db_lock', 100),
	('text_templates', 1),
	('admin_groups', 100),
	('admin_users', 100);
/*!40000 ALTER TABLE `db_sequence` ENABLE KEYS */;


-- Dumping structure for table erp32_database.finance
DROP TABLE IF EXISTS `finance`;
CREATE TABLE IF NOT EXISTS `finance` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `type_id` varchar(255) NOT NULL DEFAULT '',
  `date` date NOT NULL DEFAULT '0000-00-00',
  `customer_id` varchar(255) NOT NULL DEFAULT '',
  `amount` decimal(15,2) NOT NULL DEFAULT '0.00',
  `ticket_id` varchar(255) DEFAULT NULL,
  `creator` varchar(255) DEFAULT NULL,
  `notes` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `type_id` (`type_id`),
  KEY `customer_id` (`customer_id`),
  KEY `ticket_id` (`ticket_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.finance: 2 rows
DELETE FROM `finance`;
/*!40000 ALTER TABLE `finance` DISABLE KEYS */;
/*!40000 ALTER TABLE `finance` ENABLE KEYS */;


-- Dumping structure for table erp32_database.finance_types
DROP TABLE IF EXISTS `finance_types`;
CREATE TABLE IF NOT EXISTS `finance_types` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.finance_types: 6 rows
DELETE FROM `finance_types`;
/*!40000 ALTER TABLE `finance_types` DISABLE KEYS */;
INSERT INTO `finance_types` (`id`, `name`) VALUES
	('1002', 'doctype_invoice_payment'),
	('1003', 'doctype_invoice_charge'),
	('1004', 'doctype_receipt_payment'),
	('1005', 'doctype_receipt_charge'),
	('1006', 'doctype_payment_free'),
	('1007', 'doctype_charge_free');
/*!40000 ALTER TABLE `finance_types` ENABLE KEYS */;


-- Dumping structure for table erp32_database.locations
DROP TABLE IF EXISTS `locations`;
CREATE TABLE IF NOT EXISTS `locations` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  `address` varchar(255) NOT NULL DEFAULT '',
  `description` varchar(1000) DEFAULT NULL,
  `prefix` varchar(255) DEFAULT NULL,
  `pricelist_id` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `pricelist_id` (`pricelist_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.locations: 3 rows
DELETE FROM `locations`;
/*!40000 ALTER TABLE `locations` DISABLE KEYS */;
INSERT INTO `locations` (`id`, `name`, `address`, `description`, `prefix`, `pricelist_id`) VALUES
	('1000', 'Office', 'Main Location', '', NULL, '1000');
/*!40000 ALTER TABLE `locations` ENABLE KEYS */;


-- Dumping structure for table erp32_database.prices
DROP TABLE IF EXISTS `prices`;
CREATE TABLE IF NOT EXISTS `prices` (
  `pricelist_id` varchar(255) NOT NULL DEFAULT '',
  `product_id` varchar(255) NOT NULL DEFAULT '',
  `pricebuy` decimal(15,2) NOT NULL DEFAULT '0.00',
  `pricesell` decimal(15,2) NOT NULL DEFAULT '0.00',
  PRIMARY KEY (`pricelist_id`(30),`product_id`(30))
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.prices: 13 rows
DELETE FROM `prices`;
/*!40000 ALTER TABLE `prices` DISABLE KEYS */;
/*!40000 ALTER TABLE `prices` ENABLE KEYS */;


-- Dumping structure for table erp32_database.price_list
DROP TABLE IF EXISTS `price_list`;
CREATE TABLE IF NOT EXISTS `price_list` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.price_list: 2 rows
DELETE FROM `price_list`;
/*!40000 ALTER TABLE `price_list` DISABLE KEYS */;
INSERT INTO `price_list` (`id`, `name`) VALUES
	('1000', 'doctype_main');
/*!40000 ALTER TABLE `price_list` ENABLE KEYS */;


-- Dumping structure for table erp32_database.products
DROP TABLE IF EXISTS `products`;
CREATE TABLE IF NOT EXISTS `products` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  `barcode` varchar(255) DEFAULT NULL,
  `unit_id` varchar(255) NOT NULL DEFAULT '',
  `category_id` varchar(255) NOT NULL DEFAULT '',
  `tax_id` varchar(255) NOT NULL DEFAULT '',
  `track_stock` varchar(1) DEFAULT NULL,
  `image` mediumblob,
  `hidden` int(1) NOT NULL DEFAULT '0',
  `creator` varchar(255) DEFAULT NULL,
  `lastmod` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `unit_id` (`unit_id`),
  KEY `category_id` (`category_id`),
  KEY `tax_id` (`tax_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.products: 10 rows
DELETE FROM `products`;
/*!40000 ALTER TABLE `products` DISABLE KEYS */;
/*!40000 ALTER TABLE `products` ENABLE KEYS */;


-- Dumping structure for table erp32_database.products_mat
DROP TABLE IF EXISTS `products_mat`;
CREATE TABLE IF NOT EXISTS `products_mat` (
  `product_id` varchar(255) NOT NULL DEFAULT '',
  `material_id` varchar(255) NOT NULL DEFAULT '',
  `amount` decimal(15,2) DEFAULT NULL,
  PRIMARY KEY (`product_id`(30),`material_id`(30))
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.products_mat: 1 rows
DELETE FROM `products_mat`;
/*!40000 ALTER TABLE `products_mat` DISABLE KEYS */;
/*!40000 ALTER TABLE `products_mat` ENABLE KEYS */;


-- Dumping structure for table erp32_database.stockcurrent
DROP TABLE IF EXISTS `stockcurrent`;
CREATE TABLE IF NOT EXISTS `stockcurrent` (
  `product_id` varchar(255) NOT NULL DEFAULT '',
  `location_id` varchar(255) NOT NULL DEFAULT '',
  `amount` decimal(15,2) NOT NULL DEFAULT '0.00',
  `reservation` decimal(15,2) NOT NULL DEFAULT '0.00',
  `on_order` decimal(15,2) NOT NULL DEFAULT '0.00',
  `min_stock` decimal(15,2) DEFAULT NULL,
  `max_stock` decimal(15,2) DEFAULT NULL,
  PRIMARY KEY (`product_id`(30),`location_id`(30))
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.stockcurrent: 27 rows
DELETE FROM `stockcurrent`;
/*!40000 ALTER TABLE `stockcurrent` DISABLE KEYS */;
/*!40000 ALTER TABLE `stockcurrent` ENABLE KEYS */;


-- Dumping structure for table erp32_database.taxes
DROP TABLE IF EXISTS `taxes`;
CREATE TABLE IF NOT EXISTS `taxes` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  `rate` decimal(15,4) NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.taxes: 4 rows
DELETE FROM `taxes`;
/*!40000 ALTER TABLE `taxes` DISABLE KEYS */;
INSERT INTO `taxes` (`id`, `name`, `rate`) VALUES
	('1000', '0 %', 0.0000),
	('1001', '23 %', 0.2300),
	('1002', '20 %', 0.2000),
	('1003', '18 %', 0.1800),
	('1004', '10 %', 0.1000),
	('1005', '8,5 %', 0.0850),
	('1006', '8 %', 0.0800),
	('1007', '4 %', 0.0400);
/*!40000 ALTER TABLE `taxes` ENABLE KEYS */;


-- Dumping structure for table erp32_database.text_templates
DROP TABLE IF EXISTS `text_templates`;
CREATE TABLE IF NOT EXISTS `text_templates` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  `text` varchar(1000) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.text_templates: 2 rows
DELETE FROM `text_templates`;
/*!40000 ALTER TABLE `text_templates` DISABLE KEYS */;
INSERT INTO `text_templates` (`id`, `name`, `text`) VALUES
	('1-1c3600f24b', 'Template 001', 'Sample text template');
/*!40000 ALTER TABLE `text_templates` ENABLE KEYS */;


-- Dumping structure for table erp32_database.tickets
DROP TABLE IF EXISTS `tickets`;
CREATE TABLE IF NOT EXISTS `tickets` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `counter` int(11) NOT NULL DEFAULT '0',
  `ticket_number` varchar(255) NOT NULL DEFAULT '',
  `external_number` varchar(255) DEFAULT NULL,
  `date_created` date NOT NULL DEFAULT '0000-00-00',
  `date_payment` date NOT NULL DEFAULT '0000-00-00',
  `date_product_issue` date NOT NULL DEFAULT '0000-00-00',
  `ticket_sum` decimal(15,2) NOT NULL DEFAULT '0.00',
  `ticket_payment` decimal(15,2) NOT NULL DEFAULT '0.00',
  `location_id` varchar(255) NOT NULL DEFAULT '',
  `customer_id` varchar(255) NOT NULL DEFAULT '',
  `type_id` varchar(255) NOT NULL DEFAULT '',
  `status_id` varchar(255) NOT NULL DEFAULT '1000',
  `creator` varchar(255) DEFAULT NULL,
  `lastmod` varchar(255) DEFAULT NULL,
  `notes` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `location_id` (`location_id`),
  KEY `customer_id` (`customer_id`),
  KEY `type_id` (`type_id`),
  KEY `status_id` (`status_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.tickets: 4 rows
DELETE FROM `tickets`;
/*!40000 ALTER TABLE `tickets` DISABLE KEYS */;
/*!40000 ALTER TABLE `tickets` ENABLE KEYS */;


-- Dumping structure for table erp32_database.ticket_lines
DROP TABLE IF EXISTS `ticket_lines`;
CREATE TABLE IF NOT EXISTS `ticket_lines` (
  `ticket_id` varchar(255) NOT NULL DEFAULT '',
  `line` int(11) NOT NULL DEFAULT '0',
  `product_id` varchar(255) NOT NULL DEFAULT '',
  `units` decimal(15,2) NOT NULL DEFAULT '0.00',
  `units_diff` decimal(15,2) NOT NULL DEFAULT '0.00',
  `fix_productname` varchar(255) NOT NULL DEFAULT 'n',
  `fix_price` decimal(15,2) NOT NULL DEFAULT '0.00',
  `fix_pricebuy` decimal(15,2) NOT NULL DEFAULT '0.00',
  `fix_discount` decimal(15,2) NOT NULL DEFAULT '0.00',
  `fix_taxrate` decimal(15,4) NOT NULL DEFAULT '0.0000',
  `fix_taxname` varchar(255) NOT NULL DEFAULT '',
  `fix_unitsymbol` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`ticket_id`,`line`),
  KEY `product_id` (`product_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.ticket_lines: 10 rows
DELETE FROM `ticket_lines`;
/*!40000 ALTER TABLE `ticket_lines` DISABLE KEYS */;
/*!40000 ALTER TABLE `ticket_lines` ENABLE KEYS */;


-- Dumping structure for table erp32_database.ticket_status
DROP TABLE IF EXISTS `ticket_status`;
CREATE TABLE IF NOT EXISTS `ticket_status` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.ticket_status: 5 rows
DELETE FROM `ticket_status`;
/*!40000 ALTER TABLE `ticket_status` DISABLE KEYS */;
INSERT INTO `ticket_status` (`id`, `name`) VALUES
	('1000', 'doctype_status_open'),
	('1001', 'doctype_status_finished'),
	('1002', 'doctype_status_hidden'),
	('1003', 'doctype_status_returned'),
	('1004', 'doctype_status_waiting');
/*!40000 ALTER TABLE `ticket_status` ENABLE KEYS */;


-- Dumping structure for table erp32_database.ticket_types
DROP TABLE IF EXISTS `ticket_types`;
CREATE TABLE IF NOT EXISTS `ticket_types` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.ticket_types: 8 rows
DELETE FROM `ticket_types`;
/*!40000 ALTER TABLE `ticket_types` DISABLE KEYS */;
INSERT INTO `ticket_types` (`id`, `name`) VALUES
	('1001', 'doctype_quote'),
	('1002', 'doctype_receipt'),
	('1003', 'doctype_creditnote'),
	('1004', 'doctype_invoice'),
	('1005', 'doctype_purchaseorder'),
	('1006', 'doctype_inventory'),
	('1007', 'doctype_internal_transfer_out'),
	('1008', 'doctype_internal_transfer_in');
/*!40000 ALTER TABLE `ticket_types` ENABLE KEYS */;


-- Dumping structure for table erp32_database.units
DROP TABLE IF EXISTS `units`;
CREATE TABLE IF NOT EXISTS `units` (
  `id` varchar(255) NOT NULL DEFAULT '',
  `name` varchar(255) NOT NULL DEFAULT '',
  `symbol` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Dumping data for table erp32_database.units: 4 rows
DELETE FROM `units`;
/*!40000 ALTER TABLE `units` DISABLE KEYS */;
INSERT INTO `units` (`id`, `name`, `symbol`) VALUES
	('1000', 'doctype_piece', 'doctype_piece_symbol'),
	('1001', 'doctype_liter', 'doctype_liter_symbol'),
	('1002', 'doctype_kilogram', 'doctype_kilogram_symbol');
/*!40000 ALTER TABLE `units` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
