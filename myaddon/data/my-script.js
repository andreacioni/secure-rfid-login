//https://www.portaleargo.it/argoweb/scuolanext/;jsessionid=82BFF40C528E121CD4E66B8A6085F085.node1
var socket;
var n_message = 0;
var error_occured = false;

function securedAuth() 
{
	if(error_occured == false)
		socket.send("INIT");  	
}


// Check for the various File API support.
if (window.WebSocket) 
{  
 	 //Aggiunge un nuovo bottone alla scheramata di login
	$(".footer").append("<table width='100%'><tr><td style='padding-right: 170px; padding-top: 10px' > <input id='secure_access' type='button' name='secure' 							value='Attendi...' class='button' /> </td></tr> <tr><td style='padding-top: 10px' > <p align=center><font size='2'>Se hai problemi con l'accesso sicuro visita <a href='http://www.andysite.altervista.org'>questa pagina...</a></font></p> </td></tr></table>");
	
	document.getElementById("secure_access").addEventListener('click', securedAuth, false);	
	
	//Apri web socketz
  		socket = new WebSocket("wss://127.0.0.1:7681/","dumb-increment-protocol");    
  		
  		socket.onopen = function () {
			document.getElementById('secure_access').value = "Accesso sicuro";
	    	//alert("Opened!");
		};
	
		// Errori (nella console)
		socket.onerror = function (error) {
	    	//console.log('WebSocket Error ' + error);
	    	document.getElementById('secure_access').value = "Errore!";
			error_occured = true;
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
			document.getElementById('user').focus();
			$("#j_password").val(buff);
			document.getElementById('j_password').blur();
			n_message = 0;
			}
		};

	//Riempie i campi
	//$("#utente").val("user");
	//$("#j_password").val("pass");
  
} else { //Not supported
  alert('The File APIs are not fully supported in this browser, update Firefox!');
}
