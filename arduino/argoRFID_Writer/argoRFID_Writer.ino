/*--------------------------------------------------------------------------------------
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               52                MOSI
 * SPI MISO   12               51                MISO
 * SPI SCK    13               50                SCK
 *
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com. 
 */

#include "MifareRFID.h"
#include <SPI.h>
#include <MFRC522.h>

#define BAUD_RATE 115200
#define LED 6
#define SPEAKER_PIN 3
#define TONE_1 440
#define TONE_2 415
#define KEY_LENGHT 6

void check_and_fill();
void empty_serial();

const char init_msg[] = "INIT\n";

unsigned char nome[MAX_BLOCK_SIZE];
unsigned char cognome[MAX_BLOCK_SIZE];
unsigned char user[MAX_BLOCK_SIZE];
unsigned char psw[MAX_BLOCK_SIZE];

unsigned char b_key[KEY_LENGHT];
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

int num_of_block;
int num_of_byte;

int connection_done;
int block_ready;

void setup()
{
  Serial.begin(BAUD_RATE);
  SPI.begin();
  
  num_of_block = 0;
  num_of_byte = 0;
  
  block_ready = 0;
  connection_done = 0;
  
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  
  for(int i=0;i<KEY_LENGHT;i++)
    b_key[i] = 0xFF;

  mifare_init(&mfrc522,&key,b_key);
  
  tone(SPEAKER_PIN, TONE_1);
  delay(200);
  noTone(SPEAKER_PIN);
}

void loop()
{
  if(connection_done == 0)
  {
    while(Serial.available() > 0)
    {
       int num=0;
       
       for(int i=0;i<5;i++)
       {
         if(Serial.read() == init_msg[i])
           num++;
       }          
       
       if(num==5)
       {
        Serial.write("OK!");
        digitalWrite(LED,LOW);
        tone(SPEAKER_PIN, TONE_2);
        delay(200);
        noTone(SPEAKER_PIN);
        delay(500);
        connection_done = 1;
       }
    }
   
   empty_serial();    
  }
  else
  {
    if(block_ready == 0)
    {      
      //Attendi un messaggio
      while(Serial.available() > 0)
      {        
        
        switch(num_of_block)
        {
          case 0:
            nome[num_of_byte] = Serial.read();
            break;
          case 1:
            cognome[num_of_byte] = Serial.read();
            break;
          case 2:
            user[num_of_byte] = Serial.read(); 
            break;
          case 3:
            psw[num_of_byte] = Serial.read();
            break;
        }
        
        if(num_of_byte == 64)
        {
          Serial.print("\n64 byte arrived!\n");
          
          check_and_fill();
   
          empty_serial();     
          
        }
        else
          num_of_byte++; 
        
        //Serial.print(nome[num_of_byte],HEX);      
      }
    }
    else
    {
      //Attendi la carta...
      
      empty_serial();
      
      if(select_card(&mfrc522) == -1)
        return;
        
        /*tone(speakerPin, tones[8]);
        delay(500);
        noTone(speakerPin);*/
        
	// Dump debug info about the card. PICC_HaltA() is automatically called.
	//mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

        Serial.print("Card found! ");
        
        print_serial(&mfrc522);
        
        digitalWrite(LED,LOW);
        
        Serial.write("Write starting...\n");
        
        write_blocks(&mfrc522,&key,BLOCK_NAME,nome);
        write_blocks(&mfrc522,&key,BLOCK_COGN,cognome);
        write_blocks(&mfrc522,&key,BLOCK_USER,user);
        write_blocks(&mfrc522,&key,BLOCK_PASS,psw);
        
        stop_write(&mfrc522);
    }
  }
   
   
   delay(100);
  
}

void check_and_fill()
{
  switch(num_of_block)
  {
    case 0:
        if(nome[num_of_byte] == (num_of_block + 0x30))
          {
            num_of_byte = 0;            
            num_of_block++;
            
            tone(SPEAKER_PIN, TONE_1);
            delay(200);
            noTone(SPEAKER_PIN);
            
            //empty_serial();   
            
            Serial.print("Message ready!\n");
          }
          else
          {
            Serial.print("ERROR! Invalid message!\n");
            connection_done = 0;
            num_of_byte = 0;
            
            //empty_serial();   
          }
    break;
    
    case 1:
        if(cognome[num_of_byte] == (num_of_block + 0x30))
          {
            num_of_byte = 0;            
            num_of_block++;
            
            tone(SPEAKER_PIN, TONE_1);
            delay(200);
            noTone(SPEAKER_PIN);
            
            //empty_serial();   
            
            Serial.print("Message ready!\n");
          }
          else
          {
            Serial.print("ERROR! Invalid message!\n");
            connection_done = 0;
            num_of_byte = 0;
            
            //empty_serial();   
          }
    break;
    
    case 2:
        if(user[num_of_byte] == (num_of_block + 0x30))
          {
            num_of_byte = 0;              
            num_of_block++;
            
            tone(SPEAKER_PIN, TONE_1);
            delay(200);
            noTone(SPEAKER_PIN);
            
            //empty_serial();   
            
            Serial.print("Message ready!\n");

          }
          else
          {
            Serial.print("ERROR! Invalid message!\n");
            connection_done = 0;
            num_of_byte = 0;
            
            //empty_serial();   
          }
    break;
    
    case 3:
        if(psw[num_of_byte] == (num_of_block + 0x30))
          {
            num_of_byte = 0;
            
            //Serial.print("Blocks ready!\n");
              
            tone(SPEAKER_PIN, TONE_2);
            delay(1000);
            noTone(SPEAKER_PIN);
             
            block_ready = 1;
            num_of_byte = 0;
            num_of_block = 0;
             
            break;
            
            //Serial.print("Message ready!\n");
          }
          else
          {
            Serial.print("ERROR! Invalid message!\n");
            connection_done = 0;
            num_of_byte = 0;
            
            //empty_serial();   
          }
    break;
    
  }
  /*if(nome[num_of_byte] == (num_of_block + 0x30))
          {
            num_of_byte = 0;
            
            if(num_of_block == 3)
            {
              //Serial.print("Blocks ready!\n");
              
              tone(speakerPin, tones[9]);
              delay(1000);
              noTone(speakerPin);
              
              block_ready = true;
              num_of_byte = 0;
              num_of_block = 0;
              
              break;
            }              
            num_of_block++;
            
            /*tone(speakerPin, tones[9]);
            delay(500);
            noTone(speakerPin);
            
            empty_serial();   
            
            //Serial.print("Message ready!\n");
            break;
          }
          else
          {
            Serial.print("ERROR! Invalid message!\n");
            connection_done = false;
            num_of_byte = 0;
            
            empty_serial();   
          }  */
}

void empty_serial()
{
  while(Serial.available() > 0)
    Serial.read();
}



