<?php

class ia32node extends atkNode
{


	var $std_att_size = array(255, 30, 30);
	var $std_num_size = array(10,10,10);


	/*
	 * setTable overloader - load per user db and table
	*
	* if parent = true, do not use table prefix
	*/
	function setTable($tablename, $seq="", $db=NULL, $parent=false)
	{
		$db = $this->getUserDB();

		// sometimes we need parent behaviour (atkrecyclebin)
		if ($parent)
		{
			parent::setTable($tablename, $seq, $db);
			return;
		}

		$tablename = $this->getUserTablePrefix().$tablename;

		$this->m_table      = $tablename;
		if ($seq=="") $seq = $tablename;
		$this->m_seq        = $seq;
		$this->m_db = $db;

	}

	/*
	 * reload current node and action
	*/
	function reloadPage($time=0)
	{
		global $ATK_VARS;
		$reloadlink = 'index.php?atknodetype='.$ATK_VARS['atknodetype'].'&atkaction='.$ATK_VARS['atkaction'];
		header("Refresh: ".$time."; url=$reloadlink");
	}

	/*
	 * return user db
	*/
	function getUserDB()
	{
		$db = getUser();
		return $db[atkconfig('auth_dbconfigfield')];
	}

	/*
	 * return core db name
	*/
	function getCoreDB()
	{
		$db = 'admin';
		return $db;
	}


	/*
	 * return user prefix
	*/
	function getUserTablePrefix()
	{
		$prefix = getUser();
		return $prefix[atkconfig('auth_tblprefixfield')];
	}

	/*
	 * Get creator dbfield template
	* user @ date
	*
	*/
	function getUserTrack()
	{
		global $ATK_VARS;
		$userobj = getuser();
		$ret = $userobj['user']." @ ".date(atktext('date_time_format'));

		return $ret;
	}


	function uuid($prefix = '')
	{
		$chars = md5(uniqid(mt_rand(), true));
		$uuid  = substr($chars,0,8) . '-';
		$uuid .= substr($chars,8,4) . '-';
		$uuid .= substr($chars,12,4) . '-';
		$uuid .= substr($chars,16,4) . '-';
		$uuid .= substr($chars,20,12);
		return $prefix . $uuid;
	}


	function formatNumber($number, $decimals=2, $noThousandsSeparators=NULL)
	{
		$decimalSeparator = atktext('decimal_separator');
		$thousandsSeparator = atktext('thousands_separator');
		if($noThousandsSeparators) $thousandsSeparator = '';

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
	 * same as notification but dies at the end!
	*/
	function display_error($content, $width=250, $height=60)
	{
		$this->display_notification($content, $width, $height);
		die;
	}

	/*
	 * notification
	*
	* WARNING - watch if die is necessary after this function (double display bug)
	* do not use die on succesfull return pre/post functions
	*/
	function display_notification($content, $width=250, $height=60)
	{
		$full_content .= '<table border="0" width="'.$width.'" height="'.$height.'">';
		$full_content .= '<tr>';
		$full_content .= '<td align="center">';
		$full_content .= $content;
		$full_content .= '</td>';
		$full_content .= '</tr>';
		$full_content .= '</table>';

		$ui = &$this->getUi();
		$theme = &atkTheme::getInstance();
		$page = &$this->getPage();
		$page->register_style($theme->stylePath("style.css"));

		$page->head(atktext("Delete log"));
		$page->body();
		$box = $ui->renderBox(array("title"=>$this->actionTitle($this->m_action),"content"=>$full_content));
		$actionpage = $this->renderActionPage("view", array($box));
		$page->addContent($actionpage);

		$output = &atkOutput::getInstance();

		$output->output($page->render($windowtitle));
		$output->outputFlush();

		return;
	}


	/**
	 * change stock volume
	 *
	 * @param integer $direction  Can be add = 1 or delete = -1
	 * @param string $ticketid This is ticket id
	 * @param boolean $reservation If true this will be reservation change, else it's amount
	 */
	function changeStock($ticketid, $direction=1, $reservation=false, $order=false, $inventory=false, $location_send=false)
	{
		if ($reservation)
			$column = 'reservation';
		elseif ($order)
		$column = 'on_order';
		else
			$column = 'amount';

		$location_data = $this->getLocationData($location_send);
		$location_id = $location_data['id'];

		// find next id
		$sql =  "SELECT * FROM ia32_ticket_lines WHERE ticket_id = '".stripquotes($ticketid)."'";

		$recs = $this->db32select($sql);

		unset($sql);
		// if product is not in the stockcurrent it is not volume-tracked
		foreach ($recs as $product)
		{
			if ($inventory)
			{
				$sql .=  "UPDATE ia32_stockcurrent SET ".$column." = ".$column." + ".$product['units_diff']." WHERE product_id = '".$product['product_id']."' AND location_id = '".$location_id."' |nextsql_delimiter|";
			}
			else
				$sql .=  "UPDATE ia32_stockcurrent SET ".$column." = ".$column." + ".$direction * $product['units']." WHERE product_id = '".$product['product_id']."' AND location_id = '".$location_id."' |nextsql_delimiter|";
		}

		$this->db32update($sql);
			
		return;

	}


	/**
	 * check customer credit limit / history
	 *
	 */
	function getCustomerCreditStatus($customerid, $amount)
	{
		if (!$customerid) return 'ok';

		$sql =  "SELECT * FROM ia32_customers WHERE id = '".stripquotes($customerid)."'";
		$recs = $this->db32select($sql);

		// if credit status = 1002 (stop) return false
		if ($recs[0]['credit_status_id'] == '1002') return atktext('customer_in_blocade');

		// return ok if no credit limit or = 0
		if (!$recs[0]['credit_limit'] || $recs[0]['credit_limit'] == '0')
			return 'ok';

		// check if credit limit exceeds amount
		if ($recs[0]['credit_limit'] + $recs[0]['finance_sum'] <= $amount)
			return atktext('exceeds_credit_limit');

		return 'ok';
	}


	/**
	 * get customer data
	 *
	 */
	function getCustomerData($id)
	{
		$sql =  "SELECT * FROM ia32_customers WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		return $recs[0];
	}


	/**
	 * get customer from ticket
	 *
	 */
	function getCustomerFromTicket($id)
	{
		$sql =  "SELECT customer_id FROM ia32_tickets WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		return $this->getCustomerData($recs[0]['customer_id']);
	}


	/**
	 * get ticket data
	 *
	 */
	function getTicketData($id)
	{
		$sql =  "SELECT * FROM ia32_tickets WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		return $recs[0];
	}


	/**
	 * get logo image name (top firm logo for pdf reports)
	 *
	 * return array: logo_picture, logo_width
	 */
	function getLogoImage()
	{
		$sql =  "SELECT logo_picture, logo_width FROM ia32_config";
		$recs = $this->db32select($sql);
		return $recs[0];
	}


	/**
	 * return ticket type; array(type_id, name)
	 *
	 */
	function getTicketType($ticket_id)
	{
		if (!$ticket_id) return 'none';

		$sql =  "SELECT type_id FROM ia32_tickets WHERE id = '".stripquotes($ticket_id)."'";
		$recs = $this->db32select($sql);

		$ret = array();

		$ret['id'] = $recs[0]['type_id'];

		switch ($ret['id'])
		{
			case 1004:
				$ret['name'] = 'invoice';
				break;

			case 1001:
				$ret['name'] = 'quote';
				break;

			case 1002:
				$ret['name'] = 'receipt';
				break;

			case 1003:
				$ret['name'] = 'credit_note';
				break;

			case 1005:
				$ret['name'] = 'purchase_order';
				break;

			case 1006:
				$ret['name'] = 'inventory';
				break;

			case 1007:
				$ret['name'] = 'internal_transfer_out';
				break;

			case 1008:
				$ret['name'] = 'internal_transfer_in';
				break;

			default:
				$ret['name'] = 'none';
				break;
		}

		return $ret;
	}


	/**
	 * get tax rate based on tax_id
	 *
	 */
	function getTaxRate($tax_id)
	{
		$sql =  "SELECT rate FROM ia32_taxes WHERE id = '".stripquotes($tax_id)."'";
		$recs = $this->db32select($sql);

		return $recs[0]['rate'];
	}


	/**
	 * get PRODUCT category name based on id
	 *
	 */
	function getCategoryName($id)
	{
		$sql =  "SELECT name FROM ia32_category WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		return $recs[0]['name'];
	}


	/**
	 * get finance type name based on id
	 *
	 */
	function getFinanceTypeName($id)
	{
		$sql =  "SELECT name FROM ia32_finance_types WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		return $recs[0]['name'];
	}


	/**
	 * get pricelist name based on id
	 *
	 */
	function getPricelistName($id)
	{
		$ret = $this->getPricelistData($id);

		return $ret['name'];
	}


	/**
	 * get pricelist data by id
	 *
	 */
	function getPricelistData($id)
	{
		$sql =  "SELECT * FROM ia32_price_list WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		return $recs[0];
	}


	/**
	 * get pricelist id by (current) location
	 *
	 */
	function getPricelistId($id=null)
	{
		if (!$id) $id=$this->getCurrentLocationId();

		$sql =  "SELECT pricelist_id FROM ia32_locations WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		return $recs[0]['pricelist_id'];
	}


	/**
	 * get product name based on id
	 *
	 */
	function getProductName($id)
	{
		return $this->getProductData($id, 'name');
	}


	/**
	 * get location name based on id
	 *
	 */
	function getLocationName($id)
	{
		$ret = $this->getLocationData($id);
		return $ret['name'];
	}


	/**
	 * get customer name based on id
	 *
	 */
	function getCustomerName($id)
	{
		$ret = $this->getCustomerData($id);
		return $ret['name'];
	}


	/**
	 * get products data
	 *
	 */
	function getProductData($id, $field=null)
	{
		$sql =  "SELECT * FROM ia32_products WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		if ($field) return $recs[0][$field];
		else
			return $recs[0];
	}


	/**
	 * get customer status data
	 *
	 */
	function getCustomerStatusData($id)
	{
		$sql =  "SELECT * FROM ia32_customer_status WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		return $recs[0];
	}


	/**
	 * get customer type data
	 *
	 */
	function getCustomerTypeData($id)
	{
		$sql =  "SELECT * FROM ia32_customer_types WHERE id = '".stripquotes($id)."'";
		$recs = $this->db32select($sql);

		return $recs[0];
	}


	/**
	 * get currency symbol/rate array based on id
	 *
	 */
	function getCurrencyData($id)
	{
		$data = $this->db32select("SELECT * FROM ia32_currency WHERE id = '".stripQuotes($id)."'");
		return array('symbol'=>$data[0]['symbol'],'rate'=>$data[0]['rate']);
	}


	/**
	 * get configuration
	 *
	 */
	function getConfig()
	{
		$data = $this->db32select("SELECT * FROM ia32_config");
		return $data[0];
	}

	/**
	 * update finance data by ticket_id
	 *
	 */
	function financeUpdateByTicket($ticket_id, $reverse=false)
	{
		// find next id TODO: count can be slow, use db_sequence instead!
		$sql =  "SELECT count(*) AS lastnum FROM ia32_finance";
		$recs = $this->db32select($sql);
		$nextid = $recs[0]['lastnum'] + 1001; // start from 1000 rather than 1
		$chars = md5(uniqid(mt_rand(), true));
		$uuid  = substr($chars,0,10);
		$nextid = $nextid."-".$uuid;

		$sql =  "SELECT * FROM ia32_tickets WHERE id = '".stripquotes($ticket_id)."'";
		$recs = $this->db32select($sql);

		$type_id = $recs[0]['type_id'];
		$customer_id = $recs[0]['customer_id'];
		$amount = $recs[0]['ticket_sum'];
		$creator = $this->getUserTrack();
		$date = $recs[0]['date_created'];
		if (!$notes) $notes = $recs[0]['ticket_number'];

		switch ($type_id)
		{
			case '1004': // invoice - charge
				$mflow_type = 1003; // doctype_invoice_charge
				$amount = $amount * -1;
				break;

			case '1003': // credit note - discharge (return money)
				$mflow_type = 1002; // doctype_invoice_payment
				break;

			case '1002': // receipt - discharge (must pay him now)
				$mflow_type = 1005; // doctype_invoice_charge
				break;

				// inventory disabled
				//case '1006': // inventory  - self charge/discharge
				//	$mflow_type = 1006; // doctype_inventory
				//	break;

			default:

				var_dump('financeUpdateByTicket: invalid type - '.$type_id);
				die;
		}

		// i.e. on ticket delete
		if ($reverse)
		{
			$amount = $amount * -1;
		}

		// create finance data
		$sql =  "INSERT INTO ia32_finance
				(id, type_id, date, customer_id, amount, ticket_id, creator, notes)
				VALUES ('".$nextid."' , '".$mflow_type."' , '".$date."' , '".$customer_id."' , ".$amount." , '".$ticket_id."' , '".$creator."' , '".$notes."')";
		$this->db32insert($sql);


		// update finance on customer table also
		$sql =  "UPDATE ia32_customers SET finance_sum = finance_sum + $amount
		WHERE id = '".$customer_id."' ";
		$this->db32update($sql);

		return;

	}


	/*
	 * add leading zero's
	*/
	function addLeadingZerros($number)
	{
		if ($number < 10000)  $leading_zero = '0';
		if ($number < 1000)  $leading_zero = '00';
		if ($number < 100)  $leading_zero = '000';
		if ($number < 10)  $leading_zero = '0000';

		return $leading_zero.$number;
	}


	/*
	 * sql select short
	*/
	function db32select($sql)
	{
		$conn = $this->getUserDB();

		$db = &atkGetDb($conn);
		$user_prefix = $this->getUserTablePrefix();

		$sql = str_replace('ia32_', $user_prefix, $sql);

		$ret = $db->getrows($sql);

		return $ret;
	}


	/*
	 * sql update short
	*
	* use |nextsql_delimiter| as delimiter for multi-line sql
	*/
	function db32update($sql)
	{
		$conn = $this->getUserDB();
		$db = &atkGetDb($conn);
		$user_prefix = $this->getUserTablePrefix();

		$sql = str_replace('ia32_', $user_prefix, $sql);

		$multi_sql = explode('|nextsql_delimiter|', $sql);

		foreach ($multi_sql as $key => $single_sql) {
			if ($single_sql)
				$db->query($single_sql);
		}

		$db->commit();

		return $ret;
	}


	/*
	 * sql commit
	*
	* this is usefull before die() on i.e. postAdd
	*/
	function db32commit()
	{
		$conn = $this->getUserDB();
		$db = &atkGetDb($conn);
		$db->commit();
	}



	/*
	 * sql insert short, same as update
	*/
	function db32insert($sql)
	{
		return $this->db32update($sql);
	}


	/*
	 * sql delete short, same as update
	*/
	function db32delete($sql)
	{
		return $this->db32update($sql);
	}

	/**
	 *  find product's buying price
	 *
	 * @param $product_id
	 * @return price
	 */
	function getPriceBuy($product_id, $pricelist_id=null)
	{
		if (!$pricelist_id) $pricelist_id=$this->getPricelistId();

		$sql = "SELECT pricebuy
				FROM ia32_prices
				WHERE product_id = '".stripQuotes($product_id)."' AND pricelist_id = '".stripQuotes($pricelist_id)."'";

		$recs = $this->db32select($sql);

		if (!$recs[0]['pricebuy']) $recs[0]['pricebuy'] = 0;

		$pricebuy = $recs[0]['pricebuy'];

		return $pricebuy;
	}


	/*
	 * get location data
	*/
	function getLocationData($id=null)
	{
		if (!$id) $id = $this->getCurrentLocationId();

		$data = $this->db32select("SELECT * FROM ia32_locations WHERE id = '".stripQuotes($id)."'");
		return $data[0];
	}


	/**
	 * get all locations
	 *
	 */
	function getAllLocations()
	{
		$sql =  "SELECT * FROM ia32_locations";
		$recs = $this->db32select($sql);

		return $recs;
	}


	/**
	 * get last ticket id number
	 *
	 */
	function getLastTicketCounter($type_id, $location_id=false)
	{
		if (!$location_id) $location_id = $this->getCurrentLocationId();

		$sql =  "
				SELECT max(counter) AS last_id
				FROM ia32_tickets
				WHERE
				type_id = '".stripQuotes($type_id)."'
				AND location_id = '$location_id' ";


		// reset document numbers on each year
		// this can be buggy when creating documents with last year dates
		// DISABLED!
		// $sql = $sql." AND YEAR(date_created) = YEAR(CURDATE())";

		$recs = $this->db32select($sql);

		return $recs[0]['last_id'];
	}


	/**
	 * get next ticket number
	 *
	 */
	function getNextDocumentNumber($type_id, $next_counter=null, $location_id=false)
	{
		if (!$next_counter)
			$nextid = $this->getLastTicketCounter($type_id, $location_id) + 1;
		else
			$nextid = $next_counter;

		$nextid = $next_counter + atkconfig('document_number_offset');

		if ($type_id == 1004 && atkconfig('use_731_checkdigit')){
			$nextid = $this->addCheckDigit731($nextid);

			if (atkconfig('add_leading_zeros')){
				$nextid = $this->addLeadingZerros($nextid);
			}

			return $nextid;
		}

		if ($type_id == 1004 && atkconfig('use_luhn_checkdigit')){
			$nextid = $this->addLuhnDigit($nextid);

			if (atkconfig('add_leading_zeros')){
				$nextid = $this->addLeadingZerros($nextid);
			}

			return $nextid;
		}

		if (atkconfig('add_leading_zeros')){
			$nextid = $this->addLeadingZerros($nextid);
		}

		$ret = $this->getDocumentPrefix($type_id).$this->getLocationPrefix($location_id).date("y").'-'.$nextid;

		return $ret;
	}

	function addCheckDigit731($sViite) {

		$iKerroin = 7;
		$iSumma = 0;

		if (strlen($sViite) > 19) {
			echo "Document number too big! > 19";
			die;
		} else if (strlen($sViite) == 0) {
			echo "Document number not numeric!";
			die;
		}

		if (!is_numeric($sViite)) {
			echo "Document number not numeric!";
			die;
		}

		for ($iLaskuri = strlen($sViite); $iLaskuri > 0; $iLaskuri--) {
			$iSumma += substr($sViite, $iLaskuri - 1, 1) * $iKerroin;
			switch ($iKerroin) {
				case 7:
					$iKerroin = 3;
					break;
				case 3:
					$iKerroin = 1;
					break;
				case 1:
					$iKerroin = 7;
					break;
			}
		}

		$iSumma = substr(10 - ($iSumma % 10), -1);

		$ret = $sViite . $iSumma;
		settype($ret, "integer");
		return $ret;
	}


	function addLuhnDigit($number) {
	
		$pre = $number;
	
		if (strlen($number)%2 == 1){
			$number = $number*10;
		}
	
		$luhnnumber = 0;
		while ($luhnnumber == 0){
			$sum=0;$digit=0;$first=0;$second=0;$x=0;
			for ($x=0; $x<strlen($number); $x++) {
				$digit = substr($number,$x,1);
				if ($x%2 == 0){
					$digit *= 2;
					if (strlen($digit) == 2){
						$digit = substr($digit,0,1) + substr($digit,1,1);
					}
				}
				$sum += $digit;
			}
			if($sum%10!=0){
				$number = $number+1;
			}
			else{
				$luhnnumber = 1;
			}
		}
	
		return (int)$pre . (int)$digit;
	}
	

	/**
	 * get document prefix
	 *
	 */
	function getDocumentPrefix($type_id)
	{
		switch ($type_id)
		{
			case '1001':
				$name = "quote";
				break;

			case '1002':
				$name = "receipt";
				break;
					
			case '1003':
				$name = "creditnote";
				break;

			case '1004':
				$name = "invoice";
				break;

			case '1005':
				$name = "purchaseorder";
				break;

			case '1006':
				$name = "inventory";
				break;

			case '1007':
				$name = "internal_transfer_out";
				break;

			case '1008':
				$name = "internal_transfer_in";
				break;

			default:
				echo('error: no prefix for type - '.$type_id);
				die;
		}

		$config = $this->db32select("SELECT prefix_$name AS prefix FROM ia32_config");
		return $config[0]['prefix'];

	}


	/**
	 * get location prefix
	 *
	 */
	function getLocationPrefix($location_id=false)
	{
		$location_data = $this->getLocationData($location_id);

		if ($location_data)
			return $location_data['prefix'];
		else
			return '';
	}


	/**
	 *  find product's buying/selling price
	 *
	 * @param $product_id
	 * @return $prices array()
	 */
	function getPrices($product_id, $pricelist_id=null)
	{
		if (!$pricelist_id) $pricelist_id=$this->getPricelistId();

		$sql = "SELECT pricesell, pricebuy
				FROM ia32_prices
				WHERE product_id = '".stripQuotes($product_id)."' AND pricelist_id = '".stripQuotes($pricelist_id)."'";

		$recs = $this->db32select($sql);

		if (!$recs[0]['pricesell']) $recs[0]['pricesell'] = 0;
		if (!$recs[0]['pricebuy']) $recs[0]['pricebuy'] = 0;

		$prices = array("pricesell" => $recs[0]['pricesell'], "pricebuy" => $recs[0]['pricebuy']);

		return $prices;
	}


	/**
	 *  store product's buying/selling price to db
	 *
	 * pricebuy, pricesell
	 * @param $product_id
	 * @return $prices array()
	 */
	function setPrices($product_id, $pricelist_id=null, $pricebuy, $pricesell)
	{
		if (!$pricelist_id) $pricelist_id=$this->getPricelistId();

		if (!$pricebuy) $pricebuy = 0;
		if (!$pricesell) $pricesell = 0;

		$sql =  "SELECT product_id FROM ia32_prices WHERE product_id = '".stripquotes($product_id)."' AND pricelist_id = '".stripquotes($pricelist_id)."'";
		$recs = $this->db32select($sql);

		// new pricelist entry
		if (!$recs[0]['product_id'])
		{
			$sql =  "INSERT INTO ia32_prices (pricelist_id, product_id, pricebuy, pricesell) VALUES ('".stripquotes($pricelist_id)."', '".stripquotes($product_id)."', ".$pricebuy.", ".$pricesell.")";
			$this->db32insert($sql);

			return;
		}

		// just update existing prices
		$sql =  "UPDATE ia32_prices SET pricebuy = ".$pricebuy.", pricesell = ".$pricesell." WHERE product_id = '".stripquotes($product_id)."' AND pricelist_id = '".stripquotes($pricelist_id)."'";
		$this->db32update($sql);

	}


	/**
	 *  set product's buying price
	 *
	 * @param $product_id
	 * @return price
	 */
	function setPriceBuy($product_id, $pricebuy, $pricelist_id=null)
	{
		if (!$pricelist_id) $pricelist_id = $this->getPricelistId();

		$sql = "UPDATE ia32_prices SET pricebuy = $pricebuy
		WHERE product_id = '".stripQuotes($product_id)."' AND pricelist_id = '".stripQuotes($pricelist_id)."'";

		$recs = $this->db32update($sql);

		return;
	}


	/**
	 *  set product's selling price
	 *
	 * @param $product_id
	 * @return price
	 */
	function setPriceSell($product_id, $pricesell, $pricelist_id=null)
	{
		if (!$pricelist_id) $pricelist_id = $this->getPricelistId();

		$sql = "UPDATE ia32_prices SET pricesell = $pricesell
		WHERE product_id = '".stripQuotes($product_id)."' AND pricelist_id = '".stripQuotes($pricelist_id)."'";

		$recs = $this->db32update($sql);

		return;
	}


	/**
	 *  return product's current stock
	 *
	 * @param $product_id
	 * @param $location_id
	 * @return price
	 */
	function getCurrentStock($product_id)
	{
		$location_id = $this->getCurrentLocationId();

		$sql = "SELECT amount + reservations as stock
				FROM ia32_stockcurrent
				WHERE product_id = '".stripQuotes($product_id)."' AND location_id = '".stripQuotes($location_id)."'";

		$recs = $this->db32select($sql);
		$stock = $recs[0]['stock'];

		return $stock;
	}

	//TODO: fix this, DANGER - SCRIPT DIES HERE! not suitable for postAdd and similar
	function actionRequireLogout()
	{
		$page = &$this->getPage();
		$page->register_loadscript("alert('".atktext('action_require_logout')."');");

		session_regenerate_id(true);
		$this->redirect("index.php?atklogout=1");
		die();
	}

	/*
	 * get current location
	*/
	function getCurrentLocationId()
	{
		$user = &atkGetUser();

		$location_id = $user['location_id'];

		// default location is 1000
		if ($location_id === null) $location_id = 1000;

		return $location_id;
	}


	/*
	 * draw hidden fieldset(s)
	*
	* here goes all holder attributtes
	*
	*/
	function drawHiddenFieldset($count=1)
	{
		$ret = '
				<table border="0">
				<tr>
				<td style="display: none;">';

		for ($i=1; $i<=$count; $i++){
			$ret .= '
					[holder_'.$i.'.field]
							';
		}

		$ret .='
				</td>
				</tr>
				</table>';

		return $ret;

	}

	/**
	 * ...similar exist in atkNumberAttribute
	 * convert a formatted number to a real number
	 * @param String $number The number that needs to be converted
	 * @param String $decimal_separator override decimal separator
	 * @param String $thousands_separator override thousands separator
	 * @return String The converted number
	 */
	function removeSeparators($number, $decimal_separator="", $thousands_separator="")
	{
		if (empty($decimal_separator))   $decimal_separator = atktext('decimal_separator');
		if (empty($thousands_separator)) $thousands_separator = atktext('thousands_separator');

		if ($decimal_separator == $thousands_separator)
		{
			atkwarning('invalid thousandsseparator. identical to the decimal_separator');
			$thousands_separator = '';
		}

		if (strstr($number,$decimal_separator) !== false)
		{
			// check invalid input
			if(substr_count($number,$decimal_separator) > 2) return $number;

			$number = str_replace($thousands_separator,'',$number);
			$number = str_replace($decimal_separator,'.',$number);

			if(substr_count($number,'.') > 1)
			{
				$parts = explode('.',$number);
				$decimals = array_pop($parts);
				$number = implode('',$parts).'.'.$decimals;
			}
		}
		else
		{
			$number = str_replace($thousands_separator,'',$number);
		}

		return $number;
	}


	/**
	 * @param array: Column headings
	 * @param array: Content to go in CSV
	 *
	 * @return string: CSV format string from input
	 *
	 * @author  : Jim Hollington
	 * @date    : 2010-03-24
	 * @desc    : A simple class that takes 2 arrays and outputs a CSV file
	 * @version : v0.1
	 * @license : GPL
	 * http://oddvalue.co.uk/blog/2010/03/export-to-csv-php-class/
	 *
	 * usage:
	 * $columns = array(
		'col1' => 'Column 1',
		'col2' => 'Column 2',
		'col3' => 'Column 3'

		$data = array(
		0 => array(
		'col1' => 'Row 1',
		'col2' => 'Row 1',
		'col3' => 'Row 1'
		),
		1 => array(
		'col1' => 'Row 2',
		'col2' => 'Row 2',
		'col3' => 'Row 2'
		),
		2 => array(
		'col1' => 'Row 3',
		'col2' => 'Row 3',
		'col3' => 'Row 3'
		))
	 */
	public function buildCSV($columns, $inputdata, $format=NULL)
	{
		// watch! enclose only when cell has delimiter, that is more than ok!
		$enclosure = '"';
		$delimiter = ';';

		$csv = ''; // initialise csv variable

		foreach($columns as $key=>$heading) // csv column headings
		{
			$csv .= $heading.$delimiter; // concat heading onto row
			//$data[$key] = $inputdata[$key];
		}
		$csv .= "\n"; // all the headings have been added so move to new line for csv content

		$i=0;
		foreach($inputdata AS $key=>$value)
		{
			foreach($columns as $columnkey=>$heading) // csv column headings
			{
				if($format && $format[$columnkey]=="number")
				{
					$data[$i][$columnkey] = $this->formatNumber($value[$columnkey],2,TRUE);
				}
				else
				{
					$data[$i][$columnkey] = $value[$columnkey];
				}
			}
			$i++;
		}

		foreach($data as $row) // csv table content
		{
			foreach($columns as $column => $t)
			{
				// remove new lines from wors
				if(strpos($row[$column],"\n")) {
					$row[$column] = str_replace(array("\n","\r")," ",$row[$column]);
				}

				if(strpos($row[$column],$delimiter)) // if cell content has a comma in it...
				{
					// ...double any existing quotes to escape them...
					$row[$column] = str_replace($enclosure,$enclosure.$enclosure,$row[$column]);
					// ...and wrap the cell in quotes so the comma doesn't break everything.
					$row[$column] = $enclosure.$row[$column].$enclosure;
				}
				$csv .= $row[$column].$delimiter; // concat the value onto the row
				if($t==end($columns))
				{
					// if we're at the end of a row move to a new line for next row
					$csv .= "\n";
				}
			}
		}

		return $csv;
	}


}

?>