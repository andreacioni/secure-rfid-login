#include<stdio.h>
#include <mysql.h>				//This include and other releted are present in VC install directory

#include "DatabaseManager.h"

BOOL insert_serial(unsigned char * four_serial_byte)
{
	MYSQL * con = mysql_init(NULL);

	char query[100] = {0x00};
	sprintf(query,"INSERT INTO allowcard (serialnumber) VALUES ('%x%x%x%x')",four_serial_byte[0],four_serial_byte[1],four_serial_byte[2],four_serial_byte[3]);

	 if (con == NULL) 
		return FALSE;

	 if(mysql_real_connect(con,"localhost","root","","argorfid", 0, NULL, 0) == NULL)
	 {
		 mysql_close(con);
		 return FALSE;
	 }

	 if(mysql_query(con,query))
	 {
		 mysql_close(con);
		 return FALSE;
	 }	 

	 mysql_close(con);

}
int is_serial_present(unsigned char * four_serial_byte)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL * con = mysql_init(NULL);

	char fetched_serial[9] = {0x00};
	char string_serial[9]  = {0x00};
	char query[100] = {0x00};
	
	sprintf(query,"SELECT serialnumber FROM allowcard WHERE serialnumber='%x%x%x%x'",four_serial_byte[0],four_serial_byte[1],four_serial_byte[2],four_serial_byte[3]);

	 if (con == NULL) 
		return -1;

	 if(mysql_real_connect(con,"localhost","root","","argorfid", 0, NULL, 0) == NULL)
	 {
		 mysql_close(con);
		 return -1;
	 }

	 if(mysql_query(con,query) != 0)
	 {
		 mysql_close(con);
		 return -1;
	 }	 

	 result = mysql_store_result(con);

	 if(result == NULL)
	 {
		 mysql_close(con);
		 return -1;
	 }

	 

	 if((row = mysql_fetch_row(result)) != NULL)
	 {
		 sprintf(string_serial,"%x%x%x%x", four_serial_byte[0],four_serial_byte[1],four_serial_byte[2],four_serial_byte[3]);
		 sprintf(fetched_serial,"%s", row[0]);

		 if(strcmp(string_serial,fetched_serial) == 0)
		 {
			 return 1;
		 }
	 }

	 mysql_close(con);

	 return 0;

}
