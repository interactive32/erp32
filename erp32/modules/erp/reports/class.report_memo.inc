<?php

class report_memo extends ia32node
{

	function report_memo()
	{

		$this->atkNode("report_memo");

		$this->addFieldSet('dummy',atktext('report_memo_info'),AF_NOLABEL);
	

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

		$pdf = new atkpdf();

		$our_firm = $this->getOurFirm();

		// Report Title ; Portrait or Landcape
		$top_header = $our_firm['firm_name'];
		$top_info = $our_firm['description'];

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


		// file name, Display "I"nline / or "D"ownload / F
		$pdf->Output(atktext($this->m_type).'_'.date('d.m.Y').'.pdf', 'I');

	}



	function getOurFirm()
	{
		$data = $this->db32select("SELECT * FROM ia32_config");
		return $data[0];
	}

}

?>