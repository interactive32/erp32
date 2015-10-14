<?php
//
// CONFIGURATION FILE
//


// -------------------------------------------
//            EXTERNAL MODULES
// -------------------------------------------


module("loginmanager", "../modules/utils/loginmanager/");
module("dashboard", "../modules/utils/dashboard/");
module("search", "../modules/utils/search/");
module("admin_tools", "../modules/utils/admin_tools/");

module("erp", "../modules/erp/");
module("config", "../modules/erp/config/");
module("products", "../modules/erp/products/");
module("customers", "../modules/erp/customers/");
module("locations", "../modules/erp/locations/");
module("tickets", "../modules/erp/tickets/");
module("stocklist", "../modules/erp/stocklist/");
module("pricelist", "../modules/erp/pricelist/");
module("finance", "../modules/erp/finance/");
module("reports", "../modules/erp/reports/");

// -------------------------------------------
//           LOGS
// -------------------------------------------

// Atk can write security events to a logfile.
// There are several values you can choose for $config_logging.
// 0 - No logging
// 1 - Log logins
// 2 - Log actions ("User x performed action x on module y")
$config_debug = 0;
$config_logging = 2;
$config_logfile = "../userlog.log";

// -------------------------------------------
//           LAYOUT CONFIGURATION
// -------------------------------------------

// In admin pages, Achievo shows you a number of records with previous and
// next buttons. You can specify the number of records to show on a page.
//

$config_recordsperpage=10;

// num of rows on paging_logic, MAX=10
$config_num_of_rows=8;

// max pages on paging_logic
$config_pages_limit=10;

// max pricelist user can create
$config_pricelists_limit=100;

// max locations user can create
$config_locations_limit=100;

// The theme defines the layout of Achievo. You can see which
// themes there are in the directory themes. Users can choose their own
// theme in the user preferences section.
//
$config_defaulttheme = "stillblue";

// The language of the application. You can use any language for which
// a language file is present in the languages directory.
//
$config_language="en"; 
$config_defaultlanguage="en"; // fallback language  

// The menu to use
$config_menu_layout = "modern";

// This variable determines wether all manytoone dropdowns should have
// the autocompletion feature (instead of dropdowns)
//
$config_manytoone_autocomplete_default = false;

// -------------------------------------------
//           SECURITY CONFIGURATION
// -------------------------------------------

// The method to use for user/password validation.
$config_authentication = "db";

// The password to use for guest login.
// A guest password that is empty will *DISABLE* guest login!
$config_guestpassword = ""; // disabled!, also disabled in atksecuritymanager

// If you set $config_auth_dropdown to true, the login screen will contain
// a list of available usernames. If you set it to false, there will be
// an input box to manually enter the userid. Use the dropdown only if
// the number of users is small.
//
$config_auth_dropdown = false;

// As an extra security measure, to prevent session highjacking, Achievo
// will regenerate it's session id on each hit. This does not work
// properly on some (usually older) PHP installations. If you experience
// login trouble, set the next value to false.
$config_session_regenerate = false;

// If set to true, a cookie with username/password is written, so
// users will stay logged in, even if they close their browser.
$config_authentication_cookie = false;


// -------------------------------------------
//             PIM MODULE
// -------------------------------------------
// If you don't want a pim, set an URL in the config below to let PIM
// redirect to the specified url. If not set or empty this feature is
// disabled.
$config_pim_redirect_to = "";


// -------------------------------------------
//             DOCMANAGER MODULE
// -------------------------------------------
// If you a docmanager to other nodes add the node and file location here
$config_docmanageroptions = array(
        "project.project"=>array("dir"=>"./documents/project/", "url"=>"documents/project/"),
        "quotation.quotation"=>array("dir"=>"./documents/quotations/", "url"=>"documents/quotations/"),
);


// -------------------------------------------
//            GENERAL SETTINGS
// -------------------------------------------
// Startday is
// 6 = saturday
// 0 = sunday
// 1 = monday (ISO)
// 3 = wednesday
$config_startday = 1;

$config_lose_changes_warning = false;

// First week contains has 3 valid options
// 1 = Starts on 1 january
// 4 = First week with 4 days (ISO)
// 7 = First full week
$config_firstweekcontains = 4;

$config_identifier = "ia32";
$config_top_frame = 1;

$config_authorization = "db";
$config_auth_usernode = "loginmanager.users";
$config_auth_emailfield = "email";

//$config_auth_accountenableexpression = "role='employee' AND status='active'";
$config_auth_accountenableexpression = "1=1";

// config per user
$config_auth_usertable   = "admin_users";
$config_auth_logtable   = "admin_log";
$config_auth_userfield   = "user";
$config_auth_passwordfield = "password";
$config_auth_accesstable = "admin_accessrights";
$config_auth_leveltable  = "admin_users";
$config_auth_levelfield  = "group_id";
$config_auth_languagefield   = "language";
$config_auth_userpk = "id";
$config_auth_userfk = "employee_id";
$config_auth_dbconfigfield = "db_config"; // user database config
$config_auth_tblprefixfield = "tbl_prefix"; // user table prefix config
$config_auth_usersgroup = 3; // for checks 
$config_auth_demogroup = 4; // for demo checks


$config_max_loginattempts = 10; // resolve error with index.php?atklogout=1
$config_auth_enablepasswordmailer = true;
$config_auth_loginform = true;
$config_auth_userdescriptor = "[userid]";
$config_authentication_md5 = true;
$config_securityscheme = "group";

$config_atktempdir = $config_atkroot."app_tmp/";
$config_tplcompiledir = $config_atktempdir."compiled/tpl/";
$config_auth_grantall_privilege = "employee.profile.grantall";
$config_durationformat = 0;
$config_defaultfavico = "atk/themes/ia32.ico";

$config_supported_languages_module = $config_atkroot.'atk/languages/';
$config_autoload_classes = true;

$config_top_search_node = 'search.search';
$config_top_search_minchars = 2;

$config_compress_pdf = false;

// -------------------------------------------
//        DOCUMENT WRITER CONFIGURATION
// -------------------------------------------

// For document attributes, ATK automatically searches for template
// documents in a specific directory. The base directory to search in
// can be specified below. The document templates must be put in a
// specific directory structure under this base directory: first of all
// a subdirectory must be made for every module for which you want to
// include document templates (equal to the modulename of that module, as
// set in config.inc.php). Then a subdirectory in that directory must be
// made according to the name of the node for which you want to include
// document templates. In this subdirectory you can put your document
// template files. So if you have $config_doctemplatedir set to
// "doctemplates/", then you can put your documents in
// "doctemplates/modulename/nodename/".
//
$config_doctemplatedir = "doctemplates/";



// ---------------------------------------
//          AJAX CONFIG
// ---------------------------------------
// This variable configures the visibility of the parameters in printed
// reports such as the hoursurvey.
$config_report_hide_parameters = false;

// This variable determines how many characters a user must enter
// before an autocompletion search is performed.
//
$config_manytoone_autocomplete_minchars = 2;

//The search mode of the autocomplete fields.
//Can be 'startswith', 'exact' or 'contains'.
$config_manytoone_autocomplete_searchmode = "contains";

// Value determines wether the search of the autocompletion is case-sensitive.
$config_manytoone_autocomplete_search_case_sensitive = false;


// Should all many-to-one relations that have the AF_LARGE flag set also
// have the AF_RELATION_AUTOCOMPLETE flag set?
//
$config_manytoone_autocomplete_large = true;


// ---------------------------------------
//          SESSIONS
// ---------------------------------------


// Session cache expire (minutes)
// @var int

$config_session_cache_expire = 180;


// Session cache limiter
//
// Possible values:
// - nocache
// - public  (permits caching by proxies and clients
// - private (permits caching by clients
// - private_no_expire (permits caching by clients but not sending expire
//   headers >PHP4.2.0)
// @var String
$config_session_cache_limiter = "nocache";

