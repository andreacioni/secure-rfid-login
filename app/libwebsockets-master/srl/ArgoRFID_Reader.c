#include<stdio.h>
#include<windows.h>

#include "ArduinoRFID.h"
#include "EncDec.h"
#include "DatabaseManager.h"

#define TIMEOUT 10 //Number of seconds to wait a message after SS flag was captured
#define SERIAL_LENGHT 4

char buff[(MAX_MESSAGE_LENGHT*4)+4];
char name[MAX_MESSAGE_LENGHT] = {0x00},surname[MAX_MESSAGE_LENGHT] = {0x00},username[MAX_MESSAGE_LENGHT] = {0x00},password[MAX_MESSAGE_LENGHT] = {0x00};
NOTIFYICONDATA *nData;
BOOL is_data_present = FALSE;

void sendInfoNotification();
void start(char * cmd);

int port = -1;

int start_rfid_reader(NOTIFYICONDATA *notify)
{
	
	unsigned char enc_username[MAX_MESSAGE_LENGHT] = {0x00},enc_password[MAX_MESSAGE_LENGHT] = {0x00};
	unsigned char serialnumber[SERIAL_LENGHT];

	port = wait_for_device();
	nData = notify;

	while(port != -1) //Ciclo continuo che manda/riceve il keep alive
	{
		send_keep_alive(port);
		if(receive_keep_alive(port) == SS_RECEIVED)
		{
			//One message is arriving...poll to the COM port for max. 10s
			
			is_data_present = FALSE;

			//	!!!!!!!Non funziona il timeout!!!!!	
			if(wait_message(port,TIMEOUT,(unsigned char *) buff, ((MAX_MESSAGE_LENGHT*4)+4)) == 1)
			{
				printf("[SERIAL]: ");
				printf("message received!\n");

				wait_serial_number(port,serialnumber);

				//Clean the buffer to prevent error
				memset(name,0,MAX_MESSAGE_LENGHT);
				memset(surname,0,MAX_MESSAGE_LENGHT);
				memset(username,0,MAX_MESSAGE_LENGHT);
				memset(password,0,MAX_MESSAGE_LENGHT);

				//Set the data into the correct array
				memcpy(name,&buff[(MAX_MESSAGE_LENGHT+1)*0],MAX_MESSAGE_LENGHT);
				memcpy(surname,&buff[(MAX_MESSAGE_LENGHT+1)*1],MAX_MESSAGE_LENGHT);
				memcpy(enc_username,&buff[(MAX_MESSAGE_LENGHT+1)*2],MAX_MESSAGE_LENGHT);
				memcpy(enc_password,&buff[(MAX_MESSAGE_LENGHT+1)*3],MAX_MESSAGE_LENGHT);

				//Serial database checking
				if(is_serial_present(serialnumber) != 1)
					printf("[DATABASE] Invalid serial card number!\n");
				else
				{
					//Data are ready and correctly sent
					is_data_present = TRUE;

					//Decrypt data
					decrypt_data(enc_username,enc_password,username,password);

					//Send notification
					sendInfoNotification();
	
					start("START firefox https://www.portaleargo.it/argoweb/home.seam");
				}
				
			}
			else
			{	
				printf("[SERIAL]: ");
				printf("INVALID message received!\n");
			}
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

int get_name(char* buff)
{
	int num = -1;
	if(is_data_present == TRUE)
	{
		num = strlen(name);
		
		if(num > MAX_MESSAGE_LENGHT)
		{
			buff = NULL;
			num = -1;
		}
		else
			strcpy_s(buff,MAX_MESSAGE_LENGHT,name);

		return num;		
	}

	//if there aren't data to send
	buff = NULL;

	return -1;
}

int get_surname(char* buff)
{
	int num = -1;
	if(is_data_present == TRUE)
	{
		num = strlen(surname);
		
		if(num > MAX_MESSAGE_LENGHT)
		{
			buff = NULL;
			num = -1;
		}
		else
			strcpy_s(buff,MAX_MESSAGE_LENGHT,surname);

		return num;		
	}

	//if there aren't data to send
	buff = NULL;

	return -1;
}

int get_username(char* buff)
{
	int num = -1;
	if(is_data_present == TRUE)
	{
		num = strlen(username);
		
		if(num > MAX_MESSAGE_LENGHT)
		{
			buff = NULL;
			num = -1;
		}
		else
			strcpy_s(buff,MAX_MESSAGE_LENGHT,username);

		return num;		
	}

	//if there aren't data to send
	buff = NULL;

	return -1;
}

int get_password(char* buff)
{
	int num = -1;
	if(is_data_present == TRUE)
	{
		num = strlen(password);
		
		if(num > MAX_MESSAGE_LENGHT)
		{
			buff = NULL;
			num = -1;
		}
		else
			strcpy_s(buff,MAX_MESSAGE_LENGHT,password);

		return num;		
	}

	//if there aren't data to send
	buff = NULL;

	return -1;
}

void start(char * cmd)
{
	system(cmd);
}

void clean_data()
{
	memset(name,0x00,MAX_MESSAGE_LENGHT);
	memset(surname,0x00,MAX_MESSAGE_LENGHT);
	memset(username,0x00,MAX_MESSAGE_LENGHT);
	memset(password,0x00,MAX_MESSAGE_LENGHT);
}

void close_rfid_reader()
{
	port = -1;
	RS232_CloseComport(port);
}