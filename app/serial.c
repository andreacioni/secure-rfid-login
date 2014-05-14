#include"serial.h"

void read_serial(int USB, char *buf);
bool close_UART(int USB);

bool setting_parameters(int USB);
int send_On_serial(int USB);
int send_Off_serial(int USB);

int init()
{	
  debug("Opening serial port...");
  int USB = open(LINUX_UART, O_RDWR| O_NOCTTY);
  
  if(USB>0)
  {
    debug("Opened!");
    
    debug("Setting parameters...");
    if(setting_parameters(USB) == false)
    {
    	printf("Si è verificato un errore, contattare lo sviluppatore.\n Codice di errore 1.\n");
    	return -1;
    }
    else
    {
      debug("Settings applied correctly!");
      
      if(send_On_serial(USB) < 0)
      	return -1;            
    }
    
    return USB;
    }
  else
  {
    debug("Error on opening serial port");
    return -1;
  }
}

void wait_for_message(int USB, char *buf)
{
  read_serial(USB,buf);
}

bool setting_parameters(int USB)
{
  struct termios tty;
  struct termios tty_old;
  memset (&tty, 0, sizeof tty);

/* Error Handling */
if ( tcgetattr ( USB, &tty ) != 0 )
{
  printf("Error!");
  return false;
}

/* Save old tty parameters */
tty_old = tty;

/* Set Baud Rate */
cfsetospeed (&tty, B9600);
cfsetispeed (&tty, B9600);

/* Setting other Port Stuff */
tty.c_cflag     	&=  ~PARENB;        	// Make 8n1
tty.c_cflag     	&=  ~CSTOPB;
tty.c_cflag     	&=  ~CSIZE;
tty.c_cflag     	|=  CS8;

tty.c_cflag     	&=  ~CRTSCTS;       	// no flow control
tty.c_cc[VMIN]      	=   READ_LENGHT;                  // read doesn't block
tty.c_cc[VTIME]     	=   0.5;                  // 0.5 seconds read timeout
tty.c_cflag     	|=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

tty.c_iflag &= ~(IXON | IXOFF | IXANY);
tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
tty.c_oflag &= ~OPOST;

/* Make raw */
//cfmakeraw(&tty);

if ( tcsetattr ( USB, TCSANOW, &tty ) != 0)
{
  printf("Error!");
  return false;
}
 usleep(1000*1000*5);
 
 /* Flush Port, then applies attributes */
 tcflush( USB, TCIFLUSH );
 return true;

}

void read_serial(int USB, char *buf)
{
  debug("Start reading...");
  
  int ret = read(USB,buf,READ_LENGHT);
  
  printf("Read %i bytes\n",ret);
  
  if(ret<0)
  {
    debug("Error! Cannot read!");
    close_UART(USB);
    return;  
  }
  
  debug("Everything read successful!");
  
  close_UART(USB);
    
}

int send_On_serial(int USB)
{
  debug("Try to send the standard message...");
  
  int ret = write(USB,"INIT\n",5);
  
  if(ret<0)
   debug("Cannot send On message over serial interface!");
  else
   debug("OK message sent!");
   
   return ret;
   
}

int send_Off_serial(int USB)
{
  debug("Try to send the standard message...");
  
  int ret = write(USB,"STOP\n",5);
  
  if(ret<0)
   debug("Cannot send On message over serial interface!");
  else
   debug("OK message sent!");
   
   return ret;
   
}

bool close_UART(int USB)
{
  debug("Closing the serial interface...");
  
  int ret = close(USB);
  
  if(ret == 0)
  {
    debug("Closed!");
    return true;
  }
  else
  {
    debug("Cannot close that port!");
    return false;
  }
}
