<?php

class report_finance_list extends ia32node
{

	function report_finance_list()
	{
		$this->atkNode("report_finance_list");

		$this->add(new atkManyToOneRelation("finance_types", "finance.finance_types"));

		$this->add(new atkManyToOneRelation("customer_id", "customers.customer", AF_RELATION_AUTOCOMPLETE));

		$this->add(new atkDateAttribute("date_from", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY));
		$this->add(new atkDateAttribute("date_to", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY));

	}


	function initial_values()
	{
		$tomorow  = mktime(0, 0, 0, date("m")  , date("d")+1, date("Y"));
		$default['date_from'] = date("Ymd", 0);
		$default['date_to'] = date("Ymd");

		return $default;
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

		$filter = explode("=",$g_sessionManager->stackVar('finance_types'));
		$finance_id= stripQuotes($filter[1]);

		$filter = explode("=",$g_sessionManager->stackVar('customer_id'));
		$customer = stripQuotes($filter[1]);

		if ($finance_id)
		{
			$sql_search .= "AND ia32_finance_types.id = '$finance_id' ";
			$display_filter1 .= atktext('finance_types').': '.atktext($this->getFinanceTypeName($finance_id));
			
			if ($customer) $display_filter1 .= ', ';
		}

		if ($customer)
		{
			$sql_search .= "AND ia32_finance.customer_id = '$customer' ";
			$display_filter1 .= atktext('customer_id').': '.atktext($this->getCustomerName($customer));
		}

		$filter_date_from = $g_sessionManager->stackVar('date_from');
		$date_from_mysql = dateutil::arr2str($filter_date_from,atktext('date_format_mysql'));
		$date_from_display = dateutil::arr2str($filter_date_from,atktext('date_format_pdfreport'));

		$filter_date_to = $g_sessionManager->stackVar('date_to');
		$date_to_mysql = dateutil::arr2str($filter_date_to,atktext('date_format_mysql'));
		$date_to_display = dateutil::arr2str($filter_date_to,atktext('date_format_pdfreport'));

		$display_filter2 = atktext('date_span').': '.$date_from_display.' - '.$date_to_display;

		$pdf = new atkpdf();

		$location_id = $this->getCurrentLocationId();

		// ticket lines
		$sql = "SELECT
		ia32_finance.id, 
		ia32_finance.date,	
		ia32_finance.creator, 			
		ia32_finance_types.name AS finance_type_name, 	
		ia32_customers.name AS customer_name, 				
		ia32_tickets.ticket_number, 
		ia32_finance.amount
				
		FROM  ia32_finance 
		INNER JOIN ia32_finance_types ON ia32_finance_types.id = ia32_finance.type_id
		LEFT JOIN ia32_tickets ON ia32_tickets.id = ia32_finance.ticket_id
		LEFT JOIN ia32_customers ON ia32_customers.id = ia32_finance.customer_id
	
		WHERE
		ia32_finance.date >= '$date_from_mysql' AND ia32_finance.date <= '$date_to_mysql'
 			 
		".$sql_search." 
		
		ORDER BY ia32_finance.date";

		$data = $this->db32select($sql);

		$this->fixRows($data);

		// export
		if($g_sessionManager->stackVar('export'))
		{
			// fix columns for export
			foreach($data as &$row) {
			unset($row['id']);
			}
						
		$headerCSV = array(
		"date" => atktext('date'),
		"creator" => atktext('creator'),
		"finance_type_name" => atktext('finance_type'),
		"customer_name" => atktext('customer_id'),
		"ticket_number" => atktext('document_link'),
		"amount" => atktext('money_amount')
		);
		
		$formatCSV = array(
		"date" => "date",
		"creator" => "string",
		"finance_type_name" => "string",
		"customer_name" => "string",
		"ticket_number" => "string",
		"amount" => "number"
		);		

		$display_filter = $display_filter2." ".$display_filter1;
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
				
		// data formatting (x = no formatting; d = date; s:100 = string lenght is 100;  5 = round with 5 decimal places)
		$format = array('x','d','x','x','x','x',2);

		// Column header titles
		$header = array(
		atktext('id'),
		atktext('date'),
		atktext('creator'),
		atktext('finance_type'),
		atktext('customer_id'),
		atktext('document_link'),
		atktext('money_amount'));

		// Width od columns, Portrait_sum=180 Landscape_sum=267
		$width = array(0, 20, 20, 40, 40, 30, 30);

		// Alignment L/R/C
		$align = array("","L","L","L","L","L","R");

		// Column Summary and Grouping
		// 1 = summary
		// 2 = grouping
		// 3 = grouping + summary only in groups
		// 4 = last value in group (npr tekuca zaliha ostaje koja je poslednja)
		$summary = array(0,0,0,0,0,0,1);

		// Report Title ; Portrait or Landcape
		$pdf->set_doc_properties(atktext($this->m_type),'P');

		// display filter bellow title
		if ($display_filter1)
		{
			$pdf->set_font_bold();
			$pdf->Cell(0, $cheight, $display_filter1, 0, 0, 'C', 0);
			$pdf->Ln(8);
		}
		if ($display_filter2)
		{
			$pdf->set_font_bold();
			$pdf->Cell(0, $cheight, $display_filter2, 0, 0, 'C', 0);
			$pdf->Ln(8);
		}

		// Print table
		$pdf->print_table($header,$data,$width,$align,$summary,$format, true, true);

		// file name, Display "I"nline / or "D"ownload / F
		$pdf->Output(atktext($this->m_type).'_'.date('d.m.Y').'.pdf', 'I');
			
	}

	function fixRows(&$data)
	{

		foreach($data as &$row) {

			// fix multilanguage doctypes
			$row['finance_type_name'] = atktext($row['finance_type_name']);

			// show creator only - no date
			$creator = explode("@",$row['creator']);
			$row['creator'] = $creator[0];

		}

	}


}

?>