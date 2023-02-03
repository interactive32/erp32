<?php

// 1003

include_once ("class.ticket_base.inc");

class ticket_creditnote extends ticket_base
{

	function ticket_creditnote()
	{
		parent::ticket_base("ticket_creditnote", NF_NO_ADD|NF_SPECIFIC_5);

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
		$this->getAttribute("customer_info")->setInitialValue('1000');
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



		$this->ticket_type = 1003;
		$this->ticket_typename = atktext("doctype_creditnote");

		$this->addFilter($this->getUserTablePrefix()."tickets.type_id = ".$this->ticket_type);
			
	}



	/*
	 * pre add
	 */
	function preAdd(&$record)
	{
		// created by
		$this->getAttribute('creator')->removeFlag(AF_READONLY);
		$record['creator'] = atkGetUserId();

		// get next document number
		$record["counter"] = $this->getLastTicketCounter('1003') + 1;
		$record["ticket_number"] = $this->getNextDocumentNumber('1003', $record["counter"]);

		// store sum of ticket_lines to ticket
		$this->pagingLogic_pushData($record, $record['current_page']);
		$other_page_sums = $this->pagingLogic_retriveSums($record);
		$record["ticket_sum"] = $other_page_sums[1];

		return true;
	}


	/*
	 * post add
	 */
	function postAdd(&$record)
	{
		$this->pagingLogic_fill_Ticket_lines($record);

		// for some reason we must strip quotes here
		$record["id"] = stripQuotes($record["id"]);

		// fill our stock back
		$this->changeStock($record["id"], 1);
		// return money to customer
		$this->financeUpdateByTicket($record["id"]);
			
		return;
	}


	/*
	 * pre delete
	 */
	function preDelete($record)
	{
		// credit note cannot be deleted
		return false;
	}


	/*
	 * post update
	 */
	function postUpdate(&$record)
	{
		// credit note cannot be updated!
		return false;
	}


	/*
	 * pre update
	 */
	function preUpdate(&$record)
	{
		// creditnote cannot be updated!
		return false;
	}


	/*
	 * specifics as vendor/customer info
	 */
	function setDocumentTypeSpecifics(&$record, $mode=false)
	{
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

		$ret["status_id"]["id"] = 1001;

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
			// form of create-copy
			$buttons.= '<form id="entryform_copy" name="entryform_copy" action="index.php" method="get">';

			// cancel button
			$url = dispatch_url($homepage, "admin");
			$buttons.= '<input type="button" class="btn_cancel atkdefaultbutton" name="atkback"  value="'.atktext('back_button').'" onClick="location.href=\''.$url.'\'">';

			// copy button
			$buttons.= '<input type="hidden" name="atkaction" value="add">';
			$buttons.= '<input type="hidden" name="copyfrom" value="'. $this->m_postvars["atkselector"].'">';
			$buttons.= '<input type="hidden" name="atknodetype" value="'.$this->m_postvars["atknodetype"].'">';
			$buttons.= '&nbsp;&nbsp;<input type="submit" class="btn_save" value="'.atktext('create_copy').'">';

			// print button
			$params = array("ticket_id"=>$record['atkprimkey']);
			$url = dispatch_url("reports.report_tickets", "pdfprint", $params);
			$buttons .= '&nbsp;&nbsp;<input type="button" name="button" value="'.atktext('print_creditnote').'" onClick="launch_pdf(this.form,\''.$url.'\')">';

			$buttons.= '</form>';

			return $buttons;
		}

		if ($mode=='add')
		{

			// confirm works on submit without need for confirmSensitiveAction js!
			$buttons.= '<input type="submit" class="btn_saveandclose atkdefaultbutton" name="atksaveandclose"  value="'.atktext('saveandclose').'" onClick="return confirm(\''.atktext('create_creditnote_confirm').'\')">';

			// cancel button
			$url = dispatch_url($homepage, "admin");
			$buttons.= '&nbsp;&nbsp;<input type="button" class="btn_cancel" name="atkcancel"  value="'.atktext('cancel').'" onClick="location.href=\''.$url.'\'">';

			return $buttons;

		}

		// bad action, return to admin
		$this->redirect(dispatch_url($homepage, "admin"));
		die;
	}


}

?>
