#include <mysql.h>				//This include and other releted are present in VC install directory

#include "DatabaseManager.h"

BOOL insert_serial(unsigned char * four_serial_byte)
{
	MYSQL * con = mysql_init(NULL);

	 if (con == NULL) 
		return FALSE;

	 if(mysql_real_connect(con,"localhost","root","","ArgoRFID", 0, NULL, 0))
	 {
		 mysql_close(con);

		 return FALSE;
	 }

	 if(mysql_query(con,strcat("INSERT INTO allowcard (serialnumber) value ", ))
	 {
		 mysql_close(con);

		 return FALSE;
	 }	 

}
BOOL is_serial_present(unsigned char * four_serial_byte)
{

}
