<?php

class report_product_card extends ia32node
{

	function report_product_card()
	{

		$this->atkNode("report_product_card");

		$this->add(new atkManyToOneRelation("product_id", "products.product", AF_RELATION_AUTOCOMPLETE));
		$this->add(new atkManyToOneRelation("category_id", "products.category"));

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

		$filter_1 = explode("=",$g_sessionManager->stackVar('category_id'));
		$filter_2 = explode("=",$g_sessionManager->stackVar('product_id'));
		$category = stripQuotes($filter_1[1]);
		$product = stripQuotes($filter_2[1]);

		$filter_date_from = $g_sessionManager->stackVar('date_from');
		$date_from_mysql = dateutil::arr2str($filter_date_from,atktext('date_format_mysql'));
		$date_from_display = dateutil::arr2str($filter_date_from,atktext('date_format_pdfreport'));

		$filter_date_to = $g_sessionManager->stackVar('date_to');
		$date_to_mysql = dateutil::arr2str($filter_date_to,atktext('date_format_mysql'));
		$date_to_display = dateutil::arr2str($filter_date_to,atktext('date_format_pdfreport'));

		$location_data = $this->getLocationData();
		$location_id = $location_data['id'];
		$location_name = $location_data['name'];

		$display_filter_category = $this->getCategoryName($category);
		$display_filter_product = $this->getProductName($product);
		$display_filter = atktext('date_span').': '.$date_from_display.' - '.$date_to_display;
		if ($display_filter_category) $display_filter .= ', '.atktext('category_id').': '.$display_filter_category;
		if ($display_filter_product) $display_filter .= ', '.atktext('product_id').': '.$display_filter_product;

		if ($category)
		{
			$sql_search .= "AND ia32_products.category_id = '$category' ";
		}

		if ($product)
		{
			$sql_search .= "AND ia32_products.id = '$product' ";
		}

		$pdf = new atkpdf();

		$grouping_title = atktext('product');

		// ticket lines
		$sql = "SELECT
		 		 CONCAT(ia32_products.id, '|SEPARATOR|', '$grouping_title :', '|SEPARATOR|', ia32_products.name) AS product_id,
 				 ia32_tickets.date_created, 
 				 ia32_products.name AS product_name,
 				 ia32_tickets.ticket_number,			 
 				 ia32_ticket_types.name AS ticket_type_name, 
 				 ia32_tickets.type_id AS ticket_type,
  				 ia32_ticket_lines.fix_price, 
  				 ia32_ticket_lines.fix_pricebuy,
  				 ia32_ticket_lines.units,
  				 0 AS stock
  				 
				FROM ia32_ticket_lines 
				INNER JOIN ia32_products ON ia32_ticket_lines.product_id = ia32_products.id
 			    INNER JOIN ia32_tickets ON ia32_tickets.id = ia32_ticket_lines.ticket_id
 			    INNER JOIN ia32_ticket_types ON ia32_ticket_types.id = ia32_tickets.type_id
 			    
 			    WHERE 
 			    ia32_tickets.date_created >= '$date_from_mysql' AND ia32_tickets.date_created <= '$date_to_mysql'
 			    
 			    AND ia32_products.track_stock = '1'
 			    
 			    AND (ia32_tickets.type_id = 1002 
  			      OR ia32_tickets.type_id = 1003 			      
  			      OR ia32_tickets.type_id = 1004 	
  			      OR ia32_tickets.type_id = 1006
  			      OR ia32_tickets.type_id = 1007
  			      OR (ia32_tickets.type_id = 1008 AND ia32_tickets.status_id = 1001)
  			      )	

  			    AND ia32_tickets.location_id = '".$location_id." '
 			      ".$sql_search." 
 			    
 			    ORDER BY ia32_products.id, ia32_tickets.date_created";

		$data = $this->db32select($sql);

		// calculate rows and fixes
		$this->fixRows($data);

		// export
		if($g_sessionManager->stackVar('export'))
		{
			// fix columns for export
			foreach($data as &$row) {
			unset($row['product_id']);
			unset($row['ticket_type']);
			}
						
			$headerCSV = array(
			'date_created' => atktext('date'), 
			'product_name' => atktext('product'), 
			'ticket_number' => atktext('ticket_number'), 
			'ticket_type_name' => atktext('document_type'),
			'fix_price' => atktext('pricesell_short'), 
			'fix_pricebuy' => atktext('pricebuy_short'), 
			'units' => atktext('units'), 
			'stock' => atktext('stock')
			);
			
			$formatCSV = array(
			'date_created' => "date", 
			'product_name' => "string", 
			'ticket_number' => "string", 
			'ticket_type_name' => "string", 
			'fix_price' => "number", 
			'fix_pricebuy' => "number", 
			'units' => "number", 
			'stock' => "number"
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
		$format = array('x','d','x','x','x','x',2,2,2,2);

		// Column header titles
		$header = array(
		'id', 
		atktext('date'), 
		atktext('product'), 
		atktext('ticket_number'), 
		atktext('document_type'), 
		atktext('type_id'), 
		atktext('pricesell_short'), 
		atktext('pricebuy_short'), 
		atktext('units'), 
		atktext('stock'));

		// Width od columns, Portrait_sum=180 Landscape_sum=267
		$width = array(0, 20, 0, 30, 30, 0, 25, 25, 25, 25);

		// Alignment L/R/C
		$align = array("","L","L","L","L","L","R","R","R","R");

		// Column Summary and Grouping
		// 1 = summary
		// 2 = grouping
		// 3 = grouping + summary only in groups
		// 4 = last value in group (npr tekuca zaliha ostaje koja je poslednja)
		$summary = array(2,0,0,0,0,0,0,0,0,4);

		// Report Title ; Portrait or Landcape
		$pdf->set_doc_properties(atktext($this->m_type),'P');

		// display filter bellow title
		$pdf->set_font_bold();
		$pdf->Cell(0, $cheight, $location_name, 0, 0, 'C', 0);
		$pdf->Ln(8);
		$pdf->Cell(0, $cheight, $display_filter, 0, 0, 'C', 0);
		$pdf->Ln(10);

		// Print table
		$pdf->print_table($header,$data,$width,$align,$summary,$format, true, true);

		// file name, Display "I"nline / or "D"ownload / F
		$pdf->Output(atktext($this->m_type).'_'.date('d.m.Y').'.pdf', 'I');
			
	}

	function fixRows(&$data)
	{
		$grouping_value = $data[0]['product_id'];

		foreach($data as &$row) {

			// fix ticket_type_name
			$row['ticket_type_name'] = atktext($row['ticket_type_name']);

			switch ($row['ticket_type'])
			{
				case 1004: // invoice
				case 1007: // internal transfer - out
					$row['units'] = -1 * $row['units'];
					break;

				case 1006: // inventory
					$previous = 0;
					break;

			}

			if ($grouping_value == $row['product_id'])
			{
				$row['stock'] = $previous + $row['units'];
				if (!$row['stock']) $row['stock'] = '0';
				$previous = $row['stock'];
			}else
			{
				$grouping_value = $row['product_id'];
				$previous = $row['stock'] = $row['units'];
			}
		}

		return;
	}
}
?>