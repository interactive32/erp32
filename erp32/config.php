<?php
// -------------------------------------------
//           MAIN CONFIG FILE
// -------------------------------------------

$config_version = "erp32 v2.3";

// The administrator password.
$config_administratorpassword = "admin123";

// -------------------------------------------
//           DATABASE CONFIGURATION
// -------------------------------------------

$config_db["default"]["host"] = "db";
$config_db["default"]["db"]   = "erp32_database";
$config_db["default"]["user"] = "root";
$config_db["default"]["password"] = "mypass";

// advanced
$config_db["default"]["driver"] = "mysqli";
$config_db["default"]["charset"] = "utf8";
$config_db["default"]["collate"] = "utf8_general_ci";
$config_lock_type = "db";

// table prefix allows you to host multiple companies on same database. 
// copy all tables (except admin_*) and use different table prefix for each user
$config_use_table_prefixes = false;

// -------------------------------------------
//           DOCUMENT NUMBERS
// -------------------------------------------

// use offset for document numbers
$config_document_number_offset = 0;

// add leading zeros to document numbers
$config_add_leading_zeros = true;

// add checksum digit on invoices based on 731 algorithm
// used in Finland (viitenumero)
$config_use_731_checkdigit = false;

// add checksum digit on invoices based on luhn algorithm (Mod-10)
// used in Norway
$config_use_luhn_checkdigit = false;

// -------------------------------------------
//           LAYOUT
// -------------------------------------------

// You can specify the number of records to show on a page.
$config_recordsperpage=15;

// Display sidebar on left side (true/false)
$config_display_left_sidebar = true;
$config_left_sidebar_width = 100;


// -------------------------------------------
//          LOGIN CONFIGURATION
// -------------------------------------------

// max login attempts
// recover with index.php?atklogout=1
$config_max_loginattempts = 10; 

// use forgotten password feature?
$config_auth_enablepasswordmailer = false;

// email settings for sending reset password email
$config_mail_sender = "info@example.com";
$config_mail_subject = "New Password";
$config_mail_smtp_host = "smtp.example.com";
$config_mail_smtp_username = "info@example.com";
$config_mail_smtp_password = "mypass";


// ---------------------------------------
//          AJAX CONFIG
// ---------------------------------------

// This variable determines how many characters a user must enter
// before an autocompletion search is performed.
$config_manytoone_autocomplete_minchars = 2;

//The search mode of the autocomplete fields.
//Can be 'startswith', 'exact' or 'contains'.
$config_manytoone_autocomplete_searchmode = "contains";

// Value determines wether the search of the autocompletion is case-sensitive.
$config_manytoone_autocomplete_search_case_sensitive = false;


// -------------------------------------------
//           LANGUAGE
// -------------------------------------------

// The language of the application. You can use any language for which
// a language file is present in the languages directory /app/atk/languages/
$config_language="en";
 
// fallback language (default)  
$config_defaultlanguage="en"; 

// Set default timezone (see: http://php.net/manual/en/timezones.php)
date_default_timezone_set('America/Los_Angeles');

// -------------------------------------------
//           LOGS
// -------------------------------------------

// Write security events to a logfile.
// There are several values you can choose for $config_logging.
// 0 - No logging
// 1 - Log logins
// 2 - Log actions ("User x performed action x on module y")

$config_logging = 0;
$config_logfile = "../userlog.log";

