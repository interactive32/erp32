<?php

class report_inventory_list extends ia32node
{

	function report_inventory_list()
	{

		$this->atkNode("report_inventory_list");

		$this->add(new atkManyToOneRelation("category_id", "products.category"));

		$this->add(new atkBoolAttribute("show_current_stock"));

	}


	/*
	 * load initial values - add mode
	 */
	function initial_values()
	{
		$ret["show_current_stock"] = 1;
		return $ret;
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

		$show_current_stock = $g_sessionManager->stackVar('show_current_stock');
		if ($show_current_stock === null) $show_current_stock = '1';

		if ($category)
		{
			$sql_search = "AND ia32_products.category_id = '$category' ";
			$display_filter = atktext('category_id').': '.atktext($this->getCategoryName($category));
		}

		$pdf = new atkpdf();

		$location = $this->getLocationData();
		$location_id = $location['id'];
		$location_name = $location['name'];

		// ticket lines
		$sql = "SELECT
		ia32_products.id, 
		ia32_category.name AS category_name,		
		ia32_products.name, 
		ia32_products.barcode AS barcode, 
		ia32_units.symbol AS unit_symbol,	
		ia32_stockcurrent.amount + ia32_stockcurrent.reservation AS amount,						
		ia32_stockcurrent.amount AS available,
		ia32_stockcurrent.reservation AS on_reservation,
		null AS on_inventory
		
		FROM  ia32_products 
		INNER JOIN ia32_units ON ia32_units.id = ia32_products.unit_id
		INNER JOIN ia32_category ON ia32_category.id = ia32_products.category_id
		INNER JOIN ia32_stockcurrent ON ia32_stockcurrent.product_id = ia32_products.id
	
		WHERE ia32_products.track_stock = 1 AND ia32_stockcurrent.location_id = '$location_id'
		".$sql_search." 
		
		ORDER BY ia32_category.name, ia32_products.name";

		$data = $this->db32select($sql);

		$this->fixRows($data);

				
		// data formatting (x = no formatting; d = date; s:100 = string lenght is 100;  5 = round with 5 decimal places)
		$format = array('x','x','x','x','x', 2, 2, 2, 'x');

		// Column header titles
		$header = array(
		atktext('id'),
		atktext('category_id'),
		atktext('name'),
		atktext('barcode'),
		atktext('unit_short'),
		atktext('amount'),
		atktext('amount_available'),
		atktext('on_reservation'),
		atktext('amount_on_inventory_short'));

		// Width od columns, Portrait_sum=180 Landscape_sum=267
		if ($show_current_stock == '1')
		$width = array(0, 20, 40, 30, 10, 20, 20, 20, 20);
		else
		$width = array(0, 30, 70, 40, 10, 0, 0, 0, 30);

		// Alignment L/R/C
		$align = array("","L","L","C","C","R","R","R","C");

		// Column Summary and Grouping
		// 1 = summary
		// 2 = grouping
		// 3 = grouping + summary only in groups
		// 4 = last value in group (npr tekuca zaliha ostaje koja je poslednja)
		$summary = array(0,0,0,0,0,0,0,0,0);

		// Report Title ; Portrait or Landcape
		$pdf->set_doc_properties(atktext($this->m_type).' - '.$location_name,'P');

		// display filter bellow title
		if ($display_filter)
		{
			$pdf->set_font_bold();
			$pdf->Cell(0, $cheight, $display_filter, 0, 0, 'C', 0);
			$pdf->Ln(8);
		}

		// Print table
		$pdf->print_table($header,$data,$width,$align,$summary,$format, true, true);

		// Print signatures
		$pdf->Ln(10);
		$pdf->set_font_bold();
		$pdf->Cell(130, 17, '', 0, 0, 'C', 0);
		$pdf->Cell(30, $cheight, atktext('inventory_signature').':', '', 0, 'C', 0);
			
			
		// file name, Display "I"nline / or "D"ownload / F
		$pdf->Output(atktext($this->m_type).'_'.date('d.m.Y').'.pdf', 'I');
			
	}

	function fixRows(&$data)
	{

		foreach($data as &$row) {

			// fix multilanguage doctypes
			$row['category_name'] = atktext($row['category_name']);
			$row['unit_symbol'] = atktext($row['unit_symbol']);

		}

	}


}

?>