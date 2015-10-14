/**
 * alcal: new js
 */

function launch_pdf_with_params(entryform, url) {

	var final_url = "";

	var elem = document.getElementById('entryform').elements;

	// add all elements to url that have id
	for ( var i = 0; i < elem.length; i++) {

		if (elem[i].id) {
			final_url += "&" + elem[i].id + "=" + elem[i].value;
		}
	}

	final_url = url + final_url.replace(/\s+/g,"_");

	window.open(final_url);

}

function launch_pdf(entryform, url) {

	window.open(url);

}

function confirmSensitiveAction(text, url) {
	
    var yesno = confirm(text);
    
    if (yesno)
    {
    	 window.location.replace(url);
    }
    
    return false;
  }

function promptForData(text, option, url) {
	
	   var input = new String();
	   var input = prompt(text, '');

	    if (input != null && input != ' ' && input != '')
	    {
	    	destination_url = url + '&' + option + '=\'' + input + '\'';
	    	 window.location.replace(destination_url);
	    }
	    
	  return false;
  }
  
  function simplePopup(url, target) {
	window.open(url, target);
	return false;
}

