<?php

// 1006

include_once ("class.ticket_base.inc");

class ticket_inventory extends ticket_base
{

	function ticket_inventory()
	{

		parent::ticket_base("ticket_inventory", NF_NO_ADD|NF_SPECIFIC_5);

		$prefix = $this->getUserTablePrefix();

		$this->add(new atkAttribute("id", AF_AUTOKEY|AF_HIDE, array(255,10,10)));

		$this->add(new atkManyToOneRelation("type_id", "tickets.ticket_types", AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkManyToOneRelation("status_id", "tickets.ticket_status", AF_HIDE_LIST|AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkAttribute("ticket_number", AF_LARGE|AF_SEARCHABLE|AF_READONLY|AF_UNIQUE));

		$this->add(new atkAttribute("counter", AF_HIDE));

		$this->add(new atkAttribute("external_number", AF_SEARCHABLE));
		$this->getAttribute("external_number")->setLabel(atktext('inventory_number'));
		$this->getAttribute("external_number")->setAttribSize(array(255,25,25));

		$this->add(new atkManyToOneRelation("customer_id", "customers.customer", AF_HIDE));

		$this->add(new atkManyToOneRelation("location_id", "locations.location", AF_HIDE_LIST|AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkDateAttribute("date_created", AF_OBLIGATORY|AF_DATE_EDIT_NO_DAY|AF_DATE_NO_CALENDAR));

		$this->add(new atkNumberAttribute("ticket_sum", AF_SEARCHABLE, 10, 2));

		$this->add(new atkTextAttribute("notes", array("cols"=>95,"rows"=>5), AF_HIDE_LIST));

		$this->add(new atkBoolAttribute("has_notes", AF_HIDE_LIST|AF_HIDE_VIEW));
		$this->getAttribute("has_notes")->setloadType(NOLOAD);
		$this->getAttribute("has_notes")->setstorageType(NOSTORE);
		$this->getAttribute("has_notes")->addDependency(array($this, "refresh_notes"));

		$this->add(new atkAttribute("notes_holder", AF_HIDE, 10));
		$this->getAttribute("notes_holder")->setloadType(NOLOAD);
		$this->getAttribute("notes_holder")->setstorageType(NOSTORE);
		$this->getAttribute("notes_holder")->setLabel('none');

		// group master
		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			$this->add(new atkManyToOneRelation("product_id$i", "products.product", AF_RELATION_AUTOCOMPLETE));
			$this->getAttribute("product_id$i")->setloadType(NOLOAD);
			$this->getAttribute("product_id$i")->setstorageType(NOSTORE);
			$this->getAttribute("product_id$i")->setAttribSize(array(255,25,25));
			$this->getAttribute("product_id$i")->addDependency(array($this, "refresh_dep_row$i"));
			$this->getAttribute("product_id$i")->setAutoCompleteSearchFields(array("name", "barcode"));
			// filter only track_stock products
			$this->getAttribute("product_id$i")->m_destinationFilter = $prefix."products.track_stock = '1'";
			// TODO: when only one is found, move to next field (useful for barcode scanners)

			// product change holder
			$this->add(new atkDummyAttribute("product_idchange$i", " ", AF_READONLY|AF_HIDE_LIST));
			$this->getAttribute("product_idchange$i")->setloadType(NOLOAD);
			$this->getAttribute("product_idchange$i")->setstorageType(NOSTORE);

			$this->add(new atkAttribute("fix_productname_$i", AF_READONLY|AF_HIDE, 10, 2));
			$this->getAttribute("fix_productname_$i")->setloadType(NOLOAD);
			$this->getAttribute("fix_productname_$i")->setstorageType(NOSTORE);


			$this->add(new atkNumberAttribute("stock_holder_$i", AF_READONLY|AF_HIDE, 10, 2));
			$this->getAttribute("stock_holder_$i")->setloadType(NOLOAD);
			$this->getAttribute("stock_holder_$i")->setstorageType(NOSTORE);

			$this->add(new atkNumberAttribute("available_holder_$i", AF_READONLY|AF_HIDE, 10, 2));
			$this->getAttribute("available_holder_$i")->setloadType(NOLOAD);
			$this->getAttribute("available_holder_$i")->setstorageType(NOSTORE);

			$this->add(new atkNumberAttribute("reservation_holder_$i", AF_READONLY|AF_HIDE, 10, 2));
			$this->getAttribute("reservation_holder_$i")->setloadType(NOLOAD);
			$this->getAttribute("reservation_holder_$i")->setstorageType(NOSTORE);

			$this->add(new atkNumberAttribute("units_$i", AF_HIDE, 6, 2 ));
			$this->getAttribute("units_$i")->setloadType(NOLOAD);
			$this->getAttribute("units_$i")->setstorageType(NOSTORE);
			$this->getAttribute("units_$i")->addDependency(array($this, "refresh_dep_row$i"));

			$this->add(new atkNumberAttribute("differences_holder_$i", AF_READONLY|AF_HIDE, 10, 2));
			$this->getAttribute("differences_holder_$i")->setloadType(NOLOAD);
			$this->getAttribute("differences_holder_$i")->setstorageType(NOSTORE);

			// row holder
			$this->add(new atkAttribute("row_holder_$i", AF_HIDE, 10));
			$this->getAttribute("row_holder_$i")->setloadType(NOLOAD);
			$this->getAttribute("row_holder_$i")->setstorageType(NOSTORE);
			$this->getAttribute("row_holder_$i")->setLabel('none');

		}

		// show first row initialy
		$this->getAttribute("row_holder_1")->setLabel('display');

		// mode holder, i.e. hide some rows on view mode
		$this->add(new atkAttribute("mode_holder", AF_HIDE, 10));
		$this->getAttribute("mode_holder")->setloadType(NOLOAD);
		$this->getAttribute("mode_holder")->setstorageType(NOSTORE);
		$this->getAttribute("mode_holder")->setLabel('none');

		// at least one record holder
		$this->add(new atkAttribute("at_least_one_product", AF_OBLIGATORY|AF_HIDE, 10, 2));
		$this->getAttribute("at_least_one_product")->setloadType(NOLOAD);
		$this->getAttribute("at_least_one_product")->setstorageType(NOSTORE);

		// page holder
		for ($i=1; $i<=atkconfig('pages_limit'); $i++){

			// add dummy for holding page data
			$this->add(new atkDummyAttribute("page_stack_".$i, " ", AF_READONLY|AF_HIDE_LIST));
			$this->getAttribute("page_stack_".$i)->setloadType(NOLOAD);
			$this->getAttribute("page_stack_".$i)->setstorageType(NOSTORE);

			// atkListAttribute arrays
			$options[] = atktext("page_num")." ".$i;
			$values[] = $i;
		}

		$this->add(new atkListAttribute("page_holder",$options, $values, AF_LIST_NO_NULL_ITEM|AF_HIDE_LIST));
		$this->getAttribute("page_holder")->setloadType(NOLOAD);
		$this->getAttribute("page_holder")->setstorageType(NOSTORE);
		$this->getAttribute("page_holder")->addDependency(array($this, "refresh_page"));
		//	current page
		$this->add(new atkNumberAttribute("current_page",AF_READONLY|AF_HIDE_LIST, 2, 0));
		$this->getAttribute("current_page")->setloadType(NOLOAD);
		$this->getAttribute("current_page")->setstorageType(NOSTORE);

		$this->addFieldSet('group_header',$this->drawHeader(),AF_HIDE_LIST|AF_NOLABEL,NULL, 1);

		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			$this->addFieldSet('group_line_'.$i,$this->drawLine($i),AF_HIDE_LIST|AF_NOLABEL,NULL, 10 + $i);
		}

		$this->addFieldSet('group_footer',$this->drawFooter(),AF_HIDE_LIST|AF_NOLABEL,NULL, 40);
		$this->addFieldSet('group_top',$this->drawTopGroup(),AF_HIDE_LIST|AF_NOLABEL,NULL, 200);
		$this->addFieldSet('group_master',$this->drawMasterGroup(),AF_HIDE_LIST|AF_NOLABEL,NULL, 220);
		$this->addFieldSet('group_notes',$this->drawNotes(),AF_HIDE_LIST|AF_NOLABEL,NULL, 240);

		$this->setOrder("date_created DESC");
		$this->setTable("tickets");


		$this->ticket_type = 1006;
		$this->ticket_typename = atktext("doctype_inventory");

		$this->getAttribute("external_number")->addFlag(AF_OBLIGATORY);

		$this->addFilter($this->getUserTablePrefix()."tickets.type_id = ".$this->ticket_type);

	}

	/*
	 * display top group - cusomer and dates
	 */
	function drawTopGroup()
	{
		$ret = '
		 <table width="100%" cellpadding="0" cellspacing="0">
		 	<tr>
		 		<td style="display:none;">
		 			<table>
						<tr>
							<td>
							[customer_id.field]
							</td> 	
						</tr>
					</table>
					
				</td>
				
				<td valign="top" align="left">
					<table cellpadding="0" cellspacing="3" style="border: 1px dashed lightGray; padding: 10px;">	
						<tr>
							<td  align="right">'.atktext('place_of_inventory').'</b>:</td>
							<td >[location_id.field]</td>
						</tr>						
						<tr>
							<td align="right">'.atktext('ducument_date').'</b>:</td>
							<td >[date_created.field]</td>
						</tr>
						<tr height=40>
							<td align="right">'.atktext('inventory_number').':<img align="top" src="'.atktext('required_image').'" border="0"></td>
							<td align="left">[external_number.field]</td>
						</tr>						
					</table>	
				</td>

			</tr>
			
			<tr height=30>
				<td colspan=2>
				
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
			
			<tr style="display:none"><td>[status_id.field]</td></tr> <!-- refresh only -->
						
		 </table>
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
				[current_page.field]';

		// add hiden dummy fields
		for ($i=1; $i<=atkconfig('pages_limit'); $i++){
			$ret .= '
			[page_stack_'.$i.'.field]
			';
		}

		$ret .='</td>
			</tr>					
		</table>
		 
		 <table cellpadding="4" cellspacing="0" style="color: white; background-color: #749ABE; ">

		 	<tr valign="middle">
				<th width=210 style="border-right: 1px solid white; text-align: center;">'.atktext('product_name').': </th>
				<th width=80 style="display:[mode_holder.label]; border-right: 1px solid white; text-align: center;">'.atktext('on_stock_sum').'</th>
				<th width=80 style="display:[mode_holder.label]; border-right: 1px solid white; text-align: center;">'.atktext('amount_available').'</th>
				<th width=80 style="display:[mode_holder.label]; border-right: 1px solid white; text-align: center;">'.atktext('reservation').'</th>
				<th width=80 style="border-right: 1px solid white; text-align: center;">'.atktext('amount_on_inventory').'</th>
				<th width=80 style="text-align: center;">'.atktext('differences').'</th>
				
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
				<td style="border-left: 1px solid lightGray; border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;">[product_id'.$line_num.'.field]</td>
				<td style="display:[mode_holder.label]; border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;"> [stock_holder_'.$line_num.'.field] </td>
				<td style="display:[mode_holder.label]; border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;"> [available_holder_'.$line_num.'.field] </td>
				<td style="display:[mode_holder.label]; border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;"> [reservation_holder_'.$line_num.'.field] </td>
				<td style="border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;">[units_'.$line_num.'.field]</td>
				<td style="border-right: 1px solid lightGray; border-bottom: 1px solid lightGray;">[differences_holder_'.$line_num.'.field]</td>
				
				<td style="display: none;"> /* must reload here */								
				[fix_productname_'.$line_num.'.field]
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
			<tr >
				<td colspan=5 height=10 align="right"  style="color: white; background-color: #749ABE;">
				</td>
				
				<td style="display: none;"> /* must reload here */								
				[ticket_sum.field]
			</tr>
			
		 </table>';
	}


	/*
	 * load initial values - add mode
	 */
	function initial_values()
	{
		$ret = parent::initial_values();

		$ret["status_id"]["id"] = 1001;
		$ret["customer_id"]["id"] = 1000;

		$lastnum = 1 + $this->getLastTicketCounter(1006);
		$ret["external_number"] = date('my').'-'.$lastnum;

		// display all rows on add
		$this->getAttribute("mode_holder")->setLabel('display');

		return $ret;
	}


	/*
	 * specifics as vendor/customer info
	 */
	function setDocumentTypeSpecifics(&$record, $mode=false)
	{

		$this->getAttribute("external_number")->setLabel('display');

	}


	/*
	 * refresh row data logic
	 */
	function refreshRow($line, &$record)
	{
		// don't hide rows
		$this->getAttribute("mode_holder")->setLabel('display');

		// see if $product_change_occured
		if ($record["product_id$line"]["id"] != $record["product_idchange$line"])
		{
			$record["product_idchange$line"] = $record["product_id$line"]["id"];
			$product_change_occured = true;
		}

		// restrict negative values
		if ($record["units_$line"]<0) $record["units_$line"] = 0;

		if ($record["product_id$line"])
		{

			if ($product_change_occured)
			{
				$product_id = $record["product_id$line"]["id"];

				$location_id = $this->getCurrentLocationId();

				$sql = "
						SELECT
						  ia32_products.name AS productname, 
						  ia32_units.symbol AS unitsymbol, 
						  ia32_stockcurrent.amount AS available,
						  ia32_stockcurrent.reservation AS reservation
						  			  
						FROM
						  ia32_products 
						  INNER JOIN ia32_units ON ia32_units.id = ia32_products.unit_id 
						  LEFT JOIN ia32_stockcurrent ON ia32_stockcurrent.product_id = ia32_products.id AND ia32_stockcurrent.location_id = '$location_id'
					
						WHERE
						ia32_products.id = '$product_id'
						";	

				$dbrec = $this->db32select($sql);
				$dbrecords = $dbrec[0];

				// add stock info
				$record["stock_holder_$line"] = $dbrecords["available"] + $dbrecords["reservation"];
				$record["available_holder_$line"] = $dbrecords["available"] * 1; // format bux fixed - force number format with math
				$record["reservation_holder_$line"] = $dbrecords["reservation"] * 1; // format bux fixed - force number format with math
					
				// on-load - unit reflects current stock
				$record["units_$line"] = $record["stock_holder_$line"];

				$record["fix_productname_$line"] = $dbrecords["productname"];
					
			}

			// calculate differences
			$record["differences_holder_$line"] = $record["units_$line"] - $record["stock_holder_$line"];

		} else {


			unset($record["product_id$line"]);
			unset($record["fix_productname_$line"]);
			unset($record["stock_holder_$line"]);
			unset($record["available_holder_$line"]);
			unset($record["reservation_holder_$line"]);
			unset($record["differences_holder_$line"]);
			unset($record["units_$line"]);
			unset($record["fix_price_$line"]);
			unset($record["fix_unitsymbol_$line"]);


			$this->delete_row_from_grid($record, $line);
		}

		$this->calculate_grandSum_total($record);

	}


	/*
	 * push data from grid to stack
	 */
	function pagingLogic_pushData(&$record, $page)
	{

		for ($i=1; $i<=atkconfig('num_of_rows'); $i++){
			$stackrec["product_id$i"] = $record["product_id$i"];
			$stackrec["fix_productname_$i"] = $record["fix_productname_$i"];
			$stackrec["stock_holder_$i"] = $record["stock_holder_$i"];
			$stackrec["available_holder_$i"] = $record["available_holder_$i"];
			$stackrec["reservation_holder_$i"] = $record["reservation_holder_$i"];
			$stackrec["units_$i"] = $record["units_$i"];
			$stackrec["differences_holder_$i"] = $record["differences_holder_$i"];

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
			$record["fix_productname_$i"] = $stackrec["fix_productname_$i"];
			$record["stock_holder_$i"] = $stackrec["stock_holder_$i"];
			$record["available_holder_$i"] = $stackrec["available_holder_$i"];
			$record["reservation_holder_$i"] = $stackrec["reservation_holder_$i"];
			$record["units_$i"] = $stackrec["units_$i"];
			$record["differences_holder_$i"] = $stackrec["differences_holder_$i"];

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
				$record["fix_productname_$offset"] = $stackrec["fix_productname_$i"];
				$record["stock_holder_$offset"] = $stackrec["stock_holder_$i"];
				$record["available_holder_$offset"] = $stackrec["available_holder_$i"];
				$record["reservation_holder_$offset"] = $stackrec["reservation_holder_$i"];
				$record["units_$offset"] = $stackrec["units_$i"];
				$record["differences_holder_$offset"] = $stackrec["differences_holder_$i"];
					
					
			}
		}

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
					$stackrec["fix_productname_$i"] = $record["fix_productname_$offset"];
					$stackrec["stock_holder_$i"] = $record["stock_holder_$offset"];
					$stackrec["available_holder_$i"] = $record["available_holder_$offset"];
					$stackrec["reservation_holder_$i"] = $record["reservation_holder_$offset"];
					$stackrec["units_$i"] = $record["units_$offset"];
					$stackrec["differences_holder_$i"] = $record["differences_holder_$offset"];

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
			$record["stock_holder_$i"] = $record["stock_holder_$line2"];
			$record["available_holder_$i"] = $record["available_holder_$line2"];
			$record["reservation_holder_$i"]	= $record["reservation_holder_$line2"];
			$record["units_$i"] = $record["units_$line2"];
			$record["differences_holder_$i"]	= $record["differences_holder_$line2"];

			// change is not occured, init for refreshRow()
			$record["product_idchange$i"] = $record["product_id$line2"]["id"];
		}

		$this->pagingLogic_storeRecordsToPages($record, $record['current_page']);

	}


	/*
	 * calculate grand total and hide/show rows
	 */
	function calculate_grandSum_total(&$record, $mode="")
	{
		$this->show_hide_rows($record);

		if ($mode=="view")
		$count = atkconfig('num_of_rows') * atkconfig('pages_limit');
		else
		$count = atkconfig('num_of_rows');

		for ($i=1; $i<=$count; $i++){

			// calculate differences
			if ($record["product_id$i"])
			{
				$record["differences_holder_$i"] = $record["units_$i"] - $record["stock_holder_$i"];
			}
		}

		// add other page sums to grand sum
		$this->pagingLogic_pushData($record, $record['current_page']);

		$this->setDocumentTypeSpecifics($record); // i.e. hide discount on receipt
	}


	/*
	 * fix buttons
	 */
	function getFormButtons($mode, $record)
	{
		// ia32.js for print and confirm window
		$page = &$this->getPage();
		$page->register_script(atkconfig("atkroot")."atk/javascript/ia32.js");
			
		$homepage = "tickets.ticket_stock";

		if ($mode=='view')
		{
			// cancel button
			$url = dispatch_url($homepage, "admin");
			$buttons.= '<input type="button" class="btn_cancel atkdefaultbutton" name="atkback"  value="'.atktext('back_button').'" onClick="location.href=\''.$url.'\'">';

			// print button
			$params = array("ticket_id"=>$record['atkprimkey']);
			$url = dispatch_url("reports.report_tickets", "pdfprint", $params);
			$buttons .= '&nbsp;&nbsp;<input type="button" name="button" value="'.atktext('print_inventory').'" onClick="launch_pdf(this.form,\''.$url.'\')">';

			return $buttons;
		}

		if ($mode=='add')
		{

			// confirm works on submit without need for confirmSensitiveAction js!
			$buttons.= '<input type="submit" class="btn_saveandclose atkdefaultbutton" name="atksaveandclose"  value="'.atktext('saveandclose').'" onClick="return confirm(\''.atktext('create_inventory_confirm').'\')">';

			// cancel button
			$url = dispatch_url($homepage, "admin");
			$buttons.= '&nbsp;&nbsp;<input type="button" class="btn_cancel" name="atkcancel"  value="'.atktext('cancel').'" onClick="location.href=\''.$url.'\'">';

			return $buttons;

		}

		// bad action, return to admin
		$this->redirect(dispatch_url($homepage, "admin"));
		die;
	}


	/*
	 * fill db table ticket_lines with grid data
	 */
	function fill_Ticket_lines($record, $line_offset=0)
	{
		// find number of records
		$max_rows_posibile = atkconfig('num_of_rows') * atkconfig('pages_limit');


		//consolidate products by id
		$consol = array();
		for ($i=1; $i<=$max_rows_posibile; $i++)
		{
			if ($record["product_id$i"])
			{
				$consol[$record["product_id$i"]["id"]]["units"] += $record["units_$i"];
				$consol[$record["product_id$i"]["id"]]["stock_holder"] = $record["stock_holder_$i"];
				$consol[$record["product_id$i"]["id"]]["differences_holder"] = $consol[$record["product_id$i"]["id"]]["units"] - $consol[$record["product_id$i"]["id"]]["stock_holder"];
				$consol[$record["product_id$i"]["id"]]["fix_productname"] = $record["fix_productname_$i"];

			}else
			break;
		}


		foreach ($consol as $id => $product)
		{

			// find current prices
			$prices = $this->getPrices($id);
			$fix_price = $prices['pricesell'];
			$fix_pricebuy = $prices['pricebuy'];

			$ticket_id = "'".stripquotes($record["id"])."'";
			++$line_offset; // line number
			$product_id = "'".stripquotes($id)."'";
			$units = $product["units"];
			$units_diff = $product["differences_holder"];
			$fix_productname = "'".$product["fix_productname"]."'";
			$fix_discount = 0;
			$fix_taxrate = 0;
			$fix_taxname = "' '";
			$fix_unitsymbol = "' '";

			$sql =  "insert into ia32_ticket_lines (
					ticket_id,
					line,
					product_id,
					units,
					units_diff,
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
				$units_diff,
				$fix_productname,
				$fix_price,
				$fix_pricebuy,
				$fix_discount,
				$fix_taxrate,
				$fix_taxname,
				$fix_unitsymbol
				)";

				$this->db32update($sql);
					
		}

		return $line_offset;
	}



	/*
	 * loading values from ticket lines
	 *
	 * before editing the record.
	 */
	function getTicket_lines(&$record, $mode="")
	{
		$sql =  "SELECT
    		
					ia32_ticket_lines.ticket_id AS ticket_id,
					ia32_ticket_lines.product_id AS product_id,
					ia32_ticket_lines.units AS units,
					ia32_ticket_lines.units_diff AS units_diff,		
					ia32_ticket_lines.units_diff * ia32_ticket_lines.fix_price AS sell_value,	
					ia32_ticket_lines.units_diff * ia32_ticket_lines.fix_pricebuy AS buy_value,	
					ia32_ticket_lines.fix_productname AS fix_productname,
					ia32_products.name AS productname								
					
					FROM 
					ia32_ticket_lines 
					LEFT JOIN ia32_products ON ia32_products.id = ia32_ticket_lines.product_id
					
					WHERE ticket_id = '".stripquotes($record['id'])."'
					";


		$recs = $this->db32select($sql);

		$record["ticket_sum"] = 0;
		$i = 0;
		foreach ($recs as $line)
		{
			++$i;
			$record["id"] = $line['ticket_id'];

			$record["product_id$i"]["id"] = $line['product_id'];
			$record["product_id$i"]["name"] = $line['productname'];	// must load name here

			$record["fix_productname_$i"] = $line['fix_productname'];

			// value of differences
			$record["ticket_sum"] += $line['buy_value'];

			$record["units_$i"] = $line['units'];
			$record["differences_holder_$i"] = $line['units_diff'];
		}

		return $record;
	}




	/*
	 * pre add
	 */
	function preAdd(&$record)
	{
		$record['ticket_sum'] = 0; // fill - obligatory

		// created by
		$this->getAttribute('creator')->removeFlag(AF_READONLY);
		$record['creator'] = atkGetUserId();

		// get next document number
		$record["counter"] = $this->getLastTicketCounter('1006') + 1;
		$record["ticket_number"] = $this->getNextDocumentNumber('1006', $record["counter"]);

		return true;
	}


	/*
	 * post add,
	 */
	function postAdd(&$record)
	{

		$this->pagingLogic_fill_Ticket_lines($record);

		// for some reason we must strip quotes here
		$record["id"] = stripQuotes($record["id"]);

		// fix stock
		$this->changeStock($record["id"], 0, false, false, true);

		// apply finance
		// DISABLED FOR NOW:
		//$this->financeUpdateByTicket($record["id"]);

		return;

	}


	/*
	 * pre delete
	 */
	function preDelete($record)
	{
		// inventory cannot be deleted
		return false;
	}


	/*
	 * post update
	 */
	function postUpdate(&$record)
	{
		// inventory cannot be updated
		return false;
	}


	/*
	 * refresh paging if user changes page
	 */
	function refresh_page(atkEditFormModifier $modifier)
	{
		// don't hide rows
		$this->getAttribute("mode_holder")->setLabel('display');

		parent::refresh_page($modifier);

	}


}

?>
