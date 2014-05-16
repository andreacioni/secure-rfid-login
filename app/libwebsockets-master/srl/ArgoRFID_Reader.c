#include<stdio.h>
#include<windows.h>
#include "ArduinoRFID.h"

#define TIMEOUT 10000
char buff[(MAX_MESSAGE_LENGHT*4)+4];
char name[MAX_MESSAGE_LENGHT] = {0},surname[MAX_MESSAGE_LENGHT] = {0},username[MAX_MESSAGE_LENGHT] = {0},password[MAX_MESSAGE_LENGHT] = {0};
void sendInfoNotification();
NOTIFYICONDATA *nData;

int start_rfid_reader(NOTIFYICONDATA *notify)
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

				//Clean the buffer to prevent error
				memset(name,0,MAX_MESSAGE_LENGHT);
				memset(surname,0,MAX_MESSAGE_LENGHT);
				memset(username,0,MAX_MESSAGE_LENGHT);
				memset(password,0,MAX_MESSAGE_LENGHT);

				//Set the data into the correct array
				memcpy(name,&buff[MAX_MESSAGE_LENGHT*0],64);
				memcpy(surname,&buff[MAX_MESSAGE_LENGHT*1],64);
				memcpy(username,&buff[MAX_MESSAGE_LENGHT*2],64);
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
	strcpy(nData->szInfo, strcat(name,surname));
	strcpy(nData->szInfoTitle, "Tag detected! Starting Firefox...");
	nData->dwInfoFlags = NIIF_INFO;

	Shell_NotifyIcon(NIM_MODIFY,nData);

}

char* get_name()
{
	return name;
}

char* get_surname()
{
	return surname;
}

char* get_username()
{
	return username;
}

char* get_password()
{
	return password;
}


