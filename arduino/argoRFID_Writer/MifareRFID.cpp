#include "MifareRFID.h"

byte auth_trailer(MFRC522 *mfrc522,MFRC522::MIFARE_Key *key,byte trailer);

void mifare_init(MFRC522 *mfrc522,MFRC522::MIFARE_Key *key, const unsigned char b_key[])
{  
  mfrc522->PCD_Init();
    
  for (byte i = 0; i < 6; i++) 
     key->keyByte[i] = b_key[i];  
}

int select_card(MFRC522 *mfrc522)
{
  // Look for new cards
	if ( ! mfrc522->PICC_IsNewCardPresent()) {
		return -1;
	}

	// Select one of the cards
	if ( ! mfrc522->PICC_ReadCardSerial()) {
		return -1;
	}

    return 0;
}

void print_serial(MFRC522 *mfrc522)
{
  for (byte i = 0; i < (&(mfrc522->uid))->size; i++)
	Serial.print((&(mfrc522->uid))->uidByte[i],HEX);
}



void stop_write(MFRC522 *mrfc522)
{
  // Halt PICC
  mrfc522->PICC_HaltA();

  // Stop encryption on PCD
  mrfc522->PCD_StopCrypto1();
  
}

byte auth_trailer(MFRC522 *mfrc522,MFRC522::MIFARE_Key *key,byte trailer)
{
  byte ret = mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailer, key, &(mfrc522->uid));

  return ret;  
}

void write_blocks(MFRC522 *mfrc522,MFRC522::MIFARE_Key *key,int select,unsigned char buff[])
{
  byte stat;
    switch(select)
    {
      // ------------------------------------------------------ INIZIO BLOCCO NOME --------------------------------------------------------
      case BLOCK_NAME:
      
      stat = auth_trailer(mfrc522,key,TRAILER_1);
      if(error_dump(mfrc522,ERROR_AUTH,stat)!=0)
        break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_NOME_1,&buff[0], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
        break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_NOME_2,&buff[16], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
        break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_NOME_3, &buff[32], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
        break;
      
      stat = auth_trailer(mfrc522,key,TRAILER_2);
      if(error_dump(mfrc522,ERROR_AUTH,stat)!=0)
        break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_NOME_4,&buff[48], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
      
      //Play a bip if name was wrote!
      /*tone(speakerPin, tones[9]);
      delay(500);
      noTone(speakerPin);*/
      Serial.write("Write: NAME\n");    

      
      /*byte b[18];
      byte siz = sizeof(b);
      stat = mfrc522.MIFARE_Read(BLOCK_NOME_1, b, &siz);
      if (stat != MFRC522::STATUS_OK) 
      {
          Serial.print("PCD_Read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(stat));
          break;
      }
      
      for(int i=0;i<16;i++)
        Serial.write(b[i]);*/
      
      break;
    
    // ------------------------------------------------------ INIZIO BLOCCO COGNOME --------------------------------------------------------
    case BLOCK_COGN:
      //stat = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, TRAILER_2, &key, &(mfrc522.uid)); //si usa l'autenticazione fatta prima
      
      stat = mfrc522->MIFARE_Write(BLOCK_COGN_1,&buff[0], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_COGN_2,&buff[16], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
      
      stat = stat = auth_trailer(mfrc522,key,TRAILER_3);
      if(error_dump(mfrc522,ERROR_AUTH,stat)!=0)
        break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_COGN_3, &buff[32], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_COGN_4,&buff[48], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
       
      //Play a bip if name was wrote!
     /* tone(speakerPin, tones[9]);
      delay(500);
      noTone(speakerPin);*/
      Serial.write("Write: SURNAME\n");
      
      /*byte b[18];
      byte siz = sizeof(b);
      stat = mfrc522.MIFARE_Read(BLOCK_NOME_1, b, &siz);
      if (stat != MFRC522::STATUS_OK) 
      {
          Serial.print("PCD_Read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(stat));
          break;
      }
      
      for(int i=0;i<16;i++)
        Serial.write(b[i]);*/
      
      break;  
      
      // ------------------------------------------------------ INIZIO BLOCCO USERNAME --------------------------------------------------------
      case BLOCK_USER:
      
      //stat = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, TRAILER_2, &key, &(mfrc522.uid)); //si usa l'autenticazione fatta prima
      
      stat = mfrc522->MIFARE_Write(BLOCK_USER_1,&buff[0], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
       
      stat = stat = auth_trailer(mfrc522,key,TRAILER_4);
      if(error_dump(mfrc522,ERROR_AUTH,stat)!=0)
        break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_USER_2,&buff[16], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_USER_3, &buff[32], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_USER_4,&buff[48], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
       
      //Play a bip if name was wrote!
      /* tone(speakerPin, tones[9]);
      delay(500);
      noTone(speakerPin);*/
      Serial.write("Write: USERNAME\n");
      
      /*byte b[18];
      byte siz = sizeof(b);
      stat = mfrc522.MIFARE_Read(BLOCK_NOME_1, b, &siz);
      if (stat != MFRC522::STATUS_OK) 
      {
          Serial.print("PCD_Read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(stat));
          break;
      }
      
      for(int i=0;i<16;i++)
        Serial.write(b[i]);*/
      
      break;  
       // ------------------------------------------------------ INIZIO BLOCCO PASSWORD --------------------------------------------------------
      case BLOCK_PASS:
      //stat = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, TRAILER_2, &key, &(mfrc522.uid)); //si usa l'autenticazione fatta prima
      
      stat = stat = auth_trailer(mfrc522,key,TRAILER_5);
      if(error_dump(mfrc522,ERROR_AUTH,stat)!=0)
        break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_PASS_1,&buff[0], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_PASS_2,&buff[16], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_PASS_3, &buff[32], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
       
      stat = mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, TRAILER_6, key, &(mfrc522->uid));
      if(error_dump(mfrc522,ERROR_AUTH,stat)!=0)
        break;
      
      stat = mfrc522->MIFARE_Write(BLOCK_PASS_4,&buff[48], 16);
      if(error_dump(mfrc522,ERROR_WRITE,stat)!=0)
       break;
       
      //Play a bip if name was wrote!
     /* tone(speakerPin, tones[9]);
      delay(500);
      noTone(speakerPin);*/
      Serial.write("Write: PASSWORD\n");
      
      /*byte b[18];
      byte siz = sizeof(b);
      stat = mfrc522.MIFARE_Read(BLOCK_NOME_1, b, &siz);
      if (stat != MFRC522::STATUS_OK) 
      {
          Serial.print("PCD_Read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(stat));
          break;
      }
      
      for(int i=0;i<16;i++)
        Serial.write(b[i]);*/
      
      break;    
    }    
}

int error_dump(MFRC522 *mfrc522,int error_code,byte res)
{
    switch(error_code)
    {
      case ERROR_AUTH:
        
        if (res != MFRC522::STATUS_OK) 
        {
            Serial.print("PCD_Authenticate() failed: ");
            Serial.println(mfrc522->GetStatusCodeName(res));
            
            return ERROR_AUTH;
        }
        
        break;
      case ERROR_WRITE:
        
        if (res != MFRC522::STATUS_OK) 
        {
            Serial.print("PCD_Write() failed: ");
            Serial.println(mfrc522->GetStatusCodeName(res));
            
            return ERROR_WRITE;
        }
        
        break;
      case ERROR_READ:
        
        if (res != MFRC522::STATUS_OK) 
        {
            Serial.print("PCD_Read() failed: ");
            Serial.println(mfrc522->GetStatusCodeName(res));
            
            return ERROR_READ;
        }
        
        break;
    }
    
    return 0;
}


