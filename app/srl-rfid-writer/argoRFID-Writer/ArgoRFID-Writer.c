#include<stdio.h>
#include<conio.h>
#include<ctype.h> 
#include<string.h>
#include<windows.h>

#include "ArduinoRFID.h"
#include "EncDec.h"

#define MAX_LENGHT 64
#define MAX_LENGHT_S "%s63"

void secure_input(char * buffer);

int main()
{
	char nome[MAX_LENGHT] = {0x00},cognome[MAX_LENGHT] = {0x00},username[MAX_LENGHT] = {0x00},password[MAX_LENGHT] = {0x00};
	unsigned char enc_username[MAX_LENGHT]  = {0};
	unsigned char enc_psw[MAX_LENGHT]		= {0};
	int port;
	int ret;
	char select = 0x00;
	
	printf("Inserisci il tuo nome: ");
	ret = scanf_s(MAX_LENGHT_S,&nome,MAX_LENGHT);		//Only 63 characters!
	//size += strlen(nome);

	if(ret != 1)
	{	
		printf("You can enter only %i characters!\n");
		return -1;
	}

	printf("Inserisci il tuo cognome: ");
	ret = scanf_s(MAX_LENGHT_S,&cognome,MAX_LENGHT);	//Only 63 characters!

	if(ret != 1)
	{	
		printf("You can enter only %i characters!\n");
		return -1;
	}
	//size += strlen(cognome);

	printf("Inserisi il tuo nome utente: ");
	ret = scanf_s(MAX_LENGHT_S,&username,MAX_LENGHT);	//Only 63 characters!

	if(ret != 1)
	{	
		printf("You can enter only %i characters!\n", MAX_LENGHT);
		return -1;
	}

	printf("Inserisci la tua password: ");
	secure_input(password);

	printf("Vuoi rivedere i dati inseriti prima di scriverli? (s/n)");
	
	while((select != 'n') && (select != 's'))
		select = getchar();

	if(select == 's')
	{
		printf("%s\n",nome);
		printf("%s\n",cognome);
		printf("%s\n",username);
		printf("%s\n",password);

		printf("\nIf data are incorrect terminate the application and restart it!\n");

		system("pause");
	}
	else
		printf("Dati confermati!\n");


	printf("Crypting data...");

	if(encrypt_data(username,password,enc_username,enc_psw) == FALSE)
	{
		printf("Error on file criptying!\n");
		system("pause");
		return -1;
	}

	printf("DONE!\n");

	port = wait_for_device();
	
	//TODO controllare che la dimensione di quello che è in input(stringa) sia uguale all'output(stringa*codifica)
	//unsigned char output[MAX_LENGHT*4]

	write_block(port,(unsigned char *) nome,MAX_LENGHT,0x30);
	write_block(port,(unsigned char *) cognome,MAX_LENGHT,0x31);
	write_block(port,(unsigned char *) enc_username,MAX_LENGHT,0x32);
	write_block(port,(unsigned char *) enc_psw,MAX_LENGHT,0x33);

	printf("\nData sent to the writing device, to write them leave your RFID tag on the top of the device while he emits beep...\n\n");

	system("pause");

	return 0;

}

void secure_input(char * buff)
{
  int ch;
  int i = 0;
 
  fflush(stdout);
 
  while ((ch = getch()) != EOF
          && ch != '\n'
          && ch != '\r'
		  && i < MAX_LENGHT - 1)
  {
    if (ch == '\b' && i > 0)
    {
      printf("\b \b");
      fflush(stdout);
      i--;
      buff[i] = '\0';
    }
    else if (/*isalnum(ch)*/ 1 )
    {
      putchar('*');
      buff[i++] = (char)ch;
    }
  }
 
  buff[i] = '\0';

  printf("\n");
 
  //printf ("\nYou entered >%s<", buff);
  
 }