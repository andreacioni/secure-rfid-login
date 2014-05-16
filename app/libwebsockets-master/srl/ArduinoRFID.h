#define HEY_MESSAGE "HEY!"
#define RESPONSE_BUFFER_LENGHT 2
#define KA_RECEIVED 0
#define SS_RECEIVED 1
#define MAX_MESSAGE_LENGHT 64

int wait_for_device();
void send_keep_alive(int port);
int receive_keep_alive(int port);
int receive_block(int port,char *buffer,size_t len);
void write_block(int port,unsigned char * block, size_t len,unsigned char block_num);
int wait_message(int port, int timeout, unsigned char *buff);
int check_message(unsigned char *buffer,int lenght,unsigned char ack_byte);