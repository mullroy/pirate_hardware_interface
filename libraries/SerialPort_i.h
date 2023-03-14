
#ifndef _SERIALPORT_I_H_
#define _SERIALPORT_I_H_

#ifndef _SERIALPORT_CPP_
#error This file is private to SerialPort.c
#endif  /*  */
  
#ifdef __cplusplus
extern "C" 
{
  
#endif  /*  */
  

#include "ring_buf.h"
#include <signal.h>
#include <pthread.h>            
  

#define MAX_PORTS 10
#define RINGBUF_SIZE 6000
   typedef struct 
  {
    int hComPort;
      char cPCComPortName[20]; 
      PortSettings_s sPortSettings;
       bool_t bStatus_PortIsOpen;
      uint8_t bStatus_comms_error;
       Buffer_s RingBuf_Rx;
      Buffer_s RingBuf_Tx;
      uint8_t cRingBuf_Rx_Array[RINGBUF_SIZE];
      uint8_t cRingBuf_Tx_Array[RINGBUF_SIZE];
       pthread_mutex_t Mutex_TX;
      pthread_mutex_t Mutex_RX;
      pthread_t hThread;
      uint32_t wThreadCounter;
      bool_t bCloseThread;     
  } serial_T;
    
#define THREAD_SLEEP_DELAY 75   
    serial_T sSerialArray[MAX_PORTS];
    int8_t cSerialArrayCount = 0;
      void *my_thread_func (void *arg);
   int8_t setBaudRate (BaudRateType_e baudRate,
                        struct termios *pPosix_CommConfig);
   int8_t setDataBits (DataBitsType_e eDataBits, StopBitsType_e eStopBits,
                        struct termios *pPosix_CommConfig);
   int8_t setStopBits (StopBitsType_e eStopBits,
                        struct termios *pPosix_CommConfig);
   int8_t setParity (ParityType_e eParity,
                      struct termios *pPosix_CommConfig);
   
#ifdef __cplusplus
} 
#endif  /*  */
 
#endif  /*  */
 
