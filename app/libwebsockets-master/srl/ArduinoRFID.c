#include<time.h>

#include "ArduinoRFID.h"
#include "rs232.h"

const char *hey_message = HEY_MESSAGE;				//Message sent to the Arduino board to do keep-alive
char response_buffer[RESPONSE_BUFFER_LENGHT];	//It's a buffer containing the byte expected from keep-alive

//Used by receive_keep_alive
int compare_keep_alive();

//Check message and return KA/SS_RECEIVED
int check_message(unsigned char *buffer,int lenght,unsigned char ack_byte);

int wait_for_device()
{
	int port_number = 8,received = 0;
	char *INIT = "INIT\n";
	char OK[4];
	char uid[4];

	//char buffer[278 + 1];
	
	printf("[SERIAL]: ");
	printf("Searching device...");

	while(port_number < 16)
	{
		if(RS232_OpenComport(port_number,BAUD_RATE))
		{
			//printf("non posso aprire la porta COM%i\n",port_number+1);
		}
		else
		{
			//printf("porta aperta: COM%i\n",port_number+1);

			Sleep(SETUP_WAIT); //Allows arduino to setup itself
			
			if(RS232_SendBuf(port_number,(unsigned char *) INIT,5)==1)
			{
				printf("[SERIAL]: ");
				printf("send error: COM%i\n",port_number+1);
			}
			else
			{
				//printf("buffer sent!\n");
				
				Sleep(100);
				
				received = RS232_PollComport(port_number,(unsigned char *) OK,4);
				
				if(received > 0)
				{						
					//printf("Qualcosa è arrivato(%i)\n",received);

					OK[3] = 0;
					
					if(strcmp(OK,"OK!")==0)
					{
						printf("[SERIAL]: ");
						printf("Device discovered!\n"); // lettore riconosciuto!

						Sleep(500);
						return port_number;
						/*unsigned char ring_buff[4];
						int offset = 0;
						while(1)
						{
							if((received = RS232_PollComport(port_number,(unsigned char *) uid,4)) > 0);
							{
								int i;
								for(i=0;((i<received)&&(offset<4));i++,offset++)
								{
									ring_buff[offset] = uid[i];
								}

								if(offset == 4)
								{
									printf("Card found:");
									for(i=0;i<4;i++)
										printf(" 0x%x",ring_buff[i]);
									printf("\n");
									return port_number;
								}
							}
							Sleep(100);
						}*/

						
					}
					else
					{
						printf("[SERIAL]: ");
						printf("messaggio non valido\n");
					}
					
				}
				else
				{
					printf("[SERIAL]: ");
					printf("non e' arrivato niente!\n");
				}			
				
				
			}

			port_number++;
			RS232_CloseComport(port_number);
			printf("[SERIAL]: ");
			printf("port closed!\n");

		}

		Sleep(1000);
	}

	return -1;
}

void send_keep_alive(int port)
{
	RS232_cputs(port, hey_message);
	printf("[SERIAL]: ");
	printf("KA sent!\n");
}

int receive_keep_alive(int port)
{
	int ret=-1;
	RS232_PollComport(port,(unsigned char *) response_buffer,RESPONSE_BUFFER_LENGHT);
	
	switch(compare_keep_alive())
	{
		case KA_RECEIVED:
			printf("[SERIAL]: ");
			printf("KA response received!\n");
			ret = KA_RECEIVED;
			break;
		case SS_RECEIVED:
			printf("[SERIAL]: ");
			printf("SS response received!\n");
			ret = SS_RECEIVED;
			break;
	}
	
	memset(response_buffer,0,RESPONSE_BUFFER_LENGHT);

	return ret;
	
}

int compare_keep_alive()
{
	if( (response_buffer[0] == 'K') && (response_buffer[1] == 'A') )
		return KA_RECEIVED;
	if( (response_buffer[0] == 'S') && (response_buffer[1] == 'S') )
		return SS_RECEIVED;

}

void write_block(int port,unsigned char * block, size_t len,unsigned char block_num)
{
	unsigned char my_block[80] = {0};
	int i;

	if(len!=64)
	{
		printf("[SERIAL]: ");
		printf("Must be 64 kByte!\n");
		return;
	}

	memcpy(my_block,block,len);
	my_block[64] = block_num;

	//for(i=0;i<65;i++)
	//{
	//	Sleep(50);
	//	if(RS232_SendByte(port,my_block[i]) != 0)
	//		printf("error!\n");
	//}
	//printf("sent!\n");

	for(i=0;i<5;i++)
	{
		Sleep(500);
		if(RS232_SendBuf(port,&my_block[i*16],16) != -1)
		{
			printf("[SERIAL]: ");
			printf("sent!\n");
		}
	}

	Sleep(500);
	printf("[SERIAL]: ");
	printf("Block wrote!\n");

}

int wait_message(int port,int timeout, unsigned char *buff, int buff_lenght)
{
	int result = 0;
	int something_arrived = 0;
	int num_of_byte = 0;
	int num_of_blocks = 0;
	int len = 0,received = 0;
	//unsigned char buff[(MAX_MESSAGE_LENGHT*4)+4]; //(64*4) + 4 ACK byte --- Wait the entire message, after we MUST check the ACK byte
	time_t end_time	= time(NULL) + timeout;
	
	if(buff_lenght != ((MAX_MESSAGE_LENGHT*4)+4))
		return result;
	
	printf("[SERIAL]: ");
	printf("start timer...\n");

	while((time(NULL) < end_time) && (received < buff_lenght)) //It stops when timer timeout accurred or when received is at the max size that the buffer could contain
	{
		if((len = RS232_PollComport(port,&buff[received],(buff_lenght - received))) > 0) //The lenght of the buff decrease everytime arrive something; when new bytes arrive they are placed where the last one was placed
		{
			received += len;
			something_arrived = 1;
			//printf("I've received %i\n",received);
			//printf("I've %i bytes free\n",(((MAX_MESSAGE_LENGHT*4)+4) - received));
		}
		
		//Sleep(1000);
	}
	
	printf("[SERIAL]: ");
	printf("finished timer!\n");
	
	if(something_arrived == 1)
	{
		printf("[SERIAL]: ");
		printf("something arrived!\n");
		if((check_message(&buff[0],MAX_MESSAGE_LENGHT+1,0x30)&&
		check_message(&buff[65],MAX_MESSAGE_LENGHT+1,0x31)&&
		check_message(&buff[130],MAX_MESSAGE_LENGHT+1,0x32)&&
		check_message(&buff[195],MAX_MESSAGE_LENGHT+1,0x33)) == 1)
			result = 1;
	}

	return result;
}

int check_message(unsigned char *buffer,int lenght,unsigned char ack_byte)
{
	if(buffer[64] == ack_byte)
		return 1;
	else
		return 0;
}
