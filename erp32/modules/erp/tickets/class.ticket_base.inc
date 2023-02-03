<?php


class ticket_base extends ia32Node
{

	protected $ticket_type = 0;
	protected $ticket_typename = '';

	/**
	 * base ticket constructor
	 */
	function ticket_base($name, $flags=0)
	{
		$this->atkNode($name, $flags);

		$this->add(new atkAttribute("creator", AF_READONLY|AF_HIDE_ADD|AF_HIDE_EDIT|AF_HIDE_LIST, $this->std_att_size));
		$this->add(new atkAttribute("lastmod", AF_READONLY|AF_HIDE, $this->std_att_size));

		// main filter at the top of every other - current location_id
		$location_id = $this->getCurrentLocationId();
		$this->addFilter("location_id = '$location_id'");

		// text templates
		$this->add(new atkManyToOneRelation("text_templates", "config.text_templates", AF_HIDE_LIST));
		$this->getAttribute("text_templates")->setloadType(NOLOAD);
		$this->getAttribute("text_templates")->setstorageType(NOSTORE);
		$this->getAttribute("text_templates")->addDependency(array($this, "refresh_text_template"));

		$this->add(new atkNumberAttribute("ticket_payment", AF_HIDE_ADD|AF_HIDE_EDIT|AF_HIDE_LIST, 10, 2));
	}

	function descriptor_def()
	{
		return "[ticket_number]";
	}



	/*
	 * load initial values - add mode
	 */
	function initial_values()
	{
		$ret = array();

		$ret["page_holder"] = 1;
		$ret["current_page"] = 1;
		$ret["discount_full"] = 0;
		$ret["has_notes"] = "0";

		// this will get it's final value in preAdd, because of multiuser insert
		$ret["ticket_number"] = atktext('new_document');

		$ret["creator"] = $this->getUserTrack();

		$today  = mktime(0, 0, 0, date("m")  , date("d"), date("Y"));
		$tomorow  = mktime(0, 0, 0, date("m")  , date("d")+5, date("Y"));

		$today_array = dateutil::stamp2arr($today);
		$tomorow_array = dateutil::stamp2arr($tomorow);
			
		$ret["date_created"] = $today_array;
		$ret["date_payment"] = $today_array;
		$ret["date_product_issue"] = $today_array;

		$ret["type_id"]["id"] = $this->ticket_type;

		// document is set to current location_id
		$ret['location_id']['id'] = $this->getCurrentLocationId();

		return $ret;
	}



	/*
	 * display top group - cusomer and dates
	 */
	function drawTopGroup()
	{
		$ret = '
		 <table width="762" cellpadding="0" cellspacing="0">
		 	<tr>
		 		<td>
		 			<table valign="top" cellpadding="2" cellspacing="0" style="width: 100%; border: 1px dashed lightGray; padding: 10px;" cellpadding="0" cellspacing="3">
						<tr>
							<td><b>[customer_info.label]:</b>
							<img align="top" src="'.atktext('required_image').'" border="0">
							</td>
							
							<td align="right" style="display:[discount_full.label];">
							'.atktext('discount_full').'	
							</td>
						</tr>
						
						<tr>
							<td align="left">
							[customer_id.field]
							</td> 
						
							<td align="right" style="display:[discount_full.label];">
							[discount_full.field]
							</td>
							
						</tr>
						<tr style="height: 60px;">
							<td colspan=2>
							 <p>
							 	[customer_info.field]
							 </p>
							 </td>
						</tr>
	
					</table>
					
				</td>
				
				<td valign="top" align="right">
					<table cellpadding="0" cellspacing="3" style="border: 1px dashed lightGray; padding: 10px;">	
						<tr>
							<td  align="right">'.atktext('place_of_issue').'</b>:</td>
							<td >[location_id.field]</td>
						</tr>	
						<tr>
							<td align="right">'.atktext('ducument_date').'</b>:</td>
							<td >[date_created.field]</td>
						</tr>					
						<tr>
							<td align="right">'.atktext('date_product_issue').'</b>:</td>
							<td >[date_product_issue.field]</td>
						</tr>						
						<tr>
							<td align="right">'.atktext('date_payment').'</b>:</td>
							<td >[date_payment.field]</td>
						</tr>
						<tr>
							<td align="right">'.atktext('payment_days').':</td>
							<td align="left">[payment_days.field]</td>
							

						</tr>	
					</table>	
				</td>

			</tr>
			
			<tr height=40>
				<td colspan=2 style="font-size:20px; text-align:center;">
				<b> [type_id.field] </b> 
				</td>	
			</tr>

			<tr height=20>
				<td colspan=2 style="text-align:center;">
				<b>'.atktext('ticket_number').': [ticket_number.field] </b>
				</td>	
			</tr>
			
			<tr height=40 style="display:[external_number.label];">
				<td colspan=2 style="text-align:center;" >
				<b>'.atktext('external_number').': [external_number.field] </b>
				</td>	
			</tr>
	
			<tr style="display:none"><td>[status_id.field]</td></tr> <!-- refresh only -->
			
		 </table>
		 ';
			
		return $ret;
	}


	/*
	 * master entry grid
	 */
	function drawMasterGroup()
	{
		$ret = '
		 [group_header.field]
			';

		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			$ret .= '
			[group_line_'.$i.'.field]
			';
		}

		$ret .= '
			[group_footer.field]
		 ';

		return $ret;
	}


	/*
	 * master entry grid - header
	 */
	function drawHeader()
	{
		$ret = '
		
		<table width=100% border="0" align="center" cellpadding="0">
			<tr>
				<td align="right">
				[page_holder.field]
				</td>	
					
				<!-- holder for hiden fields, must be here for refresh reason -->
				<td style="display:none;">
				[current_page.field] [ticket_sum.field]';

		// add hiden dummy fields
		for ($i=1; $i<=atkconfig('pages_limit'); $i++){
			$ret .= '
			[page_stack_'.$i.'.field]
			';
		}

		$ret .='</td>
			</tr>					
		</table>
		 
		 <table cellpadding="4" cellspacing="0" style="color: white; background-color: #749ABE;  border-top: 1px solid lightGray; border-bottom: 1px solid lightGray; border-left: 1px solid lightGray; border-right: 1px solid lightGray;">

		 	<tr valign="middle">
				<th width=190 style="border-right: 1px solid white; text-align: center;">'.atktext('product_name').': </th>
				<th width=110 colspan=2 style="border-right: 1px solid white; text-align: center;">'.atktext('units').'</th>
				<th width=100 style="border-right: 1px solid white; text-align: center;">'.atktext('pricesell2').'</th>
				<th width=61 style="border-right: 1px solid white; text-align: center;">'.atktext('discount_percent').'</th>
				<th width=90 style="border-right: 1px solid white; text-align: center;">'.atktext('discount_price').'</th>
			    <th width=60 style="border-right: 1px solid white; text-align: center;">'.atktext('tax_rate').'</th> 
		<!-- 	<th width=61 style="border-right: 1px solid white; text-align: center;">'.atktext('tax_price').'</th> -->
				<th width=90 style="text-align: center;">'.atktext('total_without_tax').'</th>
			</tr>

			';

		return $ret;
	}


	/*
	 * master entry grid - one line
	 */
	function drawLine($line_num)
	{
		return '
			<tr valign="middle" style="display:[row_holder_'.$line_num.'.label]; color: #414141; background-color: white;">
				<td style="border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;">[product_id'.$line_num.'.field]</td>
				<td style="border-bottom: 1px solid lightGray;">[units_'.$line_num.'.field]</td>
				<td style="border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;">[fix_unitsymbol_'.$line_num.'.field]</td>
				<td style="border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;">[fix_price_'.$line_num.'.field] <b>[fix_price_'.$line_num.'.label]</b></td>
				<td style="border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;">[fix_discount_'.$line_num.'.field]</td>
				<td style="border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;">[discount_price_'.$line_num.'.field]</td>				
				<td style="border-right: 1px solid lightGray; border-bottom: 1px solid lightGray; text-align: center;">[fix_taxname_'.$line_num.'.field]</td>
				<td style="border-bottom: 1px solid lightGray;">[sum_'.$line_num.'.field]</td>

				<td style="display: none;"> /* must reload here */
				[price_original_holder_'.$line_num.'.field]
				[stock_holder_'.$line_num.'.field]
				[reservation_holder_'.$line_num.'.field]								
				[fix_productname_'.$line_num.'.field]
				[fix_taxrate_'.$line_num.'.field]
				[product_idchange'.$line_num.'.field]
				</td>
	
			</tr>
			';
	}


	/*
	 * master entry grid - footer with sum
	 */
	function drawFooter()
	{
		return '
			<tr>
				<td colspan=9 height=15 align="right"  style="color: white; background-color: #749ABE;">
				</td>
			</tr>
			
			<tr>
				<td colspan=7 align="right"  style="color: #414141; background-color: white"><b>'.atktext('grand_total_without_tax').':</b>
				</td>
				<td width=90px style="color: #414141; background-color: white">[sum_holder.field]</b>
				</td>
			</tr>
			
			<tr>
				<td colspan=7 align="right" style="color: #414141; background-color: white"><b>'.atktext('grand_total').':</b>
				</td>
				
				<td width=90px style="color: #414141; background-color: white">[sumtax_holder.field]</b>
				</td>
			</tr>

			</table>';
	}


	/*
	 * notes
	 */
	function drawNotes()
	{
		return '
		<table width="100%" cellpadding="2" cellspacing="0" >
		 <tr>
		 	<td>[has_notes.field] [has_notes.label]</td>
		 	<td></td>
		 </tr>
		 <tr style="display:[notes_holder.label]; ">
		 	<td></td>
		 	<td align="right">[text_templates.label] [text_templates.field]</td>
		 </tr>		 
		</table>
		<table>
		 <tr style="display:[notes_holder.label]; ">
		 	<td>[notes.field]</td>
		 </tr>
		 </table>
			';
	}


	/*
	 * refresh top table, customers
	 */
	function refresh_top(atkEditFormModifier $modifier)
	{
		$record = &$modifier->getRecord();
			
		// refresh discount only if ajax update
		if($this->m_postvars['atklevel']== -3
		&& $this->m_postvars['doctype'] != 1002 && $this->m_postvars['doctype'] != 1005 && $record['type_id']['id'] != 1002 && $record['type_id']['id'] != 1005)
		{
			$record['discount_full'] = $this->find_customer_discount($record);
		}
		else
		$record['discount_full'] = 0;

		$this->setDocumentTypeSpecifics($record);

		$modifier->refreshAttribute('group_top');
	}


	/*
	 * refresh notes
	 */
	function refresh_notes(atkEditFormModifier $modifier)
	{
		$record = &$modifier->getRecord();

		// TODO: when returns from distant search - don't work wery well

		if ($record['has_notes']==1)
		$this->getAttribute("notes_holder")->setLabel('display');
		else
		{
			$this->getAttribute("notes_holder")->setLabel('none');
			$record['notes'] = '';
		}

		$modifier->refreshAttribute('group_notes');
	}


	/*
	 * load text template
	 */
	function refresh_text_template(atkEditFormModifier $modifier)
	{
		$record = &$modifier->getRecord();

		$sql =  "SELECT text FROM ia32_text_templates WHERE id = '".$record['text_templates']['id']."'";
		$recs = $this->db32select($sql);

		// add text only if ajax update
		if($this->m_postvars['atklevel']== -3)	$record['notes'] .= $recs[0]['text'];

		if ($record['notes'])
		{
			$this->getAttribute("notes_holder")->setLabel('display');
			$record['has_notes'] = 1;
		}else
		{
			$this->getAttribute("notes_holder")->setLabel('none');
			$record['notes'] = '';
			unset($record['has_notes']);
		}

		$modifier->refreshAttribute('group_notes');
	}


	/*
	 * find customer's discount
	 */
	function find_customer_discount($record)
	{

		$customerid = $record['customer_id']['id'];

		$sql = "	SELECT
						 discount
						FROM
						  ia32_customers 
						WHERE
						ia32_customers.ID = '$customerid'
						";	
			
		$dbrec = $this->db32select($sql);
		$dbrecords = $dbrec[0];

		// discount can't be null or negative!
		if (!$dbrecords["discount"] || $dbrecords["discount"]<0) $dbrecords["discount"] = 0;

		return $dbrecords["discount"];

	}


	/*
	 * refresh document discount
	 */
	function refresh_discount(atkEditFormModifier $modifier)
	{
		// refresh pages only on atklevel ajax update = -1 ?? right?
		if($this->m_postvars['atklevel']!= -3) return;

		$record = &$modifier->getRecord();

		if (!$record['discount_full'] || $record['discount_full'] <0 || $record['discount_full'] >100)
		$record['discount_full'] = 0;

		$this->inject_new_discount($record, $record['discount_full']);

		$this->pagingLogic_removeEmptyPages($record);

		$modifier->refreshAttribute('group_master');

		$this->setDocumentTypeSpecifics($record);
		$modifier->refreshAttribute('group_top');

	}


	/*
	 * inject discount to all rows/pages
	 */
	function inject_new_discount(&$record, $customer_discount)
	{
		// discount can't be null or negative
		if (!$customer_discount || $customer_discount<0) $customer_discount = 0;

		// push current grid to pages
		$this->pagingLogic_pushData($record, $record['current_page']);
			
		$this->pagingLogic_retriveAllData($record);

		for ($p=1; $p<=atkconfig('pages_limit'); $p++){

			unset($stackrec);
			// retrive from dummy attribute and unserialize
			$stackrec = unserialize($record["page_stack_".$p]);

			for ($i=1; $i<=atkconfig('num_of_rows'); $i++){

				++$offset;

				if ($record["product_id$offset"])
				{

					$record["product_id$offset"] = $stackrec["product_id$i"];
					$record["units_$offset"] = $stackrec["units_$i"];
					$record["fix_productname_$offset"] = $stackrec["fix_productname_$i"];
					$record["price_original_holder_$i"] = $stackrec["price_original_holder_$i"];
					$record["stock_holder_$i"] = $stackrec["stock_holder_$i"];
					$record["reservation_holder_$i"] = $stackrec["reservation_holder_$i"];
					$record["fix_taxrate_$offset"] = $stackrec["fix_taxrate_$i"];
					$record["fix_taxname_$offset"] = $stackrec["fix_taxname_$i"];
					$record["fix_unitsymbol_$offset"] = $stackrec["fix_unitsymbol_$i"];

					// calculate new discount
					$record["fix_price_$offset"] = $stackrec["fix_price_$i"];
					$record["fix_discount_$offset"] = $customer_discount;
					$record["discount_price_$offset"] =  $stackrec["fix_price_$i"] - ($stackrec["fix_price_$i"] / 100 * $customer_discount);

				}else
				break;
			}
		}

		$this->pagingLogic_storeRecordsToPages($record, $record['current_page']);

		$this->calculate_grandSum_total($record);

		return;
	}


	/*
	 * display customer info fields, as display - not edit (overloader)
	 */
	function customer_info_edit(&$record)
	{
		return $this->m_attribList["customer_info"]->display($record);
	}


	/*
	 * top table, refresh number of days
	 */
	function refresh_paymentDay(atkEditFormModifier $modifier)
	{
		$record = &$modifier->getRecord();

		$stamp_product_issue = adodb_mktime(12,0,0,$record['date_product_issue']['month'],$record['date_product_issue']['day'],$record['date_product_issue']['year']);
		$stamp_date_payment = adodb_mktime(12,0,0,$record['date_payment']['month'],$record['date_payment']['day'],$record['date_payment']['year']);
		
		if ($stamp_date_payment < $stamp_product_issue)
		{
			$record['payment_days'] = 0;
			$record['date_payment'] = $record['date_product_issue'];
		}else
		$record['payment_days'] = ($stamp_date_payment - $stamp_product_issue) / 86400;
		

		$this->setDocumentTypeSpecifics($record);
		$modifier->refreshAttribute('group_top');
	}


	/*
	 * top table, refresh date based on days
	 */
	function refresh_paymentDate(atkEditFormModifier $modifier)
	{
		$record = &$modifier->getRecord();

		$stamp_product_issue = adodb_mktime(12,0,0,$record['date_product_issue']['month'],$record['date_product_issue']['day'],$record['date_product_issue']['year']);
		$stamp_date_payment = adodb_mktime(12,0,0,$record['date_payment']['month'],$record['date_payment']['day'],$record['date_payment']['year']);
		
		
			
		if ($record['payment_days'] < 0)
		{
			$record['payment_days'] = 0;
			$record['date_payment'] = $record['date_product_issue'];
		}else
		{
			$stamp_date_payment = $stamp_product_issue + ($record['payment_days'] * 86400);
			$record['date_payment'] = dateutil::stamp2arr($stamp_date_payment);
		}
		
		$this->setDocumentTypeSpecifics($record);
		$modifier->refreshAttribute('group_top');
	}


	/*
	 * refresh row data logic
	 */
	function refreshRow($line, &$record)
	{
		// see if $product_change_occured
		if ($record["product_id$line"]["id"] != $record["product_idchange$line"])
		{
			$record["product_idchange$line"] = $record["product_id$line"]["id"];
			$product_change_occured = true;
		}

		if (!$product_change_occured && $record["units_$line"]==0)
		unset($record["product_id$line"]);

		// restrict negative values - revert to positive
		if ($record["units_$line"]<0) $record["units_$line"] = -1 * $record["units_$line"];
		
		// fix 28.03.2013
		if (!$record["fix_price_$line"]) $record["fix_price_$line"] = 0;

		if ($record["product_id$line"])
		{

			if ($product_change_occured)
			{
				$product_id = $record["product_id$line"]["id"];

				$pricelist_id = $this->getPricelistId();
				$location_id = $this->getCurrentLocationId();

				$sql = "
						SELECT
						  ia32_products.name AS productname, 
						  ia32_units.symbol AS unitsymbol, 
						  ia32_taxes.rate AS taxrate, 
						  ia32_taxes.name AS taxname, 		  
						  ia32_prices.pricesell AS price,
						  ia32_prices.pricebuy AS pricebuy,
						  ia32_stockcurrent.amount AS stock,
						  ia32_stockcurrent.reservation AS reservation
						  			  
						FROM
						  ia32_products 
						  INNER JOIN ia32_taxes ON ia32_taxes.id = ia32_products.tax_id 
						  INNER JOIN ia32_units ON ia32_units.id = ia32_products.unit_id 
						  LEFT JOIN ia32_prices ON ia32_prices.product_id = ia32_products.id AND ia32_prices.pricelist_id = '".$pricelist_id."'
					      LEFT JOIN ia32_stockcurrent ON ia32_stockcurrent.product_id = ia32_products.id AND ia32_stockcurrent.location_id = '$location_id'
					
						WHERE
						ia32_products.id = '$product_id'
						";	


				$dbrec = $this->db32select($sql);
				$dbrecords = $dbrec[0];


				// add 1 unit
				$record["units_$line"] = 1;

				// if we cant find prices in db, fill with zero
				if (!$dbrecords["price"]) $dbrecords["price"] = 0;
				if (!$dbrecords["pricebuy"]) $dbrecords["pricebuy"] = 0;

				// important!
				// if this is receipt or purchase order, replace price with pricebuy
				if ($record['type_id']['id'] == 1002 || $record['type_id']['id'] == 1005)
				$dbrecords["price"] = $dbrecords["pricebuy"];

				// add stock info
				if ($dbrecords["stock"] === null)
				{
					$record["stock_holder_$line"] = -99999999.99;
				}else
				{
					$record["stock_holder_$line"] = $dbrecords["stock"];
				}

				$record["reservation_holder_$line"] = $dbrecords["reservation"];

				// price sell
				$record["fix_price_$line"] = $dbrecords["price"];
				$record["price_original_holder_$line"] = $dbrecords["price"];
				$this->getAttribute("fix_price_$line")->setLabel(' ');

				$record["fix_productname_$line"] = $dbrecords["productname"];
				$record["fix_unitsymbol_$line"] = atktext($dbrecords["unitsymbol"]);
				$record["fix_taxrate_$line"] = $dbrecords["taxrate"];
				$record["fix_taxname_$line"] = $dbrecords["taxname"];

				$record["fix_discount_$line"] = $record['discount_full'];

			}


			if (!$record["fix_discount_$line"] || $record["fix_discount_$line"]>100 || $record["fix_discount_$line"]<0)
			$record["fix_discount_$line"] = 0;


		} else {
			unset($record["product_id$line"]);
			unset($record["fix_productname_$line"]);
			unset($record["units_$line"]);
			unset($record["fix_price_$line"]);
			unset($record["fix_unitsymbol_$line"]);
			unset($record["fix_taxname_$line"]);
			unset($record["fix_discount_$line"]);
			unset($record["discount_price_$line"]);
			unset($record["sum_$line"]);

			$this->delete_row_from_grid($record, $line);
		}

		$this->calculate_grandSum_total($record);

	}



	/*
	 * deleted one row from grid and shift others
	 * we don't need page logic here
	 */
	function delete_row_from_grid(&$record, $line)
	{
		$this->pagingLogic_pushData($record, $record['current_page']);

		$this->pagingLogic_retriveAllData($record);

		$line_offset = $line + (atkconfig('num_of_rows') * ($record['current_page'] - 1));

		for ($i=$line_offset; $i<=(atkconfig('num_of_rows') * atkconfig('pages_limit')); $i++)
		{
			$line2 = $i+1;

			$record["product_id$i"] = $record["product_id$line2"];
			$record["fix_productname_$i"] = $record["fix_productname_$line2"];
			$record["units_$i"] = $record["units_$line2"];
			$record["fix_price_$i"] = $record["fix_price_$line2"];
			$record["price_original_holder_$i"]	= $record["price_original_holder_$line2"];
			$record["stock_holder_$i"]	= $record["stock_holder_$line2"];
			$record["reservation_holder_$i"]	= $record["reservation_holder_$line2"];
			$record["fix_unitsymbol_$i"] = $record["fix_unitsymbol_$line2"];
			$record["fix_taxname_$i"] = $record["fix_taxname_$line2"];
			$record["fix_taxrate_$i"] = $record["fix_taxrate_$line2"];
			$record["fix_discount_$i"] = $record["fix_discount_$line2"];
			$record["product_id$i"] = $record["product_id$line2"];
			//$record["sum_$i"] = $record["sum_$line2"];

			// change is not occured, init for refreshRow()
			$record["product_idchange$i"] = $record["product_id$line2"]["id"];
		}

		$this->pagingLogic_storeRecordsToPages($record, $record['current_page']);

	}


	/*
	 * hide empty rows + extra row for editing a new line
	 */
	function show_hide_rows($record)
	{
		for ($i=1; $i<=atkconfig('num_of_rows'); $i++)
		{
			$nextrow = $i+1;
			if ($record["product_id$i"])
			{
				$this->getAttribute("row_holder_$i")->setLabel('display');
				// display extra row for editing new line
				if ($i <= (atkconfig('num_of_rows') -1))
				$this->getAttribute("row_holder_$nextrow")->setLabel('display');
			}
		}
	}


	/*
	 * calculate grand total and hide/show rows
	 */
	function calculate_grandSum_total(&$record, $mode="")
	{
		$record["sum_holder"] = 0;
		$record["sumtax_holder"] = 0;
			
		if ($mode=="view")
		$count = atkconfig('num_of_rows') * atkconfig('pages_limit');
		else
		$count = atkconfig('num_of_rows');

		for ($i=1; $i<=$count; $i++){

			// calculate discount price and sum
			if ($record["product_id$i"])
			{

				if (!$record["fix_discount_$i"])
				$record["discount_price_$i"] = $record["fix_price_$i"];
				else
				$record["discount_price_$i"] = $record["fix_price_$i"] - ($record["fix_price_$i"] / 100 * $record["fix_discount_$i"]);

				$record["sum_$i"] = $record["units_$i"] * $record["discount_price_$i"];
			}else
			unset($record["sum_$i"]);


			// quote & invoice - if price is not original, show info through "!" symbol
			if (($record['type_id']['id']==1001 || $record['type_id']['id']==1004)
			&& $record["fix_price_$i"] && $record["fix_price_$i"] != $record["price_original_holder_$i"] && $mode != "view")
			{
				$this->getAttribute("fix_price_$i")->setLabel('!');
				$this->getAttribute("fix_price_$i")->setTitle(atktext('original_price').': '.$record["price_original_holder_$i"]);
			}

			// if has stock and this is quote or invoice - display stock info
			if (($record["stock_holder_$i"] || $record["reservation_holder_$i"]) && ($record['type_id']['id']==1001 || $record['type_id']['id']==1004))
			{
				$stock_text = atktext('amount_available').': '.$record["stock_holder_$i"].' '.atktext('on_reservation').': '.$record["reservation_holder_$i"];
				$this->getAttribute("units_$i")->setTitle($stock_text);

				if ($record["stock_holder_$i"] == -99999999.99)
				$this->getAttribute("units_$i")->setTitle(atktext('no_track_stock'));
			}

			$nextrow = $i+1;

			$this->show_hide_rows($record);
		}

		// add other page sums to grand sum
		$this->pagingLogic_pushData($record, $record['current_page']);
		$other_page_sums = $this->pagingLogic_retriveSums($record);
		$record["sum_holder"] = $other_page_sums[0];
		$record["sumtax_holder"] = $other_page_sums[1];

		$this->setDocumentTypeSpecifics($record); // i.e. hide discount on receipt
	}


	/*
	 * focus when product_id is filled
	 */
	function setFocus($line, $modifier, &$record)
	{
		$second_line = $line + 1;
		if ($line >= atkconfig('num_of_rows')) // last line - focus on pager
		{
			$modifier->scriptCode("document.getElementById('page_holder').focus();");
			return;
		}
		if ($record["units_$line"]==1)
		$modifier->scriptCode("document.getElementById('units_".$line."').focus();");
		else {
			$modifier->scriptCode("document.getElementById('product_id".$second_line."_search').focus();");
		}
	}


	/*
	 * refresh template function,
	 * initialy rows refresh on refresh_dep_row* which then sends data here
	 */
	function refresh_dep_row_template($line, &$modifier)
	{

		$record = &$modifier->getRecord();

		// return on empty product line to save execution time
		if ($this->m_postvars["product_id$line"]=='' && $record["product_idchange$line"]=='')
		return;

		$this->refreshRow($line, $record);

		if ($this->pagingLogic_checkPageFull($record))
		{
			$this->pagingLogic_pushData($record, $record['current_page']);
		}

		// refresh pages only on atklevel ajax update = -1 ?? right?
		if($this->m_postvars['atklevel']== -3)
		$this->pagingLogic_removeEmptyPages($record);


		// refresh
		$modifier->refreshAttribute('group_master');

		// focus on current element, m_postvar also works with m2o return from page
		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			if ($this->m_postvars["product_id$i"]!='')
			{
				$this->setFocus($i, $modifier, $record);
			}
		}

	}


	// max 10 rows, more will be slow
	function refresh_dep_row1(atkEditFormModifier $modifier){$this->refresh_dep_row_template(1, $modifier);}
	function refresh_dep_row2(atkEditFormModifier $modifier){$this->refresh_dep_row_template(2, $modifier);}
	function refresh_dep_row3(atkEditFormModifier $modifier){$this->refresh_dep_row_template(3, $modifier);}
	function refresh_dep_row4(atkEditFormModifier $modifier){$this->refresh_dep_row_template(4, $modifier);}
	function refresh_dep_row5(atkEditFormModifier $modifier){$this->refresh_dep_row_template(5, $modifier);}
	function refresh_dep_row6(atkEditFormModifier $modifier){$this->refresh_dep_row_template(6, $modifier);}
	function refresh_dep_row7(atkEditFormModifier $modifier){$this->refresh_dep_row_template(7, $modifier);}
	function refresh_dep_row8(atkEditFormModifier $modifier){$this->refresh_dep_row_template(8, $modifier);}
	function refresh_dep_row9(atkEditFormModifier $modifier){$this->refresh_dep_row_template(9, $modifier);}
	function refresh_dep_row10(atkEditFormModifier $modifier){$this->refresh_dep_row_template(10, $modifier);}


	/*
	 * fill db table ticket_lines with grid data
	 */
	function fill_Ticket_lines($record, $line_offset=0)
	{
		// find number of records
		$max_rows_posibile = atkconfig('num_of_rows') * atkconfig('pages_limit');

		for ($i=1; $i<=$max_rows_posibile; $i++)
		{
			if ($record["product_id$i"]
			&& $record["units_$i"]
			&& $record["units_$i"]!='0')
			{
				// if receipt or purchase order, store price as pricebuy
				if($record['type_id']['id'] == 1002 || $record['type_id']['id'] == 1005)
				{
					$fix_price = 0;
					$fix_pricebuy = $record["fix_price_$i"];

					// setPriceBuy only on receipt
					if($record['type_id']['id'] == 1002)
					$this->setPriceBuy($record["product_id$i"]["id"], $fix_pricebuy);
				}
				// if return, copy prices
				elseif($record['type_id']['id'] == 1003)
				{
					$fix_price = $record["fix_price_$i"];
					$fix_pricebuy = 0;
				}
				// if invoice or quote, find pricebuy to store into db
				elseif($record['type_id']['id'] == 1004 || $record['type_id']['id'] == 1001)
				{
					$fix_price = $record["fix_price_$i"];
					$fix_pricebuy = $this->getPriceBuy($record["product_id$i"]["id"]);
				}else
				{
					var_dump('fill_Ticket_lines: wrong type_id: '.$record['type_id']['id']); die;
				}


				$ticket_id = "'".stripquotes($record["id"])."'";
				++$line_offset; // line number
				$product_id = "'".$record["product_id$i"]["id"]."'";
				$units = $record["units_$i"];
				$fix_productname = "'".$record["fix_productname_$i"]."'";
				$fix_discount = $record["fix_discount_$i"];
				$fix_taxrate = $record["fix_taxrate_$i"];
				$fix_taxname = "'".$record["fix_taxname_$i"]."'";
				$fix_unitsymbol = "'".$record["fix_unitsymbol_$i"]."'";

				$sql =  "insert into ia32_ticket_lines (
					ticket_id,
					line,
					product_id,
					units,
					fix_productname,
					fix_price,
					fix_pricebuy,					
					fix_discount,
					fix_taxrate,
					fix_taxname,
					fix_unitsymbol					
				)
				values (
				$ticket_id,
				$line_offset,
				$product_id,
				$units,
				$fix_productname,
				$fix_price,
				$fix_pricebuy,
				$fix_discount,
				$fix_taxrate,
				$fix_taxname,
				$fix_unitsymbol
				)";

				$this->db32insert($sql);
			}
		}

		return $line_offset;

	}


	/*
	 * modification of validate - validate at least one product line
	 */
	function validate(&$record, $mode, $ignoreList=array())
	{
		// validate at least one product is there
		$record['at_least_one_product'] = '';
		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			if ($record["product_id$i"] || $record['current_page']>1)
			{
				$record['at_least_one_product'] = 'ok';
				break;
			}
		}


		// continue with parent functionality
		$parent = parent::validate($record, $mode, $ignoreList);

		return $parent;
	}


	/*
	 * catch errors here on mode=add
	 */
	function preAddToEditArray(&$record, $mode="add")
	{
		// catch copy from existing document
		$template = $this->m_postvars['copyfrom'];
		if ($template)
		{
			$pair = explode("=",$template);
			$template_document_id = stripQuotes($pair[1]);
			// load from template ticket_lines
			$this->getTicket_lines($record, $mode, $template_document_id);
			// store records to pages and activate paging
			$this->pagingLogic_storeRecordsToPages($record, 1);
		}

		// this will fix errors on add new document (i.e. obligatory error)
		if ($record['atkerror'] && $mode=="add")
		{
			$record['page_holder'] = $record['current_page'];

			$this->pagingLogic_pushData($record, $record['current_page']);

			// revert to "new" document number
			$record["ticket_number"] = atktext('new_document');
		}
	}


	/*
	 * remove data from ticket_lines
	 */
	function delete_Ticket_lines(&$record)
	{
		$ticket_id = $record["id"];

		$sql = "DELETE FROM ia32_ticket_lines WHERE ticket_id = '$ticket_id'";

		$this->db32delete($sql);

	}


	/*
	 * loading values from ticket lines
	 *
	 * before editing the record.
	 */
	function getTicket_lines(&$record, $mode="", $template_document=false)
	{

		// $template_document is used on document-copy
		if (!$template_document)
		$ticket_id = $record['id'];
		else
		$ticket_id = $template_document;

		$pricelist_id = $this->getPricelistId();
		$location_id = $this->getCurrentLocationId();

		$sql =  "SELECT
    		
					ia32_ticket_lines.ticket_id AS ticket_id,
					ia32_ticket_lines.product_id AS product_id,
					ia32_ticket_lines.units AS units,
					ia32_ticket_lines.fix_productname AS fix_productname,
					ia32_ticket_lines.fix_price AS fix_price,
					ia32_ticket_lines.fix_pricebuy AS fix_pricebuy,					
					ia32_ticket_lines.fix_discount AS fix_discount,
					ia32_ticket_lines.fix_taxrate AS fix_taxrate,
					ia32_ticket_lines.fix_taxname AS fix_taxname,
					ia32_ticket_lines.fix_unitsymbol AS	fix_unitsymbol,
					ia32_prices.pricesell AS original_price,
					ia32_products.name AS productname,
					ia32_stockcurrent.amount AS stock_holder,
					ia32_stockcurrent.reservation AS reservation_holder										
					
					FROM 
					ia32_ticket_lines 
					LEFT JOIN ia32_products ON ia32_products.id = ia32_ticket_lines.product_id
				    LEFT JOIN ia32_prices ON ia32_prices.product_id = ia32_ticket_lines.product_id AND ia32_prices.pricelist_id = '".$pricelist_id."'
	    			LEFT JOIN ia32_stockcurrent ON ia32_stockcurrent.product_id = ia32_ticket_lines.product_id AND ia32_stockcurrent.location_id = '".$location_id."'
	  
					WHERE ticket_id = '".stripquotes($ticket_id)."'
					";


		$recs = $this->db32select($sql);

		$i = 0;
		foreach ($recs as $line)
		{
			++$i;
			$record["id"] = $line['ticket_id'];

			$record["product_id$i"]["id"] = $line['product_id'];
			$record["product_id$i"]["name"] = $line['productname'];	// must load name here

			$record["units_$i"] = $line['units'];
			$record["fix_productname_$i"] = $line['fix_productname'];

			// switch prices if receipt or order
			if ($record["type_id"]["id"] == 1002 || $record["type_id"]["id"] == 1005)
			$record["fix_price_$i"] = $line['fix_pricebuy'];
			else
			$record["fix_price_$i"] = $line['fix_price'];

			$record["fix_pricebuy_$i"] = $line['fix_pricebuy'];
			$record["price_original_holder_$i"] = $line['original_price'];

			// add stock info
			if ($line['stock_holder'] === null)
			{
				$record["stock_holder_$i"] = -99999999.99;
			}else
			{
				$record["stock_holder_$i"] = $line['stock_holder'];
			}
				
			$record["reservation_holder_$i"] = $line['reservation_holder'];
			$record["fix_discount_$i"] = $line['fix_discount'];
			$record["fix_taxrate_$i"] = $line['fix_taxrate'];
			$record["fix_taxname_$i"] = $line['fix_taxname'];
			$record["fix_unitsymbol_$i"] = $line['fix_unitsymbol'];
		}

		return $record;
	}


	/*
	 * system calls this method to override the values of a record
	 * before editing the record.
	 */
	function edit_values($record)
	{
		// on first load, set current_page to 1
		if (!$record['current_page']) $record['current_page']=1;

		// refresh same page
		$record['page_holder'] = $record['current_page'];

		// important: if returning from distant page selection - don't load from ticket_lines
		if ($this->m_postvars['atkprevlevel'] > $this->m_postvars['atklevel'])
		{
			return $record;
		}

		if ($record['notes']) $record['has_notes'] = "1";

		$record['discount_full'] = $this->find_customer_discount($record);

		// else, first load, populate from ticket_lines
		$this->getTicket_lines($record, $mode, $template_document_id);

		// store records to pages and activate paging
		$this->pagingLogic_storeRecordsToPages($record, 1);

		return $record; // don't change this!
	}



	/*
	 * recordview actions
	 */
	function recordActions(&$record, &$actions, &$mraactions)
	{

		// replace original nodes with type-specific nodes
		switch ($record['type_id']['id'])
		{
			case 1001:
				$run = 'ticket_quote';
				break;

			case 1002:
				$run = 'ticket_receipt';
				break;

			case 1003:
				$run = 'ticket_creditnote';
				break;

			case 1004:
				$run = 'ticket_invoice';
				break;

			case 1005:
				$run = 'ticket_purchaseorder';
				break;

			case 1006:
				$run = 'ticket_inventory';
				break;

			case 1007:
				$run = 'ticket_internal_transfer';
				break;

			case 1008:
				$run = 'ticket_internal_transfer';
				break;
		}

		// replace original nodes here i.e. ticket_out -> ticket_creditnote
		if (strpos($actions["view"], 'ticket_out'))
		{
			$actions["view"] = str_replace('ticket_out', $run, $actions["view"]);
			$actions["delete"] = str_replace('ticket_out', $run, $actions["delete"]);

		}elseif (strpos($actions["view"], 'ticket_in'))
		{
			$actions["view"] = str_replace('ticket_in', $run, $actions["view"]);
			$actions["delete"] = str_replace('ticket_in', $run, $actions["delete"]);

		}elseif (strpos($actions["view"], 'ticket_stock'))
		{
			$actions["view"] = str_replace('ticket_stock', $run, $actions["view"]);
			$actions["delete"] = str_replace('ticket_stock', $run, $actions["delete"]);

		}elseif (strpos($actions["view"], 'ticket_transfer'))
		{
			$actions["view"] = str_replace('ticket_transfer', $run, $actions["view"]);
			$actions["delete"] = str_replace('ticket_transfer', $run, $actions["delete"]);
		}

		// remove edit action
		$actions["edit"] = '';

		// remove delete for closed documents, returns and waits
		if ($record['status_id']['id']==1001 || $record['status_id']['id']==1003 || $record['status_id']['id']==1004)
		$actions["delete"] = '';
	}


	/*
	 * refresh paging if user changes page
	 */
	function refresh_page(atkEditFormModifier $modifier)
	{
		$record = &$modifier->getRecord();

		$this->pagingLogic_pushData($record, $record['current_page']);
		$this->pagingLogic_retriveData($record, $record['page_holder']);

		// set new current page
		$record['current_page'] = $record['page_holder'];

		$this->pagingLogic_removeEmptyPages($record);

		// calculate sum and hide/show rows
		$this->calculate_grandSum_total($record);

		$modifier->refreshAttribute('group_master');

	}


	/*
	 * remove empty pages from selection
	 */
	function pagingLogic_removeEmptyPages($record)
	{

		// if first page not filled yet - hide pager via disabled="disabled"
		if ($record["current_page"]==1 && !$this->pagingLogic_checkPageFull($record))
		{
			$this->getAttribute("page_holder")->addFlag(AF_SPECIFIC_5);
			return;
		}

		// remove unused pages
		for ($i=atkconfig('pages_limit'); $i>=1; $i--){

			$stackrec = unserialize($record["page_stack_".$i]);
			$prev = $i-1;
			$prevstackrec = unserialize($record["page_stack_".$prev]);

			if (!$stackrec["product_id1"] && !$this->pagingLogic_checkPageFull($prevstackrec))
			{
				$this->getAttribute("page_holder")->removeOption(atktext("page_num")." ".$i);
			}else
			break;
		}

		return;
	}


	/*
	 * check if all rows are occupied
	 */
	function pagingLogic_checkPageFull($record)
	{
		// count if grid full
		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			if ($record["product_id$i"])	++$filled;
		}

		if ($filled == atkconfig('num_of_rows'))
		{
			return true;
		}
		return false;
	}


	/*
	 * push data from grid to stack
	 */
	function pagingLogic_pushData(&$record, $page)
	{
		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			$stackrec["product_id$i"] = $record["product_id$i"];
			$stackrec["units_$i"] = $record["units_$i"];
			$stackrec["fix_productname_$i"] = $record["fix_productname_$i"];
			$stackrec["fix_price_$i"] = $record["fix_price_$i"];
			$stackrec["fix_pricebuy_$i"] = $record["fix_pricebuy_$i"];
			$stackrec["price_original_holder_$i"] = $record["price_original_holder_$i"];
			$stackrec["stock_holder_$i"] = $record["stock_holder_$i"];
			$stackrec["reservation_holder_$i"] = $record["reservation_holder_$i"];
			$stackrec["fix_discount_$i"] = $record["fix_discount_$i"];
			$stackrec["discount_price_$i"] = $record["discount_price_$i"];
			$stackrec["fix_taxrate_$i"] = $record["fix_taxrate_$i"];
			$stackrec["fix_taxname_$i"] = $record["fix_taxname_$i"];
			$stackrec["fix_unitsymbol_$i"] = $record["fix_unitsymbol_$i"];
			//$stackrec["sum_$i"] = $record["sum_$i"];
		}

		// serialize and store to dummy attribute
		$record["page_stack_".$page] = serialize($stackrec);
	}


	/*
	 *  retrive data from one page stack
	 */
	function pagingLogic_retriveData(&$record, $page)
	{
		// retrive from dummy attribute and unserialize
		$stackrec = unserialize($record["page_stack_".$page]);

		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			$record["product_id$i"] = $stackrec["product_id$i"];
			$record["units_$i"] = $stackrec["units_$i"];
			$record["fix_productname_$i"] = $stackrec["fix_productname_$i"];
			$record["fix_price_$i"] = $stackrec["fix_price_$i"];
			$record["fix_pricebuy_$i"] = $stackrec["fix_pricebuy_$i"];
			$record["price_original_holder_$i"] = $stackrec["price_original_holder_$i"];
			$record["stock_holder_$i"] = $stackrec["stock_holder_$i"];
			$record["reservation_holder_$i"] = $stackrec["reservation_holder_$i"];
			$record["fix_discount_$i"] = $stackrec["fix_discount_$i"];
			$record["discount_price_$i"] = $stackrec["discount_price_$i"];
			$record["fix_taxrate_$i"] = $stackrec["fix_taxrate_$i"];
			$record["fix_taxname_$i"] = $stackrec["fix_taxname_$i"];
			$record["fix_unitsymbol_$i"] = $stackrec["fix_unitsymbol_$i"];

			// change is not occured, init for refreshRow()
			$record["product_idchange$i"] = $record["product_id$i"]["id"];
		}

	}


	/*
	 *  retrive data from all pages, and calculate grand_sum
	 */
	function pagingLogic_retriveAllData(&$record)
	{
		for ($p=1; $p<=atkconfig('pages_limit'); $p++){

			unset($stackrec);
			// retrive from dummy attribute and unserialize
			$stackrec = unserialize($record["page_stack_".$p]);

			for ($i=1; $i<=atkconfig('num_of_rows'); $i++){

				++$offset;

				$record["product_id$offset"] = $stackrec["product_id$i"];
				$record["units_$offset"] = $stackrec["units_$i"];
				$record["fix_productname_$offset"] = $stackrec["fix_productname_$i"];
				$record["fix_price_$offset"] = $stackrec["fix_price_$i"];
				$record["fix_pricebuy_$offset"] = $stackrec["fix_pricebuy_$i"];
				$record["price_original_holder_$offset"] = $stackrec["price_original_holder_$i"];
				$record["stock_holder_$offset"] = $stackrec["stock_holder_$i"];
				$record["reservation_holder_$offset"] = $stackrec["reservation_holder_$i"];
				$record["fix_discount_$offset"] = $stackrec["fix_discount_$i"];
				$record["discount_price_$offset"] = $stackrec["discount_price_$i"];
				$record["fix_taxrate_$offset"] = $stackrec["fix_taxrate_$i"];
				$record["fix_taxname_$offset"] = $stackrec["fix_taxname_$i"];
				$record["fix_unitsymbol_$offset"] = $stackrec["fix_unitsymbol_$i"];
					
				$record["sum_holder"] += $stackrec["units_$i"] * $stackrec["discount_price_$i"];

			}
		}

	}


	/*
	 *  retrive sums from stack, do not return current page
	 */
	function pagingLogic_retriveSums($record)
	{
		for ($i=1; $i<=atkconfig('pages_limit'); $i++){
			$stackrec = unserialize($record["page_stack_".$i]);

			for ($j=1; $j<=atkconfig('num_of_rows'); $j++){

				if ($stackrec["product_id$j"])
				{
					if ($stackrec["fix_discount_$j"])
					{
						$pages_grand_sum += $stackrec["units_$j"] * ($stackrec["fix_price_$j"] - ($stackrec["fix_price_$j"] / 100 * $stackrec["fix_discount_$j"]));
						$pages_grand_sumtax += (1 + $stackrec["fix_taxrate_$j"])  * $stackrec["units_$j"] * ($stackrec["fix_price_$j"] - ($stackrec["fix_price_$j"] / 100 * $stackrec["fix_discount_$j"]));
					}else
					{
						$pages_grand_sum += $stackrec["units_$j"] * $stackrec["fix_price_$j"];
						$pages_grand_sumtax += (1 + $stackrec["fix_taxrate_$j"])  * $stackrec["units_$j"] * $stackrec["fix_price_$j"];

					}
				}
			}
		}
		return array($pages_grand_sum, $pages_grand_sumtax);
	}


	/*
	 * fill ticket_lines with all pages
	 */
	function pagingLogic_fill_Ticket_lines(&$record)
	{
		// store current grid to stack also
		$this->pagingLogic_pushData($record, $record['current_page']);

		$this->pagingLogic_retriveAllData($record);

		$this->fill_Ticket_lines($record);
	}


	/*
	 * fill pages
	 */
	function pagingLogic_storeRecordsToPages(&$record, $current=null)
	{
		$offset = 1;
		for ($page=1; $page<=atkconfig('pages_limit'); $page++)
		{
			unset($stackrec);

			for ($i=1; $i<=atkconfig('num_of_rows'); $i++){

				if ($record["product_id$offset"])
				{
					$stackrec["product_id$i"] = $record["product_id$offset"];
					$stackrec["units_$i"] = $record["units_$offset"];
					$stackrec["fix_productname_$i"] = $record["fix_productname_$offset"];
					$stackrec["fix_price_$i"] = $record["fix_price_$offset"];
					$stackrec["fix_pricebuy_$i"] = $record["fix_pricebuy_$offset"];
					$stackrec["price_original_holder_$i"] = $record["price_original_holder_$offset"];
					$stackrec["stock_holder_$i"] = $record["stock_holder_$offset"];
					$stackrec["reservation_holder_$i"] = $record["reservation_holder_$offset"];
					$stackrec["fix_discount_$i"] = $record["fix_discount_$offset"];
					$stackrec["discount_price_$i"] =  $record["fix_price_$offset"] - ($record["fix_price_$offset"] / 100 * $record["fix_discount_$offset"]);
					$stackrec["fix_taxrate_$i"] = $record["fix_taxrate_$offset"];
					$stackrec["fix_taxname_$i"] = $record["fix_taxname_$offset"];
					$stackrec["fix_unitsymbol_$i"] = $record["fix_unitsymbol_$offset"];

					//if ($record["fix_discount_$offset"])
					//$stackrec["sum_$i"] = $record["units_$offset"] * ($record["fix_price_$offset"] - ($record["fix_price_$offset"] / 100 * $record["fix_discount_$offset"]));
					//else
					//$stackrec["sum_$i"] = $record["units_$offset"] * ($record["fix_price_$offset"] - $record["fix_price_$offset"]);

					++$offset;
				}
			}

			$record["page_stack_".$page] = serialize($stackrec);
		}


		if ($current)
		{
			// retrive first page data
			$this->pagingLogic_retriveData($record, $current);
			// calculate sum and hide/show rows
			$this->calculate_grandSum_total($record);
		}

		return $record;
	}


	/*
	 * action view
	 */
	function action_view(&$handler)
	{
		$record = &$handler->getRecord();

		$this->setDocumentTypeSpecifics($record, "view");

		// populate ajax records
		$stamp_product_issue = adodb_mktime(12,0,0,$record['date_product_issue']['month'],$record['date_product_issue']['day'],$record['date_product_issue']['year']);
		$stamp_date_payment = adodb_mktime(12,0,0,$record['date_payment']['month'],$record['date_payment']['day'],$record['date_payment']['year']);
		$record['payment_days'] = ($stamp_date_payment - $stamp_product_issue) / 86400;
		$record['customer_info']['id'] = $record['customer_id']['id'];

		// don't display has_notes label-text
		$this->getAttribute("has_notes")->setLabel(' ');

		// get all ticket lines and push them to pages
		$this->getTicket_lines($record, "view");

		$this->pagingLogic_storeRecordsToPages($record);

		// hide text_templates field
		$this->getAttribute("text_templates")->setLabel(' ');
		$record['text_templates'] = ' ';

		if ($record['notes']) $this->getAttribute("notes_holder")->setLabel('display');

		// reset grand total
		$record["sum_holder"] = 0;

		for ($p=1; $p<=atkconfig('pages_limit'); $p++)
		{
			$this->pagingLogic_retriveData($record, $p);

			for ($i=1; $i<=atkconfig('num_of_rows'); $i++)
			{

				if (!$record["product_id$i"]) break;

				if ($record["type_id"]["id"] == 1002 || $record["type_id"]["id"] == 1005)
				$record["fix_price_$i"] = $record["fix_pricebuy_$i"];

				$record["discount_price_$i"] = $record["fix_price_$i"] - ($record["fix_price_$i"] / 100 * $record["fix_discount_$i"]);
				$record["sum_$i"] = $record["units_$i"] * $record["discount_price_$i"];
				$record["sum_holder"] += $record["sum_$i"];
				$record["sumtax_holder"] += (1 + $record["fix_taxrate_$i"]) * $record["units_$i"] * $record["discount_price_$i"];

				// don't display "!" price warning
				//$this->getAttribute("fix_price_$i")->setLabel(' ');

				// display all rows
				$this->getAttribute("row_holder_$i")->setLabel(' ');

				$lines_display.= $this->drawLine($i);
			}

			// save rendered fieldset before next page is loaded
			$lines_display = $this->renderFieldSet($lines_display, $record);
		}
			
		$output.= '<table border="0" cellpadding="3" cellspacing="3">';

		$output.= '<tr><td>'.$this->drawTopGroup().'</td></tr>';

		$output.= '<tr><td>'.$this->drawHeader();
		// concate all pages to output
		$output.= $lines_display;

		$output.= $this->drawFooter().'</td></tr>';;

		$output.= '</td></tr>';

		$output.= '<tr><td>'.$this->drawNotes().'</td></tr>';

		$output.= '<tr><td align="center">'.$this->getFormButtons("view", $record).'</td></tr>';

		$output.= '</table>';


		// replace group field labels with real field content
		$output = $this->renderFieldSet($output, $record);

		$ui = &$this->getUi();
		$this->addStyle("style.css");
		$page = &$this->getPage();
		$contentbox = $ui->renderBox(array("title"=>$this->actionTitle($this->m_action), "content"=>$output));
		$fullpagecontent = $this->renderActionPage("run_report",$contentbox);

		$page->addContent($fullpagecontent);

		return;
	}


	/**
	 * Renders the fieldset.
	 *
	 * @return string rendered HTML
	 */
	protected function renderFieldSet($content, $record)
	{
		$replacements = array();

		$fields = array_unique($this->findFieldsetRecords($content));

		foreach ($fields as $field)
		{
			@list($attrName, $part) = explode('.', $field);
			$attr = $this->getAttribute($attrName);
			if ($attr)
			{
				switch ($part)
				{
					case 'label':
						$replacements[$attrName][$part] = $attr->getLabel($record, $mode);
						break;

					case 'field':
						$replacements[$attrName][$part] = $attr->display($record, $mode);
						break;
				}
			}
		}

		return $this->parse($content, $replacements);
	}


	/**
	 * Parse data into the string.
	 *
	 */
	function parse($string, $data)
	{
		// $string = $this->m_string;

		$fields = $this->findFieldsetRecords($string);

		foreach ($fields as $field)
		{
			$value = $data;

			$elements = explode(".", $field);

			foreach ($elements as $i => $el)
			{
				if (isset($value[$el]))
				{
					$value = $value[$el];
				}
			}

			$string = str_replace("[".$field."]", $value, $string);
		}
		return $string;
	}


	/*
	 * find fieldset records
	 */
	function findFieldsetRecords($content)
	{
		atkimport('atk.utils.atkstringparser');
		$parser = new atkStringParser($content);
		$matches = array();
		preg_match_all("/\[[^\]]*\]|[^[]+/",$parser->m_string,$matches);
		$fieldset = array();
		foreach ($matches[0] as $match)
		{
			// Check if need to parse the match
			if(strpos($match,'[')!==false && strpos($match,']')!==false)
			{
				$remove = array("[","]");
				$fieldset[] = str_replace($remove,"",$match);
			}
		}
		return $fieldset;
	}


	function checkCustomerCreditStatus($record, $display_window=true)
	{
		// store sum of ticket_lines to ticket
		$this->pagingLogic_pushData($record, $record['current_page']);
		$other_page_sums = $this->pagingLogic_retriveSums($record);
		$record["ticket_sum"] = $other_page_sums[1];

		$credit_status = $this->getCustomerCreditStatus($record['customer_id']['id'], $record["ticket_sum"]);
		if ($credit_status !='ok' && $display_window)
		{
			$page = &$this->getPage();
			$page->register_loadscript("alert('$credit_status');");
		}

		return $credit_status;

	}

}

?>
