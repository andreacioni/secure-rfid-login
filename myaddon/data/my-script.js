//https://www.portaleargo.it/argoweb/scuolanext/;jsessionid=82BFF40C528E121CD4E66B8A6085F085.node1

var socket;							//Socket that connect to the local app
var port_number = 7681;

var n_message = 0;				//Message 0 is USERNAME and message 1 is PASSWORD
var error_occurred = false;

var init_message = "INIT";

var get_param;
var autologin;

function securedAuth() 
{
	if(error_occurred == false)
		socket.send(init_message);  	
}

function getSearchParameters() {
      var prmstr = window.location.search.substr(1);
      return prmstr != null && prmstr != "" ? transformToAssocArray(prmstr) : {};
}

function transformToAssocArray( prmstr ) {
    var params = {};
    var prmarr = prmstr.split("&");
    for ( var i = 0; i < prmarr.length; i++) {
        var tmparr = prmarr[i].split("=");
        params[tmparr[0]] = tmparr[1];
    }
    return params;
}

// Check for the various File API support.
if (window.WebSocket) 
{  
 	//Aggiunge un nuovo bottone alla scheramata di login
	$(".footer").append("<table width='100%'><tr><td style='padding-right: 170px; padding-top: 10px' > <input id='secure_access' type='button' name='secure' value='Attendi...' class='button' /> </td></tr> <tr><td style='padding-top: 10px' > <p align=center><font size='2'>Se hai problemi con l'accesso sicuro visita <a href='http://www.andysite.altervista.org'>questa pagina...</a></font></p> </td></tr></table>");
	
	document.getElementById('secure_access').addEventListener('click', securedAuth, false);	
	
	document.getElementById('secure_access').value = "Connecting...";

	
	//Apri web socketz
  		socket = new WebSocket("wss://localhost:" + port_number + "/","dumb-increment-protocol");    
  		
  		socket.onopen = function () {
			document.getElementById('secure_access').value = "Accesso sicuro";
	    	//alert("Opened!");
		};
	
		// Errori (nella console)
		socket.onerror = function (error) {
	    	//console.log('WebSocket Error ' + error);
	    	document.getElementById('secure_access').value = "Errore!";
			error_occurred = true;
		};
	
		// Messaggi dal server
		socket.onmessage = function (e) {
	    	//console.log('Server: ' + e.data);
	    	var buff = e.data;
			//console.log("Server: " + e.data);
			if(n_message == 0){
				document.getElementById('j_username').focus();
				$("#j_username").val(buff);
				document.getElementById('j_username').blur();
				n_message = 1;
			} else {
			document.getElementById('j_password').focus();
			$("#j_password").val(buff);
			document.getElementById('j_password').blur();
			n_message = 0;
			}
		};
		
		
		get_param = getSearchParameters();
		autologin = get_param['autologin'];
		
		if(autologin == 'true')
		{
			//TODO
		}
  
} else { //Not supported
  alert('The WebSocket API is not fully supported in this browser, update Firefox!');
}
