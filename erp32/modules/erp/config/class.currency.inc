<?php


class currency extends ia32Node
{

	function currency()
	{
		$this->atkNode("currency", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$this->add(new atkAttribute("id", AF_AUTOKEY));
		$this->add(new atkAttribute("name", AF_OBLIGATORY|AF_SEARCHABLE, array(255,20,20)));
		$this->add(new atkAttribute("symbol", AF_OBLIGATORY|AF_SEARCHABLE, array(3,3,3)));
		

		$this->add(new atkNumberAttribute("rate", AF_OBLIGATORY|AF_SEARCHABLE, 6, 5));
		$this->getAttribute("rate")->setLabel(atktext('currency_rate'));
		$this->getAttribute("rate")->addDependency(array($this, "refresh_info_reverse_rate"));
	
		$this->add(new atkNumberAttribute("info_reverse_rate", AF_HIDE_LIST, 6, 5));
		$this->getAttribute("info_reverse_rate")->setLabel(atktext('currency_rate'));
		$this->getAttribute("info_reverse_rate")->setloadType(NOLOAD);
		$this->getAttribute("info_reverse_rate")->setstorageType(NOSTORE);
		$this->getAttribute("info_reverse_rate")->addDependency(array($this, "refresh_rate"));
		
		
		$this->setOrder("id");
		$this->setTable("currency");
	}


	function descriptor_def()
	{
		return "[symbol]";
	}


	/*
	 * system calls this method to override the values of a record
	 * before editing the record.
	 */
	function edit_values($record)
	{
		// if main currency - disable rate, always is 1
		if ($record['id'] == 1000)
		{
			$this->getAttribute("rate")->addFlag(AF_READONLY|AF_HIDE);	
            $record['rate'] = 1;
            
            $this->getAttribute("info_reverse_rate")->addFlag(AF_READONLY|AF_HIDE);

            //$this->add(new atkDummyAttribute("main", atktext("main_currency_info")."<hr>", AF_NOLABEL), "*", 10);
            
		}

		return $record; // don't change this!
	}


	/*
	 * recordview actions on admin mode grid
	 */
	function recordActions(&$record, &$actions, &$mraactions)
	{
		// remove all actions for main record
		if (is_numeric($record['id']) && $record['id']<=1000)
		{
			//$actions["view"] = '';
			//$actions["edit"] = '';
			$actions["delete"] = '';
		}
	}

	/*
	 * don't delete currency if used in config
	 */
	function preDelete($record)
	{

		$sql =  "SELECT display_currency FROM ia32_config";
		$recs = $this->db32select($sql);

		// return false if type is used
		if($recs[0]["display_currency"] == $record["id"])
		{
			$this->display_error(atktext("feedback_delete_constrain_error"));
			return false;
		}

		return true;
	}
	
	
	/*
	 * refresh info on info_pricetax change
	 */
	function refresh_info_reverse_rate($modifier)
	{
		$record = &$modifier->getRecord();
		
		$main_currency = $this->getCurrencyData("1000");
		
		$this->getAttribute("rate")->setLabel(atktext('currency_rate')." (1 ".$record['symbol']." = ".$main_currency['symbol'].")");
		$this->getAttribute("info_reverse_rate")->setLabel(atktext('currency_rate')." (1 ".$main_currency['symbol']." = ".$record['symbol'].")");

		$record['info_reverse_rate'] = 1 / $record['rate'];

		$modifier->refreshAttribute('rate');
		$modifier->refreshAttribute('info_reverse_rate');

	}	
	
	/*
	 * refresh info on info_pricetax change
	 */
	function refresh_rate($modifier)
	{
		$record = &$modifier->getRecord();
		
		$main_currency = $this->getCurrencyData("1000");
		
		$this->getAttribute("rate")->setLabel(atktext('currency_rate')." (1 ".$record['symbol']." = ".$main_currency['symbol'].")");
		$this->getAttribute("info_reverse_rate")->setLabel(atktext('currency_rate')." (1 ".$main_currency['symbol']." = ".$record['symbol'].")");

		$record['rate'] = 1 / $record['info_reverse_rate'];

		$modifier->refreshAttribute('rate');
		$modifier->refreshAttribute('info_reverse_rate');


	}		
}

?>
