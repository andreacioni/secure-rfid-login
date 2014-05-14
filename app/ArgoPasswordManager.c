//Header generici
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>

//Header Linux
#ifdef LINUX
 #include<sys/param.h>
 #include<errno.h> 
 #include <termios.h> 
#endif

//Header Windows
#ifdef WINDOWS


#endif


//Miei header(s)
#include "config.h"
#include "sha1.h"
#include "serial.h"
#include "debug.h"

void launch_firefox();
void listen_for_incoming_message(char *buf);
void decrypt_password(char *password,char* buf);

int main( int argc, const char* argv[] )
{
      unsigned char incoming_buffer[1024]; //Buffer di appoggio per i dati in arrivo da seriale

      printf("Benvenuto %s nel programma di gestione credenziali Argo ScuolaNext!\n", OWNER);   
      
      printf("Programma creato in data: %s\n",COMPILED_DATE);
      
      printf("Più informazioni su questo programma sono accessibili lanciando ArgoPasswordManager -h\n");
      
      printf("Passare la propria carta o avvicinare il proprio telefono dotato di chip NFC al lettore di carte...\n");
      
      //Rimane in ascolto per i dati provenienti dal lettore di tag RFID (Arduino)
      char encoded_message[READ_LENGHT];
      listen_for_incoming_message(encoded_message);
      
      /*Adesso abbiamo il messaggio contenente la password codificato, deve essere decodificato
  		opportunamente per sbloccare la password*/
  		char message[READ_LENGHT]; 
      decrypt_password(encoded_password,p);
      
      //Lancia Firefox direttamente all'indirizzo del Portale Argo
      //launch_firefox();
      

      //debug("User: USERNAME\nPassword: PASSWORD\n"); 

	
		//1.Controllo checksum
		//2.Controllo USB
		//3.Generazione token
	
		//4.Avvio Firefox
		//system("firefox www.andysite.altervista.org");
	
		//create_token();    
      
}

void listen_for_incoming_message(char *buf)
{
	  int USB = init();
	  
	  if(USB == -1)
	  {
		 printf("Si è verificato un errore, contattare lo sviluppatore.\n Codice di errore 1.\n");
		 return;
	  }
	  
	  wait_for_message(USB,buf);
		 
	  if(buf == NULL)
	  {
		 printf("Errore nella lettura del TAG!");
		 return;  
	  }
	  
	  //printf("%s\n",&p);
}

void decrypt_password(char *password,char* buf)
{
	/*SHA1Context context;
	
	SHA1Reset(&context);
	SHA1Input(&context, (const unsigned char *) password, strlen(READ_LENGHT));
	
	if (!SHA1Result(&context))
   {
        debug("ERROR-- could not compute message digest");
   }
   else
   {
       printf("\t");
       for(i = 0; i < 5 ; i++)
       {
       	  
           printf("%X ", context.Message_Digest[i]);
       }
       printf("\n");
   }*/
	
}

void launch_firefox()
{
  system("firefox https://www.securen-argo.com/argoweb/scuolanext/common/login_form.jsp#");
}
