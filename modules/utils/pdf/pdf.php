<?php
//@set_time_limit(240);
/*******************************************************************************
 * FPDF MOD 4 ATK                                                              *
 *                                                                             *
 * Author:  Alcal                                                              *
 *******************************************************************************/
require('tcpdf/config/lang/eng.php');

if (atkconfig('compress_pdf') == true)
	require('tcpdf/tcpdf_compressed.php');
else
	require('tcpdf/tcpdf.php');

//define('FPDF_FONTPATH',$_SERVER['DOCUMENT_ROOT'].'/tokyo/moduli/pdf/fpdf/font/');

// dejavusans = utf8 **very slow!, helvetica = non-utf8 fast!
// TCPDF function _putresources() -> _putfonts() @8830 is slow for utf8
define('MAIN_ATK_FONT', 'dejavusans');

class atkpdf extends TCPDF
{
	public function atkpdf()
	{
		//call original constructor
		$this->__construct(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);
	}


	function set_doc_properties($report_title, $orientation='P',$header=array(),$image='',$imagewidth=0)
	{

		//define ('DATE_FORMATTING', 'd/M/Y H:i');
		define ('DATE_FORMATTING', atktext('date_format_pdfreport'));

		define ('ATK_PDF_SETCREATOR', $header['CREATOR']);
		define ('ATK_PDF_SETAUTHOR', $header['AUTHOR']);
		define ('ATK_PDF_SETTITLE', $header['TITLE']);
		define ('ATK_PDF_SETSUBJECT', $header['SUBJECT']);

		// settings from atk_pdf_config.php
		$this->SetCreator(ATK_PDF_SETCREATOR);
		$this->SetAuthor(ATK_PDF_SETAUTHOR);
		$this->SetTitle(ATK_PDF_SETTITLE);
		$this->SetSubject(ATK_PDF_SETSUBJECT);
		$this->SetKeywords(ATK_PDF_SETKEYWORDS);

		$this->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);
		$this->setImageScale(PDF_IMAGE_SCALE_RATIO);


		$language_meta = Array();
		$language_meta['a_meta_charset'] = 'UTF-8';
		$language_meta['w_datetime'] = '';
		$language_meta['w_page'] = atktext('pdf_page_num');
		$this->setLanguageArray($language_meta);

		$this->setFontSubsetting(true);

		//set margins
		$this->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
		$this->SetHeaderMargin(PDF_MARGIN_HEADER);
		$this->SetFooterMargin(PDF_MARGIN_FOOTER);

		$this->setHeaderFont(Array(MAIN_ATK_FONT, '', 8, '', true));
		$this->setFooterFont(Array(MAIN_ATK_FONT, '', 8, '', true));
		$this->SetHeaderData($image, $imagewidth, ATK_PDF_SETAUTHOR,ATK_PDF_SETTITLE,ATK_PDF_SETSUBJECT);
		$this->SetHeaderExtraData($report_title);

		$this->set_font_regular();

		$this->AddPage($orientation);
	}


	function set_font_regular()
	{
		$this->SetFont(MAIN_ATK_FONT, '', 8, '', true);
	}

	function set_font_bold()
	{
		$this->SetFont(MAIN_ATK_FONT, 'B', 8, '', true);
	}


	function print_table($header, $data, $width, $align, $summary_and_grouping, $format, $usefill=false, $useheaderfill=false)
	{

		$this->check_report_params($data, $summary_and_grouping);

		// Colors, line width and bold font

		//if ($useheaderfill)
		//$this->SetFillColor(222, 222, 222); // header fill is gray
		//else
		//$this->SetFillColor(255, 255, 255); // header fill is white

		$this->SetTextColor(0);
		//$this->SetDrawColor(61, 61, 61);
		$this->SetLineWidth(0.3);

		// if not gouping, draw header only once
		if (!in_array(2,$summary_and_grouping) && !in_array(3,$summary_and_grouping))
		{
			if ($useheaderfill)
			$this->SetFillColor(224, 235, 255); // fill is blue
			else
			$this->SetFillColor(255, 255, 255); // header fill is white

			$this->draw_header($header, $width, $align, $summary_and_grouping);
		}


		// Color and font restoration
		if ($usefill)
		$this->SetFillColor(224, 235, 255); // fill is blue
		else
		$this->SetFillColor(255, 255, 255); // fill is white

		$this->SetTextColor(0);
		$this->set_font_regular();

		// Data
		$scaling = 1; // horizontal scaling if necessary
		$fill = 0;
		$row_counter = 0;
		$NumOfRows = count($data[0]); // count rows on first data


		foreach($data as $row) {

			$key = array_search(2, $summary_and_grouping);
			$header_key = array_keys($row);

			// if grouping start
			//
			if (in_array(2,$summary_and_grouping) || in_array(3,$summary_and_grouping))
			{

				// first time: draw header and set grouping value
				if($row_counter == 0)
				{
					$this->draw_header($header, $width, $align, $summary_and_grouping,$row[$header_key[$key]]);
					$prev_grouping_value = $row[$header_key[$key]];
				}

				// grouping value from row
				$grouping_value = $row[$header_key[$key]];

				// if grouping value has changed!
				if ($prev_grouping_value != $grouping_value)
				{
					$fill = 0;

					// summary for previous group
					if (array_search(3, $summary_and_grouping) || array_search(4, $summary_and_grouping))
					{

						$this->draw_group_summary($NumOfRows, $sum, $align, $width, $summary_and_grouping, $last_group_value);

						// and reset grouping sum
						$grand_sum[] = $sum;
						unset($sum);
					} else {
						$this->Cell(array_sum($width), 0, '', 'T'); // finish with top line
					}


					$this->Ln(); $this->Ln();

					$prev_grouping_value = $row[$header_key[$key]]; //$grouping_value;
					$this->draw_header($header, $width, $align, $summary_and_grouping, $grouping_value);
				}

			}
			// grouping end


			// draw row
			for ($i=0; $i < $NumOfRows ; $i++)	{

				if ($i == 0){
					$row_data = current($row);
				} else	{
					$row_data = next($row);
				}
					
				$sum[$i] =  $sum[$i] + $row_data;
				$this->format($row_data,$format[$i]);

				// do not display hidden and grouping row
				if($width[$i]<>0 && $summary_and_grouping[$i]<>2)
				$this->Cell($width[$i], 6, $row_data, '', 0, $align[$i], $fill,'',$scaling);
			}

			// if report uses last value in group then retain that field
			if (in_array(4,$summary_and_grouping)) {
				$lv_idx = array_search(4, $summary_and_grouping);
				$lv_name = array_keys($row);
				$last_group_value = $row[$lv_name[$lv_idx]];
			}

			$this->Ln();
			$fill=!$fill;
			$row_counter += 1;
		}

		// ending summary for last group
		if (array_search(3, $summary_and_grouping) || array_search(4, $summary_and_grouping))
		$this->draw_group_summary($NumOfRows, $sum, $align, $width, $summary_and_grouping, $last_group_value);

		// create grand_sum from sum
		$grand_sum[] = $sum;
		if ($grand_sum[1]) // if grouping sum
		{
			for ($i=0; $i < count($grand_sum) ; $i++)	{
				for ($z=0; $z < $NumOfRows ; $z++)	{
					$grand_sum_c[$z] += $grand_sum[$i][$z];
				}
			}
		} else $grand_sum_c = $sum;

		$this->Cell(array_sum($width), 0, '', 'T');
		$this->Ln();

		// make more space if there is grouping and summary
		if (array_search(2, $summary_and_grouping) && array_search(1, $summary_and_grouping))
		{
			$this->Ln();
			$this->Ln();
		}

		if (array_search(1, $summary_and_grouping))
		$this->draw_ending_summary($NumOfRows, $grand_sum_c, $align, $width, $summary_and_grouping);

	}






	/*
	 * check input fields
	 * for blank
	 * error: multiply grouping is not yet supported
	 * error: multiply grouping is not yet supported
	 */
	function check_report_params($data, $summary_and_grouping )
	{
		if (!$data)
		{
			$this->blank_report();
		}


		// TODO: support for multiply grouping
		$test_grouping_deep = array_keys($summary_and_grouping, 2);
		if (count($test_grouping_deep)>1)
		{
			echo 'error: multiply grouping is not yet supported';
			die;
		}

		// TODO: support for multiply last value
		$test_grouping_deep = array_keys($summary_and_grouping, 4);
		if (count($test_grouping_deep)>1)
		{
			echo 'error: multiply grouping is not yet supported';
			die;
		}
	}

	/*
	 * format data values to display
	 *
	 * x - no format
	 * d - date
	 * s - string (s:30 - max is 30 chars)
	 *
	 */
	function format(&$value, $format)
	{

		if ($value == null){
			$value = atktext('report_null');
			return;
		}

		if ($format == 'x') return;

		if (is_numeric($format))
		{
			//$value = number_format($value,$format);
			$value = $this->formatNumber($value,$format);
			return;
		}

		if ($format == 'd' && is_string($value))
		{
			$date = explode("-",$value);
			$date_day   = $date[2];
			$date_month = $date[1];
			$date_year  = $date[0];
			$timestamp = adodb_mktime(12,0,0,$date_month,$date_day,$date_year);
			$value = date(DATE_FORMATTING,$timestamp);
			return;
		}

		if ($format == 'dt' && is_string($value))
		{
			$val = explode(" ",$value);
			$date = explode("-",$val[0]);
			$time = explode(":",$val[1]);
			$timestamp = mktime($time[0],$time[1],$time[2],$date[1],$date[2],$date[0]);
			$value = date(DATE_FORMATTING,$timestamp);
			return;
		}

		if (is_string($format) && is_string($value))
		{
			$value = substr($value, 0, substr($format,2));
			return;
		}

		echo('bad formatting!'); die;
	}


	function formatNumber($number, $decimals=2)
	{
		$decimalSeparator = atktext('decimal_separator');
		$thousandsSeparator = atktext('thousands_separator');

		if ($decimalSeparator == $thousandsSeparator)
		{
			atkwarning('invalid thousandsseparator. identical to the decimal_separator');
			$thousandsSeparator = '';
		}

		// alcal: fix negative number bug
		if ($number<0)
		{
			$negative = "-";
			$number = $number * -1;
		}

		$tmp1 = round((float)$number, $decimals);
		$tmp1 .= $decimals > 0 && strpos($tmp1, '.') === false ? '.' : '';
		$tmp1 .= str_repeat('0', max($decimals - strlen(substr($tmp1, strpos($tmp1, '.') + 1)), 0));

		while (($tmp2 = preg_replace('/(?<!.)(\d+)(\d\d\d)/', '\1 \2', $tmp1)) != $tmp1)
		{
			$tmp1 = $tmp2;
		}

		return $negative.strtr($tmp1, array(' ' => $thousandsSeparator, '.' => $decimalSeparator));
	}


	/*
	 * draw summary on group ending
	 */
	function draw_group_summary($NumOfRows, $sum, $align, $width, $summary_and_grouping, $last_group_value)
	{
			
		//$this->SetFillColor(222, 222, 222);
		$this->set_font_bold();

		for ($i=0; $i < $NumOfRows ; $i++)	{
			if ($summary_and_grouping[$i] == 1 || $summary_and_grouping[$i] == 3)
			{
				// correct 0 to null on summary
				if ($sum[$i]==0) {
					$null_formatted=$this->formatNumber(0);
				} else $null_formatted = $this->formatNumber($sum[$i]); //number_format($sum[$i],2);

				if($width[$i]<>0) $this->Cell($width[$i], 6, $null_formatted, 'TB', 0, $align[$i], 1);
			}
			elseif ($summary_and_grouping[$i] == 4)
			{

				// correct 0 to null on summary
				if ($sum[$i]==0) {
					$null_formatted= $this->formatNumber(0);
				} else $null_formatted = $this->formatNumber($sum[$i]); //number_format($sum[$i],2);


				//if($width[$i]<>0) $this->Cell($width[$i], 6, number_format($last_group_value,2), 'TB', 0, $align[$i], 1);
				if($width[$i]<>0) $this->Cell($width[$i], 6, $this->formatNumber($last_group_value), 'TB', 0, $align[$i], 1);

			}
			else // draw blank cell
			{
				if($width[$i]<>0) $this->Cell($width[$i], 6, '', 'TB', 0, $align[$i], 1);
			}
		}
		$this->set_font_regular();
		//$this->SetFillColor(224, 235, 255);
		$this->Ln();

	}

	/*
	 * draw total on report end
	 */
	function draw_ending_summary($NumOfRows, $sum, $align, $width, $summary_and_grouping, $summary_value=null)
	{
		//$this->SetFillColor(222, 222, 222);
		$this->set_font_bold();

		$this->Cell(30, 6, atktext('total').':', 0, 0, $align[$i], 0);
		$this->Ln();

		for ($i=0; $i < $NumOfRows ; $i++)	{
			if ($summary_and_grouping[$i] == 1)
			{
				// correct 0 to null on summary
				if ($sum[$i]==0) {
					$null_formatted= $this->formatNumber(0);
				} else $null_formatted = $this->formatNumber($sum[$i]); //number_format($sum[$i],2);


				//if($width[$i]<>0) $this->Cell($width[$i], 6, number_format($sum[$i],2), '', 0, $align[$i], 1);
				if($width[$i]<>0) $this->Cell($width[$i], 6, $null_formatted, 'TB', 0, $align[$i], 1);
			}
			else // draw blank cell
			{
				if($width[$i]<>0) $this->Cell($width[$i], 6, '', 'TB', 0, $align[$i], 1);
			}
		}
		$this->set_font_regular();
		$this->Ln();

		// draw ending line
		//$this->Cell(array_sum($width), 0, '', 'T');
		$this->Ln();
	}



	/*
	 * draw header
	 */
	function draw_header($header, $width, $align, $summary_and_grouping, $summary_value=null)
	{

		$this->set_font_bold();
		$num_headers = count($header);

		for($i = 0; $i < $num_headers; ++$i) {
			if ($summary_and_grouping[$i] == 2)
			{
				// separator is user to display different grouping value instead of hardcore ID fields which do the grouping itself
				$tmp = explode("|SEPARATOR|",$summary_value);

				if ($tmp[1])
				{
					$this->Cell(20, 7, $tmp[1], 0, 0, '', 0);
					$this->Cell(50, 7, $tmp[2], 0, 0, 'L', 0);
					$this->Ln();
					break;
				}

				$this->Cell(30, 7, $header[$i] . ": ", 0, 0, '', 0);
				$this->Cell(30, 7, $summary_value, 0, 0, '', 0);
				$this->Ln();
			}
		}

		for($i = 0; $i < $num_headers; ++$i) {

			if($width[$i]<>0 && $summary_and_grouping[$i] <> 2)
			$this->Cell($width[$i], 7, $header[$i], 'TB', 0, $align[$i], 1);

		}
		$this->set_font_regular();
		$this->Ln();
	}




	/*
	 * error handler
	 */
	function blank_report()
	{
		$content .= atktext('blank_report');
		show_error_window($content);
		die;
	}


}
?>