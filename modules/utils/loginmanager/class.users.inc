<?php

/*
 * MAIN AUTH NODE
 */

useattrib("atkpasswordattribute");
userelation("atkmanytoonerelation");
useattrib("atkemailattribute");

class users extends atkNode
{

	function users()
	{
		$this->atkNode("users", NF_ADD_LINK);

		$this->add(new atkAttribute("firm_id", AF_READONLY|AF_OBLIGATORY|AF_SEARCHABLE, 8));
				
		$this->add(new atkAttribute("user", AF_OBLIGATORY|AF_UNIQUE|AF_SEARCHABLE));
		$this->add(new atkManyToOneRelation("language","loginmanager.languages", AF_OBLIGATORY));
		
		$this->add(new atkPasswordAttribute("password", AF_OBLIGATORY|AF_HIDE_LIST|AF_PASSWORD_NOVALIDATE));
		
		
		/*
		$this->add(new atkAttribute("allowed_ip", AF_SEARCHABLE|AF_HIDE_LIST));

		*/
		
				
		$this->add(new atkAttribute("id", AF_AUTOKEY|AF_HIDE_LIST));
		
		$this->add(new atkManyToOneRelation("group_id", "loginmanager.user_groups", AF_OBLIGATORY|AF_RELATION_AUTOLINK));
		
		$this->add(new atkManyToOneRelation("location_id", "locations.location", AF_OBLIGATORY|AF_SEARCHABLE));
		
		$this->add(new atkEmailAttribute("email", false, AF_HIDE_LIST,70));
	
		$this->add(new atkAttribute("db_config", AF_READONLY|AF_SEARCHABLE|AF_HIDE_LIST));
		
		if (atkconfig('use_table_prefixes')){
			$this->add(new atkAttribute("tbl_prefix", AF_SEARCHABLE, 10));
		}
		
				
		$this->add(new atkBoolAttribute("is_suspended", AF_SEARCHABLE));
		$this->add(new atkTextAttribute("notes", array("cols"=>60,"rows"=>10), AF_HIDE_LIST), 'notes');
	


		$this->setOrder("firm_id DESC");

		$this->setTable("admin_users");
	}

	function descriptor_def()
	{
		return "[user]";
	}

	function initial_values()
	{
		return array("db_config"=>"default",
					"firm_id"=>"1000",
					"language"=>array("id"=>"en"),
					"group_id"=>array("id"=>"3")	
					);
	}

	function preAddToEditArray(&$record, $mode="edit")
	{
		$unserialized = unserialize(base64_decode($record["customlang"]));

		// fill all records
		$current_cache = $this->get_cache($record["language"]["id"]);

		if (isset($full_cache))
		foreach($current_cache as $key=>$value){
			$i++;
			$record["key$i"]=$key;

			// if user has his own version then replace it
			if($unserialized[$key])
			$list .= "[ $value ] = ".$unserialized[$key]."\n";


		}
			
		$record["customlang"] = $list;

		// set logo picture
		if (file_exists("./images/users/logo/".$record["firm_id"].".jpg"))
		$picLinks .= '<img width="'.atkconfig("left_sidebar_width").'" style="border:2px dotted;" src="./images/users/logo/'.$record["firm_id"].'.jpg" />';

		if (file_exists("./images/users/logo/".$record["firm_id"].".gif"))
		$picLinks .= '<img width="'.atkconfig("left_sidebar_width").'" style="border:2px dotted;" src="./images/users/logo/'.$record["firm_id"].'.gif" />';

		if (file_exists("./images/users/logo/".$record["firm_id"].".png"))
		$picLinks .= '<img width="'.atkconfig("left_sidebar_width").'" style="border:2px dotted;" src="./images/users/logo/'.$record["firm_id"].'.png" />';

		//$this->getAttribute("logo_picture")->setLabel($picLinks);

	}


	function get_cache($language='en')
	{
		atkimport("atk.atklanguage");
		$atklanguage =& atkLanguage::getInstance();
		$full_cache = $atklanguage->m_cachedlang['atk'][$language];

		if (isset($full_cache))
		foreach($full_cache as $key=>$value){
			if(substr($key, 0, 4)=='pdf_'){
				$short_cache[$key]=$value;
			}
		}

		$this->numOfRecords = count($short_cache);

		return $short_cache;
	}


	function action_SaveDB(&$handler)
	{
		$user = $this->m_postvars["user"];

		$result = "ArchiveID: ".$this->backup_tables($user, true, 'admin saved db');

		$output = $this->getUI()->renderBox(array("title"=>'Database Dump',
                                 "content"=>"Dumping DB for user: $user  <br></br>$result"));
		$this->addStyle("style.css");
		$this->getPage()->addContent($this->renderActionPage("SaveDB", $output));

	}


	function action_archiveDB(&$handler)
	{
		// TODO: Archive account with multiply users

		// confirmation
		if($handler->m_node->m_postvars['cancel'])
		{
			$this->redirect();
			die;
		}elseif(empty($handler->m_node->m_postvars['confirm']))
		{
			$user = $this->m_postvars["user"];
			$selector = "admin_users.user = '$user'";

			$page = &$handler->getPage();
			$page->addContent($handler->m_node->renderActionPage("archiveDB", $handler->m_node->confirmAction($selector, "archiveDB")) );
			return;
		}

		$user = $this->m_postvars["descriptor"];

		$lastid = $this->backup_tables($user, true, 'admin archived user db');
		if(!strstr($lastid, 'ERROR'))
		{
			$this->drop_tables($user);
			$this->update_user($user, "tbl_prefix", "db:$lastid");

			$result = "OK!";
		}
		else
		{
			$result = $lastid;
		}


		$output = $this->getUI()->renderBox(array("title"=>'Curent user Database DROP',
                                "content"=>"DROPING DB for user: $user  <br></br>$result"));
		$this->addStyle("style.css");
		$this->getPage()->addContent($this->renderActionPage("archiveDB", $output));

	}


	function action_dbdownload(&$handler)
	{
		$user = $this->m_postvars["user"];

		$database = $this->backup_tables($user);

		$output = $this->getUI()->renderBox(array("title"=>'Database Download',
                                 "content"=>"Downloading DB for user: $user"));
		$this->addStyle("style.css");

		$filename = "$user";
		// @param String $type The type (csv / excel / xml)
		$type = "sql";
		$ext = "sql";

		$exporter = &atknew("atk.utils.atkfileexport");
		$exporter->export($database, $filename, $type, $ext, $compression);

		$this->getPage()->addContent($this->renderActionPage("dbdownload", $output));

	}


	function recordActions($record, &$actions, &$mraactions)
	{
		return; 
		
		// no db actions if archived
		if(strpos($record["tbl_prefix"], "db:")!==false) return;
		// no db actions if not activated
		// must remove AF_HIDE_LIST if(!strstr($record["activationkey"], "activated")) return;

		// dump user tables to core
		$actions["SaveDB"]= session_url(dispatch_url($this->atknodetype(),
                                                         "SaveDB",
		array("user"=>$record["user"])));

		// download user tables
		$actions["dbdownload"]= session_url(dispatch_url($this->atknodetype(),
                                                         "dbdownload",
		array("user"=>$record["user"])));


		// archive user tables
		$actions["archiveDB"]= session_url(dispatch_url($this->atknodetype(),
                                                         "archiveDB",
		array("user"=>$record["user"])));
	}





	/*
	 * drop user tables (looking at current settings in core db)
	 */
	function drop_tables($user)
	{
		$usertable = atkconfig("auth_usertable");
		$userfield = atkconfig("auth_userfield");
		$query = "SELECT * FROM $usertable WHERE $userfield ='$user'";
		$db = &atkGetDb(atkconfig("auth_database"));
		$recs = $db->getrows($query);

		$username = $recs[0]["user"];
		$dbconfig = $recs[0]["db_config"];
		$tbl_prefix = $recs[0]["tbl_prefix"];

		if(!$dbconfig || !$tbl_prefix) die("drop_tables: prefix/db expected");

		$dbname = &atkGetDb($dbconfig);

		$tables = $dbname->table_names();

		foreach($tables as $table)
		{
			$startstring = substr($table['table_name'], 0, strlen($tbl_prefix));

			if($startstring == $tbl_prefix)
			{
				// WARNING: USE WITH CARE
				$dbname->query("DROP TABLE ".$table['table_name']);
			}

		}

		return true;
	}



	function restore_archive($user, $deleteArchive = true)
	{

		// TODO: Restore account with multiply users

		$coredb = $this->getDb();
		$usertable = atkconfig("auth_usertable");
		$userfield = atkconfig("auth_userfield");
		$query = "SELECT * FROM $usertable WHERE $userfield ='$user'";
		$recs = $coredb->getrows($query);

		$tbl_prefix = $recs[0]['tbl_prefix'];
		$exploded = explode(":", $tbl_prefix);

		if ($exploded[0] != 'db') atkhalt('restore_archive: not an archive', "critical");

		// get dump
		$archiveID = $exploded[1];
		$query = "SELECT prefix, db_dump FROM admin_userdb_backups WHERE id = $archiveID";
		$userrecs = $coredb->getrows($query);
		$archived_tbl_prefix = $userrecs[0]['prefix'];
		$encoded_dump = $userrecs[0]['db_dump'];
		$dump = base64_decode($encoded_dump);

		// import from dump
		$dbconfig = $recs[0]["db_config"];
		$userdb = &atkGetDb($dbconfig);
		$sql_array = explode(";\n", $dump);
		foreach ($sql_array as $key => $val) {
			if(trim($val))
			$userdb->query($val);
		}
		$userdb->commit();


		$this->update_user($user, 'tbl_prefix', $archived_tbl_prefix);

		if($deleteArchive)
		{
			$query = "DELETE FROM admin_userdb_backups WHERE id = $archiveID";
			$coredb->query($query);
			$coredb->commit();

		}

		return $archived_tbl_prefix;
	}


	function update_user($user, $field, $value, $valueIsNumber=false)
	{
		$usertable = atkconfig("auth_usertable");
		$userfield = atkconfig("auth_userfield");

		if(!$valueIsNumber) $value = "'".$value."'";

		$query = "UPDATE $usertable SET $field = $value WHERE $userfield ='$user'";
		$db = &atkGetDb(atkconfig("auth_database"));
		$db->query($query);
		$db->commit();

		return true;
	}


	/*
	 * update database up to current database
	 */
	function update_database_version($user, $dbconfig, $tbl_prefix, $from_version)
	{
		// current db version
		$current_version = 4;

		/*
		 * updates start
		 * 
		 * use "exist" for checks and "query" for updates
		 */

		$multisql = array();

		// apply 2nd version updates
		if($from_version < 2)
		{
			$multisql[] = array("query" => "ALTER TABLE `{$tbl_prefix}taxes`  CHANGE COLUMN `rate` `rate` DECIMAL(15,4) NOT NULL DEFAULT '0.00' AFTER `name`;");
			$multisql[] = array("query" => "ALTER TABLE `{$tbl_prefix}ticket_lines`  CHANGE COLUMN `fix_taxrate` `fix_taxrate` DECIMAL(15,4) NOT NULL DEFAULT '0.00' AFTER `fix_discount`;");
		}

		// apply 3rd version updates
		if($from_version < 3)
		{
			// adding new columns, check them if exists first
			$multisql[] = array(
				"exist" => "SHOW COLUMNS FROM `{$tbl_prefix}config` LIKE 'strict_stock';",
				"query" => "ALTER TABLE `{$tbl_prefix}config`  ADD COLUMN `strict_stock` VARCHAR(1) NULL DEFAULT '1' AFTER `logo_width`;");

		}
			
		// apply 4th...
		if($from_version < 4)
		{
			// adding new columns, check them if exists first
			$multisql[] = array(
				"exist" => "SHOW COLUMNS FROM `{$tbl_prefix}config` LIKE 'display_currency';",
				"query" => "ALTER TABLE `{$tbl_prefix}config` ADD COLUMN `display_currency` VARCHAR(255) NOT NULL DEFAULT '1000' AFTER `strict_stock`;");
		}

		/*
		 * updates end
		 */


		// run only if we need to update something
		if(!empty($multisql))
		{
			$db = &atkGetDb($dbconfig);

			// run all update queries
			foreach ($multisql as $sql){

				if($sql["exist"])
				{
					$db->query($sql["exist"]);
					$check = $db->next_record();
					if($check == FALSE) $db->query($sql["query"]);
				}
				else
				{
					$db->query($sql["query"]);
				}


			}

			// if everything is ok, write current version back to core db
			$this->update_user($user, "db_version", $current_version, true);


		}

		return $current_version;
	}


	/* backup user tables */
	function backup_tables($user, $backup2db = false, $notes='', $tables = '*')
	{
		$usertable = atkconfig("auth_usertable");
		$userfield = atkconfig("auth_userfield");
		$query = "SELECT * FROM $usertable WHERE $userfield ='$user'";
		$db = &atkGetDb(atkconfig("auth_database"));
		$recs = $db->getrows($query);

		$username = $recs[0]["user"];
		$dbconfig = $recs[0]["db_config"];
		$tbl_prefix = $recs[0]["tbl_prefix"];
		$db_version = $recs[0]["db_version"];

		// get tables size in MB
		// you can use atkconfig("db") instead of global
		global $config_db;
		$dbname = $config_db[$dbconfig]["db"];
		$userdb = &atkGetDb($dbconfig);
		$find_size = "
       SELECT sum(round(((data_length + index_length) / 1024 / 1024),2)) 'Size_in_MB'
			      FROM information_schema.TABLES 
				  WHERE table_schema = '$dbname' 
		   		  AND TABLE_NAME LIKE '$tbl_prefix%'";

		$userdbrecs = $userdb->getrows($find_size);
		$userDbSize = $userdbrecs[0]['Size_in_MB'];

		$link = mysql_connect($config_db[$dbconfig]["host"],$config_db[$dbconfig]["user"],$config_db[$dbconfig]["password"]);
		mysql_query('SET NAMES utf8');
		mysql_select_db($config_db[$dbconfig]["db"],$link);


		//get all of the tables
		if($tables == '*')
		{
			$tables = array();
			$result = mysql_query("SELECT TABLE_NAME
									FROM information_schema.TABLES 
									WHERE TABLE_NAME LIKE '$tbl_prefix%'; ");
			while($row = mysql_fetch_row($result))
			{
				$tables[] = $row[0];
			}
		}
		else
		{
			$tables = is_array($tables) ? $tables : explode(',',$tables);
		}

		if(!$tables) return "ERROR! No tables to dump!";

		$return = "
					/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
					/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
					/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
					/*!40101 SET NAMES utf8 */;
					/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
					/*!40103 SET TIME_ZONE='+00:00' */;
					/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
					/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
					/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
					/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;\n\n";
		//cycle through
		foreach($tables as $table)
		{
			$result = mysql_query('SELECT * FROM '.$table);
			$num_fields = mysql_num_fields($result);

			$return.= 'DROP TABLE IF EXISTS '.$table.';';
			$row2 = mysql_fetch_row(mysql_query('SHOW CREATE TABLE '.$table));
			$return.= "\n\n".$row2[1].";\n\n";

			for ($i = 0; $i < $num_fields; $i++)
			{
				while($row = mysql_fetch_row($result))
				{
					$return.= 'INSERT INTO '.$table.' VALUES(';
					for($j=0; $j<$num_fields; $j++)
					{
						$row[$j] = addslashes($row[$j]);
						$row[$j] = ereg_replace("\n","\\n",$row[$j]);
						if (isset($row[$j])) { $return.= '"'.$row[$j].'"' ; } else { $return.= '""'; }
						if ($j<($num_fields-1)) { $return.= ','; }
					}
					$return.= ");\n";
				}
			}
			$return.="\n\n\n";
		}

		if($backup2db){
			//save dump to db
			$mysqldate = date("Y-m-d H:i:s");
			$encoded_dump = base64_encode($return);
			$update_query = "
		       INSERT INTO admin_userdb_backups (user, date, prefix, dbname, db_version, size, notes, db_dump)
		       VALUES ('$username', '$mysqldate', '$tbl_prefix', '$dbconfig', $db_version, $userDbSize, '$notes', '$encoded_dump')
		       ";

			$db->query($update_query);
			$lastid = $db->getInsertId();
			$db->commit();

			// return generated id (autoincrement id)
			return $lastid;
		}

		return $return;

		// file save
		//$handle = fopen('db-backup-'.time().'-'.(md5(implode(',',$tables))).'.sql','w+');
		//fwrite($handle,$return);
		//fclose($handle);
	}

}

?>