
#ifndef _SERIALPORT_I_H_
#define _SERIALPORT_I_H_

#ifndef _SERIALPORT_CPP_
#error This file is private to SerialPort.c
#endif  /* 
  
#ifdef __cplusplus
extern "C" 
{
  
#endif  /* 
  

#include "ring_buf.h"
#include <signal.h>
#include <pthread.h>            
  

#define MAX_PORTS 10
#define RINGBUF_SIZE 6000
  
  {
    
     
     
     
     
     
     
     
     
     
     
     
     
     
  } serial_T;
   
#define THREAD_SLEEP_DELAY 75   
   
   
   
   
                        struct termios *pPosix_CommConfig);
   
                        struct termios *pPosix_CommConfig);
   
                        struct termios *pPosix_CommConfig);
   
                      struct termios *pPosix_CommConfig);
   
#ifdef __cplusplus
} 
#endif  /* 
 
#endif  /* 
 