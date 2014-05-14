#include<stdio.h>
#include<windows.h>
#include "ArduinoRFID.h"

#define TIMEOUT 10000
char buff[(MAX_MESSAGE_LENGHT*4)+4];
char nome[MAX_MESSAGE_LENGHT] = {0},cognome[MAX_MESSAGE_LENGHT] = {0},nomeUtente[MAX_MESSAGE_LENGHT] = {0},password[MAX_MESSAGE_LENGHT] = {0};
void sendInfoNotification();
NOTIFYICONDATA *nData;

int startArgoRFIDReader(NOTIFYICONDATA *notify)
{
	int port = wait_for_device();

	nData = notify;

	while(1) //Ciclo continuo che manda/riceve il keep alive
	{
		send_keep_alive(port);
		if(receive_keep_alive(port) == SS_RECEIVED)
		{
			//One message is arriving...poll to the COM port for max. 10s
			if(wait_message(port,TIMEOUT,(unsigned char *) buff) == 1) //			!!!!!Non funziona il timeout!!!!!	
			{
				printf("message received!\n");

				//Set the data into the correct array
				memcpy(nome,&buff[MAX_MESSAGE_LENGHT*0],64);
				memcpy(cognome,&buff[MAX_MESSAGE_LENGHT*1],64);
				memcpy(nomeUtente,&buff[MAX_MESSAGE_LENGHT*2],64);
				memcpy(password,&buff[MAX_MESSAGE_LENGHT*3],64);

				//Send notification
				sendInfoNotification();
			}
			else
				printf("INVALID message received!\n");
		}
		Sleep(1000);
	}
}

void sendInfoNotification()
{
	nData->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO | NIF_STATE;
	strcpy(nData->szInfo, strcat(nome,cognome));
	strcpy(nData->szInfoTitle, "Tag detected! Starting Firefox...");
	nData->uTimeout = 1000;
	nData->dwInfoFlags = NIIF_INFO;

	Shell_NotifyIcon(NIM_MODIFY,nData);

}
