#include "MifareRFID.h"
#include <SPI.h>
#include <MFRC522.h>

#define BAUD_RATE 115200
#define LED 6
#define SPEAKER_PIN 8
#define TONE_1 440
#define TONE_2 294
#define BEEP_ERROR 1
#define BEEP_DONE 2
#define KEY_LENGHT 6
#define TIME_TO_WAIT 2000
#define WAIT_BETWEEN_MESSAGE 100

void check_and_fill();
void empty_serial();
void send_data();
void emit_beep(int);

#define INIT_MSG "INIT\n"
#define HEY_MSG "HEY!"

unsigned char nome[MAX_BLOCK_SIZE];
unsigned char cognome[MAX_BLOCK_SIZE];
unsigned char user[MAX_BLOCK_SIZE];
unsigned char psw[MAX_BLOCK_SIZE];

unsigned char b_key[KEY_LENGHT];
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

int num_of_block;
int num_of_byte;

boolean somthing_to_send = false;
boolean connection_done = false;

void setup()
{
  Serial.begin(BAUD_RATE);
  SPI.begin();

  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);

  for(int i=0;i<KEY_LENGHT;i++)
    b_key[i] = 0xFF;

  mifare_init(&mfrc522,&key,b_key);

  emit_beep(BEEP_DONE);
}

void loop()
{

  if(connection_done == false)
  {
    while(Serial.available() > 0)
    {
      if(Serial.find(INIT_MSG))
      {
        Serial.write("OK!");
        digitalWrite(LED,LOW);
        
        delay(500);
        
        connection_done = true;
      }
    }

    empty_serial();    
  }
  else
  {
    while(Serial.available() > 0)
    {
      if(Serial.find(HEY_MSG)) //Messaggio di INIT riconosciuto
      {
        if(somthing_to_send) //C'è qualcosa da inviare al computer?
        {
          Serial.write("SS"); //Send Something To Send signal, one card have released its data...

          //Inizia la trasmissione...
          //Wait before send to allow the computer to start listening
          delay(TIME_TO_WAIT);
          
          send_data();

          somthing_to_send = false;
        }
        else
          Serial.write("KA"); //Send Keep-Alive signal, nothing to send!
      }
    }

    empty_serial();    

    if(select_card(&mfrc522) == 0)
    {
      //Una carta è presente, comincia a leggere...
      short int i = 0; // Only 4 unit
      
      if(!read_blocks(&mfrc522,&key,BLOCK_NAME,nome))
      {
        emit_beep(BEEP_ERROR);
        return;
      }
      nome[MAX_BLOCK_SIZE-1] = (0x30 + i); i++;
      //Send the name in blocks of 16...
      
      if(!read_blocks(&mfrc522,&key,BLOCK_COGN,cognome))
      {
        emit_beep(BEEP_ERROR);
        return;
      }
      cognome[MAX_BLOCK_SIZE-1] = (0x30 + i); i++;
      //Send the surname in blocks of 16...

      if(!read_blocks(&mfrc522,&key,BLOCK_USER,user))
      {
        emit_beep(BEEP_ERROR);
        return;
      }
      user[MAX_BLOCK_SIZE-1] = (0x30 + i); i++;
      //Send the surname in blocks of 16...
      
      if(!read_blocks(&mfrc522,&key,BLOCK_PASS,psw))
      {
        emit_beep(BEEP_ERROR);
        return;
      }
      psw[MAX_BLOCK_SIZE-1] = (0x30 + i); i++;
      //Send the surname in blocks of 16...
      
      stop_write(&mfrc522);
      
      //We have all data ready to send
      somthing_to_send = true;
      emit_beep(BEEP_DONE);

    }
  }
  delay(100);

}

void send_data()
{
  int num_of_block = 0;
  for(int i=0;i<NUM_OF_BLOCKS;i++)
  {
    Serial.write(&nome[i*16],16);
    delay(WAIT_BETWEEN_MESSAGE);
  }

  Serial.write(0x30 + num_of_block);
  num_of_block++;

  for(int i=0;i<NUM_OF_BLOCKS;i++)
  {
    Serial.write(&cognome[i*16],16);
    delay(WAIT_BETWEEN_MESSAGE );
  }

  Serial.write(0x30 + num_of_block);
  num_of_block++;

  for(int i=0;i<NUM_OF_BLOCKS;i++)
  {
    Serial.write(&user[i*16],16);
    delay(WAIT_BETWEEN_MESSAGE );
  }

  Serial.write(0x30 + num_of_block);
  num_of_block++;

  for(int i=0;i<NUM_OF_BLOCKS;i++)
  {
    Serial.write(&psw[i*16],16);
    delay(WAIT_BETWEEN_MESSAGE );
  }

  Serial.write(0x30 + num_of_block);
  num_of_block++;

}

void empty_serial()
{
  while(Serial.available() > 0)
    Serial.read();
}

void emit_beep(int mode)
{
    switch(mode)
    {
      case BEEP_DONE:
        tone(SPEAKER_PIN, TONE_1);
        delay(200);
        noTone(SPEAKER_PIN);
      break;
      
      case BEEP_ERROR:
        tone(SPEAKER_PIN, TONE_2);
        delay(350);
        noTone(SPEAKER_PIN);
        tone(SPEAKER_PIN, TONE_1);
        delay(200);
        noTone(SPEAKER_PIN);
      break;
      
      
    }
}




