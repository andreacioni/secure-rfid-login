#include"debug.h"

#include<stdbool.h>
#include <stdio.h>
#include <string.h>
#include"config.h"

#ifdef LINUX
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <sys/uio.h>
  #include <fcntl.h>
  #include <termios.h>
  #include <unistd.h>
  
  #define LINUX_UART "/dev/ttyACM0"
#endif

#define READ_LENGHT 129 //ATTENZIONE 128 + 1, 0x00 è l'ultimo carattere che non è parte della stringa!!!

int init();
void wait_for_message(int USB, char *buf);
