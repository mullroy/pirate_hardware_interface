#define _SERIALPORT_CPP_
/*--[ INCLUDE FILES ]--------------------------------------------------------*/
#include "SerialPort.h"
#include "SerialPort_lin_i.h"
#include "stdio.h"

/*--[ FUNCTION ]***************************************************************
*
* FUNCTION NAME : setBaudRate()
*
*
* DESCRIPTION : Set the baud rate
*
* INPUT PARAMETERS : eBaudRate - Requested baud rate
*                    pPosix_CommConfig - configuration structure
*
* RETURN PARAMETERS : int8_t 0 - Success
*
* Note :
*
*
******************************************************************************/
int8_t setBaudRate (BaudRateType_e eBaudRate,
                    struct termios *pPosix_CommConfig)
{
  int8_t cReturnCode;
  cReturnCode = 0;
  switch (eBaudRate) {
    case (BAUD9600):
      pPosix_CommConfig->c_cflag &= (~CBAUD);
      pPosix_CommConfig->c_cflag |= B9600;
      break;
    case (BAUD38400):
      pPosix_CommConfig->c_cflag &= (~CBAUD);
      pPosix_CommConfig->c_cflag |= B38400;
      break;
    case (BAUD57600):
      pPosix_CommConfig->c_cflag &= (~CBAUD);
      pPosix_CommConfig->c_cflag |= B57600;
      break;
    case (BAUD115200):
      pPosix_CommConfig->c_cflag &= (~CBAUD);
      pPosix_CommConfig->c_cflag |= B115200;
      break;
    case (BAUD230400):
      pPosix_CommConfig->c_cflag &= (~CBAUD);
      pPosix_CommConfig->c_cflag |= B230400;
      break;            
    case (BAUD460800):
      pPosix_CommConfig->c_cflag &= (~CBAUD);
      pPosix_CommConfig->c_cflag |= B460800;
      break;      
    case (BAUD921600):
      pPosix_CommConfig->c_cflag &= (~CBAUD);
      pPosix_CommConfig->c_cflag |= B921600;
      break;
    default:
      printf("Unsupported baudrate\n");
      exit(-1);
  }
  return cReturnCode;
}

/*--[ FUNCTION ]***************************************************************
*
* FUNCTION NAME : setDataBits()
*
*
* DESCRIPTION : Set the data size
*
* INPUT PARAMETERS : eDataBits - Number of data bits
*                    eStopBits - Number of stop bits
*                    pPosix_CommConfig - configuration structure
*
* RETURN PARAMETERS : int8_t  0 - Success
*                            -1 - Unknown data size
*                            -2 - Illegal stop bit size,for the given data size
*
* Note :
*
*
******************************************************************************/
int8_t setDataBits (DataBitsType_e eDataBits, StopBitsType_e eStopBits,
                    struct termios * pPosix_CommConfig)
{
  int8_t cReturnCode;

  cReturnCode = 0;
  switch (eDataBits) {
    case (DATA_5):
      if (eStopBits == STOP_2) {
        return -2;
      }
      else {
        pPosix_CommConfig->c_cflag &= (~CSIZE);
        pPosix_CommConfig->c_cflag |= CS5;
      }
      break;
    case (DATA_6):
      if (eStopBits == STOP_1_5) {
        return -2;
      }
      else {
        pPosix_CommConfig->c_cflag &= (~CSIZE);
        pPosix_CommConfig->c_cflag |= CS6;
      }
      break;
    case (DATA_7):
      if (eStopBits == STOP_1_5) {
        return -2;
      }
      else {
        pPosix_CommConfig->c_cflag &= (~CSIZE);
        pPosix_CommConfig->c_cflag |= CS7;
      }
      break;
    case (DATA_8):
      if (eStopBits == STOP_1_5) {
        return -2;
      }
      else {
        pPosix_CommConfig->c_cflag &= (~CSIZE);
        pPosix_CommConfig->c_cflag |= CS8;
      }
      break;
    default:
      cReturnCode = -1;
  }
  return cReturnCode;
}

/*--[ FUNCTION ]***************************************************************
*
* FUNCTION NAME : setStopBits()
*
*
* DESCRIPTION : Set the number of stop bits
*
* INPUT PARAMETERS : eStopBits - Number of stop bits
*                    pPosix_CommConfig - configuration structure
*
* RETURN PARAMETERS : int8_t  0 - Success
*                            -1 - Unknown stop bit size
*                            -2 - Illegal stop bit size
*
* Note :
*
*
******************************************************************************/
int8_t setStopBits (StopBitsType_e eStopBits,
                    struct termios * pPosix_CommConfig)
{
  int8_t cReturnCode;

  cReturnCode = 0;
  switch (eStopBits) {
    case (STOP_1):
      pPosix_CommConfig->c_cflag &= (~CSTOPB);
      break;
    case (STOP_1_5):
      cReturnCode = -2;
      break;
    case (STOP_2):
      pPosix_CommConfig->c_cflag |= CSTOPB;
      break;
    default:
      cReturnCode = -1;
  }
  return cReturnCode;
}

/*--[ FUNCTION ]***************************************************************
*
* FUNCTION NAME : setParity()
*
*
* DESCRIPTION : Set the parity bits
*
* INPUT PARAMETERS : eParity - Requested parity
*                    pPosix_CommConfig - configuration structure
*
* RETURN PARAMETERS : int8_t  0 - Success
*                            -1 - Unknown parity type
*
* Note :
*
*
******************************************************************************/
int8_t setParity (ParityType_e eParity, struct termios * pPosix_CommConfig)
{
  int8_t cReturnCode;

  cReturnCode = 0;
  switch (eParity) {
    case (PAR_NONE):
      pPosix_CommConfig->c_cflag &= (~PARENB);
      break;
    case (PAR_EVEN):
      pPosix_CommConfig->c_cflag &= (~PARODD);
      pPosix_CommConfig->c_cflag |= PARENB;
      break;
    case (PAR_ODD):
      pPosix_CommConfig->c_cflag |= (PARENB | PARODD);
      break;
    default:
      cReturnCode = -1;
  }
  return cReturnCode;
}

/*--[ FUNCTION ]***************************************************************
*
* FUNCTION NAME : setFlowControl()
*
*
* DESCRIPTION : Set the hardware flow control
*
* INPUT PARAMETERS : eFlow - Requested flow control
*                    pPosix_CommConfig - configuration structure
*
* RETURN PARAMETERS : int8_t  0 - Success
*                            -1 - Unknown flow type
*
* Note :
*
*
******************************************************************************/
int8_t setFlowControl (FlowType_e eFlow, struct termios * pPosix_CommConfig)
{
  int8_t cReturnCode;

  cReturnCode = 0;
  switch (eFlow) {
    case (FLOW_OFF):
      pPosix_CommConfig->c_cflag &= (~CRTSCTS);
      pPosix_CommConfig->c_iflag &= (~(IXON | IXOFF | IXANY));
      break;
    case (FLOW_XONXOFF):
      pPosix_CommConfig->c_cflag &= (~CRTSCTS);
      pPosix_CommConfig->c_iflag |= (IXON | IXOFF | IXANY);
      break;
    case (FLOW_HARDWARE):
      pPosix_CommConfig->c_cflag |= CRTSCTS;
      pPosix_CommConfig->c_iflag &= (~(IXON | IXOFF | IXANY));
      break;
    default:
      cReturnCode = -1;
  }
  return cReturnCode;
}

/*--[ FUNCTION ]***************************************************************
*
* FUNCTION NAME : SP_OpenPort()
*
*
* DESCRIPTION :This functions open the specific serial port, with the
*             : required baudrate propertiesl
*
* INPUT PARAMETERS : spSerialSettings - Port parameters
*                    pcSerialPortHandle - Handle to the serial port
*
* RETURN PARAMETERS : int8_t  0 : Success
*                            -1 : There are no more ports available
*                            -2 : The port could not be opened
*                            -3 : Internal error: Slot already occupied
*
* Note :
*
*
******************************************************************************/
int8_t SP_OpenPort (SerialSettings_s * spSerialSettings,
                    uint8_t * pcSerialPortHandle)
{
  int rc;
  struct termios Posix_CommConfig;
  pthread_mutexattr_t sMutex_attr;
  int fd;
  if (cSerialArrayCount >= (MAX_PORTS - 1)) {
    return -1;
  }
  if (sSerialArray[cSerialArrayCount].bStatus_PortIsOpen != FALSE) {
    return -3;
  }
  fd = open(spSerialSettings->cPCComPortName, O_RDWR | O_NOCTTY | O_NDELAY );
  if (fd == -1) {
    return -2;
  }
  printf("Opened %s\n",spSerialSettings->cPCComPortName);
  
  tcgetattr (fd, &Posix_CommConfig);
  cfmakeraw (&Posix_CommConfig);
  Posix_CommConfig.c_cflag |= CREAD | CLOCAL;
  Posix_CommConfig.c_lflag &=
    (~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG));
  Posix_CommConfig.c_iflag &=
    (~(INPCK | IGNPAR | PARMRK | ISTRIP | ICRNL | IXANY));
  Posix_CommConfig.c_oflag &= (~OPOST);
  Posix_CommConfig.c_cc[VMIN] = 0;

#ifdef _POSIX_VDISABLE
  const long vdisable = fpathconf (fd, _PC_VDISABLE);
  Posix_CommConfig.c_cc[VINTR] = vdisable;
  Posix_CommConfig.c_cc[VQUIT] = vdisable;
  Posix_CommConfig.c_cc[VSTART] = vdisable;
  Posix_CommConfig.c_cc[VSTOP] = vdisable;
  Posix_CommConfig.c_cc[VSUSP] = vdisable;
#endif
  setBaudRate (spSerialSettings->eBaudRate,
               &Posix_CommConfig);
  setDataBits (spSerialSettings->eDataBits,
               spSerialSettings->eStopBits,
               &Posix_CommConfig);
  setStopBits (spSerialSettings->eStopBits,
               &Posix_CommConfig);
  setParity (spSerialSettings->eParity,
             &Posix_CommConfig);
  setFlowControl (spSerialSettings->eFLowType,
                  &Posix_CommConfig);
  if (spSerialSettings->cTimeout == 0) {
    fcntl (fd, F_SETFL, O_NDELAY);
  }
  else {
    fcntl (fd, F_SETFL, O_SYNC);
  }
  Posix_CommConfig.c_cc[VTIME] =
    spSerialSettings->cTimeout / 100;
  tcsetattr (fd, TCSAFLUSH, &Posix_CommConfig);

  
  memcpy (&sSerialArray[cSerialArrayCount].sPortSettings,
          spSerialSettings,
          sizeof (SerialSettings_s)
         );
  sSerialArray[cSerialArrayCount].hComPort = fd;

  sSerialArray[cSerialArrayCount].bStatus_PortIsOpen = TRUE;
  *pcSerialPortHandle = cSerialArrayCount;
  printf("Copies to local: %s\n",sSerialArray[cSerialArrayCount].sPortSettings.cPCComPortName);
  
  cSerialArrayCount++;
  return 0;
}

/*--[ FUNCTION ]***************************************************************
*
* FUNCTION NAME : SP_ClosePort()
*
*
* DESCRIPTION : This functions closes the open communication port
*
* INPUT PARAMETERS : hComPort - Handle to the opened ComPort.
*
* RETURN PARAMETERS : int8_t  0 : Success
*                            -1 : Illegal input/output parameters (NULL 
*                                 pointers)
*                            -2 : Port is not open.
*                            -3 : Could not terminate the thread in which the 
*                                 serial port lives
*                            -4 : Could not close the connection.
*
* Note :
*
*
******************************************************************************/
int8_t SP_ClosePort (uint8_t * pcSerialPortHandle)
{
  bool_t bReturnCode;
  if ((pcSerialPortHandle == NULL) ||
      (*pcSerialPortHandle >= (MAX_PORTS - 1))) 
  {
    return -1;
  }
  
  if (sSerialArray[*pcSerialPortHandle].bStatus_PortIsOpen == FALSE) 
  {
    return -2;
  }
  
  //sSerialArray[*pcSerialPortHandle].bCloseThread = TRUE;
  //sleep (1);
  bReturnCode = close (sSerialArray[*pcSerialPortHandle].hComPort);
  if (bReturnCode != 0) 
  {
    return -4;
  }
  sSerialArray[*pcSerialPortHandle].bStatus_PortIsOpen = FALSE;  
  *pcSerialPortHandle=-1;
  return 0;
}

/*--[ FUNCTION ]***************************************************************
*
* FUNCTION NAME : SP_IsOpen()
*
*
* DESCRIPTION : This function returns if the port is open or not.
*
* INPUT PARAMETERS : *pcSerialPortHandle   - Handle to the ComPort.
*
* RETURN PARAMETERS : int8_t   1 : Port is open
*                              0 : Port is close
*                             -1 : Illegal input/output parameters (NULL 
*                                  pointers)
*
* Note :
*
*
******************************************************************************/
int8_t SP_IsOpen (uint8_t * pcSerialPortHandle, uint32_t * pwThreadCounter)
{
  bool_t bReturnCode;
  if ((pcSerialPortHandle == NULL) ||
      (*pcSerialPortHandle >= (MAX_PORTS - 1))) {
    return -1;
  }
  //*pwThreadCounter = sSerialArray[*pcSerialPortHandle].wThreadCounter;
  bReturnCode = sSerialArray[*pcSerialPortHandle].bStatus_PortIsOpen;
  return bReturnCode;
}


int16_t SP_Write (uint8_t *pcSerialPortHandle, uint8_t *pcBuffer, int16_t iCount)
{
  #define BLOCKSIZE 100
  int16_t iReturnCode;

  if (*pcSerialPortHandle >= (MAX_PORTS - 1))
  {
    return -1;
  }
  if (sSerialArray[*pcSerialPortHandle].bStatus_PortIsOpen == FALSE) {
    return -2;
  }

  
  uint8_t cBlocks = iCount / BLOCKSIZE;
  uint16_t iRemainder = iCount % BLOCKSIZE;
  uint16_t iPosition = 0;
  uint16_t iTotal = 0;
  
  for (int iI=0;iI<cBlocks;iI++)
  {
    //fprintf(stderr,"Sending from %d: 512 bytes\n",iPosition);
    iReturnCode = write (sSerialArray[*pcSerialPortHandle].hComPort, &pcBuffer[iPosition], BLOCKSIZE);
    iPosition+=BLOCKSIZE;
    iTotal+=iReturnCode;

    //fprintf(stderr," Total=%d\n",iTotal);
    //usleep(6000); // @921600 it takes 5.5ms. Sleep 10 milli's
    usleep( BLOCKSIZE / 11520 * 1000000 * 2);
  }
  if (iRemainder!=0)
  {
    iReturnCode = write (sSerialArray[*pcSerialPortHandle].hComPort, &pcBuffer[iPosition], iRemainder);
    iTotal+=iReturnCode;
    //fprintf(stderr," Total=%d\n", iTotal);
    usleep( iRemainder / 11520 * 1000000 * 2);
  }  

  return iTotal;
}


int16_t SP_Read (uint8_t * pcSerialPortHandle, uint8_t * pcBuffer, int16_t iCount)
{

  int16_t wBytesRead;

  if (*pcSerialPortHandle >= (MAX_PORTS - 1))
  {
    return -1;
  }
  if (sSerialArray[*pcSerialPortHandle].bStatus_PortIsOpen == FALSE) 
  {
    return -2;
  }

  wBytesRead = read (sSerialArray[*pcSerialPortHandle].hComPort, pcBuffer, iCount);

  return wBytesRead;
}


