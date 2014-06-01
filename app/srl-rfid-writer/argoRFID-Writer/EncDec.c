#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<string.h>
#include<time.h>

#include "aes.h"

#include "crypt.h"

#define KEY_LENGHT 128
#define IV_LENGHT 16
#define MAX_MESSAGE_SIZE 256
#define MAX_BLOCK_SIZE 64
#define NUM_OF_BLOCK 4

BOOL enc_cfb(size_t len, unsigned char * in, unsigned char * out);
BOOL dec_cfb(size_t len, unsigned char * in, unsigned char * out);


BOOL encrypt_data(char * username,char * password ,unsigned char * enc_username, unsigned char * enc_password)
{

	if(enc_cfb(MAX_BLOCK_SIZE,(unsigned char *)username,enc_username) == FALSE) return FALSE;
	if(enc_cfb(MAX_BLOCK_SIZE,(unsigned char *) password,enc_password) == FALSE) return FALSE;

	return TRUE;	
}

BOOL decrypt_data(unsigned char * enc_username, unsigned char * enc_password,char * username,char * password)
{
	if(dec_cfb(MAX_BLOCK_SIZE,enc_username,(unsigned char*) username) == FALSE) return FALSE;
	if(dec_cfb(MAX_BLOCK_SIZE,enc_password,(unsigned char*) password) == FALSE) return FALSE;

	return TRUE;
}

BOOL enc_cfb(size_t len, unsigned char * in, unsigned char * out)
{
	aes_context ctx;
	unsigned char iv[IV_LENGHT];

	memcpy(iv,random_iv,IV_LENGHT);

	if(aes_setkey_enc(&ctx,static_key,KEY_LENGHT) != 0)
	{
		return FALSE;
	}

	if(aes_crypt_cfb8(&ctx,AES_ENCRYPT,len,iv,in,out) == 0)
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

BOOL dec_cfb(size_t len, unsigned char * in, unsigned char * out)
{
	aes_context ctx;
	unsigned char iv[IV_LENGHT];

	memcpy(iv,random_iv,IV_LENGHT);

	if(aes_setkey_enc(&ctx,static_key,KEY_LENGHT) != 0)
	{
		return FALSE;
	}

	if(aes_crypt_cfb8(&ctx,AES_DECRYPT,len,iv,in,out) == 0)
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}