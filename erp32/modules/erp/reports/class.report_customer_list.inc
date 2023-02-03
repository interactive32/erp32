<?php

class report_customer_list extends ia32node
{

	function report_customer_list()
	{

		$this->atkNode("report_customer_list");

		$this->add(new atkManyToOneRelation("customer_type", "customers.customer_types"));

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

		$filter = explode("=",$g_sessionManager->stackVar('customer_type'));

		$customer_type = stripQuotes($filter[1]);

		if ($customer_type)
		{
			$sql_search = "WHERE ia32_customers.type_id = '$customer_type' ";

			$type_name = $this->getCustomerTypeData($customer_type);
			$display_filter = atktext('customer_type').': '.atktext($type_name['name']);

		}

		$pdf = new atkpdf();

		$sql = "SELECT
		
		ia32_customers.name, 
		ia32_customers.taxid, 
		CONCAT(ia32_customers.zip, ' ', ia32_customers.city) AS city, 
		ia32_customers.address, 
		ia32_customers.telephone, 
		ia32_customers.email

		FROM  ia32_customers
		".$sql_search." 
		
		ORDER BY ia32_customers.name";

		$data = $this->db32select($sql);

		// export
		if($g_sessionManager->stackVar('export'))
		{
			
			$headerCSV = array(
			"name" => atktext('customer'),
			"taxid" => atktext('taxid'),
			"city" => atktext('city'),
			"address" => atktext('address'),
			"telephone" => atktext('telephone'),
			"email" => atktext('email')
			);
			
			$formatCSV = array(
			"name" => "string",
			"taxid" => "string",
			"city" => "string",
			"address" => "string",
			"telephone" => "string",
			"email" => "string",
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

		
		// data formatting (x = no formatting; d = date; s:100 = string lenght is 100;  5 = round with 5 decimal places)
		$format = array('x','x','x','x','x','x','x');

		// Column header titles
		$header = array(
		atktext('customer'),
		atktext('taxid'),
		atktext('city'),
		atktext('address'),
		atktext('telephone'),
		atktext('email'));

		// Width od columns, Portrait_sum=180 Landscape_sum=267
		$width = array(67, 20, 40, 50, 40, 50);

		// Alignment L/R/C
		$align = array("L","L","L","L","L","L");

		// Column Summary and Grouping
		// 1 = summary
		// 2 = grouping
		// 3 = grouping + summary only in groups
		// 4 = last value in group (npr tekuca zaliha ostaje koja je poslednja)
		$summary = array(0,0,0,0,0,0);

		// Report Title ; Portrait or Landcape
		$pdf->set_doc_properties(atktext($this->m_type),'L');

		// display filter bellow title
		if ($display_filter)
		{
			$pdf->set_font_bold();
			$pdf->Cell(0, $cheight, $display_filter, 0, 0, 'C', 0);
			$pdf->Ln(8);
		}

		// Print table
		$pdf->print_table($header,$data,$width,$align,$summary,$format, true, true);

		// file name, Display "I"nline / or "D"ownload / F
		$pdf->Output(atktext($this->m_type).'_'.date('d.m.Y').'.pdf', 'I');
			
	}



}

?>