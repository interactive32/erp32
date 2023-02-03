<?php

class report_customer_card extends ia32node
{

	function report_customer_card()
	{

		$this->atkNode("report_customer_card");

		$this->add(new atkManyToOneRelation("customer_id", "customers.customer", AF_RELATION_AUTOCOMPLETE));

		$this->add(new atkDateAttribute("date_from", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY));
		$this->add(new atkDateAttribute("date_to", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY));
			
			
	}


	function actionTitle($action, $record="")
	{
		return atktext($this->m_type);
	}


	function initial_values()
	{
		$tomorow  = mktime(0, 0, 0, date("m")  , date("d")+1, date("Y"));
		$default['date_from'] = date("Ymd", 0);
		$default['date_to'] = date("Ymd");

		return $default;
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
			$buttons .= '<input type="button" class="btn_save atkdefaultbutton" name="button" value="'.atktext('print_report').'" onClick="launch_pdf_with_params(this.form,\''.$url.'\')">';

			// export button
			$url = dispatch_url($this->atknodetype(), "pdfprint", array("export"=>"yes"));
			$buttons .= '<input type="button" class="btn_save atkdefaultbutton" name="button" value="'.atktext('download_CSV').'" onClick="launch_pdf_with_params(this.form,\''.$url.'\')">';
						
			
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

		$filter = explode("=",$g_sessionManager->stackVar('customer_id'));
		$customer = stripQuotes($filter[1]);

		if ($customer)
		{
			$sql_search = "AND ia32_customers.id = '$customer' ";
		}

		$filter_date_from = $g_sessionManager->stackVar('date_from');
		$date_from_mysql = dateutil::arr2str($filter_date_from,atktext('date_format_mysql'));
		$date_from_display = dateutil::arr2str($filter_date_from,atktext('date_format_pdfreport'));

		$filter_date_to = $g_sessionManager->stackVar('date_to');
		$date_to_mysql = dateutil::arr2str($filter_date_to,atktext('date_format_mysql'));
		$date_to_display = dateutil::arr2str($filter_date_to,atktext('date_format_pdfreport'));

		$display_filter = atktext('date_span').': '.$date_from_display.' - '.$date_to_display;

		$pdf = new atkpdf();

		$grouping_title = atktext('customer');

		$sql = "SELECT
		 		 CONCAT(ia32_customers.id, '|SEPARATOR|', '$grouping_title :', '|SEPARATOR|', ia32_customers.name) AS customer_id,
 				 ia32_tickets.date_created AS date, 
 				 ia32_tickets.date_payment AS payment_date,	  				 
 				 ia32_ticket_types.name AS ticket_type,  		 
 				 ia32_tickets.ticket_number,					 	
  				 ia32_tickets.ticket_sum AS amount,				 
  				 ia32_tickets.ticket_sum - ia32_tickets.ticket_payment AS amount_payment,
				 IF(ia32_tickets.date_payment < CURDATE(), ia32_tickets.ticket_sum - ia32_tickets.ticket_payment, 0) AS payment_exceeded,    				 
  			  	 ia32_tickets.id AS ticket_id
  				 
				FROM ia32_tickets 		
			    INNER JOIN ia32_customers ON ia32_customers.id = ia32_tickets.customer_id
				INNER JOIN ia32_ticket_types ON ia32_ticket_types.id = ia32_tickets.type_id				
 			     			    
 			    WHERE 
 			    ia32_tickets.date_created >= '$date_from_mysql' AND ia32_tickets.date_created <= '$date_to_mysql'
 			    AND (ia32_tickets.type_id = 1002 OR ia32_tickets.type_id = 1003 OR ia32_tickets.type_id = 1004)
 			    
 			    $sql_search
 			    
 			    ORDER BY ia32_customers.id, ia32_tickets.date_created";		

		$data = $this->db32select($sql);

		// calculate rows and fixes
		$this->fixRows($data);

		// export
		if($g_sessionManager->stackVar('export'))
		{
			// fix columns for export
			foreach($data as &$row) {
			$tmp = explode("|SEPARATOR|",$row['customer_id']);
			$row['customer_id'] = $tmp[2];
			unset($row['ticket_id']);
			}

			$headerCSV = array(
			"customer_id" => atktext('customer'),
			"date" => atktext('date'),
			"payment_date" => atktext('payment_short'),
			"ticket_type" => atktext('document_type'),
			"ticket_number" => atktext('ticket_number'),
			"amount" => atktext('money_amount'),
			"amount_payment" => atktext('payment_left'),
			"payment_exceeded" => atktext('payment_exceeded'));
			
			$formatCSV = array(
			"customer_id" => "string",
			"date" => "date",
			"payment_date" => "date",
			"ticket_type" => "string",
			"ticket_number" => "string",
			"amount" => "number",
			"amount_payment" => "number",
			"payment_exceeded" => "number",
			);			
		
		if($display_filter !== NULL) $output = $display_filter."\n\n";			
		$output .= $this->buildCSV($headerCSV,$data,$formatCSV);		

		$filename = atktext($this->m_type).'_'.date('d.m.Y');
		// @param String $type The type (csv / excel / xml)
		$type = "csv";
		$ext = "csv";

		$exporter = &atknew("atk.utils.atkfileexport");
		$exporter->export($output, $filename, $type, $ext, $compression);
			
		die;
		}
		
		
		// data formatting
		// 'x' = no formatting;
		// 'd' = date (2011-12-30);
		// 'dt' = datetime (2011-12-30 15:59);
		// 's:100' = string lenght is 100;
		// 5 = round with 5 decimal places
		$format = array('x','d','d','x','x',2,2,2,'x');

		// Column header titles
		$header = array(
		'id', 
		atktext('date'),
		atktext('payment_short'),
		atktext('document_type'),
		atktext('ticket_number'),
		atktext('money_amount'),
		atktext('payment_left'),
		atktext('payment_exceeded'),
		"");

		// Width od columns, Portrait_sum=180 Landscape_sum=267
		$width = array(0, 20, 20, 40, 40, 18, 20, 20, 0);

		// Alignment L/R/C
		$align = array("","L","L","L","L","R","R","R","");

		// Column Summary and Grouping
		// 1 = summary
		// 2 = grouping
		// 3 = grouping + summary only in groups
		// 4 = last value in group (npr tekuca zaliha ostaje koja je poslednja)
		$summary = array(2,0,0,0,0,0,0,0,0);

		// Report Title ; Portrait or Landcape
		$pdf->set_doc_properties(atktext($this->m_type).' '.atktext('on_day').': '.date(atktext('date_format_pdfreport')),'P');

		// display filter bellow title
		$pdf->set_font_bold();
		$pdf->Cell(0, $cheight, $display_filter, 0, 0, 'C', 0);
		$pdf->Ln(8);

		// Print table
		$pdf->print_table($header,$data,$width,$align,$summary,$format, true, true);
		
		// file name, Display "I"nline / or "D"ownload / F
		$pdf->Output(atktext($this->m_type).'_'.date('d.m.Y').'.pdf', 'I');
			
	}

	function fixRows(&$data)
	{

		$all =  array();

		foreach($data as &$row) {

			// fix ticket_type_name
			$row['ticket_type'] = atktext($row['ticket_type']);

			// reverse amounts on ivoice
			switch ($row['ticket_type'])
			{
				case 1004: // invoice
					$row['amount'] = -1 * $row['amount']; //?
					break;
			}

		}


		return;
	}
}
?>