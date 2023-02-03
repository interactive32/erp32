<?php


userelation("atkmanytoonerelation");


class product extends ia32Node
{

	function product()
	{
		$this->atkNode("product", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_UNIQUE|AF_OBLIGATORY|AF_SEARCHABLE, $this->std_att_size));
		$this->add(new atkManyToOneRelation("unit_id", "config.units_config", AF_OBLIGATORY|AF_HIDE_LIST));

		$this->add(new atkManyToOneRelation("tax_id", "config.taxes", AF_OBLIGATORY|AF_HIDE_LIST));
		$this->getAttribute("tax_id")->addDependency(array($this, "refresh_info"));

		$this->add(new atkManyToOneRelation("category_id", "products.category", AF_OBLIGATORY|AF_SEARCHABLE));

		$this->add(new atkBoolAttribute("track_stock", AF_SEARCHABLE));
		$this->getAttribute("track_stock")->addDependency(array($this, "refresh_trackstock"));

		$this->add(new atkAttribute("barcode", AF_UNIQUE|AF_SEARCHABLE, $this->std_att_size));

		$this->add(new atkManyToOneRelation("pricelist_id", "pricelist.price_list", AF_READONLY_ADD|AF_OBLIGATORY|AF_HIDE_LIST), atktext('price_list'));
		$this->getAttribute("pricelist_id")->setloadType(NOLOAD);
		$this->getAttribute("pricelist_id")->setstorageType(NOSTORE);
		$this->getAttribute("pricelist_id")->addDependency(array($this, "refresh_pricelist_change"));

		// holder for previous pricelist
		$this->add(new atkAttribute("holder_1", AF_HIDE_LIST|AF_HIDE_SEARCH, 10));
		$this->getAttribute("holder_1")->setloadType(NOLOAD);
		$this->getAttribute("holder_1")->setstorageType(NOSTORE);

		$this->addFieldSet("hidden_fieldset",$this->drawHiddenFieldset(), AF_HIDE_LIST|AF_HIDE_SEARCH|AF_NOLABEL);

		$this->add(new atkNumberAttribute("pricesell", AF_HIDE_LIST, 10, 2), atktext('price_list'));
		$this->getAttribute("pricesell")->setloadType(NOLOAD);
		$this->getAttribute("pricesell")->setstorageType(NOSTORE);
		$this->getAttribute("pricesell")->setLabel(atktext('pricesell_notax'));
		$this->getAttribute("pricesell")->addDependency(array($this, "refresh_info"));

		$this->add(new atkNumberAttribute("info_pricetax", AF_HIDE_LIST, 10, 2), atktext('price_list'));
		$this->getAttribute("info_pricetax")->setloadType(NOLOAD);
		$this->getAttribute("info_pricetax")->setstorageType(NOSTORE);
		$this->getAttribute("info_pricetax")->addDependency(array($this, "refresh_info_pricetaxChange"));

		$this->add(new atkNumberAttribute("pricebuy", AF_HIDE_LIST, 10, 2), atktext('price_list'));
		$this->getAttribute("pricebuy")->setloadType(NOLOAD);
		$this->getAttribute("pricebuy")->setstorageType(NOSTORE);
		$this->getAttribute("pricebuy")->setLabel(atktext('pricebuy_notax'));
		$this->getAttribute("pricebuy")->addDependency(array($this, "refresh_info"));

		$this->add(new atkNumberAttribute("info_margin", AF_SPECIFIC_5|AF_HIDE_LIST, 10, 2), atktext('price_list'));
		$this->getAttribute("info_margin")->setloadType(NOLOAD);
		$this->getAttribute("info_margin")->setstorageType(NOSTORE);

		// $this->add(new atkOneToManyRelation("sastav", "products.products_mat", "product_id", AF_HIDE_LIST|AF_CASCADE_DELETE),"Sastavnica");

		$this->add(new atkAttribute("creator", AF_READONLY|AF_HIDE_LIST, $this->std_att_size), atktext("system_tab"));
		$this->add(new atkAttribute("lastmod", AF_READONLY|AF_HIDE_LIST, $this->std_att_size), atktext("system_tab"));

		$this->add(new atkBoolAttribute("hidden", AF_HIDE_LIST), atktext("system_tab"));

		$this->setOrder("name");
		$this->setTable("products");

	}

	function action_admin(&$handler)
	{
		if ($this->getView()=="show_active") $this->addFilter("hidden <> 1");
		if ($this->getView()=="show_hidden") $this->addFilter("hidden = 1");

		return $handler->action_admin();
	}

	// show/hide hidden records
	function getView()
	{
		$sm = &atkGetSessionManager();

		if ($sm->pageVar("view")=="show_hidden")
		{
			return "show_hidden";
		}
		return "show_active";

	}


	// link to view hidden fields
	function adminFooter()
	{
		if ($this->getView()=="show_hidden")
		{
			// We are looking at the hidden. Display a link to view active records.
			return href("?view=show_active", atktext("view_active_records"));
		}
		else
		{
			// We are looking at active records. Display a link to view the hidden.
			return href("?view=show_hidden", atktext("view_hidden_records"));
		}
	}


	function descriptor_def()
	{
		return "[name]";
	}

	function initial_values()
	{
		return array("pricebuy"=>"0",
					"pricesell"=>"0",
					"track_stock"=>"1",		
					"pricelist_id" => array("id"=>"1000"), 
					"category_id" => array("id"=>"1000"));
	}


	function preAdd(&$record)
	{
		// created by
		$this->getAttribute('creator')->removeFlag(AF_READONLY);
		$record['creator'] = $this->getUserTrack();
		return true;
	}


	function preUpdate(&$record)
	{
		// lastmod by
		$this->getAttribute('lastmod')->removeFlag(AF_READONLY);
		$record['lastmod'] = $this->getUserTrack();
		return true;
	}


	function preDelete(&$record)
	{
		// return false if track_stock is disabled and product has stock<>0 or reservation<>0
		if($record['track_stock'] && $this->checkProductHasStock($record['id'], true))
		{
			$this->display_error(atktext("feedback_delete_product_with_stock"));
			die;
		}

		// return false if product is used
		if($this->checkProductIsUsed($record['id']))
		{
			$this->display_error(atktext("feedback_delete_constrain_error"));
			die;
		}

		return true;
	}


	function postAdd(&$record, $mode)
	{

		$this->setStock($record, true);
		$this->setPricelist($record);

		// if not distant selection - return to admin 
		if (!$this->m_postvars['atkpkret'])
		{
			$sess = array('atklevel'=>'0');
			$this->redirect(dispatch_url("products.product", "admin", $sess));
			
			$this->db32commit(); // commit before die;			
			die;
		}

		return true;
	}


	function postUpdate(&$record)
	{
		$this->setStock($record);
		$this->setPricelist($record);
		return true;
	}


	function postDelete(&$record)
	{
		return $this->deleteProductDependees($record);
	}


	/**
	 * set stock to "0" if product has track_stock checked
	 *
	 * @param $record
	 * @param $new boolean is new (postAdd)
	 * @return
	 */
	function setStock($record, $new=false)
	{

		$product_id = $record['id'];

		// operate with all locations here
		$locations = $this->getAllLocations();

		// new product
		if ($new)
		{
			// if track_stock - insert 0 into stockcurrent
			if ($record['track_stock'])
			{

				// insert start (0) stock value  to all locations
				foreach ($locations as $single_location)
				{
					$sql =  "INSERT INTO ia32_stockcurrent (product_id, location_id, amount) VALUES ('".stripquotes($product_id)."', '".stripquotes($single_location['id'])."', 0)";
					$this->db32insert($sql);
				}
			}

			return;
		}
			
		// existing product update
		if ($record['track_stock'])
		{
			// return if already has stockcurrent
			$sql =  "SELECT * FROM ia32_stockcurrent WHERE product_id = '".stripquotes($product_id)."'";

			$recs = $this->db32select($sql);

			if (!empty($recs)) return true;

			// if not, insert 0 into stockcurrent for all locations
			foreach ($locations as $single_location)
			{
				$sql =  "INSERT INTO ia32_stockcurrent (product_id, location_id, amount) VALUES ('".stripquotes($product_id)."', '".$single_location['id']."', 0)";
				$this->db32insert($sql);
			}

		}else // if not track_stock - remove product from all location's stockcurrent AKA this product has no track_stock anymore
		{

			foreach ($locations as $single_location)
			{
				$sql =  "DELETE FROM ia32_stockcurrent WHERE product_id = '".stripquotes($product_id)."' AND location_id = '".$single_location['id']."'";
				$this->db32delete($sql);
			}

		}
		return;
	}


	/**
	 * fill or update pricelist
	 *
	 * @param $record
	 * @param $new boolean is new (postAdd)
	 * @return
	 */
	function setPricelist($record)
	{
		$product_id = $record['id'];

		$pricelist_id = $record['pricelist_id']['id'];
		if (!$pricelist_id) $pricelist_id = 1000; // default = 1000

		$pricebuy = $record['pricebuy'];
		$pricesell = $record['pricesell'];

		if (!$pricebuy) $pricebuy = 0;
		if (!$pricesell) $pricesell = 0;

		$sql =  "SELECT product_id FROM ia32_prices WHERE product_id = '".stripquotes($product_id)."' AND pricelist_id = '".stripquotes($pricelist_id)."'";
		$recs = $this->db32select($sql);

		// new product
		if (!$recs[0]['product_id'])
		{
			$sql =  "INSERT INTO ia32_prices (pricelist_id, product_id, pricebuy, pricesell) VALUES ('".stripquotes($pricelist_id)."', '".stripquotes($product_id)."', ".$pricebuy.", ".$pricesell.")";
			$this->db32insert($sql);

			return;
		}

		// update prices
		$sql =  "UPDATE ia32_prices SET pricebuy = ".$pricebuy.", pricesell = ".$pricesell." WHERE product_id = '".stripquotes($product_id)."' AND pricelist_id = '".stripquotes($pricelist_id)."'";
		$this->db32update($sql);
			
		// set ALL pricebuy's to 0 if trackstock = 0
		if(!$record['track_stock'])
		{
			$sql =  "UPDATE ia32_prices SET pricebuy = 0 WHERE product_id = '".stripquotes($product_id)."'";
			$this->db32update($sql);
		}

	}


	function deleteProductDependees($record)
	{
		$product_id = $record['id'];

		// location is absolute here

		// delete from stockcurrent
		$sql =  "DELETE FROM ia32_stockcurrent WHERE product_id = '".stripquotes($product_id)."'";
		$this->db32delete($sql);

		// delete from prices
		$sql =  "DELETE FROM ia32_prices WHERE product_id = '".stripquotes($product_id)."'";
		$this->db32delete($sql);
			
		return true;
	}


	/*
	 * load prices
	 */
	function loadPricelist(&$record)
	{
		$product_id = $record['id'];

		$prices = $this->getPrices($product_id, $record['pricelist_id']['id']);

		$record['pricebuy'] = $prices['pricebuy'];
		$record['pricesell'] = $prices['pricesell'];
	}


	function edit_values($record)
	{
		// initial pricelist = 1000
		$record['pricelist_id']['id'] = '1000';

		$this->loadPricelist($record); //load prices
			
		return $record;
	}


	function refresh_pricelist_change($modifier)
	{
		$record = &$modifier->getRecord();

		// return if this is new add
		if (!$record['id']) return;

		//store prices from previous selected pricelist to db

		// current pricelist on empty holder
		if (!$record['holder_1']) $record['holder_1'] = $record['pricelist_id']['id'];

		$this->setPrices($record['id'], $record['holder_1'], $record['pricebuy'], $record['pricesell']);
		$record['holder_1'] = $record['pricelist_id']['id'];

		// load new prices
		$this->loadPricelist($record);

		// disable pricebuy if trackstock = 0
		if(!$record['track_stock'])
		{
			$record['pricebuy'] = 0;
			$record['info_margin'] = 0;
			$this->getAttribute("pricebuy")->addFlag(AF_SPECIFIC_5);
		}

		$taxrate = $this->getTaxRate($record['tax_id']['id']);

		if ($taxrate)
		$record['info_pricetax'] = $record['pricesell'] + $record['pricesell'] * $taxrate;
		else
		$record['info_pricetax'] = $record['pricesell'];


		if ($record['pricebuy']!=0) // division by zero patch
		$record['info_margin'] = ($record['pricesell'] - $record['pricebuy']) / $record['pricesell'] * 100;
		else
		$record['info_margin'] = 0;



		$modifier->refreshAttribute('hidden_fieldset');
		$modifier->refreshAttribute('pricesell');
		$modifier->refreshAttribute('pricebuy');
		$modifier->refreshAttribute('info_pricetax');
		$modifier->refreshAttribute('info_margin');

		return;
	}


	function refresh_trackstock($modifier)
	{
		$record = &$modifier->getRecord();

		// return false if track_stock is disabled and product has stock<>0 or reservation<>0
		if(!$record['track_stock'] && $this->checkProductHasStock($record['id'], true))
		{
			$page = &$this->getPage();
			$page->register_loadscript("alert('".atktext('error_product_has_stock')."');");
			$record['track_stock'] = 1;

			$modifier->refreshAttribute('track_stock');
			return;
		}

		$modifier->refreshAttribute('track_stock');

		if(!$record['track_stock'])
		{
			$record['pricebuy'] = 0;
			$record['info_margin'] = 0;
			$this->getAttribute("pricebuy")->addFlag(AF_SPECIFIC_5);

		}elseif (!$record['pricebuy'])
		{
			$record['pricebuy'] = 0;
			$record['info_margin'] = 0;
		}

			
		$modifier->refreshAttribute('pricebuy');
		$modifier->refreshAttribute('info_margin');

		return;
	}


	/*
	 * refresh info on tax change, pricesell or pricebuy change
	 */
	function refresh_info($modifier)
	{
		$record = &$modifier->getRecord();

		// disable pricebuy if trackstock = 0
		if(!$record['track_stock'])
		{
			$record['pricebuy'] = 0;
			$record['info_margin'] = 0;
			$this->getAttribute("pricebuy")->addFlag(AF_SPECIFIC_5);
		}

		$taxrate = $this->getTaxRate($record['tax_id']['id']);

		if ($taxrate)
		$record['info_pricetax'] = $record['pricesell'] + $record['pricesell'] * $taxrate;
		else
		$record['info_pricetax'] = $record['pricesell'];

		if ($record['pricebuy']!=0) // division by zero patch
		$record['info_margin'] = ($record['pricesell'] - $record['pricebuy']) / $record['pricesell'] * 100;
		else
		$record['info_margin'] = 0;

		$modifier->refreshAttribute('info_pricetax');
		$modifier->refreshAttribute('info_margin');

		// self refresh
		$modifier->refreshAttribute('pricesell');
		$modifier->refreshAttribute('pricebuy');

		if ($record['info_margin']<0)
		{
			//$page = &$this->getPage();
			//$page->register_loadscript("alert('".atktext('error_negativ_margin')."');");
		}
	}


	/*
	 * refresh info on info_pricetax change
	 */
	function refresh_info_pricetaxChange($modifier)
	{
		$record = &$modifier->getRecord();

		$taxrate = $this->getTaxRate($record['tax_id']['id']);

		$record['pricesell'] = $record['info_pricetax'] / (1 + $taxrate);

		if ($record['pricebuy']!=0) // division by zero patch
		$record['info_margin'] = ($record['pricesell'] - $record['pricebuy']) / $record['pricebuy'] * 100;
		else
		$record['info_margin'] = 0;

		$modifier->refreshAttribute('info_margin');
		$modifier->refreshAttribute('pricesell');

		// self refresh
		$modifier->refreshAttribute('info_pricetax');

	}


	/*
	 * fix buttons
	 */
	function getFormButtons($mode, $record)
	{
		// if add, but not add from other forms
		if ($mode=='add' && !$this->m_postvars['atkpkret'])
		{
			$buttons.= '<input type="submit" class="btn_saveandclose atkdefaultbutton" name="atksaveandclose"  value="'.atktext('saveandclose').'">';

			// cancel button
			$url = dispatch_url($this->atknodetype(), "admin");
			$buttons.= '&nbsp;&nbsp;<input type="button" class="btn_cancel" name="atkcancel"  value="'.atktext('cancel_and_return').'" onClick="location.href=\''.$url.'\'">';

			return $buttons;
		}
			
		$controller = &atkcontroller::getInstance();
		$controller->setNode($this);

		$buttons = $controller->getFormButtons($mode, $record);

		return $buttons;
	}



	/**
	 * check if product is used in (TODO: active) documents
	 *
	 */
	function checkProductIsUsed($product_id)
	{
		$sql =  "SELECT * FROM ia32_ticket_lines WHERE product_id = '".stripquotes($product_id)."'";
		$recs = $this->db32select($sql);

		if ($recs[0]) return true;

		return false;

	}


	/**
	 * check if product has stock
	 *
	 */
	function checkProductHasStock($product_id, $checkifzero=false)
	{
		$sql =  "SELECT * FROM ia32_stockcurrent WHERE product_id = '".stripquotes($product_id)."'";
		$recs = $this->db32select($sql);

		if ($checkifzero && $recs[0])
		{
			$sql =  "SELECT * FROM ia32_stockcurrent WHERE (amount<>0 OR reservation<>0) AND product_id = '".stripquotes($product_id)."'";
			$recs = $this->db32select($sql);

			if ($recs[0]) return true;

			return false;
		}

		if ($recs[0]) return true;

		return false;

	}


}
?>