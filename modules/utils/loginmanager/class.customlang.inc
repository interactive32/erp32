<?php

/*
 * CUSTOM LANGUAGES FOR USERS
 */

class customlang extends atkNode
{

	protected $numOfRecords;

	function customlang()
	{
		$this->atkNode("customlang", NF_ADD_LINK|NF_NO_EXTENDED_SEARCH);

		$current_cache = $this->get_cache();

		foreach($current_cache as $key=>$value){
			$i++;

			$this->add(new atkAttribute("key$i", AF_HIDE_EDIT));
			$this->getAttribute("key$i")->setloadType(NOLOAD);
			$this->getAttribute("key$i")->setStorageType(NOSTORE);

			$this->add(new atkAttribute("translation$i"));
			$this->getAttribute("translation$i")->setloadType(NOLOAD);
			$this->getAttribute("translation$i")->setStorageType(NOSTORE);
			$this->getAttribute("translation$i")->setLabel($value);
			$this->getAttribute("translation$i")->setAttribSize(array(255,35,255));

		}

		$this->add(new atkAttribute("customlang"));
		// hide customlang
		$this->addFieldSet('hidden','<div style="display:none;"> [customlang.field] </div> ', AF_NOLABEL);


		$this->add(new atkAttribute("id", AF_AUTOKEY));
			
		$userid = atkGetUser("id");
		$this->addFilter("id = '$userid'");

		$this->setTable("admin_users");
	}


	function descriptor_def()
	{
		return "";
	}
	
	
	function get_cache()
	{
		atkimport("atk.atklanguage");
		$lng = atkLanguage::getLanguage();
		$lng = strtolower($lng);
		$atklanguage =& atkLanguage::getInstance();
		$full_cache = $atklanguage->m_cachedlang['atk'][$lng];

		foreach($full_cache as $key=>$value){
			if(substr($key, 0, 4)=='pdf_'){
				$short_cache[$key]=$value;
			}
		}

		$this->numOfRecords = count($short_cache);

		return $short_cache;
	}



	function preAddToEditArray(&$record, $mode="edit")
	{
		$unserialized = unserialize(base64_decode($record["customlang"]));

		// fill all records
		$current_cache = $this->get_cache();

		foreach($current_cache as $key=>$value){
			$i++;
			$record["key$i"]=$key;
			
			// if user has his own version then replace it
			if($unserialized[$key]) 
			$record["translation$i"] = $unserialized[$key];


		}
			



	}


	function preUpdate(&$record)
	{
		// security check if user is updating himself
		if ($record['id'] != atkGetUser('id'))
		{
			echo atktext('error_attributeedit_update');
			session_regenerate_id(true);
			die();
		}

		// flatten to single array
		for ($i=1; $i < ($this->numOfRecords + 1); $i++){
			$singleArray[$record["key$i"]] = $record["translation$i"];
		}

		$record["customlang"] = base64_encode(serialize($singleArray));

	}


	function postUpdate($record)
	{
		session_regenerate_id(true);
		$this->redirect("index.php?atklogout=1");
		die();
	}


	/*
	 * fix buttons
	 */
	function getFormButtons($mode, $record)
	{

		$homepage = "dashboard.mainboard";

		if ($mode=='edit')
		{
			// cancel button
			$url = dispatch_url($homepage, "start");
			$buttons.= '<input type="button" class="btn_cancel atkdefaultbutton" name="atkback"  value="'.atktext('back_button').'" onClick="location.href=\''.$url.'\'">';


			// confirm
			$buttons.= '&nbsp;&nbsp;<input type="submit" class="btn_saveandclose" name="atksaveandclose"  value="'.atktext('saveandclose').'" onClick="return confirm(\''.atktext('confirm_userdata_change').'\')">';
			;

			return $buttons;
		}

		// bad action, return to admin
		$this->redirect(dispatch_url($homepage, "start"));
		die;
	}





}

?>
