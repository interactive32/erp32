<?php

class report_product_stockinfo extends ia32node
{

	function report_product_stockinfo()
	{

		$this->atkNode("report_product_stockinfo");

		$this->add(new atkManyToOneRelation("category_id", "products.category"));
		$this->add(new atkManyToOneRelation("location_id", "locations.location"));

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

		$filter = explode("=",$g_sessionManager->stackVar('location_id'));
		$location_id = stripQuotes($filter[1]);

		$display_filter_location = $this->getLocationData($location_id);
		$display_filter_category = $this->getCategoryName($category);
		if ($location_id) $display_filter = atktext('location_id').': '.$display_filter_location['name'];
		if ($category && $location_id) $display_filter .= ', ';
		if ($category) $display_filter .= atktext('category_id').': '.atktext($display_filter_category);


		if ($category)
		{
			$sql_search = "WHERE ia32_products.category_id = '$category' ";
		}


		if ($location_id && $category)
		{
			$sql_search .= "AND ia32_stockcurrent.location_id = '$location_id' ";
		}
		elseif ($location_id)
		{
			$sql_search = "WHERE ia32_stockcurrent.location_id = '$location_id' ";
		}


		$pdf = new atkpdf();

		// ticket lines
		$sql = "SELECT
		ia32_products.id, 
		ia32_products.name, 
		ia32_locations.name AS location,
		(ia32_stockcurrent.amount + ia32_stockcurrent.reservation) AS on_stock,
		ia32_stockcurrent.amount AS available,
		ia32_stockcurrent.reservation AS on_reservation,
		ia32_stockcurrent.on_order AS on_order,
		ia32_stockcurrent.min_stock AS min_stock,			
		null AS info_min,
		ia32_stockcurrent.max_stock AS max_stock,
		null AS info_max,
		ia32_prices.pricebuy,
		ia32_prices.pricesell,		
		ia32_prices.pricebuy * (ia32_stockcurrent.amount + ia32_stockcurrent.reservation) AS buy_value,
		ia32_prices.pricesell * (ia32_stockcurrent.amount + ia32_stockcurrent.reservation) AS sell_value		
		
		FROM  ia32_stockcurrent 
		INNER JOIN ia32_products ON ia32_stockcurrent.product_id = ia32_products.id		
		INNER JOIN ia32_units ON ia32_units.id = ia32_products.unit_id
		INNER JOIN ia32_category ON ia32_category.id = ia32_products.category_id
		INNER JOIN ia32_taxes ON ia32_taxes.id = ia32_products.tax_id
		INNER JOIN ia32_locations ON ia32_locations.id = ia32_stockcurrent.location_id
		INNER JOIN ia32_price_list ON ia32_price_list.id = ia32_locations.pricelist_id
		INNER JOIN ia32_prices ON ia32_prices.pricelist_id = ia32_price_list.id AND ia32_prices.product_id = ia32_products.id
				
		".$sql_search." 
		
		ORDER BY ia32_products.name, ia32_locations.name";


		$data = $this->db32select($sql);

		$this->fixRows($data);

		// export
		if($g_sessionManager->stackVar('export'))
		{
			// fix columns for export
			foreach($data as &$row) {
			unset($row['id']);
			unset($row['info_min']);
			unset($row['info_max']);
			unset($row['category_name']);
			unset($row['unit_symbol']);	
			}
						
			$headerCSV = array(
			'name' => atktext('product'),
			'location' => atktext('location_id'),
			'on_stock' => atktext('on_stock_sum'),
			'available' => atktext('amount_available'),
			'on_reservation' => atktext('reservation'),
			'min_stock' => atktext('min'),			
			'max_stock' => atktext('max'),
			'pricebuy' => atktext('pricebuy_short'),
			'pricesell' => atktext('pricesell_short'),
			'buy_value' => atktext('buy_value_short'),
			'sell_value' => atktext('sell_value_short')
			);
			
			$formatCSV = array(
			'name' => "string",
			'location' => "string",
			'on_stock' => "number",
			'available' => "number",
			'on_reservation' => "number",
			'min_stock' => "number",	
			'max_stock' => "number",
			'pricebuy' => "number",
			'pricesell' => "number",
			'buy_value' => "number",
			'sell_value' => "number",
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
		$format = array('x','x','x',2,2,2,2,2,'x',2,'x',2,2,2,2);

		// Column header titles
		$header = array(
		atktext('id'),
		atktext('product'),
		atktext('location_id'),
		atktext('on_stock_sum'),
		atktext('amount_available'),
		atktext('reservation'),
		atktext('on_order'),			
		atktext('min'),
		"", 
		atktext('max'),
		"",
		atktext('pricebuy_short'),
		atktext('pricesell_short'),				
		atktext('buy_value_short'),
		atktext('sell_value_short'));

		// Width od columns, Portrait_sum=180 Landscape_sum=267
		$width = array(0, 47, 30, 20, 20, 20, 20, 15, 1, 15, 1, 20, 20, 20, 20);

		// Alignment L/R/C
		$align = array("","L","L","R","R","R","R","R","R","R","R","R","R","R","R");

		// Column Summary and Grouping
		// 1 = summary
		// 2 = grouping
		// 3 = grouping + summary only in groups
		// 4 = last value in group (npr tekuca zaliha ostaje koja je poslednja)
		$summary = array(0,0,0,0,0,0,0,0,0,0,0,0,0,1,1);

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

			// show info "!" when min/max is reached
			if ($row['min_stock'] && $row['available'] <= $row['min_stock']) $row['info_min'] = '!';
			if ($row['max_stock'] && $row['available'] >= $row['max_stock']) $row['info_max'] = '!';

		}

	}


}

?>