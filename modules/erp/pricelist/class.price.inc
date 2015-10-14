<?php


class price extends ia32Node
{

	function price()
	{
		$this->atkNode("price", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkManyToOneRelation("pricelist_id", "pricelist.price_list", AF_SEARCHABLE|AF_PRIMARY|AF_READONLY_EDIT));
		$this->add(new atkManyToOneRelation("product_id", "products.product",  AF_SEARCHABLE|AF_PRIMARY|AF_RELATION_AUTOCOMPLETE|AF_READONLY_EDIT));
		$this->getAttribute("product_id")->setAttribSize($this->std_att_size);	
		
		$this->add(new atkNumberAttribute("pricesell", AF_OBLIGATORY, 10, 2));
		$this->getAttribute("pricesell")->setLabel(atktext('pricesell_notax'));		
		$this->getAttribute("pricesell")->addDependency(array($this, "refresh_numbers"));

		$this->add(new atkNumberAttribute("info_pricetax", AF_HIDE_LIST, 10, 2));
		$this->getAttribute("info_pricetax")->setloadType(NOLOAD);
		$this->getAttribute("info_pricetax")->setstorageType(NOSTORE);
		$this->getAttribute("info_pricetax")->addDependency(array($this, "refresh_info_pricetaxChange"));

		$this->add(new atkNumberAttribute("pricebuy", AF_OBLIGATORY, 10, 2));	
		$this->getAttribute("pricebuy")->addDependency(array($this, "refresh_numbers"));

		$this->add(new atkNumberAttribute("info_margin", AF_SPECIFIC_5|AF_HIDE_LIST, 10, 2));
		$this->getAttribute("info_margin")->setloadType(NOLOAD);
		$this->getAttribute("info_margin")->setstorageType(NOSTORE);

		$this->setOrder("pricelist_id");
		$this->setTable("prices");
	}

	function descriptor_def()
	{
		return "[product_id.name]";
	}

	/*
	 * recordview actions on admin mode grid
	 */
	function recordActions(&$record, &$actions, &$mraactions)
	{
		// remove delete action
		$actions["delete"] = '';
	}


	function initial_values()
	{

		return array("pricebuy"=>"0",
					"pricesell"=>"0",);
	}


	/*
	 * refresh numbers to look nicer
	 */
	function refresh_numbers($modifier)
	{
		$record = &$modifier->getRecord();

		$taxrate = $this->getTaxRate($this->getProductData($record['product_id']['id'], 'tax_id'));

		if ($taxrate)
		$record['info_pricetax'] = $record['pricesell'] + $record['pricesell'] * $taxrate;
		else
		$record['info_pricetax'] = $record['pricesell'];

		if ($record['pricebuy']!=0) // division by zero patch
		$record['info_margin'] = ($record['pricesell'] - $record['pricebuy']) / $record['pricebuy'] * 100;
		else
		$record['info_margin'] = 0;

		$modifier->refreshAttribute('info_pricetax');
		$modifier->refreshAttribute('info_margin');
		$modifier->refreshAttribute('pricesell');
		$modifier->refreshAttribute('pricebuy');

		if ($record['info_margin']<0)
		{
			$page = &$this->getPage();
			$page->register_loadscript("alert('".atktext('error_negativ_margin')."');");
		}

	}


	/*
	 * refresh info on info_pricetax change
	 */
	function refresh_info_pricetaxChange($modifier)
	{
		$record = &$modifier->getRecord();

		$taxrate = $this->getTaxRate($this->getProductData($record['product_id']['id'], 'tax_id'));

		$record['pricesell'] = $record['info_pricetax'] / (1 + $taxrate);

		if ($record['pricebuy']!=0) // division by zero patch
		$record['info_margin'] = ($record['pricesell'] - $record['pricebuy']) / $record['pricebuy'] * 100;
		else
		$record['info_margin'] = 0;

		$modifier->refreshAttribute('info_pricetax');
		$modifier->refreshAttribute('info_margin');
		$modifier->refreshAttribute('pricesell');

	}
}

?>
