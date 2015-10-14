<?php

class report_pricelist extends ia32node
{

	function report_pricelist()
	{

		$this->atkNode("report_pricelist");

		$this->add(new atkManyToOneRelation("pricelist_id", "pricelist.price_list"));

		$this->add(new atkBoolAttribute("group_by_prices"));

	}


	/*
	 * load initial values - add mode
	 */
	function initial_values()
	{
		$ret["group_by_prices"] = 1;
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

		$filter = explode("=",$g_sessionManager->stackVar('pricelist_id'));
		$price_list = stripQuotes($filter[1]);

		$group_by_prices = $g_sessionManager->stackVar('group_by_prices');
		if ($group_by_prices === null) $group_by_prices = '1';


		if ($price_list)
		{
			$sql_search = "WHERE ia32_prices.pricelist_id = '$price_list' ";
			$group_by_prices = '1';
		}

		if ($group_by_prices == '1')
		{
			$order = 'ORDER BY pricelist_name, ia32_products.name';
			$summary = array(0,2,0,0,0,0,0);
			$width = array(65, 0, 20, 25, 20, 25, 25);
		}
		else
		{
			$order = 'ORDER BY ia32_products.name, pricelist_name';
			$summary = array(0,0,0,0,0,0,0);
			$width = array(35, 30, 20, 25, 20, 25, 25);			
		}


		$pdf = new atkpdf();

		$sql = "SELECT
		
		ia32_products.name AS product_name,	
		ia32_price_list.name AS pricelist_name,		
		ia32_taxes.name AS tax_name,		
		ia32_prices.pricebuy,
	    (ia32_prices.pricesell - ia32_prices.pricebuy) / ia32_prices.pricebuy * 100 AS margin,
		ia32_prices.pricesell,
		ia32_prices.pricesell + ia32_prices.pricesell * ia32_taxes.rate AS info_pricetax	

		FROM  ia32_prices
		INNER JOIN ia32_products ON ia32_products.id = ia32_prices.product_id
		INNER JOIN ia32_price_list ON ia32_price_list.id = ia32_prices.pricelist_id
		INNER JOIN ia32_taxes ON ia32_taxes.id = ia32_products.tax_id
			
		".$sql_search." 
		
		$order";

		$data = $this->db32select($sql);

		$this->fixRows($data);
		

		// export
		if($g_sessionManager->stackVar('export'))
		{

			$headerCSV = array(
			"product_name" => atktext('product'),
			"pricelist_name" => atktext('pricelist_id'),
			"tax_name" => atktext('tax'),
			"pricebuy" => atktext('pricebuy'),
			"margin" => atktext('info_margin'),
			"pricesell" => atktext('pricesell_short'),			
			"info_pricetax" => atktext('info_pricetax_short'));
			
			$formatCSV = array(
			"product_name" => "string",
			"pricelist_name" => "string",
			"tax_name" => "string",
			"pricebuy" => "number",
			"margin" => "number",
			"pricesell" => "number",			
			"info_pricetax" => "number"
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
		$format = array('x','x','x',2,2,2,2);

		// Column header titles
		$header = array(
		atktext('product'),
		atktext('pricelist_id'),		
		atktext('tax'),
		atktext('pricebuy'),
		atktext('info_margin'),
		atktext('pricesell_short'),
		atktext('info_pricetax_short'));


		// Alignment L/R/C
		$align = array("L","L","R","R","R","R","R");

		// Report Title ; Portrait or Landcape
		$pdf->set_doc_properties(atktext($this->m_type),'P');

		// display filter bellow title
		$custumer_type_name = $this->getCustomerTypeData($customer_type);
		if ($custumer_type_name['name'])
		{
			$pdf->set_font_bold();
			$pdf->Cell(0, $cheight, atktext('customer_type').': '.atktext($custumer_type_name['name']), 0, 0, 'C', 0);
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
			$row['pricelist_name'] = atktext($row['pricelist_name']);

		}

	}


}

?>