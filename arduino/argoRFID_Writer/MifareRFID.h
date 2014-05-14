#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9

#define ERROR_AUTH 1
#define ERROR_WRITE 2
#define ERROR_READ 3

#define MAX_BLOCK_SIZE 65 //64 + 1 ACK byte

#define BLOCK_NAME 1
#define BLOCK_COGN 2
#define BLOCK_USER 3
#define BLOCK_PASS 4

// ------------------------------------------------ MIFARE BLOCKS DEFINITIONS 
#define BLOCK_NOME_1 0x04
#define BLOCK_NOME_2 0x05
#define BLOCK_NOME_3 0x06

#define TRAILER_1    0x07

#define BLOCK_NOME_4 0x08
#define BLOCK_COGN_1 0x09
#define BLOCK_COGN_2 0x0A

#define TRAILER_2    0x0B

#define BLOCK_COGN_3 0x0C
#define BLOCK_COGN_4 0x0D
#define BLOCK_USER_1 0x0E

#define TRAILER_3   0x0F

#define BLOCK_USER_2 0x10
#define BLOCK_USER_3 0x11
#define BLOCK_USER_4 0x12

#define TRAILER_4    0x13

#define BLOCK_PASS_1 0x14
#define BLOCK_PASS_2 0x15
#define BLOCK_PASS_3 0x16

#define TRAILER_5    0x17

#define BLOCK_PASS_4 0x18

#define TRAILER_6    0x1A
//---------------------------------------------------------- END

void mifare_init(MFRC522 *mrfc,MFRC522::MIFARE_Key *key,const unsigned char b_key[]);
int select_card(MFRC522 *mfrc522);
void print_serial(MFRC522 *mfrc522);
void stop_write(MFRC522 *mrfc);
void write_blocks(MFRC522 *mrfc522,MFRC522::MIFARE_Key *key,int select,unsigned char buff[]);
int error_dump(MFRC522 *mfrc522,int error_code,unsigned char res);


