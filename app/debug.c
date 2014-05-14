#include"debug.h"
#include "config.h"

void debug(char* string)
{
   #ifdef DEBUG
     printf("%s\n",string);
   #endif
}
