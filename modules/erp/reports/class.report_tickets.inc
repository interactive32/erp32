<?php

class report_tickets extends ia32node
{

	function report_tickets()
	{

		$this->atkNode("report_tickets");

		$this->add(new atkManyToOneRelation("ticket_id", "tickets.ticket_all", AF_RELATION_AUTOCOMPLETE|AF_OBLIGATORY));
		$this->getAttribute("ticket_id")->setLabel(atktext('ticket'));
		$this->getAttribute("ticket_id")->setAttribSize(array(255,30,30));
		$this->getAttribute("ticket_id")->addDependency(array($this, "refresh_options"));
		$this->getAttribute("ticket_id")->setDescriptorTemplate("[ticket_number] ([type_id.name])");

		$this->add(new atkListAttribute("print_option",	array(),""));

		$this->add(new atkBoolAttribute("print_notax"));

		$this->add(new atkManyToOneRelation("currency_id", "config.currency", AF_OBLIGATORY));
		$this->getAttribute("currency_id")->setAttribSize(array(3,3,3));

		$this->add(new atkTextAttribute("added_notes", array("cols"=>35,"rows"=>3), AF_HIDE_LIST));

	}

	/*
	 * load initial values - add mode
	 */
	function initial_values()
	{
		$ret["currency_id"]["id"] = 1000;
		$ret["print_notax"] = 1;

		return $ret;
	}


	function actionTitle($action, $record="")
	{
		return atktext($this->m_type);
	}


	/*
	 * refresh options based on ticket type
	 */
	function refresh_options(atkEditFormModifier $modifier)
	{
		$record = &$modifier->getRecord();


		if ($record['ticket_id']['type_id']['id'] == 1004)
		{

			$this->getAttribute("print_option")->addOption(atktext("pdf_invoice"),"invoice");
			$this->getAttribute("print_option")->addOption(atktext("pdf_invoice_with_shipment"),"invoice_shipment");
			$this->getAttribute("print_option")->addOption(atktext("pdf_shipment"),"shipment");
			$this->getAttribute("print_option")->addFlag(AF_LIST_NO_NULL_ITEM);
		}else
		{
			$this->getAttribute("print_option")->addOption(atktext(" ")," ");
			$this->getAttribute("print_option")->addFlag(AF_SPECIFIC_5);
		}

		$modifier->refreshAttribute('print_option');
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
			// print button
			$url = dispatch_url($this->atknodetype(), "pdfprint");
			$buttons .= '<input type="button" class="btn_save atkdefaultbutton" name="button" value="'.atktext('print_document').'" onClick="launch_pdf_with_params(this.form,\''.$url.'\')">';

			return $buttons;
		}

		// bad action, return to home
		$this->redirect("index.php");
		die;
	}



	/*
	 * print pdf
	 */
	function action_pdfprint()
	{
		// include pdf here
		require_once "../modules/utils/pdf/pdf.php";
		
		global $g_sessionManager;

		$ticket_filter = explode("=",$g_sessionManager->stackVar('ticket_id'));
		$ticket = stripQuotes($ticket_filter[1]);

		$print_option = $g_sessionManager->stackVar('print_option');

		$print_notax = $g_sessionManager->stackVar('print_notax');

		if ($print_notax === null) $print_notax = '1';

		$currency_filter = explode("=",$g_sessionManager->stackVar('currency_id'));
		$currency_id = stripQuotes($currency_filter[1]);

		// if currency not set in GET...
		if (!$currency_id) 
		{
			// get it from user config
			$config = $this->getConfig();
			$currency_id = $config["display_currency"];
			
			// if config fail, set it to default = 1000
			if (!$currency_id) $currency_id = 1000;
		}

		$currency = $this->getCurrencyData($currency_id);
		$currency_rate = $currency['rate'];

		$added_notes = $g_sessionManager->stackVar('added_notes');

		if ($ticket == '')
		{
			show_error_window(atktext('enter_ticket_number'));
			die;
		}

		// get ticket type
		$ticket_type = $this->getTicketType($ticket);

		// if no print options, use ticket type
		if (!$print_option) $print_option = $ticket_type['name'];


		$pdf = new atkpdf();

		$ticket_data = $this->getTicketData($ticket);
		$our_firm = $this->getOurFirm();
		$location = $this->getLocationData($ticket_data['location_id']);
		$customer = $this->getCustomerData($ticket_data['customer_id']);

		// Report Title ; Portrait or Landcape
		$top_header = $our_firm['firm_name'];
		$top_info = $our_firm['description'];
		$top_info2 = $location['description'];

		// fix dates
		/*
		$stamp = strtotime($ticket_data['date_created']);
		$monthname = strtolower(date("F", $stamp));
		$formatted_date_created = str_ireplace($monthname, atktext("pdf_$monthname"), date(atktext("date_format_view"), $stamp));

		$stamp = strtotime($ticket_data['date_product_issue']);
		$monthname = strtolower(date("F", $stamp));
		$formatted_date_product_issue = str_ireplace($monthname, atktext("pdf_$monthname"), date(atktext("date_format_view"), $stamp));

		$stamp = strtotime($ticket_data['date_payment']);
		$monthname = strtolower(date("F", $stamp));
		$formatted_date_payment = str_ireplace($monthname, atktext("pdf_$monthname"), date(atktext("date_format_view"), $stamp));
		*/
		
		// non-pdf
		$formatted_date_created = atkFormatDate(strtotime($ticket_data['date_created']), atktext("date_format_view"));
		$formatted_date_product_issue = atkFormatDate(strtotime($ticket_data['date_product_issue']), atktext("date_format_view"));
		$formatted_date_payment = atkFormatDate(strtotime($ticket_data['date_payment']), atktext("date_format_view"));


		switch ($print_option)
		{
			case 'invoice':
				$headtext = atktext('pdf_invoice');
				break;

			case 'invoice_shipment':
				$headtext = atktext('pdf_invoice_with_shipment');
				break;

			case 'shipment':
				$headtext = atktext('pdf_shipment');

				// replace ticket_number prefix with shipment prefix
				$ticket_data['ticket_number'] = str_replace(atktext('invoice_prefix'), atktext('shipment_prefix'), $ticket_data['ticket_number']);

				break;

			case 'quote':
				$headtext = atktext('pdf_quote');
				break;

			case 'receipt':
				$headtext = atktext('pdf_receipt');
				break;

			case 'credit_note':
				$headtext = atktext('pdf_creditnote');
				break;

			case 'purchase_order':
				$headtext = atktext('pdf_purchaseorder');
				break;

			case 'inventory':
				$headtext = atktext('pdf_inventory');
				break;

			case 'internal_transfer_out':
				$headtext = atktext('pdf_internal_transfer_out');
				$print_option = 'internal';
				$location_from = $this->getLocationData($ticket_data['location_id']);
				$location_to = $this->getLocationData($ticket_data['external_number']);
				$transfer_extra_header = $location_from['name'].' >> '.$location_to['name'];
				break;

			case 'internal_transfer_in':
				$headtext = atktext('pdf_internal_transfer_in');
				$print_option = 'internal';
				$location_from = $this->getLocationData($ticket_data['location_id']);
				$location_to = $this->getLocationData($ticket_data['external_number']);
				$transfer_extra_header = $location_from['name'].' << '.$location_to['name'];
				break;

			default:
				// bad type, throw error
				show_error_window(atktext('error_bad_option'));
				die;
		}

		$image_data = $this->getLogoImage();
		$image = $image_data['logo_picture'];
		$image_width = $image_data['logo_width'];
		if (!$image_width || $image_width==0) $image=false;
		$pdf->set_doc_properties('','P',array('AUTHOR'=>$top_header,'TITLE'=>$top_info,'SUBJECT'=>$top_info2),$image,$image_width);


		// header with our firm data
		$cheight = 6;
			
		$pdf->Ln();
		$pdf->SetFillColor(222, 222, 222);
		$pdf->SetTextColor(0);
		$pdf->SetDrawColor(61, 61, 61);
		$pdf->SetLineWidth(0.3);
		$pdf->SetLineStyle(array('color'=>array(65,65,65)));

		// customer data and dates
		if ($print_option == 'invoice'
		|| $print_option == 'invoice_shipment'
		|| $print_option == 'credit_note')
		{
			$pdf->Ln(15);

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Cell(80, $cheight, ' ', 0, 0, 'L', 0);
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_ticket_number').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $ticket_data['ticket_number'], 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', 'LT', 0, 'C', 0);
			$pdf->Cell(80, $cheight, $customer['name'], 0, 0, 'L', 0);
			$pdf->Cell(2, $cheight, '', 'TR', 0, 'C', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_place_of_issue').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $location['address'], 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Cell(82, $cheight, $customer['address'], 0, 0, 'L', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_ducument_date').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $formatted_date_created, 0, 0, 'L', 0);

			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Cell(82, $cheight, $customer['zip'].' '.$customer['city'], 0, 0, 'L', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_date_product_issue').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $formatted_date_product_issue, 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', 'LB', 0, 'C', 0);
			$pdf->Cell(80, $cheight, atktext('pdf_taxid').': '.$customer['taxid'], 0, 0, 'L', 0);
			$pdf->Cell(2, $cheight, '', 'BR', 0, 'C', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_date_payment').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $formatted_date_payment, 0, 0, 'L', 0);
			$pdf->Ln();
		}
		elseif ($print_option == 'shipment'
		|| $print_option == 'quote'
		|| $print_option == 'purchase_order')
		{

			$pdf->Ln(15);

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Cell(80, $cheight, ' ', 0, 0, 'L', 0);
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', 'LT', 0, 'C', 0);
			$pdf->Cell(80, $cheight, $customer['name'], 0, 0, 'L', 0);
			$pdf->Cell(2, $cheight, '', 'TR', 0, 'C', 0);
			if ($print_option == 'shipment')
			{
				$pdf->Cell(65, $cheight, atktext('pdf_ticket_number').': ', 0, 0, 'R', 0);
				$pdf->set_font_regular();
				$pdf->Cell(25, $cheight, $ticket_data['ticket_number'], 0, 0, 'L', 0);
				$pdf->Ln();

				$pdf->set_font_bold();
				$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
				$pdf->Cell(82, $cheight, $customer['address'], 0, 0, 'L', 0);
				$pdf->Cell(65, $cheight, atktext('pdf_place_of_issue').': ', 0, 0, 'R', 0);
				$pdf->set_font_regular();
				$pdf->Cell(25, $cheight, $location['address'], 0, 0, 'L', 0);


			}
			if ($print_option == 'quote' || $print_option == 'purchase_order')
			{
				$pdf->Ln();

				$pdf->set_font_bold();
				$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
				$pdf->Cell(82, $cheight, $customer['address'], 0, 0, 'L', 0);
				$pdf->Cell(65, $cheight, atktext('pdf_ticket_number').': ', 0, 0, 'R', 0);
				$pdf->set_font_regular();
				$pdf->Cell(25, $cheight, $ticket_data['ticket_number'], 0, 0, 'L', 0);

			}

			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Cell(82, $cheight, $customer['zip'].' '.$customer['city'], 0, 0, 'L', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_ducument_date').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $formatted_date_created, 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', 'LB', 0, 'C', 0);
			$pdf->Cell(80, $cheight, atktext('pdf_taxid').': '.$customer['taxid'], 0, 0, 'L', 0);
			$pdf->Cell(2, $cheight, '', 'BR', 0, 'C', 0);

			$pdf->Ln();

		}elseif ($print_option == 'receipt')
		{
			$pdf->Ln(15);

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Cell(80, $cheight, ' ', 0, 0, 'L', 0);
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Ln();
			
			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', 'LT', 0, 'C', 0);
			$pdf->Cell(80, $cheight, $customer['name'], 0, 0, 'L', 0);
			$pdf->Cell(2, $cheight, '', 'TR', 0, 'C', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_ticket_number').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $ticket_data['ticket_number'], 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Cell(82, $cheight, $customer['address'], 0, 0, 'L', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_place_of_receipt').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $location['address'], 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', '', 0, 'C', 0);
			$pdf->Cell(82, $cheight, $customer['zip'].' '.$customer['city'], 0, 0, 'L', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_ducument_date').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $formatted_date_created, 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(2, $cheight, '', 'LB', 0, 'C', 0);
			$pdf->Cell(80, $cheight, atktext('pdf_taxid').': '.$customer['taxid'], 0, 0, 'L', 0);
			$pdf->Cell(2, $cheight, '', 'BR', 0, 'C', 0);
			$pdf->Cell(65, $cheight, ' ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, '', 0, 0, 'L', 0);
			$pdf->Ln();


		}elseif ($print_option == 'inventory')
		{
			$pdf->Ln(15);

			$pdf->Cell(84, $cheight, '', 0, 0, 'L', 0);
			$pdf->set_font_bold();
			$pdf->Cell(65, $cheight, atktext('pdf_ticket_number').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $ticket_data['ticket_number'], 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(84, $cheight, '', 0, 0, 'L', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_place_of_inventory').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $location['address'], 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->Cell(84, $cheight, '', 0, 0, 'L', 0);
			$pdf->set_font_bold();
			$pdf->Cell(65, $cheight, atktext('pdf_ducument_date').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $formatted_date_created, 0, 0, 'L', 0);
			$pdf->Ln();

		}elseif ($print_option == 'internal')
		{
			$pdf->Ln(15);

			$pdf->Cell(84, $cheight, '', 0, 0, 'L', 0);
			$pdf->set_font_bold();
			$pdf->Cell(65, $cheight, atktext('pdf_ticket_number').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $ticket_data['ticket_number'], 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->set_font_bold();
			$pdf->Cell(84, $cheight, '', 0, 0, 'L', 0);
			$pdf->Cell(65, $cheight, atktext('pdf_place_of_issue').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $location['address'], 0, 0, 'L', 0);
			$pdf->Ln();

			$pdf->Cell(84, $cheight, '', 0, 0, 'L', 0);
			$pdf->set_font_bold();
			$pdf->Cell(65, $cheight, atktext('pdf_ducument_date').': ', 0, 0, 'R', 0);
			$pdf->set_font_regular();
			$pdf->Cell(25, $cheight, $formatted_date_created, 0, 0, 'L', 0);
			$pdf->Ln();
		}



		// ticket lines
		$sql = "SELECT
		line,
		product_id,
		fix_productname,
		units,
		units_diff,		
		fix_unitsymbol,
		fix_taxrate,			
		fix_taxname,	
		fix_price / $currency_rate,
		fix_pricebuy / $currency_rate,		
		fix_discount,
		(fix_price - (fix_price / 100 * fix_discount)) * units / $currency_rate AS sum,
		(fix_price - (fix_price / 100 * fix_discount)) * units * (1 + fix_taxrate) / $currency_rate AS sum_with_tax,
		fix_pricebuy / $currency_rate * units AS buysum,
		fix_pricebuy / $currency_rate * units * (1 + fix_taxrate) AS buysum_with_tax
		
		FROM  ia32_ticket_lines	WHERE ticket_id = '$ticket' ";

		$data = $this->db32select($sql);

		// get totals
		$total = $this->getTotalSums($data, $print_option);


		// data formatting (x = no formatting; d = date; s:100 = string lenght is 100;  5 = round with 5 decimal places)
		$format = array('x','x','x', 2, 2,'x','x','x', 2, 2, 2, 2, 2, 2, 2);

		// Column header titles
		$header = array(atktext('pdf_number_short'), atktext('pdf_product_id'), atktext('pdf_product'), atktext('pdf_amount'), atktext('pdf_differences'), atktext('pdf_unit_short'), atktext('pdf_tax_rate'), atktext('pdf_tax'), atktext('pdf_price'), atktext('pdf_pricebuy'),atktext('pdf_discount_percent'), atktext('pdf_ticket_sum'), atktext('pdf_total_with_tax'), atktext('pdf_ticket_sum'), atktext('pdf_total_with_tax'));
		if ($print_option == 'inventory')
		$header = array(atktext('pdf_number_short'), atktext('pdf_product_id'), atktext('pdf_product'), atktext('pdf_amount_on_inventory'), atktext('pdf_differences'), atktext('pdf_unit_short'), atktext('pdf_tax_rate'), atktext('pdf_tax'), atktext('pdf_price'), atktext('pdf_pricebuy'),atktext('pdf_discount_percent'), atktext('pdf_ticket_sum'), atktext('pdf_total_with_tax'), atktext('pdf_ticket_sum'), atktext('pdf_total_with_tax'));

		// Width od columns, Portrait_sum=180 Landscape_sum=267
		switch ($print_option)
		{
			case 'invoice':
			case 'invoice_shipment':
			case 'quote':
			case 'credit_note':
				$width = array(10, 0, 65, 15, 0, 15, 0, 15, 20, 0, 15, 25, 0, 0, 0);
				break;
					
			case 'receipt':
				$width = array(10, 0, 80, 15, 0, 15, 0, 15, 0, 20, 0, 0, 0, 25, 0);
				break;
					
			case 'inventory':
				$width = array(15, 0, 125, 20, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				break;

			case 'shipment':
			case 'purchase_order':
			case 'internal':
				$width = array(15, 0, 125, 20, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0);
				break;

			default:
				// bad type, throw error
				show_error_window(atktext('error_bad_option').': '.$print_option);
				die;
		}

		// if notax - remove tax column and add her space to product name
		if ($print_notax == '0')
		{
			$width[2] += $width[7];
			$width[7] = 0;
		}

		// Alignment L/R/C
		$align = array("C","C","L","R","R","C", "C", "C","R", "R", "R","R","R","R","R");

		// Column Summary and Grouping
		// 1 = summary
		// 2 = grouping
		// 3 = grouping + summary only in groups
		// 4 = last value in group (npr tekuca zaliha ostaje koja je poslednja)
		$summary = array(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

		// title
		$pdf->Ln(10);
		$pdf->SetFont(MAIN_ATK_FONT, 'B', 14, '', true);
		$pdf->Cell( 0, 0, $headtext.': '.$ticket_data['ticket_number'], 0, 0, 'C', 0);
		$pdf->set_font_bold();

		if ($print_option == 'receipt')
		{
			$pdf->Ln(10);
			$pdf->set_font_bold();
			$pdf->Cell(0, $cheight, atktext('pdf_external_number').': '.$ticket_data['external_number'], 0, 0, 'C', 0);
		}

		// if internal transfer...
		if ($transfer_extra_header)
		{
			$pdf->Ln(10);
			$pdf->set_font_bold();
			$pdf->Cell(0, $cheight, $transfer_extra_header, 0, 0, 'C', 0);
		}

		$pdf->Ln(15);

		// Print table
		$pdf->print_table($header,$data,$width,$align,$summary,$format);

		// Print Sums
		if ($print_option == 'invoice'
		|| $print_option == 'invoice_shipment'
		|| $print_option == 'quote'
		|| $print_option == 'receipt'
		|| $print_option == 'credit_note')
		{
			$pdf->set_font_bold();

			if ($print_notax == '1')
			{

				$pdf->Cell(150, 5, atktext('pdf_total').': ', 0, 0, 'R', 0);
				$pdf->Cell(0, 5, $total['sum'], 0, 0, 'R', 0);
				$pdf->Ln();

				$pdf->Cell(150, 5, atktext('pdf_tax_price').': ', 0, 0, 'R', 0);
				$pdf->Cell(0, 5, $total['tax'], 0, 0, 'R', 0);
				$pdf->Ln();

				$pdf->Cell(150, 5, atktext('pdf_total_amount_for_payment').' ('.$currency['symbol'].') : ', 0, 0, 'R', 0);
				$pdf->Cell(0, 5, $total['sum_with_tax'], 0, 0, 'R', 0);
			}else
			{
				$pdf->Cell(150, 5, atktext('pdf_total_amount_for_payment').' ('.$currency['symbol'].') : ', 0, 0, 'R', 0);
				$pdf->Cell(0, 5, $total['sum'], 0, 0, 'R', 0);

			}

			$pdf->set_font_regular();
			$pdf->Ln();

			if ($print_notax != '0'
			&& ($print_option == 'invoice'
			|| $print_option == 'invoice_shipment'
			|| $print_option == 'credit_note'))
			{
				// Print TAX info
				//$pdf->Cell(80, 5, atktext('pdf_tax_specification').':', '', 0, 'L', 0);
				//$pdf->Ln();
				$pdf->Cell(10, 5, ' ', '', 0, '', 0);	// spacer
				$pdf->Cell(20, 5, atktext('pdf_tax_id'), 'B', 0, 'C', 0);
				$pdf->Cell(30, 5, atktext('pdf_tax_base'), 'B', 0, 'R', 0);
				$pdf->Cell(30, 5, atktext('pdf_tax_amount'), 'B', 0, 'R', 0);
				$pdf->Ln();

				$tax_spec = $this->tax_consolidation($data);

				foreach ($tax_spec as $taxline)
				{
					$pdf->Cell(10, 5, ' ', '', 0, '', 0);	// spacer
					$pdf->Cell(20, 5, $taxline['name'], 0, 0, 'C', 0);
					$pdf->Cell(30, 5, $this->formatNumber($taxline['base']), 0, 0, 'R', 0);
					$pdf->Cell(30, 5, $this->formatNumber($taxline['value']), 0, 0, 'R', 0);
					$pdf->Ln();
				}
			}

		}else
		{
			// print some blank space instead of sums
			$pdf->Ln(5);
		}

		// notes
		$pdf->Ln(10);
		// explode new lines CR/NL
		$strings = explode("\r\n",$ticket_data['notes']);
		foreach ($strings as $newline)
		{
			$pdf->Cell(0, 7,$newline, 0, 0, 'L', 0);
			$pdf->Ln(4); // smaller spaces for text notes
		}
		// extra notes
		$pdf->Cell(0, 7,str_replace('_', ' ', $added_notes), 0, 0, 'L', 0);

		// Print signatures
		$pdf->Ln(10);
		$pdf->set_font_bold();
		if ($print_option == 'invoice'
		|| $print_option == 'quote'
		|| $print_option == 'credit_note'
		|| $print_option == 'purchase_order'
		|| $print_option == 'inventory')
		{
			$pdf->Cell(15, 17, '', 0, 0, 'C', 0);
			$pdf->Cell(110, 17, '', 0, 0, 'C', 0);
			$pdf->Cell(30, 17, atktext('pdf_signature_left').':', 'B', 0, 'C', 0);

		}else
		{
			$pdf->Cell(15, 17, '', 0, 0, 'C', 0);
			$pdf->Cell(30, 17, atktext('pdf_signature_left').':', 'B', 0, 'C', 0);
			$pdf->Cell(30, 17, '', 0, 0, 'C', 0);
			$pdf->Cell(30, 17, atktext('pdf_signature_center'), 'B', 0, 'C', 0);
			$pdf->Cell(30, 17, '', 0, 0, 'C', 0);
			$pdf->Cell(30, 17, atktext('pdf_signature_right'), 'B', 0, 'C', 0);

		}
		$pdf->set_font_regular();

		// ending

		// file name, Display "I"nline / or "D"ownload / F
		//$pdf->Output(atktext($this->m_type).'_'.date('d.m.Y').'.pdf', 'I');
		$pdf->Output($ticket_data['ticket_number'].'.pdf', 'I');

	}


	/*
	 * return tax consolidation
	 */
	function tax_consolidation($data)
	{
		$tax = array();

		foreach ($data as $row)
		{
			$tax[$row['fix_taxrate']]['base'] += $row['sum'];
			$tax[$row['fix_taxrate']]['value'] += $row['sum_with_tax'] - $row['sum'];
			$tax[$row['fix_taxrate']]['name'] = $row['fix_taxname'];
		}

		return $tax;
	}


	function getOurFirm()
	{
		$data = $this->db32select("SELECT * FROM ia32_config");
		return $data[0];
	}


	function getCustomerData($customer_id)
	{
		$data = $this->db32select("SELECT * FROM ia32_customers WHERE id = '".stripQuotes($customer_id)."'");
		return $data[0];
	}


	function getTicketData($ticket_id)
	{
		$data = $this->db32select("SELECT * FROM ia32_tickets WHERE id = '".stripQuotes($ticket_id)."'");
		return $data[0];
	}


	function getTotalSums($data, $doctype)
	{
		foreach ($data as $row)
		{

			if ($doctype=='receipt')
			{
				$sum += $row['buysum'];
				$tax += $row['buysum_with_tax'] - $row['buysum'];
				$sum_with_tax += $row['buysum_with_tax'];
			}else
			{
				$sum += $row['sum'];
				$tax += $row['sum_with_tax'] - $row['sum'];
				$sum_with_tax += $row['sum_with_tax'];
			}


		}

		return array('sum'=>$this->formatNumber($sum),
					 'tax'=>$this->formatNumber($tax), 
					 'sum_with_tax'=>$this->formatNumber($sum_with_tax));
	}

}

?>