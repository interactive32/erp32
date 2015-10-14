<?php


class main_config extends ia32Node
{

	function main_config()
	{
		$this->atkNode("main_config", NF_NO_ADD);

		// show firm_id if administrator group
		if(atkGetUser('group_id')==1)
		$this->add(new atkAttribute("firm_id", AF_PRIMARY));
		else 
		$this->add(new atkAttribute("firm_id", AF_PRIMARY|AF_HIDE));

		$this->add(new atkAttribute("firm_name", AF_OBLIGATORY, array(255, 60, 60)));

		$this->add(new atkTextAttribute("description", array("cols"=>60,"rows"=>10), AF_OBLIGATORY|AF_HIDE_LIST));
		$this->getAttribute("description")->setLabel(atktext('firm_data'));

		// max size = 500KB = 524288 bytes
		$this->add(new atkFileAttribute("logo_picture", "./images/users/logo/", AF_FILE_NO_SELECT|AF_FILE_NO_DELETE, 524288), atktext("picture_logo_tab"));
		$this->getAttribute("logo_picture")->setLabel(atktext("logo_picture").' ('.atktext("logo_picture_properties").') ');
		$this->getAttribute("logo_picture")->setImageInlineSize(300, 300);
		$this->getAttribute("logo_picture")->setFilenameTemplate(atkGetUser('firm_id'));
		$this->getAttribute("logo_picture")->setAllowedFileTypes(array('jpg', 'gif', 'png'));

		$this->add(new atkNumberAttribute("logo_width", AF_HIDE_LIST, 2, 0), atktext("picture_logo_tab"));
		
		$this->add(new atkManyToOneRelation("display_currency", "config.currency", AF_HIDE_LIST|AF_OBLIGATORY), atktext("business"));
			
		$this->add(new atkDummyAttribute("delimiter_1", "<hr>", AF_NOLABEL), atktext('business'));
				
		$this->add(new atkBoolAttribute("strict_stock"), atktext("business"));

		$this->add(new atkDummyAttribute("delimiter_2", "<hr>", AF_NOLABEL), atktext('business'));

		$this->add(new atkAttribute("prefix_quote", AF_HIDE_LIST|AF_OBLIGATORY, array(5, 5, 5)), atktext("business"));
		$this->add(new atkAttribute("prefix_receipt", AF_HIDE_LIST|AF_OBLIGATORY, array(5, 5, 5)), atktext("business"));
		$this->add(new atkAttribute("prefix_creditnote", AF_HIDE_LIST|AF_OBLIGATORY, array(5, 5, 5)), atktext("business"));
		$this->add(new atkAttribute("prefix_invoice", AF_HIDE_LIST|AF_OBLIGATORY, array(5, 5, 5)), atktext("business"));
		$this->add(new atkAttribute("prefix_purchaseorder", AF_HIDE_LIST|AF_OBLIGATORY, array(5, 5, 5)), atktext("business"));
		$this->add(new atkAttribute("prefix_inventory", AF_HIDE_LIST|AF_OBLIGATORY, array(5, 5, 5)), atktext("business"));
		$this->add(new atkAttribute("prefix_internal_transfer_out", AF_HIDE_LIST|AF_OBLIGATORY, array(5, 5, 5)), atktext("business"));
		$this->add(new atkAttribute("prefix_internal_transfer_in", AF_HIDE_LIST|AF_OBLIGATORY, array(5, 5, 5)), atktext("business"));

		$this->setOrder("firm_name");
		$this->setTable("config");
	}


	function descriptor_def()
	{
		return "[name]";
	}


	/*
	 * never delete firm
	 */
	function preDelete($record)
	{
		return false;
	}


	/*
	 * recordview actions on admin mode grid
	 */
	function recordActions(&$record, &$actions, &$mraactions)
	{
		$actions["delete"] = '';
	}


	/*
	 * modification of validate - validate unique prefixes
	 */
	function validate(&$record, $mode, $ignoreList=array())
	{
		// check if all is unique!
		if ($record['prefix_quote'] == $record['prefix_receipt']
		|| $record['prefix_quote'] == $record['prefix_creditnote']
		|| $record['prefix_quote'] == $record['prefix_invoice']
		|| $record['prefix_quote'] == $record['prefix_purchaseorder']
		|| $record['prefix_quote'] == $record['prefix_inventory']
		|| $record['prefix_quote'] == $record['prefix_internal_transfer_out']
		|| $record['prefix_quote'] == $record['prefix_internal_transfer_in']
		)$record['prefix_quote'] = '';

		if ($record['prefix_receipt'] == $record['prefix_creditnote']
		|| $record['prefix_receipt'] == $record['prefix_invoice']
		|| $record['prefix_receipt'] == $record['prefix_purchaseorder']
		|| $record['prefix_receipt'] == $record['prefix_inventory']
		|| $record['prefix_receipt'] == $record['prefix_internal_transfer_out']
		|| $record['prefix_receipt'] == $record['prefix_internal_transfer_in']
		)$record['prefix_receipt'] = '';

		if ($record['prefix_creditnote'] == $record['prefix_invoice']
		|| $record['prefix_creditnote'] == $record['prefix_purchaseorder']
		|| $record['prefix_creditnote'] == $record['prefix_inventory']
		|| $record['prefix_creditnote'] == $record['prefix_internal_transfer_out']
		|| $record['prefix_creditnote'] == $record['prefix_internal_transfer_in']
		)$record['prefix_creditnote'] = '';

		if ($record['prefix_invoice'] == $record['prefix_purchaseorder']
		|| $record['prefix_invoice'] == $record['prefix_inventory']
		|| $record['prefix_invoice'] == $record['prefix_internal_transfer_out']
		|| $record['prefix_invoice'] == $record['prefix_internal_transfer_in']
		)$record['prefix_invoice'] = '';
		
		if ($record['prefix_purchaseorder'] == $record['prefix_inventory']
		|| $record['prefix_purchaseorder'] == $record['prefix_internal_transfer_out']
		|| $record['prefix_purchaseorder'] == $record['prefix_internal_transfer_in']
		)$record['prefix_purchaseorder'] = '';
		
		if ($record['prefix_inventory'] == $record['prefix_internal_transfer_out']
		|| $record['prefix_inventory'] == $record['prefix_internal_transfer_in']
		)$record['prefix_inventory'] = '';

		if ($record['prefix_internal_transfer_out'] == $record['prefix_internal_transfer_in']
		)$record['prefix_internal_transfer_out'] = '';
				
		// continue with parent functionality
		$parent = parent::validate($record, $mode, $ignoreList);

		return $parent;
	}

}

?>
