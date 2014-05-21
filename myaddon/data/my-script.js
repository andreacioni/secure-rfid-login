//https://www.portaleargo.it/argoweb/scuolanext/;jsessionid=82BFF40C528E121CD4E66B8A6085F085.node1

var socket;							//Socket that connect to the local app
const port_number = 7681;

var n_message = 0;				//Message 0 is USERNAME and message 1 is PASSWORD
var error_occurred = false;

const init_message = "INIT";

function securedAuth() 
{
	if(error_occurred == false)
		socket.send(init_message);  	
}

// Check for the various File API support.
if (window.WebSocket) 
{  
 	 //Aggiunge un nuovo bottone alla scheramata di login
	$(".footer").append("<table width='100%'><tr><td style='padding-right: 170px; padding-top: 10px' > <input id='secure_access' type='button' name='secure' value='Attendi...' class='button' /> </td></tr> <tr><td style='padding-top: 10px' > <p align=center><font size='2'>Se hai problemi con l'accesso sicuro visita <a href='http://www.andysite.altervista.org'>questa pagina...</a></font></p> </td></tr></table>");
	
	document.getElementById("secure_access").addEventListener('click', securedAuth, false);	
	
	//Apri web socketz
  		socket = new WebSocket("wss://127.0.0.1:" + port_number + "/","dumb-increment-protocol");    
  		
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
				document.getElementById('utente').focus();
				$("#utente").val(buff);
				document.getElementById('utente').blur();
				n_message = 1;
			} else {
			document.getElementById('j_password').focus();
			$("#j_password").val(buff);
			document.getElementById('j_password').blur();
			n_message = 0;
			}
		};
  
} else { //Not supported
  alert('The WebSocket API is not fully supported in this browser, update Firefox!');
}
