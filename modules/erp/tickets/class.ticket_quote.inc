<?php

// 1001

include_once ("class.ticket_base.inc");

class ticket_quote extends ticket_base
{

	function ticket_quote()
	{
		parent::ticket_base("ticket_quote", NF_NO_ADD|NF_SPECIFIC_5);

		$this->add(new atkAttribute("id", AF_AUTOKEY|AF_HIDE, array(255,10,10)));

		$this->add(new atkManyToOneRelation("type_id", "tickets.ticket_types", AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkManyToOneRelation("status_id", "tickets.ticket_status", AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkAttribute("ticket_number", AF_LARGE|AF_SEARCHABLE|AF_READONLY|AF_UNIQUE));

		$this->add(new atkAttribute("counter", AF_HIDE));

		$this->add(new atkAttribute("external_number", AF_HIDE_LIST|AF_SEARCHABLE));
		$this->getAttribute("external_number")->setAttribSize(array(255,15,15));

		$this->add(new atkManyToOneRelation("customer_id", "customers.customer", AF_LARGE|AF_OBLIGATORY|AF_RELATION_AUTOCOMPLETE|AF_RELATION_AUTOLINK|AF_SEARCHABLE));
		$this->getAttribute("customer_id")->setLabel(atktext("customer")); // for errors
		$this->getAttribute("customer_id")->setAttribSize(array(255,25,25));
		$this->getAttribute("customer_id")->addDependency(array($this, "refresh_top"));
			
		$this->add(new atkManyToOneRelation("customer_info", "customers.customer", AF_HIDE));
		$this->getAttribute("customer_info")->setloadType(NOLOAD);
		$this->getAttribute("customer_info")->setstorageType(NOSTORE);
		$this->getAttribute("customer_info")->setDescriptorTemplate("[address]<br>[zip] [city] [state]<br> [taxid]");

		$this->add(new atkDateAttribute("date_product_issue", AF_OBLIGATORY|AF_HIDE_LIST|AF_DATE_EDIT_NO_DAY|AF_DATE_NO_CALENDAR|AF_SEARCHABLE));
		$this->getAttribute("date_product_issue")->addDependency(array($this, "refresh_paymentDay"));

		$this->add(new atkDateAttribute("date_payment", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY|AF_DATE_NO_CALENDAR|AF_HIDE_LIST));
		$this->getAttribute("date_payment")->addDependency(array($this, "refresh_paymentDay"));

		$this->add(new atkManyToOneRelation("location_id", "locations.location", AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE|AF_HIDE_LIST));
		$this->getAttribute("location_id")->setDescriptorTemplate("[address]");

		$this->add(new atkNumberAttribute("payment_days", AF_HIDE, 4, 0));
		$this->getAttribute("payment_days")->setloadType(NOLOAD);
		$this->getAttribute("payment_days")->setstorageType(NOSTORE);
		$this->getAttribute("payment_days")->addDependency(array($this, "refresh_paymentDate"));

		$this->add(new atkNumberAttribute("discount_full", AF_HIDE, 3, 2));
		$this->getAttribute("discount_full")->setloadType(NOLOAD);
		$this->getAttribute("discount_full")->setstorageType(NOSTORE);
		$this->getAttribute("discount_full")->addDependency(array($this, "refresh_discount"));

		$this->add(new atkDateAttribute("date_created", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY|AF_DATE_NO_CALENDAR));

		$this->add(new atkNumberAttribute("ticket_sum", AF_SEARCHABLE, 10, 2));

		$this->add(new atkTextAttribute("notes", array("cols"=>120,"rows"=>5), AF_HIDE_LIST));

		$this->add(new atkBoolAttribute("has_notes", AF_HIDE_LIST|AF_HIDE_VIEW));
		$this->getAttribute("has_notes")->setloadType(NOLOAD);
		$this->getAttribute("has_notes")->setstorageType(NOSTORE);
		$this->getAttribute("has_notes")->addDependency(array($this, "refresh_notes"));

		$this->add(new atkAttribute("notes_holder", AF_HIDE, 10));
		$this->getAttribute("notes_holder")->setloadType(NOLOAD);
		$this->getAttribute("notes_holder")->setstorageType(NOSTORE);
		$this->getAttribute("notes_holder")->setLabel('none');

		// group master
		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			$this->add(new atkManyToOneRelation("product_id$i", "products.product", AF_RELATION_AUTOCOMPLETE|AF_RELATION_AUTOLINK|AF_HIDE));
			$this->getAttribute("product_id$i")->setloadType(NOLOAD);
			$this->getAttribute("product_id$i")->setstorageType(NOSTORE);
			$this->getAttribute("product_id$i")->setAttribSize(array(255,18,18));
			$this->getAttribute("product_id$i")->addDependency(array($this, "refresh_dep_row$i"));
			$this->getAttribute("product_id$i")->setAutoCompleteSearchFields(array("name", "barcode"));
			// TODO: when only one is found, move to next field (useful for barcode scanners)

			// product change holder
			$this->add(new atkDummyAttribute("product_idchange$i", " ", AF_READONLY|AF_HIDE_LIST));
			$this->getAttribute("product_idchange$i")->setloadType(NOLOAD);
			$this->getAttribute("product_idchange$i")->setstorageType(NOSTORE);

			$this->add(new atkAttribute("fix_productname_$i", AF_READONLY|AF_HIDE, 10, 2));
			$this->getAttribute("fix_productname_$i")->setloadType(NOLOAD);
			$this->getAttribute("fix_productname_$i")->setstorageType(NOSTORE);

			$this->add(new atkNumberAttribute("units_$i", AF_HIDE, 6, 2 ));
			$this->getAttribute("units_$i")->setloadType(NOLOAD);
			$this->getAttribute("units_$i")->setstorageType(NOSTORE);
			$this->getAttribute("units_$i")->addDependency(array($this, "refresh_dep_row$i"));

			$this->add(new atkAttribute("fix_unitsymbol_$i", AF_READONLY|AF_HIDE, array(255,3,3) ));
			$this->getAttribute("fix_unitsymbol_$i")->setloadType(NOLOAD);
			$this->getAttribute("fix_unitsymbol_$i")->setstorageType(NOSTORE);

			$this->add(new atkNumberAttribute("fix_price_$i", AF_HIDE, 8, 2));
			$this->getAttribute("fix_price_$i")->setloadType(NOLOAD);
			$this->getAttribute("fix_price_$i")->setstorageType(NOSTORE);
			$this->getAttribute("fix_price_$i")->setLabel(' ');
			$this->getAttribute("fix_price_$i")->addDependency(array($this, "refresh_dep_row$i"));

			$this->add(new atkNumberAttribute("price_original_holder_$i", AF_HIDE, 8, 2));
			$this->getAttribute("price_original_holder_$i")->setloadType(NOLOAD);
			$this->getAttribute("price_original_holder_$i")->setstorageType(NOSTORE);

			$this->add(new atkNumberAttribute("stock_holder_$i", AF_HIDE, 10, 2));
			$this->getAttribute("stock_holder_$i")->setloadType(NOLOAD);
			$this->getAttribute("stock_holder_$i")->setstorageType(NOSTORE);

			$this->add(new atkNumberAttribute("reservation_holder_$i", AF_HIDE, 10, 2));
			$this->getAttribute("reservation_holder_$i")->setloadType(NOLOAD);
			$this->getAttribute("reservation_holder_$i")->setstorageType(NOSTORE);

			$this->add(new atkNumberAttribute("fix_discount_$i", AF_HIDE, 3, 2));
			$this->getAttribute("fix_discount_$i")->setloadType(NOLOAD);
			$this->getAttribute("fix_discount_$i")->setstorageType(NOSTORE);
			$this->getAttribute("fix_discount_$i")->addDependency(array($this, "refresh_dep_row$i"));

			$this->add(new atkNumberAttribute("discount_price_$i", AF_READONLY|AF_HIDE, 10, 2));
			$this->getAttribute("discount_price_$i")->setloadType(NOLOAD);
			$this->getAttribute("discount_price_$i")->setstorageType(NOSTORE);
			//$this->getAttribute("discount_price_$i")->addDependency(array($this, "refresh_dep_row$i"));

			$this->add(new atkAttribute("fix_taxname_$i", AF_READONLY|AF_HIDE));
			$this->getAttribute("fix_taxname_$i")->setloadType(NOLOAD);
			$this->getAttribute("fix_taxname_$i")->setstorageType(NOSTORE);

			$this->add(new atkAttribute("fix_taxrate_$i", AF_READONLY|AF_HIDE, 10, 2));
			$this->getAttribute("fix_taxrate_$i")->setloadType(NOLOAD);
			$this->getAttribute("fix_taxrate_$i")->setstorageType(NOSTORE);

			$this->add(new atkNumberAttribute("sum_$i", AF_READONLY|AF_HIDE_LIST, 10, 2));
			$this->getAttribute("sum_$i")->setloadType(NOLOAD);
			$this->getAttribute("sum_$i")->setstorageType(NOSTORE);
			//$this->getAttribute("sum_$i")->addDependency(array($this, "refresh_dep_row$i"));

			// row holder
			$this->add(new atkAttribute("row_holder_$i", AF_HIDE, 10));
			$this->getAttribute("row_holder_$i")->setloadType(NOLOAD);
			$this->getAttribute("row_holder_$i")->setstorageType(NOSTORE);
			$this->getAttribute("row_holder_$i")->setLabel('none');

		}

		// show first row initialy
		$this->getAttribute("row_holder_1")->setLabel('display');

		// at least one record holder
		$this->add(new atkAttribute("at_least_one_product", AF_OBLIGATORY|AF_HIDE, 10, 2));
		$this->getAttribute("at_least_one_product")->setloadType(NOLOAD);
		$this->getAttribute("at_least_one_product")->setstorageType(NOSTORE);

		// sum holder
		$this->add(new atkNumberAttribute("sum_holder", AF_READONLY|AF_HIDE_LIST, 10, 2));
		$this->getAttribute("sum_holder")->setloadType(NOLOAD);
		$this->getAttribute("sum_holder")->setstorageType(NOSTORE);

		// sum tax holder
		$this->add(new atkNumberAttribute("sumtax_holder", AF_READONLY|AF_HIDE_LIST, 10, 2));
		$this->getAttribute("sumtax_holder")->setloadType(NOLOAD);
		$this->getAttribute("sumtax_holder")->setstorageType(NOSTORE);

		// page holder
		for ($i=1; $i<=atkconfig('pages_limit'); $i++){

			// add dummy for holding page data
			$this->add(new atkDummyAttribute("page_stack_".$i, " ", AF_READONLY|AF_HIDE_LIST));
			$this->getAttribute("page_stack_".$i)->setloadType(NOLOAD);
			$this->getAttribute("page_stack_".$i)->setstorageType(NOSTORE);

			// atkListAttribute arrays
			$options[] = atktext("page_num")." ".$i;
			$values[] = $i;
		}
		$this->add(new atkListAttribute("page_holder",$options, $values, AF_LIST_NO_NULL_ITEM|AF_HIDE_LIST));
		$this->getAttribute("page_holder")->setloadType(NOLOAD);
		$this->getAttribute("page_holder")->setstorageType(NOSTORE);
		$this->getAttribute("page_holder")->addDependency(array($this, "refresh_page"));
		//	current page
		$this->add(new atkNumberAttribute("current_page",AF_READONLY|AF_HIDE_LIST, 2, 0));
		$this->getAttribute("current_page")->setloadType(NOLOAD);
		$this->getAttribute("current_page")->setstorageType(NOSTORE);

		$this->addFieldSet('group_header',$this->drawHeader(),AF_HIDE_LIST|AF_NOLABEL,NULL, 1);

		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			$this->addFieldSet('group_line_'.$i,$this->drawLine($i),AF_HIDE_LIST|AF_NOLABEL,NULL, 10 + $i);
		}

		$this->addFieldSet('group_footer',$this->drawFooter(),AF_HIDE_LIST|AF_NOLABEL,NULL, 40);
		$this->addFieldSet('group_top',$this->drawTopGroup(),AF_HIDE_LIST|AF_NOLABEL,NULL, 200);
		$this->addFieldSet('group_master',$this->drawMasterGroup(),AF_HIDE_LIST|AF_NOLABEL,NULL, 220);
		$this->addFieldSet('group_notes',$this->drawNotes(),AF_HIDE_LIST|AF_NOLABEL,NULL, 240);

		$this->setOrder("date_created DESC");
		$this->setTable("tickets");


		$this->ticket_type = 1001;
		$this->ticket_typename = atktext("doctype_quote");

		$this->addFilter($this->getUserTablePrefix()."tickets.type_id = ".$this->ticket_type);

	}


	/*
	 * create invoice from quote (kreiraj fakturu)
	 */
	function action_create_invoice(&$handler)
	{
		global $g_sessionManager;
		$quote_id = $g_sessionManager->stackVar('quote_id');
		$pair = explode("=",$quote_id);
		$quote_plain_id = stripQuotes($pair[1]);

		// final check of credit status
		$ticket_data = $this->getTicketData($quote_plain_id);
		$credit_status = $this->getCustomerCreditStatus($ticket_data['customer_id'], $ticket_data['ticket_sum']);

		if ($credit_status != 'ok')
		{
			$this->display_notification(atktext("error_formdataerror").' '.$credit_status);
			die;
		}

		// get next document number
		$nextid = $this->getLastTicketCounter('1004') + 1;
		$ticket_number = $this->getNextDocumentNumber('1004', $nextid);

		$lastmod = $this->getUserTrack();
		$creator =atkGetUserId();

		// change curent quote to invoice, status_id to realized=1001 because of reservations
		$sql = "UPDATE ia32_tickets SET counter = $nextid, ticket_number = '$ticket_number', status_id = 1001, type_id = 1004, lastmod = CONCAT('PREV: ',creator), creator = '$creator'  WHERE $quote_id";
		$this->db32update($sql);

		// free reservations (stock is already empty because of move)
		$this->changeStock($quote_plain_id, -1, true);

		// charge the customer
		$this->financeUpdateByTicket($quote_plain_id);

		$params = array("atkselector"=>$quote_id);
		$this->redirect(dispatch_url("tickets.ticket_invoice", "view", $params));
		die();

	}


	function checkStockLevels($record)
	{
		//consolidate products by id
		$consol = array();

		$max_rows_posibile = atkconfig('num_of_rows') * atkconfig('pages_limit');
		for ($i=1; $i<=$max_rows_posibile; $i++)
		{
			if ($record["product_id$i"])
			{
				$consol[$record["product_id$i"]["id"]]["units"] += $record["units_$i"];
				$consol[$record["product_id$i"]["id"]]["stock_holder"] = $record["stock_holder_$i"];
				$consol[$record["product_id$i"]["id"]]["reservation_holder"] = $record["reservation_holder_$i"];
				$consol[$record["product_id$i"]["id"]]["fix_productname"] = $record["fix_productname_$i"];
					
			}else
			break;
		}

		// raise error on not enough stock
		foreach ($consol as $id => $product)
		{
			if ($product['stock_holder'] != -99999999.99 && $product['units'] > $product['stock_holder'])
			{
				$error .= '<br><br>'.$product['fix_productname'].' - '.atktext('amount_available').': '.$product["stock_holder"].' '.atktext('on_reservation').': '.$product["reservation_holder"];
			}
		}

		return $error;
	}


	/*
	 * pre add
	 */
	function preAdd(&$record)
	{
		// created by
		$this->getAttribute('creator')->removeFlag(AF_READONLY);
		$record['creator'] = atkGetUserId();

		// get friendly document number - db_sequence
		$sql =  "SELECT nextid FROM ia32_db_sequence
				 WHERE seq_name = 'ia32_tickets'";

		$recs = $this->db32select($sql);
		$nextid = $recs[0]['nextid'] + 1;

		// get document number, use db_sequence here because purchase order can be deleted
		$record["counter"] = $nextid;
		$record["ticket_number"] = $this->getNextDocumentNumber('1001', $nextid);

		// store sum of ticket_lines to ticket
		$this->pagingLogic_pushData($record, $record['current_page']);
		$other_page_sums = $this->pagingLogic_retriveSums($record);
		$record["ticket_sum"] = $other_page_sums[1];

		return true;
	}


	/*
	 * post add, fill reservations
	 */
	function postAdd(&$record)
	{

		$this->pagingLogic_fill_Ticket_lines($record);

		// for some reason we must strip quotes here
		$record["id"] = stripQuotes($record["id"]);

		// remove from stock
		$this->changeStock($record["id"], -1);
		// fill reservations
		$this->changeStock($record["id"], 1, true);

		return;

	}


	/*
	 * pre update
	 */
	function preUpdate(&$record)
	{
		// lastmod by
		$this->getAttribute('lastmod')->removeFlag(AF_READONLY|AF_HIDE);
		$record['lastmod'] = $this->getUserTrack();

		$this->pagingLogic_pushData($record, $record['current_page']);

		// store sum of ticket_lines to ticket
		$other_page_sums = $this->pagingLogic_retriveSums($record);
		$record["ticket_sum"] = $other_page_sums[1];

		return true;
	}



	/*
	 * pre delete
	 */
	function preDelete($record)
	{
		// put goods from reservations back to stock
		$this->changeStock($record["id"], -1, true);
		$this->changeStock($record["id"], 1);
			
		// now we can delete ticket_lines
		$this->delete_Ticket_lines($record);

		return true;
	}


	/*
	 * post update
	 */
	function postUpdate(&$record)
	{
		// move stock back from reservations
		$this->changeStock($record["id"], -1, true);
		$this->changeStock($record["id"], 1);

		// replace ticket_lines
		$this->delete_Ticket_lines($record);
		$this->pagingLogic_retriveAllData($record);
		$this->fill_Ticket_lines($record);

		// put stock back to reservation
		$this->changeStock($record["id"], -1);
		$this->changeStock($record["id"], 1, true);

		// after update, return to view this document
		$params = array("atkselector"=>$record['atkprimkey']);
		$this->redirect(dispatch_url($this->atknodetype(), "view", $params));
		$this->db32commit();
		die;

		return true;
	}


	/*
	 * specifics as vendor/customer info
	 */
	function setDocumentTypeSpecifics(&$record, $mode=false)
	{
		if ($mode != "view")
		$this->checkCustomerCreditStatus($record);

		// save customer and refresh top
		$record['customer_info']['id'] = $record['customer_id']['id'];

		$this->getAttribute("customer_info")->setLabel(atktext('customer_data'));
		$this->getAttribute("external_number")->setLabel('none');

		// don't display "discount %" on views
		if ($mode=="view")
		$this->getAttribute("discount_full")->setLabel('none');

	}


	/*
	 * load initial values - add mode
	 */
	function initial_values()
	{
		$ret = parent::initial_values();

		$ret["status_id"]["id"] = 1000;

		return $ret;
	}


	/*
	 * fix buttons
	 */
	function getFormButtons($mode, $record)
	{
		// ia32.js for print and confirm window
		$page = &$this->getPage();
		$page->register_script(atkconfig("atkroot")."atk/javascript/ia32.js");
			
		$homepage = "tickets.ticket_out";
			
		if ($mode=='view')
		{
			$buttons.= '<table><tr>';

			// cancel button
			$buttons.= '<td>';
			$url = dispatch_url($homepage, "admin");
			$buttons.= '<input type="button" class="btn_cancel atkdefaultbutton" name="atkback"  value="'.atktext('back_button').'" onClick="location.href=\''.$url.'\'">';
			$buttons.= '</td>';

			// form of creat-copy
			$buttons.= '<td>';
			$buttons.= '<form id="entryform_copy" name="entryform_copy" action="index.php" method="get">';
			// copy button
			$buttons.= '<input type="hidden" name="atkaction" value="add">';
			$buttons.= '<input type="hidden" name="copyfrom" value="'. $this->m_postvars["atkselector"].'">';
			$buttons.= '<input type="hidden" name="atknodetype" value="'.$this->m_postvars["atknodetype"].'">';
			$buttons.= '&nbsp;&nbsp;<input type="submit" class="btn_save" value="'.atktext('create_copy').'">';
			$buttons.= '</form>';
			$buttons.= '</td>';

			// form of edit submit
			$buttons.= '<td>';
			$buttons.= '<form id="entryform" name="entryform" action="index.php" method="get">';
			// edit button
			$buttons.= '<input type="hidden" name="atkaction" value="edit">';
			$buttons.= '<input type="hidden" name="atkselector" value="'. $this->m_postvars["atkselector"].'">';
			$buttons.= '<input type="hidden" name="atknodetype" value="'.$this->m_postvars["atknodetype"].'">';
			$buttons.= '&nbsp;&nbsp;<input type="submit" class="btn_save" value="'.atktext('edit').'">';

			// create invoice for open quotes id=1000
			if ($record['status_id']['id']==1000)
			{
				// create invoice button
				$params = array("quote_id"=>$record['atkprimkey']);
				$url = dispatch_url($this->atknodetype(), "create_invoice", $params);
				$buttons .= '&nbsp;&nbsp;<input type="button" name="create_invoice" value="'.atktext('create_invoice').'" onClick="confirmSensitiveAction(\''.atktext('create_invoice_confirm').'\',\''.$url.'\')">';
			}

			// print button
			$params = array("ticket_id"=>$record['atkprimkey']);
			$url = dispatch_url("reports.report_tickets", "pdfprint", $params);
			$buttons .= '&nbsp;&nbsp;<input type="button" name="button" value="'.atktext('print_quote').'" onClick="launch_pdf(this.form,\''.$url.'\')">';

			$buttons.= '</form>';
			$buttons.= '</td>';

			$buttons.= '</tr></table>';

			return $buttons;
		}

		if ($mode=='add')
		{
			$buttons.= '<input type="submit" class="btn_saveandclose atkdefaultbutton" name="atksaveandclose"  value="'.atktext('saveandclose').'">';

			// cancel button
			$url = dispatch_url($homepage, "admin");
			$buttons.= '&nbsp;&nbsp;<input type="button" class="btn_cancel" name="atkcancel"  value="'.atktext('cancel').'" onClick="location.href=\''.$url.'\'">';

			return $buttons;

		}

		if ($mode=='edit')
		{
			// add form because of update submit
			$buttons.= '<form id="entryform" name="entryform" action="index.php" method="post" onsubmit="return globalSubmit(this)">';

			// update button
			$buttons.= '<input type="hidden" name="atkaction" value="update">';
			$buttons.= '<input type="hidden" name="atkselector" value="'. $this->m_postvars["atkselector"].'">';
			$buttons.= '<input type="hidden" name="atknodetype" value="'.$this->m_postvars["atknodetype"].'">';
			$buttons.= '<input type="submit" class="btn_saveandclose atkdefaultbutton" name="atksaveandclose"  value="'.atktext('saveandclose').'">';

			// cancel button, return to view this document
			$params = array("atkselector"=>$record['atkprimkey']);
			$url = dispatch_url($this->atknodetype(), "view", $params);
			$buttons.= '&nbsp;&nbsp;<input type="button" class="btn_cancel" name="atkcancel"  value="'.atktext('cancel').'" onClick="location.href=\''.$url.'\'">';

			$buttons.= '</form>';

			return $buttons;

		}

		if ($mode=='delete')
		{
			return;
		}

		// bad action, return to admin
		$this->redirect(dispatch_url($homepage, "admin"));
		die;
	}



	/*
	 * validate enough stock
	 */
	function validate(&$record, $mode, $ignoreList=array())
	{
		// TODO: when update - subtrack self from reservation

		// push current grid to pages
		$this->pagingLogic_pushData($record, $record['current_page']);
		// retrive all pages for check
		$this->pagingLogic_retriveAllData($record);

		$max_rows_posibile = atkconfig('num_of_rows') * atkconfig('pages_limit');

		//consolidate products by id
		$consol = array();
		for ($i=1; $i<=$max_rows_posibile; $i++)
		{
			if ($record["product_id$i"])
			{
				$consol[$record["product_id$i"]["id"]]["units"] += $record["units_$i"];
				$consol[$record["product_id$i"]["id"]]["stock_holder"] = $record["stock_holder_$i"];
				$consol[$record["product_id$i"]["id"]]["reservation_holder"] = $record["reservation_holder_$i"];
				$consol[$record["product_id$i"]["id"]]["fix_productname"] = $record["fix_productname_$i"];
					
			}else
			break;
		}

		// raise error on not enough stock
		foreach ($consol as $id => $product)
		{
			if ($product['stock_holder'] != -99999999.99 && $product['units'] > $product['stock_holder'])
			{
				$config = $this->getConfig();
				if(!$config["strict_stock"])
				{
					$label = atktext('not_enough_on_stock');
					$msg = $product['fix_productname'].' - '.atktext('amount_available').': '.$product["stock_holder"].' '.atktext('on_reservation').': '.$product["reservation_holder"];
					$record['atkerror'][] = array( "attrib_name"=> "units_1", "err" => 'err', "msg" => $msg, "tab" => '', "label" => $label);
				}
			}
		}

		// retrive current page data
		$this->pagingLogic_retriveData($record, $record['current_page']);

		// continue with parent functionality
		$parent = parent::validate($record, $mode, $ignoreList);

		return $parent;
	}

}

?>
