<?php

class report_product_list extends ia32node
{

	function report_product_list()
	{

		$this->atkNode("report_product_list");

		$this->add(new atkManyToOneRelation("category_id", "products.category"));

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

		$filter = explode("=",$g_sessionManager->stackVar('category_id'));
		$category = stripQuotes($filter[1]);

		if ($category)
		{
			$sql_search = "WHERE ia32_products.category_id = '$category' ";
			$display_filter = atktext('category_id').': '.atktext($this->getCategoryName($category));
		}

		$pdf = new atkpdf();

		$location_id = $this->getCurrentLocationId();

		// ticket lines
		$sql = "SELECT
		ia32_products.id, 
		ia32_products.name, 
		ia32_products.barcode AS barcode, 
		ia32_units.symbol AS unit_symbol, 
		ia32_taxes.name AS tax_name, 	
		ia32_products.track_stock AS track_stock, 				
		ia32_category.name AS category_name
		
		FROM  ia32_products 
		INNER JOIN ia32_units ON ia32_units.id = ia32_products.unit_id
		INNER JOIN ia32_category ON ia32_category.id = ia32_products.category_id
		INNER JOIN ia32_taxes ON ia32_taxes.id = ia32_products.tax_id
	
		".$sql_search." 
		
		ORDER BY ia32_products.name";

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
			'name' => atktext('product'), 
			'barcode' => atktext('barcode'), 
			'unit_symbol' => atktext('unit_short'),
			'tax_name' => atktext('tax'), 
			'track_stock' => atktext('track_stock'), 
			'category_name' => atktext('category_id')
			);
			
			$formatCSV = array(
			'name' => "string", 
			'barcode' => "string", 
			'unit_symbol' => "string", 
			'tax_name' => "string", 
			'track_stock' => "string", 
			'category_name' => "string", 
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
		atktext('id'), 
		atktext('product'), 
		atktext('barcode'), 
		atktext('unit_short'), 
		atktext('tax'), 
		atktext('track_stock'), 
		atktext('category_id'));

		// Width od columns, Portrait_sum=180 Landscape_sum=267
		$width = array(0, 87, 30, 40, 20, 40, 50);

		// Alignment L/R/C
		$align = array("","L","L","C","R","C","L");

		// Column Summary and Grouping
		// 1 = summary
		// 2 = grouping
		// 3 = grouping + summary only in groups
		// 4 = last value in group (npr tekuca zaliha ostaje koja je poslednja)
		$summary = array(0,0,0,0,0,0,0);

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

	function fixRows(&$data)
	{

		foreach($data as &$row) {

			// fix multilanguage doctypes
			$row['category_name'] = atktext($row['category_name']);
			$row['unit_symbol'] = atktext($row['unit_symbol']);


			if ($row['track_stock'] == '1')
			$row['track_stock'] = atktext('yes');
			else
			$row['track_stock'] = atktext('no');

		}

	}


}

?>