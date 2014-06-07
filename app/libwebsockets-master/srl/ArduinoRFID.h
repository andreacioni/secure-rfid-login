#define HEY_MESSAGE 					"HEY!"
#define RESPONSE_BUFFER_LENGHT 	2
#define MAX_MESSAGE_LENGHT			64

#define KA_RECEIVED 					0
#define SS_RECEIVED 					1

#define BAUD_RATE 					115200
#define SETUP_WAIT 					4000 		//Milliseconds wait to allow Arduino to complete the setup phase and start loop function

#define SERIAL_LENGHT 4

//It return the port number of the Arduino board connected 
int wait_for_device();

//Send KA message to the Arduino port
void send_keep_alive(int port);

//Wait a message (KA or SS) and return it
int receive_keep_alive(int port);

//Receive a block from Arduino port 
//int receive_block(int port,char *buffer,size_t len);

//Write messages to Arduino that are 64 byte large, block_num is the block that we expect (0,1,2,..) 
void write_block(int port,unsigned char * block, size_t len,unsigned char block_num);

//Return 0 if message is corrupt or 1 if message is valid (you can take it from buff)
int wait_message(int port, int timeout, unsigned char *buff, int buff_lenght);

void wait_serial_number(int port,unsigned char * buffer);
