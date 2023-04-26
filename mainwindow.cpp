#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <string.h>
#include <stdlib.h>
#include <QGraphicsDropShadowEffect>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/time.h>


#include <QFileDialog> // Executing the QFileDialog causes a sig32
                       // to trigger in gdb when the application exits.
                       // Suppress the event by adding "handle SIG32 pass nostop noprint"
                       // to Tools->Options->Debugger, Locals&Expressions tab,
                       // Debugger Helper Customization dialog

// SSL development libs: apt-get install libssl-dev
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

//For exec() function:
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "libraries/md5sum.h"



// User session key in comms
// heartbeat in the unit to restart the app if it hangs
// If comms ping/pong stops, timeout the unit
// Apply config - progress bar?
// Communication lost message on unit: Change to 'ready' after 20 seconds
// Increase I²C clock to the LCD display - Seems to be fast now with DTB:i2c3 at 400k
// Unit: "ping/pong, but handshake not established", Qt carries on...
// "Invalid session key" -- Unit: Display longer
//                          Qt:   Display error on the display
// Restore mnemonic -- When all the words are entered & checks out:
//                     QT: Give a message that the mnemonic checks out
//                     Unit: Print a message that it was accepted
//                   -- All 24 entered & incorrect Message that says there is an error
// Startup: Get serial or ID from fingerprint unit, to know that its alive
// Capture fingerprint: When 'capture is selected' change prompt to say it's in progress
// Verify fingerprint: When 'Verify' is clicked, blank the status prompt/ say we're verifying...
// Verify completed: Update screen to say all passed...
// Verify password: Change 'Back' to 'previous'
//                : Back button doesn't work
//                : Show the checksum nr to the user
// MSGID_APPLY_CONFIG : Make sure we can write to the file at startup to /opt/arrr
//                    : Change 'Start' to 'in progress', show a progress bar with countdown
//                    : Once completed, give feedback to the user & close the connection
//                    : Better to run the progress in the background & give comms&gui feedback
//                    : Unit takes very long to calculate the MD5... Maybe need to 'sync' to get true duration of the activities
//                    : Should only accept ping/pong & logout after config is done?
//                    : Change 'Ready to receive data' to 'Ready for connection'?
//                    : After configuration & future logins, verify the config files again or keep passphrase in RAM?
// Login: Hide checksum after 20 seconds; Prompt user to verify the checksum
// ping/pong, but handshake not established -- GUI most log out.
// Valid date required on the unit?
// Have to sandbox the user account?
// Print: Did the calculated md5 checksum pass?
// Print: Actual duration of calculating the md5sum
// Randomise legend more
#define CONNECTION_TIMEOUT 5 //Connection timeout (seconds)
#define MAX_PW_LENGTH 50
#define MAX_PORT_LENGTH 50
#define ASYNC_RPC_OPERATION_DEFAULT_MINERS_FEE 10000


void MainWindow::Exception()
{
  QDebug deb = qDebug();
  qDebug( "An exception occurred" );
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  try
  {
    //printf("MainWindow() Start application\n");
    ui->setupUi(this);
    /*
    //specify a new font.
    QFont newFont("Noto Sans", 12, QFont::Bold, false);
    //set font of application
    QApplication::setFont(newFont);    
    */
    //this->centralWidget()->setStyleSheet(
    //         "background-image:url(':/images/treasure.png'); background-position: center;" );
    _pixmapBg.load(":/images/piratemap.png");
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    this->setFixedSize(QSize(800, 525));


    // Connect button signal to appropriate slot
    connect(ui->btConnect,      &QPushButton::released, this, &MainWindow::btConnect_clicked);
    connect(ui->btDisconnect,   &QPushButton::released, this, &MainWindow::btDisconnect_clicked);
    connect(ui->btCancel,       &QPushButton::released, this, &MainWindow::btCancel_clicked);

    connect(ui->btDisconnect_2, &QPushButton::released, this, &MainWindow::btDisconnect_clicked);
    connect(ui->btDisconnect_3, &QPushButton::released, this, &MainWindow::btDisconnect_clicked);
    connect(ui->btDisconnect_4, &QPushButton::released, this, &MainWindow::btDisconnect_clicked);

    //Login
    connect(ui->btLogin_Up,         &QPushButton::released, this, &MainWindow::btPassword_Up_clicked);
    connect(ui->btLogin_Down,       &QPushButton::released, this, &MainWindow::btPassword_Down_clicked);
    connect(ui->btLogin_Left,       &QPushButton::released, this, &MainWindow::btPassword_Left_clicked);
    connect(ui->btLogin_Right ,     &QPushButton::released, this, &MainWindow::btPassword_Right_clicked);
    connect(ui->btLogin_Select,     &QPushButton::released, this, &MainWindow::btPassword_Select_clicked);
    connect(ui->btLogin_Reset,      &QPushButton::released, this, &MainWindow::btPassword_Reset_clicked);
    connect(ui->btLogin_Disconnect, &QPushButton::released, this, &MainWindow::btDisconnect_clicked);



    connect(ui->btSetup,                    &QPushButton::released, this, &MainWindow::btSetup_clicked);
    connect(ui->btSetupMnemonic2,           &QPushButton::released, this, &MainWindow::btSetupMnemonic2_clicked);

    connect(ui->btSetupMnemonic3,          &QPushButton::released, this, &MainWindow::btSetupMnemonic3_clicked);
    connect(ui->btSetupMnemonic3_Previous, &QPushButton::released, this, &MainWindow::btSetupMnemonic3_Previous_clicked);
    connect(ui->btSetupMnemonic3_Next,     &QPushButton::released, this, &MainWindow::btSetupMnemonic3_Next_clicked);
    connect(ui->btSetupMnemonic3_Cancel,   &QPushButton::released, this, &MainWindow::btDisconnect_clicked);

    connect(ui->btSetupMnemonic4_Cancel,   &QPushButton::released, this, &MainWindow::btDisconnect_clicked);
    connect(ui->btSetupMnemonic4_Previous, &QPushButton::released, this, &MainWindow::btSetupMnemonic4_Previous_clicked);
    connect(ui->btSetupMnemonic4_Next,     &QPushButton::released, this, &MainWindow::btSetupMnemonic4_Next_clicked);
    connect(ui->btSetupMnemonic4_Continue, &QPushButton::released, this, &MainWindow::btSetupMnemonic4_Continue_clicked);

    connect(ui->btRestoreMnemonic_Next,    &QPushButton::released, this, &MainWindow::btRestoreMnemonic_Next_clicked);
    connect(ui->btRestoreMnemonic_Previous,&QPushButton::released, this, &MainWindow::btRestoreMnemonic_Previous_clicked);
    connect(ui->btRestoreMnemonic_Clear,   &QPushButton::released, this, &MainWindow::btRestoreMnemonic_Clear_clicked);
    connect(ui->btRestoreMnemonic_Submit,  &QPushButton::released, this, &MainWindow::btRestoreMnemonic_Submit_clicked);
    connect(ui->btRestoreMnemonic_Cancel,  &QPushButton::released, this, &MainWindow::btDisconnect_clicked);
    connect(ui->btRestoreMnemonic_Continue, &QPushButton::released, this, &MainWindow::btSetupMnemonic4_Continue_clicked); // Carry on with normal registration


    connect(ui->btSetupPassword_Cancel,      &QPushButton::released, this, &MainWindow::btDisconnect_clicked);
    connect(ui->btSetupPassword_Generate,    &QPushButton::released, this, &MainWindow::btSetupPassword_Generate_clicked);
    connect(ui->btSetupPassword_Continue,    &QPushButton::released, this, &MainWindow::btSetupPassword_Continue_clicked);


    connect(ui->btVerifyPassword_Up,         &QPushButton::released, this, &MainWindow::btPassword_Up_clicked);
    connect(ui->btVerifyPassword_Down,       &QPushButton::released, this, &MainWindow::btPassword_Down_clicked);
    connect(ui->btVerifyPassword_Left,       &QPushButton::released, this, &MainWindow::btPassword_Left_clicked);
    connect(ui->btVerifyPassword_Right ,     &QPushButton::released, this, &MainWindow::btPassword_Right_clicked);
    connect(ui->btVerifyPassword_Select,     &QPushButton::released, this, &MainWindow::btPassword_Select_clicked);
    connect(ui->btVerifyPassword_Reset,      &QPushButton::released, this, &MainWindow::btPassword_Reset_clicked);
    connect(ui->btVerifyPassword_Disconnect, &QPushButton::released, this, &MainWindow::btDisconnect_clicked);
    connect(ui->btVerifyPassword_Back,       &QPushButton::released, this, &MainWindow::btVerifyPassword_Back_clicked);
    connect(ui->btVerifyPassword_Continue,   &QPushButton::released, this, &MainWindow::btVerifyPassword_Continue_clicked);

    connect(ui->btApplyConfig_Start,         &QPushButton::released, this, &MainWindow::btApplyConfig_Start_clicked);
    connect(ui->btApplyConfig_Disconnect,    &QPushButton::released, this, &MainWindow::btDisconnect_clicked);

    //connect(ui->actionSign_transactions, SIGNAL(triggered()), this, SLOT(Switchto_pageSign));

    connect(ui->btSign_Sign,            &QPushButton::released, this, &MainWindow::btSign_Sign_clicked);
    connect(ui->btSign_Back,            &QPushButton::released, this, &MainWindow::btSign_Back_clicked);
    connect(ui->btSign_Next,            &QPushButton::released, this, &MainWindow::btSign_Next_clicked);
    connect(ui->btSign_OTP,             &QPushButton::released, this, &MainWindow::btSign_OTP_clicked);
    connect(ui->btSign_Clear,           &QPushButton::released, this, &MainWindow::btSign_Clear_clicked);
    connect(ui->btSign_RetrieveAddress, &QPushButton::released, this, &MainWindow::Switchto_pageAddress);
    connect(ui->btSign_Disconnect,      &QPushButton::released, this, &MainWindow::btDisconnect_clicked);

    connect(ui->btRetrieveAddress,            &QPushButton::released, this, &MainWindow::btRetrieveAddress_clicked);
    connect(ui->btRetrieveAddress_OTP,        &QPushButton::released, this, &MainWindow::btRetrieveAddressOTP_clicked);
    connect(ui->btRetrieveAddress_Sign,       &QPushButton::released, this, &MainWindow::Switchto_pageSign);
    connect(ui->btRetrieveAddress_Disconnect, &QPushButton::released, this, &MainWindow::btDisconnect_clicked);

    connect(ui->btDownload_Browse, &QPushButton::released, this, &MainWindow::btDownload_Browse_clicked);
    connect(ui->btDownload_Start,  &QPushButton::released, this, &MainWindow::btDownload_Start_clicked);

    timerConnect = new QTimer(this);
    connect ( timerConnect, SIGNAL(timeout()),
              this,         SLOT(timerConnect_timeout())
            );

    timerSendPeriodicMsgs = new QTimer(this);
    connect ( timerSendPeriodicMsgs, SIGNAL(timeout()),
              this,                  SLOT(timerSendPeriodicMsgs_timeout())
            );

    timerSignProgress = new QTimer(this);
    connect ( timerSignProgress, SIGNAL(timeout()),
              this,              SLOT(timerSignProgress_timeout())
            );

    timerUpgradeProgress = new QTimer(this);
    connect ( timerUpgradeProgress, SIGNAL(timeout()),
              this,                 SLOT(timerUpgradeProgress_timeout())
            );


    oMsgFrame = new MsgFrame_Widget(this);
    connect ( oMsgFrame, SIGNAL(signal_FrameDetected(uint8_t, uint8_t*, uint16_t)),
              this,      SLOT(message_framedetected(uint8_t, uint8_t*, uint16_t))
            );


    QCoreApplication::instance()->installEventFilter(this);

    bLoggedIn=false;
    bHandshake=false;
    bSessionKey=false;
    iPeriodicCounter=0;
    QString   sPassword="";

    cUpgradeStatus=0;
    cEmojiPosition=0;

    cMessageQueued=0;
    ui->pbSign->setVisible(false);
    ui->teSign_Output->setVisible(true);


    ui->leConnect->clear();
    ui->leConnect->setFocus();
    ui->btConnect->setText("Connect");
    ui->lbConnect->setText("Port:");
    ui->btDisconnect->hide();

    ui->statusbar->showMessage(" ");


    //Set GUI to the login page
    CloseSerialPort();



    //Make the widgets transparent
    //ui->stackedWidget->setStyleSheet("background-color:rgba(255, 255, 255, 0);");

    QGraphicsDropShadowEffect *eff = new QGraphicsDropShadowEffect(this);
    eff->setOffset(0, 0);
    eff->setBlurRadius(5.0);
    eff->setColor(Qt::red);
    ui->centralwidget->setGraphicsEffect(eff);
    
  }
  catch(...)
  {
    Exception();
  }
}

MainWindow::~MainWindow()
{
  try
  {
    delete ui;
  }
  catch(...)
  {
    Exception();
  }
}

void MainWindow::paintEvent(QPaintEvent *)
{
  try
  {
    //QPainter myPaint = new QPainter(this);
    QPainter painter(this);
    int widWidth =  ui->centralwidget->width();
    int widHeight = ui->centralwidget->height();
    _pixmapBg = _pixmapBg.scaled(widWidth, widHeight, Qt::KeepAspectRatioByExpanding);
    painter.drawPixmap(0, 0, _pixmapBg);
  }
  catch(...)
  {   
    Exception();
  }
}

QString MainWindow::exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }

    QString sResult = QString::fromStdString(result);

    return sResult;
}


bool MainWindow::eventFilter(QObject *, QEvent *Event)
{
  if (Event->type() == QEvent::KeyPress)
  {
    QWidget *oWidget = ui->stackedWidget->currentWidget();
    if (
       (oWidget->objectName()!="pageVerifyPassword")&&
       (oWidget->objectName()!="pageLogin")
       )
    {
      return false;
    }


    QKeyEvent *KeyEvent = (QKeyEvent*)Event;

    switch(KeyEvent->key())
    {
      case Qt::Key_8:
      case 0x1000013: //Up arrow key
        btPassword_Up_clicked();
        return true;

      case Qt::Key_2:
      case 0x1000015: //Down arrow key
        btPassword_Down_clicked();
        return true;

      case Qt::Key_4:
      case 0x1000012: //Left arrow key
        btPassword_Left_clicked();
        return true;

      case Qt::Key_6:
        case 0x1000014: //Right arrow key
        btPassword_Right_clicked();
        return true;

      case Qt::Key_5:
      case 0x100000B:   //Select
        btPassword_Select_clicked();
        return true;

      default:
        ui->statusbar->showMessage("Unknown key pressed");
        break;
    }
  }
  return false;
}

void MainWindow::filetx_tick(int iPacketNr, int iPacketsTotal, int iRetries)
{
  QString sTmp;
  sTmp = sTmp.asprintf("%d / %d packets ; %d retries",iPacketNr, iPacketsTotal, iRetries);
  ui->lbDownload_ResultCommsThread->setText(sTmp);
}
void MainWindow::filetx_complete(uint8_t cStatus)
{
  QString sTmp="";
  switch (cStatus)
  {
    case 0:
      sTmp = "Transfer completed successfully";
      break;
    case 13:
      sTmp = "Checksum failed\n";
      break;
    default:
      sTmp = "Transfer aborted";
      break;
  }
  ui->lbDownload_ResultCommsThread->setText(sTmp);
}

void MainWindow::btConnect_clicked()
{
  try
  {
    int8_t cReturnCode=10;

    iPeriodicCounter=0;
    bConnectionCancel = FALSE;
    memset(&caSessionID[0],0,10);

    if (bHandshake==false)
    {
      //Connection type: Standard - Standard wallet functions
      //                 Upgrade  - Requested upgrade
      if (ui->rbConnect_Standard->isChecked()==true)
      {
        bConnectionTypeStandard=true;
      }
      else
      {
        //Upgrade
        bConnectionTypeStandard=false;
      }

      //Start the communication:
      char_t caSerialPort[MAX_PORT_LENGTH+1];
      QString qsSerialPort = ui->leConnect->text();
      if (qsSerialPort.length()==0)
      {
        ui->statusbar->showMessage("Please enter a communication port");
        return;
      }
      if (qsSerialPort.length()>MAX_PORT_LENGTH)
      {
        ui->statusbar->showMessage("Communication port string too long");
        return;
      }

      //Move to connect screen
      ui->statusbar->showMessage(" ");
      ui->barConnectProgress->setValue (0);
      ui->barConnectProgress->setMaximum (CONNECTION_TIMEOUT);
      ui->stackedWidget->setCurrentWidget(ui->pageConnect);

      memset (&caSerialPort[0],0,MAX_PORT_LENGTH+1);
      snprintf(&caSerialPort[0], MAX_PORT_LENGTH, "%s", qsSerialPort.toLocal8Bit().data() );
      cReturnCode = oMsgFrame->OpenSerialConnection(&caSerialPort[0], strlen(&caSerialPort[0]));
      if (cReturnCode==0)
      {
        //Serial port opened.
        ui->statusbar->showMessage("Communication port open. Initiating communication with the unit.");
        //Start the login counter @ 2Hz to establish communication with the unit
        wTimerConnectCount=0;
        if (timerConnect->isActive()==FALSE)
        {
          timerConnect->start(1000); //1Hz
        }
      }
      else
      {
        switch (cReturnCode)
        {
          case -1: //Connection already open
            ui->statusbar->showMessage("Connection failed: The serial port is already open.");
            break;
          case -2: //Could not open the port
            ui->statusbar->showMessage("Connection failed: Could not open the serial port.");
            ResolveSerialPorts();
            break;
          case -4: //cDeviceName_length too big.
            ui->statusbar->showMessage("Connection failed: The supplied device name is too long.");
            break;
        }
        //Cleanup the GUI
        //  Connection timer not yet started. Therefore clicking 'disconnect' won't clean up the QUI
        ui->stackedWidget->setCurrentWidget(ui->pageWelcomeScreen);

        ui->leConnect->clear();
        ui->btConnect->setText("Connect");
        ui->lbConnect->setText("Port");
        ui->btDisconnect->hide();

        ui->lbConnect_1->show();
        ui->textConnect->show();
        ui->rbConnect_Standard->show();
        ui->rbConnect_Upgrade->show();
      }
    }
    else
    {
      uint8_t caData[4];
/*
      if (cMessageQueued>0)
      {
        //This should not happen...
        ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
        return;
      }
      cMessageQueued=3; //X seconds for a reply
*/
      QString sSessionKey = ui->leConnect->text();
      if (sSessionKey.length()!=4)
      {
        ui->statusbar->showMessage("Please enter the 4 numbers for the session key");
        return;
      }
      QByteArray qBA = sSessionKey.toLocal8Bit();

      char_t cChar = qBA.at(0);
      caData[0]=(uint8_t)atoi( &cChar );

      cChar = qBA.at(1);
      caData[1]=(uint8_t)atoi( &cChar );

      cChar = qBA.at(2);
      caData[2]=(uint8_t)atoi( &cChar );

      cChar = qBA.at(3);
      caData[3]=(uint8_t)atoi( &cChar );

      cReturnCode = oMsgFrame->Pack(MSGID_SESSIONKEY, (uint8_t *)&caData[0], 4);
      if (cReturnCode!=0)
      {
        ui->statusbar->showMessage("Could not send the session key to the hardware unit");
      }
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

int8_t MainWindow::CloseSerialPort()
{
  try
  {
//    uint8_t cByte;
//    uint8_t caData[2];


    //Stop the timers:
    timerConnect->stop ();
    timerSendPeriodicMsgs->stop();

    //Clear status variables
    bConnectionCancel = false;
    bHandshake=false;
    bSessionKey=false;
    cMessageQueued=0;
    cUpgradeStatus=0; //Idle


    //Reset the GUI
    cEmojiPosition=0;

    ui->stackedWidget->setCurrentWidget(ui->pageWelcomeScreen);


    ui->leConnect->clear();
    ui->btConnect->setText("Connect");
    ui->lbConnect->setText("Port");
    ui->btDisconnect->hide();

    ui->lbConnect_1->show();
    ui->textConnect->show();
    ui->rbConnect_Standard->show();
    ui->rbConnect_Upgrade->show();

    oMsgFrame->CloseSerialConnection();

    ResolveSerialPorts();

  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
    return -1;
  }
  return 0;
}

void MainWindow::ResolveSerialPorts()
{
  sUpgradePort="(Could not auto detect)";
#ifdef __CYGWIN__
  //Get the serial ports
  QString sResult = exec("ls /dev/ttyS* 2>/dev/null  | grep -v '/dev/ttyS0' | awk '{ print $1 }' | sed -e 's/\\/dev\\/ttyS//'");
  if (sResult.length()==0)
  {
    ui->statusbar->showMessage("Could not detect the serial ports of the unit.");
    return;
  }
  QStringList lstPorts = sResult.trimmed().split("\n");

  int iParts=lstPorts.size();
  if (iParts!=2)
  {
    ui->statusbar->showMessage("Too many serial ports found. Could not uniquely identify the serial ports of the wallet.");
    return;
  }

  bool bOK=false;
  int iTest = lstPorts.at(0).toInt(&bOK);
  if (bOK!=TRUE)
  {
    ui->statusbar->showMessage("Could not isolate the serial port number");
    return;
  }
  int iOne = iTest;
  iTest = lstPorts.at(1).toInt(&bOK);
  if (bOK!=TRUE)
  {
    ui->statusbar->showMessage("Could not isolate the serial port number");
    return;
  }
  int iTwo = iTest;

  if (iOne>iTwo)
  {
    iTest=iOne;
    iOne=iTwo;
    iTwo=iTest;
  }

  QString sPort;
  sPort = sPort.asprintf("/dev/ttyS%d",iOne);
  ui->leConnect->setText(sPort);
  sUpgradePort = sPort.asprintf("/dev/ttyS%d",iTwo);
#else
  //Get the serial ports
  QString sResult = exec("find /sys/bus/usb-serial/drivers/cp210x -name 'ttyUSB*' 2>/dev/null | sed -e 's/\\/sys\\/bus\\/usb-serial\\/drivers\\/cp210x\\/ttyUSB//'");
  if (sResult.length()==0)
  {
    ui->statusbar->showMessage("Could not detect the serial ports of the unit.");
    return;
  }
  QStringList lstPorts = sResult.trimmed().split("\n");

  int iParts=lstPorts.size();
  if (iParts!=2)
  {
    ui->statusbar->showMessage("Too many serial ports found. Could not uniquely identify the serial ports of the wallet.");
    return;
  }

  bool bOK=false;
  int iTest = lstPorts.at(0).toInt(&bOK);
  if (bOK!=TRUE)
  {
    ui->statusbar->showMessage("Could not isolate the serial port number");
    return;
  }
  int iOne = iTest;
  iTest = lstPorts.at(1).toInt(&bOK);
  if (bOK!=TRUE)
  {
    ui->statusbar->showMessage("Could not isolate the serial port number");
    return;
  }
  int iTwo = iTest;

  if (iOne>iTwo)
  {
    iTest=iOne;
    iOne=iTwo;
    iTwo=iTest;
  }

  QString sPort;
  sPort = sPort.asprintf("/dev/ttyUSB%d",iOne);
  ui->leConnect->setText(sPort);
  sUpgradePort = sPort.asprintf("/dev/ttyUSB%d",iTwo);
#endif
}

void MainWindow::close_connection()
{
  CloseSerialPort();

  if (poMyController!=nullptr)
  {
    poMyController->CancelTransfer();
  }
}

void MainWindow::btDisconnect_clicked()
{
  uint8_t caData[2];
  int8_t  cReturnCode;

  ui->statusbar->clearMessage();

  caData[0]=0x55;
  caData[1]=0xAA;
  cMessageQueued=1;
  cReturnCode = oMsgFrame->Pack(MSGID_LOGOUT, &caData[0], 1);
  if (cReturnCode!=0)
  {
    ui->statusbar->showMessage("Could not send the command to the hardware unit");
  }

  close_connection();
}

void MainWindow::btCancel_clicked()
{  
  try
  {
    ui->statusbar->showMessage("");
    // Record that a request was received to cancel the connection
    bConnectionCancel = TRUE;
  }
  catch(...)
  {   
    Exception();
  }
}

void MainWindow::timerConnect_timeout()
{
  try
  {
    int8_t cReturnCode;
    wTimerConnectCount++;
    char caData[MAX_PW_LENGTH+1];

    //Send the handshake:
    if (bHandshake==false)
    {
      bCapturePasswordForLogin=FALSE;

      if (wTimerConnectCount>= (CONNECTION_TIMEOUT*2) )
      {
        ui->statusbar->showMessage("Failed to connect to the hardware wallet");
        bConnectionCancel = TRUE;
      }

      //Timer running at 2Hz. Update GUI at 1Hz
      if ((wTimerConnectCount % 2)==0)
      {
        ui->barConnectProgress->setValue( wTimerConnectCount/2 );

        //[0] - Control application communication version:
        caData[0] = (uint8_t)COMMUNICATION_VERSION;
        cReturnCode = oMsgFrame->Pack(MSGID_HANDSHAKE, (uint8_t *)&caData[0], 1);
        if (cReturnCode!=0)
        {
          ui->statusbar->showMessage("Could not send the command to the hardware unit");
        }
      }
    }
    cReturnCode=0;
    if (bConnectionCancel == TRUE)
    {
      close_connection();
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

void MainWindow::timerUpgradeProgress_timeout()
{
  try
  {
    if (poMyController==nullptr)
    {
      printf("Open upgrade port: %s\n",sUpgradePort.toStdString().c_str());
      poMyController = new MyController(&sCompleteFilename, &sUpgradePort, this);
      QObject::connect(poMyController, &MyController::TxTICK,     this, &MainWindow::filetx_tick,     Qt::QueuedConnection );
      QObject::connect(poMyController, &MyController::TxCOMPLETE, this, &MainWindow::filetx_complete, Qt::QueuedConnection );
      poMyController->operate();
    }
    else
    {
      timerUpgradeProgress->stop();
    }

  }
  catch (...)
  {
    Exception();
  }
}

void MainWindow::timerSignProgress_timeout()
{
  try
  {
    int iProgress = ui->pbSign->value();
    iProgress++;
    if (iProgress>=85)
    {
      timerSignProgress->stop();
      ui->statusbar->showMessage("Expected time for response reached.");

      if (timerSendPeriodicMsgs->isActive()==FALSE)
      {
        timerSendPeriodicMsgs->start(2000); //Start sending ping/pong
      }
    }
    else
    {
      ui->pbSign->setValue(iProgress);
    }
  }
  catch (...)
  {   
    Exception();
  }
}

void MainWindow::timerSendPeriodicMsgs_timeout()
{
  try
  {
    uint8_t cByte=0;
    int8_t cReturnCode=0;
    if (cMessageQueued>0)
    {
      cMessageQueued--;
      if (cMessageQueued<=0)
      {
        ui->statusbar->showMessage("The command timed out. Please try again");
        cMessageQueued=0;
      }
      return;
    }


    //If upgrade was initiated, poll the status to see when we should start
    //sending the file. Its a convenient place to send the status request along
    //with the ping/pong to save on implementing another timer
    if (
       (cUpgradeStatus==0)   || //idle
       (cUpgradeStatus>=100)    //Error. Transfer stopped
       )
    {
      cReturnCode = oMsgFrame->Pack(MSGID_PINGPONG, &caSessionID[0], 10);
      if (cReturnCode!=0)
      {
        ui->statusbar->showMessage("Could not send message to the hardware unit");
      }
    }
    else
    {
      cByte=2; //Status
      cReturnCode = oMsgFrame->Pack(MSGID_UPGRADE, &cByte, 1);
      if (cReturnCode!=0)
      {
        ui->statusbar->showMessage("Could not send message to the hardware unit");
      }
    }

    iPeriodicCounter++;
    if (iPeriodicCounter>10)  //20 seconds
    {
      ui->statusbar->showMessage("No response from the unit after 20 seconds. Connection closed.");
      close_connection();
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

int8_t MainWindow::Setup_GUI_for_upgrade()
{
  try
  {
    QString sData;

    cUpgradeVersionApplication=0;
    cUpgradeVersionCommunication=0;

    ui->lbDownload_Result->setText("");    
    ui->lbDownload_ResultCommsThread->setText("");
    ui->lbDownload_FileCommsVersion->setVisible(false);
    ui->lbDownload_FileVersion->setVisible(false);
    ui->lbDownload_FileCommsVersion->setText(" ");
    ui->lbDownload_FileVersion->setText(" ");

    ui->btDownload_Browse->setEnabled(true);
    ui->leDownload_Filename->setText("");
    ui->leDownload_Filename->setEnabled(true);

    ui->btDownload_Start->setEnabled(false);

    ui->stackedWidget->setCurrentWidget(ui->pageDownload);    

    //We've already established a session with the h/w wallet. We
    //know which serial port is used for that. Use the other port
    //for the upgrade path.
    ui->leDownload_Port->setText(sUpgradePort);


    sData = sData.asprintf("%u",APPLICATION_VERSION);
    ui->lbDownload_GUIVersion->setText( sData );
    sData = sData.asprintf("%u",COMMUNICATION_VERSION);
    ui->lbDownload_GUICommsVersion->setText( sData );

    sData = sData.asprintf("%u",cWalletVersionApplication);
    ui->lbDownload_WalletVersionApp->setText( sData );
    sData = sData.asprintf("%u",cWalletVersionCommunication);
    ui->lbDownload_WalletVersionComms->setText( sData );

    //Communication version mismatch between GUI and the wallet?
    if (COMMUNICATION_VERSION != cWalletVersionCommunication)
    {
      ui->teDownload_Mismatch->show();
      ui->statusbar->showMessage("Connected to the unit. Communication version mismatch");
    }
    else
    {
      ui->teDownload_Mismatch->hide();
    }
    return 0;
  }
  catch (...)
  {
    Exception();
    return -1;
  }
}

//--[ GLOBALS ]----------------------------------------------------------------
//#define COMMS_RX_BUFFER_SIZE 4096
//uint8_t    cInternalMsgID;
//uint8_t    cInternalBuffer[COMMS_RX_BUFFER_SIZE];
//uint16_t   iInternalBufferLength=0;
void MainWindow::message_framedetected(uint8_t cMsgID, uint8_t *pcaData, uint16_t iLength)
{
  try
  {
    int8_t cReturnCode=0;
    uint8_t caData[COMMS_RX_BUFFER_SIZE+1];
    QString sTmp;
    QString sData;
    QStringList sParts;
    //uint8_t cPosition=0;
    uint8_t cI,cJ;

    uint16_t iIndex;
    uint16_t iSaplingPaymentAddress_len;
    uint16_t iSaplingExtendedFullViewingKey_len;

    QString qsIndex;
    QString qsSaplingPaymentAddress;
    QString qsSaplingExtendedFullViewingKey;

    pcaData[iLength]=0;

    timerSendPeriodicMsgs->stop();
    timerSendPeriodicMsgs->start(4500);

    if (cMsgID!=MSGID_PINGPONG)
    {
      cMessageQueued=0;
      timerSignProgress->stop();
      ui->pbSign->setVisible(false);
      ui->teSign_Output->setVisible(true);
      ui->statusbar->showMessage(" ");
    }

    switch (cMsgID)
    {
      case MSGID_HANDSHAKE:
        //Received handshake.
        //Keep the connection timer running, to be able
        //to register the logout sequence.
        bHandshake=true;
        bSessionKey=false;

        ui->stackedWidget->setCurrentWidget(ui->pageWelcomeScreen);

        ui->leConnect->clear();
        ui->leConnect->setFocus();
        ui->lbConnect->setText("Session key:");
        ui->btConnect->setText("Submit");
        ui->btDisconnect->show();

        ui->lbConnect_1->hide();
        ui->textConnect->hide();
        ui->rbConnect_Standard->hide();
        ui->rbConnect_Upgrade->hide();
        break;


      case MSGID_SESSIONKEY:
        if (iLength!=15)
        {
          ui->statusbar->showMessage("Unexpected response from the unit.");
          return;
        }

        if (pcaData[0]==0) //Session key failed
        {
          timerSendPeriodicMsgs->stop();
          ui->statusbar->showMessage("Entered invalid session key");
          close_connection();
          return;
        }

        //Force setup:
        //pcaData[1]=0; //FIXIT

        //Packet format:
        //[0]=0   : Session key (handshake) failed
        //[1]     : Unit configured?
        //[2]     : Communication version match: 0=No, 1=Yes
        //[3]     : H/W wallet comms version
        //[4]     : H/W wallet app version
        //[5..14] : Session Key
        cWalletVersionCommunication=pcaData[3];
        cWalletVersionApplication=pcaData[4];

        if (
           (pcaData[2]==0) ||              //Communication version mismatch
           (bConnectionTypeStandard==false) //Upgrade requested from the GUI
           )
        {
          Setup_GUI_for_upgrade();
        }
        else
        {
          if (pcaData[1]==1) //Unit is configured
          {
              //Reset Emoji password
              cEmojiPosition=0;

              bHandshake=true;
              ui->stackedWidget->setCurrentWidget(ui->pageLogin);
              ui->statusbar->showMessage("Connected to the unit");

              caData[0]=0x55;
              caData[1]=0xAA;
              cReturnCode = oMsgFrame->Pack( MSGID_LOGIN,
                                             &caData[0],
                                             2);
              if (cReturnCode!=0)
              {
                ui->statusbar->showMessage("Could not send the command to the hardware unit");
                return;
              }                                             
          }
          else  //Unit not yet configured
          {
              bHandshake=true;
              ui->lbSetupMnemonicCurrentWord->setText("-");
              ui->stackedWidget->setCurrentWidget(ui->pageSetup);
              ui->statusbar->showMessage("Connected to the unit");
          }
        }
        memcpy( &caSessionID[0], (char *)&pcaData[5], 10 );

        iPeriodicCounter=0;
        break;
      case MSGID_RESTORE_MNEMONIC:
      case MSGID_RESTORE_MNEMONIC_NEXT:
      case MSGID_RESTORE_MNEMONIC_PREVIOUS:
      case MSGID_RESTORE_MNEMONIC_FIRST_4_CHARS:
      case MSGID_RESTORE_MNEMONIC_CLEAR:
        ui->stackedWidget->setCurrentWidget(ui->pageRestoreMnemonic);
        ui->sbRestoreMnemonic_1->setValue(1);
        ui->sbRestoreMnemonic_2->setValue(1);
        ui->sbRestoreMnemonic_3->setValue(1);
        ui->sbRestoreMnemonic_4->setValue(1);

        //[0] : Word #
        if (pcaData[1]>23) // 0..23
        {
          ui->statusbar->showMessage("Reported Mnemonic word position out of range.");
          break;
        }

        //[1] : 0 - Empty, 1 - The 3 chars doesn't match a word in the dictionary, 2 - Found a dictionary match
        if (pcaData[1]==2)
        {
          sData = QString::asprintf("Word #%d -- Matched",pcaData[0]+1);
        }
        else if (pcaData[1]==1)
        {
          sData = QString::asprintf("Word #%d -- No match found",pcaData[0]+1);
        }
        else
        {
          sData = QString::asprintf("Word #%d",pcaData[0]+1);
        }
        ui->lbRestoreMnemonic_Word->setText(sData);

        //[2] : Mnemonic checksum passed
        if (pcaData[2]==1)
        {
          ui->btRestoreMnemonic_Continue->setVisible(true);

          //Hide controls:
          ui->btRestoreMnemonic_Clear->setVisible(false);
          ui->btRestoreMnemonic_Submit->setVisible(false);
        }
        else
        {
          ui->btRestoreMnemonic_Continue->setVisible(false);
          //Show controls:
          ui->btRestoreMnemonic_Clear->setVisible(true);
          ui->btRestoreMnemonic_Submit->setVisible(true);
        }

        break;
      case MSGID_REG_STATUS:
        if (pcaData[0]==1) //Generated a new Mnemonic
        {
          ui->btSetupMnemonic3->setVisible(false);
          memset(&caMnemonicViewed[0],0,sizeof(caMnemonicViewed));
          ui->btSetupMnemonic3_Next->setFocus();
          ui->stackedWidget->setCurrentWidget(ui->pageSetupMnemonic3);
          ui->statusbar->showMessage("Unit generated a new Mnemonic");
        }
        else if (pcaData[0]==2) //Navigating the Mnemonic words on the display of the unit
        {
          if (iLength!=2)
          {
            ui->statusbar->showMessage("Unexpected length in the response from the unit.");
            return;
          }

          if (pcaData[1]>23) // 0..23
          {
            ui->statusbar->showMessage("Reported Mnemonic word position out of range.");
            return;
          }

          sData = sData.asprintf("Word #: %u", (pcaData[1]+1));
          ui->lbSetupMnemonicCurrentWord->setText(sData);
          ui->statusbar->showMessage(" ");

          //Have the user cycled through all the values?
          bool_t bAllViewed=true;
          caMnemonicViewed[ pcaData[1] ] = 1;
          for (int iI=0;iI<24;iI++)
          {
            if (caMnemonicViewed[iI] != 1)
            {
              bAllViewed=false;
              break;
            }
          }
          if (bAllViewed==true)
          {
            ui->btSetupMnemonic3->setVisible(true);
          }
        }
        else if (pcaData[0]==3) //3=Mnemonic verification initiated
        {
          ui->btSetupMnemonic4_Continue->setVisible(false);
          ui->btSetupMnemonic4_Previous->setVisible(true);
          ui->btSetupMnemonic4_Next->setVisible(true);
          ui->leSetupMnemonic4->setVisible(true);
          ui->lbSetupMnemonic4->setText("Index:");

          ui->leSetupMnemonic4->setText("");
          ui->stackedWidget->setCurrentWidget(ui->pageSetupMnemonic4);
        }
        else if (pcaData[0]==4) //4=Requested verification (next/previous) on the display
        {
          //The unit sends the value back of what we've entered previously for that position:
          if (
             (pcaData[1]<'A')||
             (pcaData[1]>'E')
             )
          {
            ui->leSetupMnemonic4->setText(""); //No value entered. Blank the LineEdit
          }
          else
          {
            sData = sData.asprintf("%c", (pcaData[1]));
            ui->leSetupMnemonic4->setText(sData);
          }

          //All the word positions entered correctly?
          if (pcaData[2]==1)
          {            
            ui->btSetupMnemonic4_Previous->setVisible(false);
            ui->btSetupMnemonic4_Next->setVisible(false);
            ui->leSetupMnemonic4->setVisible(false);
            ui->lbSetupMnemonic4->setVisible(false);
            ui->statusbar->showMessage("Mnemonic verified");
            ui->btSetupMnemonic4_Continue->setVisible(true);
          }
          else
          {
            ui->btSetupMnemonic4_Previous->setVisible(true);
            ui->btSetupMnemonic4_Next->setVisible(true);
            ui->leSetupMnemonic4->setVisible(true);
            ui->lbSetupMnemonic4->setVisible(true);
            ui->lbSetupMnemonic4->setText("Index:");
            ui->btSetupMnemonic4_Continue->setVisible(false);
          }
        }
        else
        {
          ui->statusbar->showMessage("Unexpected response from the unit.");
          return;
        }
        break;
      case MSGID_GENERATE_PASSWORD:
        if (
           (pcaData[0]!=0x55) ||
           (pcaData[1]!=0xAA)
           )
        {
          ui->statusbar->showMessage("Generate password: Invalid response from the unit.");
          return;
        }

        //Set page to: password setup
        ui->stackedWidget->setCurrentWidget(ui->pageSetupPassword);
        //Make 'continue' visisble:
        ui->btSetupPassword_Continue->setVisible(true);
        break;

      case MSGID_VERIFY_PASSWORD:
      case MSGID_LOGIN_ACK:
        // [0]=0x55 [1]=0xAA : Reset the password grid.
        // [0]=0xEE          : Submit
        //          [1]=0x00 : The position was already selected
        //             =0x01 : Selected the position
        //             =0x02 : Login:  Password correct
        //                     Verify: The verification passed
        //             =0x03 : Login: Password incorrect, some attempts remain
        //                   : Verify: The verification failed
        //             =0x04 : Login: Password incorrect, all attemts exceeded. Unit configuration cleared
        // [0]=0x33 [1]=0x01 : Up
        //             =0x02 : Down
        //             =0x03 : Left
        //             =0x04 : Right
        cI = pcaData[0];
        switch (cI)
        {
          case 0x55: //Initialised
            bLoggedIn=FALSE;

            if (cMsgID==MSGID_LOGIN_ACK)
            {
              bCapturePasswordForLogin=TRUE;
            }
            else
            {
              bCapturePasswordForLogin=FALSE;
            }

            ui->statusbar->showMessage(" ");
            if (bCapturePasswordForLogin==TRUE)
            {
              ui->btLogin_Continue->setVisible(false);
              ui->stackedWidget->setCurrentWidget(ui->pageLogin);
            }
            else
            {
              //Change GUI to password verify:
              if (bCapturePasswordForLogin==true)
              {
                //Login
                ui->btVerifyPassword_Back->setVisible(false);
              }
              else
              {
                //Register
                ui->btVerifyPassword_Back->setVisible(true);
              }

              ui->btVerifyPassword_Continue->setVisible(false);
              ui->btVerifyPassword_Up->setVisible(true);
              ui->btVerifyPassword_Down->setVisible(true);
              ui->btVerifyPassword_Left->setVisible(true);
              ui->btVerifyPassword_Right->setVisible(true);
              ui->btVerifyPassword_Select->setVisible(true);
              ui->btVerifyPassword_Reset->setVisible(true);

              ui->stackedWidget->setCurrentWidget(ui->pageVerifyPassword);
            }
            break;
          case 0x33: //Cursor move accepted
            ui->statusbar->showMessage("The position was changed");
            break;
          case 0xEE: //Submit
            cJ=pcaData[1];
            switch (cJ)
            {
              case 0:  //The position was already selected
                ui->statusbar->showMessage("The position was already selected");
                break;
              case 1:  //The position was selected
                ui->statusbar->showMessage("The position was selected");
                break;
              case 2:  //Verify password: Password test passed
                       //Login:           Password Correct
                if (bCapturePasswordForLogin==TRUE)
                {
                  //Password accepted. Enter the application
                  bLoggedIn=TRUE;
                  Switchto_pageSign();
                }
                else
                {
                  ui->statusbar->showMessage("The password verification passed");

                  ui->btVerifyPassword_Continue->setVisible(true);
                  ui->btVerifyPassword_Up->setVisible(false);
                  ui->btVerifyPassword_Down->setVisible(false);
                  ui->btVerifyPassword_Left->setVisible(false);
                  ui->btVerifyPassword_Right->setVisible(false);
                  ui->btVerifyPassword_Select->setVisible(false);
                  ui->btVerifyPassword_Reset->setVisible(false);
                }
                break;
              case 3:  //Password incorrect.
                if (bCapturePasswordForLogin==TRUE)
                {
                  ui->statusbar->showMessage("Incorrect password");
                }
                else
                {
                  ui->statusbar->showMessage("The password verification failed");
                }
                break;
              case 4:  //Password incorrect. All login attempts exceeded
                ui->statusbar->showMessage("Incorrect password. Login attempt count exceeded. The unit was reset.");
                break;

              default:
                ui->statusbar->showMessage("Unknown response from the unit");
                break;
            }
            break;
          default:
            ui->statusbar->showMessage("Unexpected response from the hardware unit");
            break;
        }
        break;
      case MSGID_APPLY_CONFIG:
        cI=pcaData[0];
        switch (cI)
        {
          case 1: //Preparing unit
            ui->lbApplyConfig_Prepare->setText("Preparing unit");
            ui->lbApplyConfig_Store->setText(" ");
            ui->lbApplyConfig_Verify->setText(" ");
            ui->statusbar->showMessage(" ");
            ui->btApplyConfig_Start->setVisible(false);
            break;
          case 2: //Store configuration
            ui->lbApplyConfig_Prepare->setText("Success");
            ui->lbApplyConfig_Store->setText("Busy");
            ui->lbApplyConfig_Verify->setText(" ");
            break;
          case 3: //Verify the data
            ui->lbApplyConfig_Prepare->setText("Success");
            ui->lbApplyConfig_Store->setText("Success");
            ui->lbApplyConfig_Verify->setText("Busy");
            break;
          case 4: //Calculated MD5 checksum
            ui->lbApplyConfig_Prepare->setText("Success");
            ui->lbApplyConfig_Store->setText("Success");
            ui->lbApplyConfig_Verify->setText("Success");

            ui->btApplyConfig_Start->setVisible(false);
            ui->btApplyConfig_Disconnect->setVisible(true);
            ui->lbApplyConfig_Result->setVisible(true);

            //Periodic ping/pong timer still running. Without it the GUI will close.

            break;
          default:
            ui->statusbar->showMessage("Apply config: Unknown response from the unit");
            break;
        }
        break;
      case MSGID_LOGOUT:
        ui->statusbar->showMessage("The unit closed the connection");
        CloseSerialPort();
        break;


      case MSGID_PINGPONG:
        iPeriodicCounter=0;
        break;

      case MSGID_RETRIEVE_ADDRESS_OTP:
        //Switch to the OTP page:
        ui->stackwidget_RetrieveAddress->setCurrentWidget(ui->pageRetrieveAddress_OTP);
        ui->leRetrieveAddress_OTP->clear();

        //If the message length>0, then it contains an error message about the OTP:
        if (iLength>0)
        {
          memcpy( &caData[0], (char *)pcaData, iLength );
          caData[iLength]=0;
          sData = sTmp.asprintf("%s",&caData[0]);
          ui->statusbar->showMessage(sData);

          //On error an OTP inpu will not be processed again. Revert back to the address page:
          ui->stackwidget_RetrieveAddress->setCurrentWidget(ui->pageRetrieveAddress_MainControl);
        }
        break;

      case MSGID_RETRIEVE_ADDRESS:
        //OTP accepted. Process the retrieved address details:
        ui->stackwidget_RetrieveAddress->setCurrentWidget(ui->pageRetrieveAddress_MainControl);

        if (iLength > COMMS_RX_BUFFER_SIZE)
        {
          ui->teSign_Output->setText("Length of the response message is invalid");
          break;
        }

        iIndex  = (uint16_t) (pcaData[0]    & 0x00FF);
        iIndex |= (uint16_t) (pcaData[1]<<8 & 0xFF00);
        qsIndex = qsIndex.asprintf("%u",iIndex);

        //sSaplingPaymentAddress length
        iSaplingPaymentAddress_len  = (uint16_t) (pcaData[2]    & 0x00FF);
        iSaplingPaymentAddress_len |= (uint16_t) (pcaData[3]<<8 & 0xFF00);
        //sSaplingPaymentAddress:
        memcpy( (char *)&caData[0], (char *)&pcaData[6], iSaplingPaymentAddress_len);
        caData[iSaplingPaymentAddress_len]=0;
        qsSaplingPaymentAddress = qsSaplingPaymentAddress.asprintf("%s", (char *)&caData[0] );

        //sSaplingExtendedFullViewingKey length
        iSaplingExtendedFullViewingKey_len  = (uint16_t) (pcaData[4]    & 0x00FF);
        iSaplingExtendedFullViewingKey_len |= (uint16_t) (pcaData[5]<<8 & 0xFF00);
        //sSaplingExtendedFullViewingKey:
        memcpy( (char *)&caData[0], (char *)&pcaData[6+iSaplingPaymentAddress_len], iSaplingExtendedFullViewingKey_len);
        caData[iSaplingExtendedFullViewingKey_len]=0;
        qsSaplingExtendedFullViewingKey = qsSaplingExtendedFullViewingKey.asprintf("%s", (char *)&caData[0] );

        ui->lbRetrieveAddress_Index->setText(qsIndex);
        ui->teRetrieveAddress_SA->setText(qsSaplingPaymentAddress);
        ui->teRetrieveAddress_EFVK->setText(qsSaplingExtendedFullViewingKey);

        ui->statusbar->showMessage("Received spending key & extended full viewing key");
        break;

      case MSGID_SIGN_OTP:
        //Switch to the OTP page:
        ui->stackwidget_Sign->setCurrentWidget(ui->pageSign_OTP);
        ui->leSign_OTP->clear();

        //If the message length>0, then it contains an error message about the OTP:
        if (iLength>0)
        {
          memcpy( &caData[0], (char *)pcaData, iLength );
          caData[iLength]=0;
          sData = sTmp.asprintf("%s",&caData[0]);
          ui->statusbar->showMessage(sData);

          //On error an OTP inpu will not be processed again. Revert back to the sign page:
          ui->stackwidget_Sign->setCurrentWidget(ui->pageSign_MainControl);
        }
        break;
      case MSGID_SIGN_NAVIGATE:
        //Feedback on navigating through the transaction:
        ui->stackwidget_Sign->setCurrentWidget(ui->pageSign_Navigate);
        //If the message length>0, then it contains an error message about the OTP:
        if (iLength>0)
        {
          memcpy( &caData[0], (char *)pcaData, iLength );
          caData[iLength]=0;
          sData = sTmp.asprintf("%s",&caData[0]);
          ui->lbSign_Detail->setText(sData);
        }
        else
        {
          ui->lbSign_Detail->setText("");
        }
        break;
      case MSGID_SIGN_ACK:
        if (timerSendPeriodicMsgs->isActive()==FALSE)
        {
          timerSendPeriodicMsgs->start(2000); //Start sending ping/pong
        }

        //OTP accepted. Process the signed transaction details:
        ui->stackwidget_Sign->setCurrentWidget(ui->pageSign_MainControl);

        //FIXIT: Add extra markers to verify that the contents are valid, like a checksum on the payload level?
        if (iLength > COMMS_RX_BUFFER_SIZE)
        {
          ui->teSign_Output->setText("Length of the response message is invalid");
          break;
        }

        memcpy(&caData[0], pcaData, iLength);
        caData[iLength]=0;

        sData = sData.asprintf("%s",&caData[0]);
        ui->teSign_Output->setText("sendrawtransaction "+sData);
        break;

      case MSGID_UPGRADE_STATUS:
        //Reset the ping/pong counter
        iPeriodicCounter=0;

        if (iLength!=1)
        {
          ui->statusbar->showMessage("Message invalid length");
          close_connection();
          break;
        }

        //[0]:0=Idle, ready to initiate the transfer
        //   :1=Communication process started
        //   :100=Transfer could not be started
        //        Could not send our version
        //        Could not send start command to the communication process
        //        Timeout waiting for the communication process
        //   :2=Transfer in progress
        //   :101=Timeout waiting for start or to resync
        //   :102=Transfer interrupted/cancelled
        //   :3=Verifying
        //   :103=Integrity test failed
        //   :104=Version mismatch
        //   :4=Transfer ready to deploy
        //   :255=Unknown state

        cI = pcaData[0];

        cUpgradeStatus=cI;
        switch (cI)
        {
          case 1: //Communication process started
            //The transfer is initiated. The user should sit back and wait for the transfer
            //to complete. Disable the buttons:
            ui->btDownload_Browse->setEnabled(false);
            ui->btDownload_Start->setEnabled(false);
            //What about the cancel button?
            //Can we signal the thread and zmodem library to stop?

            ui->lbDownload_Result->setText("The unit is ready to receive the upgrade");

            //sCompleteFilename set by the 'Browse' button when searching for the upgrade file.
            if (poMyController==nullptr)
            {
              ui->lbDownload_Result->setText("Unit ready for file transfer. Starting download");
              timerUpgradeProgress->start(50);
            }
            break;
          case 100://Transfer could not be started
            ui->lbDownload_Result->setText("Could not start the upgrade process on the hardware unit");
            ui->statusbar->showMessage    ("Could not start the upgrade process on the hardware unit");
            break;

          case 2: //Transfer in progress
            //The callback from zmodem is already printing in the result label.
            //Don't print anything if its already busy.
            ui->lbDownload_Result->setText("The transfer is in progress");
            break;

          case 101://Timeout waiting for start or to resync
            ui->lbDownload_Result->setText("Communication timeout occurred");
            ui->statusbar->showMessage    ("Communication timeout occurred");
            break;
          case 102://Transfer interrupted/cancelled
            ui->lbDownload_Result->setText("Communication was cancelled or interrupted");
            ui->statusbar->showMessage    ("Communication was cancelled or interrupted");
            break;

          case 3: //Transfer completed. Verifying data
            ui->lbDownload_Result->setText("Transfer complete, waiting for verification to complete");
            ui->statusbar->showMessage    ("Transfer complete, waiting for verification to complete");
            break;

          case 103: //File verification failed
            ui->lbDownload_Result->setText("The integrity check failed");
            ui->statusbar->showMessage    ("The integrity check failed");
            break;

          case 104: //Version mismatch
            ui->lbDownload_Result->setText("A version mismatch was detected");
            ui->statusbar->showMessage    ("A version mismatch was detected");
            break;

          case 4: //Idle, Transfer complete,  archive available for upgrade
            ui->lbDownload_Result->setText("Transfer complete. Power cycle the unit to complete the upgrade");
            ui->statusbar->showMessage    ("Transfer complete. Power cycle the unit to complete the upgrade");

            ui->lbDownload_ResultCommsThread->setVisible(false);
            break;

          default:
            ui->lbDownload_Result->setText("Unknown upgrade response from the unit");
            ui->statusbar->showMessage    ("Unknown upgrade response from the unit");
            break;
        }
        if (cUpgradeStatus==0)
        {
          //Reset GUI:
          ui->btDownload_Browse->setEnabled(true);
          ui->btDownload_Start->setEnabled(true);
        }
        break;
      case MSGID_ERROR:
        if (iLength>COMMS_RX_BUFFER_SIZE)
        {
          iLength=COMMS_RX_BUFFER_SIZE;
        }
        memcpy( &caData[0], (char *)pcaData, iLength );
        caData[iLength]=0;
        sData = sTmp.asprintf("%s",&caData[0]);
        ui->statusbar->showMessage("Error: "+sData);
        bConnectionCancel=TRUE;
        break;
      default:        
        ui->statusbar->showMessage("Error: Unknown message received");
        close_connection();
        break;
    }
  }
  catch (...)
  {   
    Exception();
  }
}


uint16_t Hex2Array(QString sInput, uint8_t *pcaOutput, uint16_t iSize)
{
  try
  {
    bool bOK;
    memset( (char *)pcaOutput, 0, iSize);
    if ((sInput.length() % 2)!=0)
    {
      return 0;
    }
    if ((sInput.length()/2)>iSize)
    {
      return 0;
    }

    for(int iI=0;iI < sInput.length()/2 ; iI++)
    {
      QString sHEXString = sInput.mid(iI*2,2);
      bOK=false;
      pcaOutput[iI] = (unsigned char) sHEXString.toUShort(&bOK,16);
      if (bOK==false)
      {
        return 0;
      }
    }
    return (sInput.length()/2);
  }
  catch (...)
  {   
    return 0;
  }
}

void MainWindow::btSign_Sign_clicked()
{
  QString sVersion;
  QString sTmp;
  int iTmp;
  bool bOK;
  QString sAmount="";
  QString sChecksumInput;
  uint64_t llTmp;
  
  try
  {
    uint8_t caTransaction[COMMS_RX_BUFFER_SIZE+1];

    ui->statusbar->showMessage(" ");
    ui->teSign_Output->setText("");


    QString sTransaction = ui->teSign_Input->toPlainText();
    if (sTransaction.length()==0)
    {
      ui->statusbar->showMessage("No transaction provided");
      return;
    }

    iTmp = sTransaction.length();
    if (iTmp > (COMMS_RX_BUFFER_SIZE) )
    {
      ui->statusbar->showMessage("Transaction too long");
      return;
    }

    //Version 1 protocol:
    //Parameter   [0]: Project - Expect 'arrr'
    //            [1]: Version - Layout of the command fields
    //            [2] Pay from address
    //            [3] Array of spending notes, which contains the funds of the 'pay from address'. Zip212 supported
    //            [4] Array of recipient: address, amount, memo
    //            [5]..[14] Blockchain parameters
    //            [15] Checksum of all the characters in the command.

    QStringList slParts = sTransaction.split(" ");
    if (slParts.count() < 3)
    {
      ui->statusbar->showMessage("Transaction format error: Too few parts");
      return;
    }

    if (slParts[0] != "z_sign_offline")
    {
      ui->teSign_Output->setText("Transaction format error: Not starting with z_sign_offline");
      return;
    }
    if (slParts[1] != "arrr")
    {
      ui->teSign_Output->setText("Transaction format error: Expected a PirateChain (arrr) transaction");
      return;
    }
    sChecksumInput=slParts[1]+" ";

    sVersion=slParts[2];
    iTmp = sVersion.toInt(&bOK);
    if (bOK==false)
    {
      ui->teSign_Output->setText("Transaction format error: Could not extract the protocol version");
      return;
    }

    //Version check:
    //Communication version 1 & 2 has the same transaction format:
    if (
       (iTmp!=1)                    &&
       (iTmp!=COMMUNICATION_VERSION)
       )
    {
      sTmp = sTmp.asprintf("Transaction format error: Version mismatch. Only supporting %u",COMMUNICATION_VERSION);
      ui->teSign_Output->setText(sTmp);
      return;
    }
    sChecksumInput+=sVersion+" ";

    if (TRANSACTION_VERSION==1)
    {
      if (slParts.count() != 17)
      {
        ui->teSign_Output->setText("Transaction format error: Invalid nr of parts for the specified version");
        return;
      }
    }
    else
    {
      //At the moment only version 1 supported
      ui->teSign_Output->setText("Transaction format error: Only transaction version 1 supported at the moment.");
      return;
    }


    //From address
    QString sFromAddress = slParts[3].replace("\"", "");
    if (sFromAddress.length() != 78)
    {
      ui->teSign_Output->setText("Transaction format error: Length of from address invalid");
      return;
    }
    sChecksumInput+=sFromAddress+" ";

    //Inputs to spend:
    QString sInputs = slParts[4].replace("[","");
    sInputs = sInputs.replace("]","");
    sInputs = sInputs.replace("'","");
    QStringList slInputsParts = sInputs.split("}");

    QString sMsg;
    sMsg = sMsg.asprintf("Witnessed: %s\n",sInputs.toStdString().c_str() );
    ui->teSign_Output->append(sMsg);

    int iCount = slInputsParts.count()-1; //Last entry empty - Matching the last }
    if (iCount < 1)
    {
      ui->teSign_Output->setText("Transaction format error: Could not detect the Witnesses");      
      return;
    }
    sMsg = sMsg.asprintf("Inputs: %d\n",iCount );
    ui->teSign_Output->append(sMsg);

    sChecksumInput+="spending notes: ";
    uint64_t llTotalIn=0;
    for (int iI=0;iI<iCount;iI++)
    {
      sMsg = slInputsParts[iI].replace(",{","");
      QStringList slWitnessParts = sMsg.split(",");

      if (slWitnessParts.count()!=7)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      if (!(slWitnessParts[0].contains("witnessposition")))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      try { sTmp = slWitnessParts[0].split(":")[1].replace("\"",""); }  catch (...)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      sChecksumInput+=sTmp+" ";


      if (!(slWitnessParts[1].contains("witnesspath")))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      try { sTmp = slWitnessParts[1].split(":")[1].replace("\"",""); }  catch (...)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      sChecksumInput+=sTmp+" ";


      if (!(slWitnessParts[2].contains("note_d")))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      try { sTmp = slWitnessParts[2].split(":")[1].replace("\"",""); }  catch (...)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      sChecksumInput+=sTmp+" ";


      if (!(slWitnessParts[3].contains("note_pkd")))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      try { sTmp = slWitnessParts[3].split(":")[1].replace("\"",""); }  catch (...)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      sChecksumInput+=sTmp+" ";


      if (!(slWitnessParts[4].contains("note_r")))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      try { sTmp = slWitnessParts[4].split(":")[1].replace("\"",""); }  catch (...)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      sChecksumInput+=sTmp+" ";


      if (!(slWitnessParts[5].contains("value")))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      try { sAmount = slWitnessParts[5].split(":")[1].replace("\"",""); }  catch (...)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      sChecksumInput+=sAmount+" ";

      unsigned long llAmount = sAmount.toULong(&bOK);
      if (bOK!=true)
      {
         ui->teSign_Output->setText("Transaction format error: Parameter not an integer");
         return;
      }
      llTotalIn+=llAmount;

      sTmp = slWitnessParts[6];
      if (!(slWitnessParts[6].contains("zip212")))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      try { sTmp = slWitnessParts[6].split(":")[1].replace("\"",""); }  catch (...)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the witness parts");
        return;
      }
      sChecksumInput+=sTmp+" ";
    }


    //Outputs
    QString sOutputs = slParts[5].replace("[","");
    sOutputs = sOutputs.replace("]","");
    sOutputs = sOutputs.replace("'","");
    QStringList slOutputsParts = sOutputs.split("}");
    if (slOutputsParts.count() < 2)  //Last entry empty - Matching the last }
    {
      ui->teSign_Output->setText("Transaction format error: Could not detect the outputs");
    }
    uint64_t llChangeAmount=0;
    sChecksumInput+="outputs: ";
    uint64_t llTotalOut=0;
    for (int iI=0;iI<slOutputsParts.count()-1;iI++)
    {
      QString sOutput = slOutputsParts[iI].replace(",{","");
      sOutput = sOutput.replace("{","");
      sOutput = sOutput.replace("},","");
      sOutput = sOutput.replace("}","");

      QStringList slComponents = sOutput.split(",");
      if ((slComponents.count()<2) || (slComponents.count()>3))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the output parts");
        return;
      }

      QStringList slSubComponents = slComponents[0].split(":");
      if (slSubComponents.length()!=2)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the output parts");
        return;
      }
      if (!(slSubComponents[0].contains("address")))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the output parts");
        return;
      }
      QString sAddress = slSubComponents[1].replace("\"","");
      if (sAddress.length()!=78)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the output parts");
        return;
      }
      sChecksumInput+=sAddress+" ";

      slSubComponents = slComponents[1].split(":");
      if (slSubComponents.length()!=2)
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the output parts");
        return;
      }
      if (!(slSubComponents[0].contains("amount")))
      {
        ui->teSign_Output->setText("Transaction format error: Could not find all the output parts");
        return;
      }
      sAmount = slSubComponents[1].replace("\"","");
      unsigned long llAmount = sAmount.toULong(&bOK);
      if (bOK!=true)
      {
         ui->teSign_Output->setText("Transaction format error: Parameter not an integer");
         return;
      }
      sChecksumInput+=sAmount+" ";

      //Have to identify the (small) 'change' address:
      if (sFromAddress==sAddress)
      {
        llChangeAmount = llAmount;
      }



      QString sMemo = "";
      if (slComponents.count()==3)
      {
        slSubComponents = slComponents[2].split(":");
        if (slSubComponents.length()!=2)
        {
          ui->teSign_Output->setText("Transaction format error: Could not find all the output parts");
          return;
        }
        if (!(slSubComponents[0].contains("memo")))
        {
          ui->teSign_Output->setText("Transaction format error: Could not find all the output parts");
          return;
        }
        sMemo = slSubComponents[1].replace("\"","");
        if (sMemo.length()>1024) //512x2
        {
          ui->teSign_Output->setText("Transaction format error: Memo too long");
          return;
        }

        uint8_t cMemo[512];
        uint16_t iMemoLength;
        iMemoLength = Hex2Array(sMemo, &cMemo[0], 512);
        if (iMemoLength!=sMemo.length()/2)
        {
          ui->teSign_Output->setText("Transaction format error: Memo convertion failed");
          return;
        }
      }
      sChecksumInput+=sMemo+" ";
      llTotalOut += llAmount;
    }


    //Further network parameters
    QString sMinconf = slParts[6];
    llTmp = sMinconf.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter not an integer");
       return;
    }
    sChecksumInput+=sMinconf+" ";


    QString sFee = slParts[7];
    unsigned long llFee = sFee.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter not an integer");
       return;
    }

    // Check that the user specified fee is not absurd.
    // This allows amount=0 (and all amount < nDefaultFee) transactions to use the default network fee
    // or anything less than nDefaultFee instead of being forced to use a custom fee and leak metadata
    // -->>The original comment seems to indicate that as long as the fee is set to nDefaultFee then
    //     any amount is allowed.
    if (llTotalOut < ASYNC_RPC_OPERATION_DEFAULT_MINERS_FEE) // Per comment above: (all amount < nDefaultFee)
    {
        if (llFee > ASYNC_RPC_OPERATION_DEFAULT_MINERS_FEE)  // Per comment above: (network fee <= nDefaultFee)
        {                                                   // Conclusion: If (TotalOut < defaultFee) then (fee must also be <= defaultFee)
          sTmp = sTmp.asprintf("Transaction format error: Small transaction amount %lu has fee %lu that is greater than the default fee %d", llTotalOut, llFee, ASYNC_RPC_OPERATION_DEFAULT_MINERS_FEE);
          ui->teSign_Output->setText(sTmp);
          return;
        }
    }
    else
    {
      if (llFee > llTotalOut)
      {
        sTmp = sTmp.asprintf("Transaction format error: The fee is greater than the transaction amount");
        ui->teSign_Output->setText(sTmp);
        return;
      }
    }
    sChecksumInput+=sFee+" ";

    //TreasureChest already added the change back to itself. Check if all the inputs
    //were send to an output:
    if (llTotalIn != (llTotalOut+llFee))
    {
      sTmp = sTmp.asprintf("Transaction error: The outputs (%lu) + fee (%lu) doesn't add up to all the inputs (%lu)", llTotalOut, llFee, llTotalIn);
      ui->teSign_Output->setText(sTmp);
      return;
    }

    //For the hardware wallet a 0.25% commission/tax/developer/maintenance fee is charged
    //Check here if its a valid transaction before submitting it to the hw unit. It will fail there if its not correct too.
    //This is just to catch it earlier for user convenience.
    //The fee will be taken out of the change. If 100% funds are spend the user will first have to decrease the send amount:
    uint64_t llCommission = 6250; //Invalid number if directly assigning the calculation...
    llCommission = llCommission  * 100000000 * 25 / 10000; //0.25%

    llCommission = llTotalOut;
    llCommission = llCommission * 25/10000; //0.25%

    if (llCommission > 1562500000) //15.625Arrr, /1000 = milli, /1000=micro / 100=satoshi
    {
      //Cap maximum commission at 6250 coin.
      llCommission = 1562500000;
    }

    //Converted to Satoshi's: Compare avaialbe change vs the commission to be paid
    if (llChangeAmount < llCommission)
    {
      double fCommission = (double)llCommission / 100000000;
      double fmCommission = (double)llCommission / 100000;
      double fuCommission = (double)llCommission / 100;
      sTmp = sTmp.asprintf("Transaction error: Insufficient change left for the 0.25%% commission of %0.4f Arrr, %0.4f mArrr, %0.4f uARRR. Please decrease the sending amount",fCommission , fmCommission, fuCommission);
      ui->teSign_Output->setText(sTmp);
      return;
    }


    QString sNextBlockHeight = slParts[8];
    llTmp = sNextBlockHeight.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter not an integer");
       return;
    }
    sChecksumInput+=sNextBlockHeight+" ";


    QString sBranchID = slParts[9];
    llTmp = sBranchID.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter not an integer");
       return;
    }
    sChecksumInput+=sBranchID+" ";


    QString sAnchor   = slParts[10].replace("\"","");
    if (sAnchor.length()!=64)
    {
      ui->teSign_Output->setText("Transaction format error: Anchor length invalid");
      return;
    }
    uint8_t cAnchor[32];
    uint16_t iAnchorLength;
    iAnchorLength = Hex2Array(sAnchor, &cAnchor[0], 32);
    if (iAnchorLength!=32)
    {
      ui->teSign_Output->setText("Transaction format error: Anchor length invalid");
      return;
    }
    sChecksumInput+=sAnchor+" ";


    QString sOverwintered = slParts[11];
    llTmp = sOverwintered.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter 10 not an integer");
       return;
    }
    sChecksumInput+=sOverwintered+" ";


    QString sExpiryHeight = slParts[12];
    llTmp = sExpiryHeight.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter 11 not an integer");
       return;
    }
    sChecksumInput+=sExpiryHeight+" ";


    QString sVersionGroupID = slParts[13];
    llTmp = sVersionGroupID.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter 12 not an integer");
       return;
    }
    sChecksumInput+=sVersionGroupID+" ";


    QString sVersion = slParts[14];
    llTmp = sVersion.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter 13 not an integer");
       return;
    }
    sChecksumInput+=sVersion+" ";


    QString sZipEnabled = slParts[15];
    llTmp = sZipEnabled.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter 14 not an integer");
       return;
    }
    sChecksumInput+=sZipEnabled+" ";


    //Parameter [15]: checksum
    //A simple checksum of the full string, to detect copy/paste errors between the wallets
    //The checksum equals the sum of the ASCII values of all the characters in the string:
    //printf("sChecksumInput:\n%s\n\n",sChecksumInput.c_str() );

    unsigned int iChecksum=0x01;
    for (int iI=0;iI<sChecksumInput.length();iI++)
    {
      QChar oChar = sChecksumInput.at(iI);
      unsigned int iVal = (unsigned int)oChar.toLatin1();
      iChecksum = iChecksum + iVal;
    }

    QString sChecksum = slParts[16];
    llTmp = sChecksum.toULong(&bOK);
    if (bOK!=true)
    {
       ui->teSign_Output->setText("Transaction format error: Parameter 15 not an integer");
       return;
    }
    if (llTmp != iChecksum)
    {
      ui->teSign_Output->setText("Transaction format error: The checksum in the message mismatches the calculated checksum");
      return;
    }


    uint16_t iSize = sTransaction.length();
    if (iSize > COMMS_RX_BUFFER_SIZE)
    {
      ui->teSign_Output->setText("Transaction too big to fit in the communication buffer");
      return;
    }
    memcpy(&caTransaction[0], sTransaction.toStdString().c_str(), iSize);
    caTransaction[ iSize ]=0;


    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }



    cMessageQueued=3;
    int8_t cReturnCode = oMsgFrame->Pack( MSGID_SIGN,
                                          (uint8_t *)&caTransaction[0],
                                          iSize);
    switch (cReturnCode)
    {
      case 0:
        ui->teSign_Output->setText("Transaction send to the hardware wallet.");
        timerSendPeriodicMsgs->stop();
        break;
      case 1:
        ui->teSign_Output->setText("Input error. Could not queue the message for transmission.");
        break;
      case 2:
        ui->teSign_Output->setText("Could not queue the message for transmission.");
        break;
     }
  }
  catch(...)
  {   
    ui->teSign_Output->setText("Transaction format error: Exception");
    Exception();
  }
}

void MainWindow::btSign_Back_clicked()
{
  try
  {
    int8_t cReturnCode;
    uint8_t caData[4];

    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply


    caData[0] = 1; //Back
    ui->statusbar->showMessage("Request previous transaction part");

    cReturnCode = oMsgFrame->Pack(MSGID_SIGN_NAVIGATE, &caData[0], 1 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
      return;
    }

  }
  catch (...)
  {
      ui->statusbar->showMessage("An error occurred.");
      Exception();
  }
}
void MainWindow::btSign_Next_clicked()
{
  try
  {
    int8_t cReturnCode;
    uint8_t caData[4];

    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply


    caData[0] = 2; //Next
    ui->statusbar->showMessage("Request next transaction part");

    cReturnCode = oMsgFrame->Pack(MSGID_SIGN_NAVIGATE, &caData[0], 1 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
      return;
    }

  }
  catch (...)
  {
      ui->statusbar->showMessage("An error occurred.");
      Exception();
  }
}
void MainWindow::btSign_OTP_clicked()
{
  try
  {
    bool bOK;
    int8_t cReturnCode;
    uint8_t caData[4];
    QString sTmp;
    int32_t iOTP;

    //Convert text input to OTP number:
    sTmp = ui->leSign_OTP->text();
    if (sTmp.length()!=4)
    {
      ui->statusbar->showMessage("Please enter the 4 numbers of the OTP as it appears on the hardware wallet");
      return;
    }

    iOTP = sTmp.toInt(&bOK,10);
    if (
       (bOK==FALSE) ||
       (iOTP < 0)
       )
    {
      ui->statusbar->showMessage("Please enter the 4 numbers of the OTP as it appears on the hardware wallet");
      return;
    }

    //Split number into its parts:
    caData[0] = iOTP % 10;           // Ones
    caData[1] = (iOTP % 100)  / 10;  // Tens
    caData[2] = (iOTP % 1000) / 100; // Hundres
    caData[3] = iOTP / 1000;         // Thousands


    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=40; //40 seconds to sign enough?
    ui->statusbar->showMessage("Request address OTP");

    //OTP send. Return to the sign details page.
    //Only 1 OTP/retrieve request. Have to initiate a new sign request to try again.    
    ui->teSign_Output->clear();
    ui->stackwidget_Sign->setCurrentWidget(ui->pageSign_MainControl);

    cReturnCode = oMsgFrame->Pack(MSGID_SIGN_OTP, &caData[0], 4 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
      return;
    }

    //With the data send, start the countdown timer
    ui->pbSign->setValue(0);
    ui->pbSign->setVisible(true);
    ui->teSign_Output->setVisible(false);
    if (timerSignProgress->isActive()==FALSE)
    {
      timerSignProgress->start(1000); //1 second interval
    }
    ui->statusbar->showMessage("It will take approx. 30 seconds to sign the transaction.");
  }
  catch (...)
  {
      ui->statusbar->showMessage("An error occurred.");
      Exception();
  }
}


void MainWindow::btSign_Clear_clicked()
{
  if (cMessageQueued==0) //Idling
  {
    ui->statusbar->showMessage(" ");
    ui->teSign_Input->clear();
    ui->teSign_Output->clear();
  }
  else
  {
    ui->statusbar->showMessage("Sign request in progress. Please wait for it to complete.");
  }
}

void MainWindow::btSetup_clicked()
{
  ui->stackedWidget->setCurrentWidget(ui->pageSetupMnemonic2);

  /*
  ui->statusbar->showMessage("Request: Register fingerprints");
  uint8_t cByte=0; //Start fingerprint registration process
  int8_t cReturnCode = oMsgFrame->Pack(MSGID_REGISTER_FINGERPRINT, &cByte, 1 );
  if (cReturnCode != 0)
  {
    ui->statusbar->showMessage("Could not send message to the unit");
  }
  */
}

void MainWindow::btSetupMnemonic2_clicked()
{
  uint8_t cByte=0;
  int8_t cReturnCode=0;

  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    if (ui->rbMnemonicNew->isChecked()==true)
    {
      cByte=0x55; //Pad length of MSGID_GEN_MNEMONIC to 1.
      cReturnCode = oMsgFrame->Pack(MSGID_GEN_MNEMONIC, &cByte, 1);

      ui->statusbar->showMessage("Request unit to generate a new Mnemonic");
    }
    else
    {
      cReturnCode = oMsgFrame->Pack(MSGID_RESTORE_MNEMONIC, &cByte, 0);
      if (cReturnCode!=0)
      {
        ui->statusbar->showMessage("Could not send the command to the hardware unit");
      }
      ui->statusbar->showMessage("Request unit to restore an existing Mnemonic");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }

  return;
}

void MainWindow::btRestoreMnemonic_Previous_clicked()
{
  int8_t cReturnCode;
  uint8_t cByte;
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    cReturnCode = oMsgFrame->Pack(MSGID_RESTORE_MNEMONIC_PREVIOUS, &cByte, 0);
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }

  return;
}
void MainWindow::btRestoreMnemonic_Next_clicked()
{
  int8_t cReturnCode;
  uint8_t cByte;
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    cReturnCode = oMsgFrame->Pack(MSGID_RESTORE_MNEMONIC_NEXT, &cByte, 0);
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }

  return;
}
void MainWindow::btRestoreMnemonic_Clear_clicked()
{
  int8_t cReturnCode;
  uint8_t cByte;
  try
  {
    if (cMessageQueued==true)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=true;

    cReturnCode = oMsgFrame->Pack(MSGID_RESTORE_MNEMONIC_CLEAR, &cByte, 0);
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }

  return;
}

void MainWindow::btRestoreMnemonic_Submit_clicked()
{
  int8_t cReturnCode;
  uint8_t caData[4];
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply

    caData[0] = (uint8_t)ui->sbRestoreMnemonic_1->value();
    caData[1] = (uint8_t)ui->sbRestoreMnemonic_2->value();
    caData[2] = (uint8_t)ui->sbRestoreMnemonic_3->value();
    caData[3] = (uint8_t)ui->sbRestoreMnemonic_4->value();

    cReturnCode = oMsgFrame->Pack(MSGID_RESTORE_MNEMONIC_FIRST_4_CHARS, &caData[0], 4);
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }

  return;
}


void MainWindow::btSetupMnemonic3_Previous_clicked()
{
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    ui->statusbar->showMessage("Request previous word from Mnemonic list");

    //Instruct the unit that we want the previous word in the Mnemonic list
    uint8_t cByte;
    int8_t cReturnCode;
    cReturnCode = oMsgFrame->Pack(MSGID_GEN_MNEMONIC_PREVIOUS, &cByte, 0 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }

}

void MainWindow::btSetupMnemonic3_Next_clicked()
{
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    ui->statusbar->showMessage("Request next word from Mnemonic list");

    //Instruct the unit that we want the next word in the Mnemonic list
    uint8_t cByte;
    int8_t cReturnCode;
    cReturnCode = oMsgFrame->Pack(MSGID_GEN_MNEMONIC_NEXT, &cByte, 0 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

void MainWindow::btSetupMnemonic3_clicked()
{
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    ui->statusbar->showMessage("Verify Mnemonic");

    //Instruct the unit that we want to verify the Mnemonic list
    uint8_t cByte;
    int8_t cReturnCode;
    cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_MNEMONIC, &cByte, 0 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
      ui->statusbar->showMessage("Transmission failed");
      Exception();
  }
}

void MainWindow::btSetupMnemonic4_Previous_clicked()
{
  char cChar;
  uint8_t cByte;
  int8_t cReturnCode;
//  int32_t iI;

  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    ui->statusbar->showMessage("Request the previous mnemonic");

    QString sData = ui->leSetupMnemonic4->text();
    //With an empty value, proceed to previous mnemonic item
    //without processing the current item.
    //Else, if length>0, process it:
    if (sData.length() != 0)
    {
      QChar qChar = sData[0].toUpper();
      cChar = qChar.toLatin1();
      if ((cChar<'A')||(cChar>'E'))
      {
        ui->statusbar->showMessage("Enter an index between A..E");
        return;
      }
    }

    cByte = (uint8_t)cChar;
    cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_MNEMONIC_PREVIOUS, &cByte, 1 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch(...)
  {
    ui->statusbar->showMessage("Enter a number between 1..24");
    Exception();
  }
}

void MainWindow::btSetupMnemonic4_Next_clicked()
{
  char cChar;
  uint8_t cByte;
  int8_t cReturnCode;
//  int32_t iI;

  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    ui->statusbar->showMessage("Request the next mnemonic word");

    QString sData = ui->leSetupMnemonic4->text();
    //With an empty value, proceed to next mnemonic item
    //without processing the current item,
    //else, process the value:
    if (sData.length() != 0)
    {
      QChar qChar = sData[0].toUpper();
      cChar = qChar.toLatin1();
      if ((cChar<'A')||(cChar>'E'))
      {
        ui->statusbar->showMessage("Enter an index between A..E");
        return;
      }
    }

    cByte = (uint8_t)cChar;
    cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_MNEMONIC_NEXT, &cByte, 1 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch(...)
  {
    ui->statusbar->showMessage("Enter a number between 1..24");
    Exception();
  }
}

void MainWindow::btSetupMnemonic4_Continue_clicked()
{
  int8_t cReturnCode=0;
  uint8_t caData[2];
  try
  {
    //Registration
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    ui->statusbar->showMessage("Request password setup");
    //Generate a new password:
    caData[0]=0x55;
    caData[1]=0xAA;
    cReturnCode = oMsgFrame->Pack(MSGID_GENERATE_PASSWORD, &caData[0], 2);
    if (cReturnCode==0)
    {
      ui->statusbar->showMessage("Request password setup");
    }
    else
    {
      ui->statusbar->showMessage("Could not send message to the unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

void MainWindow::btSetupPassword_Generate_clicked()
{
  uint8_t cByte;
  int8_t cReturnCode=0;
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    ui->statusbar->showMessage("Request password");
    cByte=0; //Unused
    cReturnCode = oMsgFrame->Pack(MSGID_GENERATE_PASSWORD, &cByte, 0 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

void MainWindow::btSetupPassword_Continue_clicked()
{
  uint8_t caData[2];

  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply

    caData[0]=0x55;
    caData[1]=0xAA;
    int8_t cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_PASSWORD, &caData[0], 2);
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the password verify initialisation to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

int8_t MainWindow::getColumn(QString sColumn)
{
  char cChar;
  int8_t cPosition=-1;
  QString sTmp;

  try
  {
    if (sColumn.length()!=1)
    {
      ui->statusbar->showMessage("Please fill in the column position");
      return -1;
    }
    cChar = sColumn.toUpper().toStdString().at(0);

    //A..J: ASCII values are 0x41..0x4A
    //      Map to interger, ranging from 0..9: (0 based)
    cPosition = cChar-0x41;
    if ((cPosition<0)||(cPosition>9))
    {
      ui->statusbar->showMessage("The column position must be between A..J");
      return -1;
    }

    return cPosition;
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
    return -1;
  }
}

int8_t MainWindow::getRow(QString sRow)
{
  char cChar;
  int8_t cPosition=-1;
  QString sTmp;

  try
  {
    if (sRow.length()!=1)
    {
      ui->statusbar->showMessage("Please fill in the row position");
      return -1;
    }
    cChar = sRow.toUpper().toStdString().at(0);

    //1..7: ASCII values are 0x31..0x37
    //      Map to interger, ranging from 0..6: (0 based)
    cPosition = cChar-0x31;
    if ((cPosition<0)||(cPosition>6))
    {
      ui->statusbar->showMessage("The row position must be between 1..7");
      return -1;
    }

    return cPosition;
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
    return -1;
  }
}


void MainWindow::btPassword_Up_clicked()
{
  uint8_t caData[2];
  int8_t cReturnCode;
  try
  {    
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply

    //[0]=0x33 [1]=0x01 : Up
    caData[0]=0x33;
    caData[1]=0x01;

    if (bCapturePasswordForLogin==TRUE)
    {
      cReturnCode = oMsgFrame->Pack(MSGID_LOGIN, &caData[0], 2);
    }
    else
    {
      cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_PASSWORD, &caData[0], 2);
    }
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}
void MainWindow::btPassword_Down_clicked()
{
  uint8_t caData[2];
  int8_t cReturnCode;
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply

    //[0]=0x33 [1]=0x02 : Down
    caData[0]=0x33;
    caData[1]=0x02;

    if (bCapturePasswordForLogin==TRUE)
    {
      cReturnCode = oMsgFrame->Pack(MSGID_LOGIN, &caData[0], 2);
    }
    else
    {
      cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_PASSWORD, &caData[0], 2);
    }

    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}
void MainWindow::btPassword_Left_clicked()
{
  uint8_t caData[2];
  int8_t cReturnCode;
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply

    //[0]=0x33 [1]=0x03 : Left
    caData[0]=0x33;
    caData[1]=0x03;

    if (bCapturePasswordForLogin==TRUE)
    {
      cReturnCode = oMsgFrame->Pack(MSGID_LOGIN, &caData[0], 2);
    }
    else
    {
      cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_PASSWORD, &caData[0], 2);
    }

    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}
void MainWindow::btPassword_Right_clicked()
{
  uint8_t caData[2];
  int8_t cReturnCode;
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply

    //[0]=0x33 [1]=0x04 : Right
    caData[0]=0x33;
    caData[1]=0x04;
    if (bCapturePasswordForLogin==TRUE)
    {
      cReturnCode = oMsgFrame->Pack(MSGID_LOGIN, &caData[0], 2);
    }
    else
    {
      cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_PASSWORD, &caData[0], 2);
    }

    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

void MainWindow::btPassword_Select_clicked()
{  
  uint8_t caData[2];
  int8_t cReturnCode;
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply

    //[0]=0xEE [1]=0xBB : Select
    caData[0]=0xEE;
    caData[1]=0xBB;
    if (bCapturePasswordForLogin==TRUE)
    {
      cReturnCode = oMsgFrame->Pack(MSGID_LOGIN, &caData[0], 2);
    }
    else
    {
      cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_PASSWORD, &caData[0], 2);
    }

    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

void MainWindow::btPassword_Reset_clicked()
{
  uint8_t caData[2];
  int8_t cReturnCode;
  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply

    //[0]=0x55 [1]=0xAA : Reset
    caData[0]=0x55;
    caData[1]=0xAA;
    if (bCapturePasswordForLogin==TRUE)
    {
      cReturnCode = oMsgFrame->Pack(MSGID_LOGIN, &caData[0], 2);
    }
    else
    {
      cReturnCode = oMsgFrame->Pack(MSGID_VERIFY_PASSWORD, &caData[0], 2);
    }

    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}


void MainWindow::btVerifyPassword_Back_clicked()
{
  uint8_t caData[2];
  int8_t  cReturnCode;

  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3; //X seconds for a reply

    ui->statusbar->showMessage("Request password setup");
    //Generate a new password:
    caData[0]=0x55;
    caData[1]=0xAA;
    cReturnCode = oMsgFrame->Pack(MSGID_GENERATE_PASSWORD, &caData[0], 2);
    if (cReturnCode==0)
    {
      ui->statusbar->showMessage("Request password setup");
    }
    else
    {
      ui->statusbar->showMessage("Could not send message to the unit");
    }
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

void MainWindow::btVerifyPassword_Continue_clicked()
{
  ui->statusbar->showMessage(" ");
  ui->stackedWidget->setCurrentWidget(ui->pageApplyConfig);

  ui->btApplyConfig_Start->setVisible(true); 
  ui->lbApplyConfig_Prepare->setText(" ");
  ui->lbApplyConfig_Store->setText(" ");
  ui->lbApplyConfig_Verify->setText(" ");

  ui->btApplyConfig_Disconnect->setVisible(false);
  ui->lbApplyConfig_Result->setVisible(false);  
}

/*
void MainWindow::btVerifyPassword_Back_clicked()
{
  ui->statusbar->showMessage(" ");
  ui->stackedWidget->setCurrentWidget(ui->pageSetupPassword);
}
*/

void MainWindow::btApplyConfig_Start_clicked()
{
  uint8_t cByte;
  int8_t cReturnCode;

  try
  {
    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=85; //78 seconds to save file -- Give it 85

    ui->statusbar->showMessage("Apply config");
    cByte=0; //Unused
    cReturnCode = oMsgFrame->Pack(MSGID_APPLY_CONFIG, &cByte, 0 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }

  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

void MainWindow::btDownload_Start_clicked()
{
  int8_t cReturnCode;
  uint8_t caData[3];
  QString sTmp;

  if (sCompleteFilename.length()==0)
  {
    ui->lbDownload_Result->setText("Provide the upgrade filename");
    return;
  }
  if (cUpgradeVersionCommunication<cWalletVersionCommunication)
  {
    ui->lbDownload_Result->setText("The communication version of the file is less than the h/w wallet");
    return;
  }
  else if (cUpgradeVersionCommunication>cWalletVersionCommunication)
  {
    ui->lbDownload_Result->setText(" ");    
  }
  else
  {
    //Comms versions are equal. Is the app version larger?
    if (cUpgradeVersionApplication<cWalletVersionApplication)
    {
      ui->lbDownload_Result->setText("The application version of the file is less than that of the wallet");
      return;
    }
  }

  //Does the specified port exist?
  if (ui->leDownload_Port->text().length()==0)
  {
    ui->lbDownload_Result->setText("Set the serial port to use for the upgrade");
    return;
  }

  sTmp = ui->leDownload_Port->text();
  QFile qFile( sTmp );
  if (!qFile.open(QIODevice::ReadWrite))
  {
    ui->lbDownload_Result->setText("Can't get read/write access to the serial port");
    return;
  }
  qFile.close();
  //The port could have been updated on the GUI. Set the internal variable to it.
  sUpgradePort = ui->leDownload_Port->text();


  caData[0]=1; //Start download
  caData[1]=cUpgradeVersionCommunication; //Comms version of upgrade file
  caData[2]=cUpgradeVersionApplication;   //App version of upgrade file

  cReturnCode = oMsgFrame->Pack( MSGID_UPGRADE,
                                 &caData[0],
                                 3);
  if (cReturnCode!=0)
  {
    ui->statusbar->showMessage("Could not send the message to the unit");
  }

}

// Verify the signature of the upgrade file
// Input: Full path to the file to verify
// return code:  0 - Success
//              -1 - Cannot access the file
//              -2 - File structure invalid
//              -3 - Cannot access the signature file
//              -4 - Signature invalid
int8_t MainWindow::Verify_Upgrade_Signature(QString sUpgradeFile, uint8_t *pcFileCommsVersion, uint8_t *pcFileAppVersion)
{
  size_t bytes = 0;
  unsigned char cSignature[1025];

  #define BUFFER_SIZE 512
  static unsigned char buffer[BUFFER_SIZE];
  unsigned int iCounter;
  int ret;
  size_t iSize;
  struct stat st;

  *pcFileCommsVersion=0;
  *pcFileAppVersion=0;

  //Expected structure:
  // (encrypted data)[version1][version2][signature_1024]

  ret = stat(sUpgradeFile.toLocal8Bit().data(), &st); //Access c-style char array of QString
  if (ret==-1)
  {
    return -1;
  }

  iSize = st.st_size;
  if (iSize<=1026)
  {
    //A file with only the version and signature?
    return -2;
  }

  //Calculate the hash of the payload (encrypted data), excluding the
  //version string and the signature:

  // Calculate SHA256 digest for datafile
  FILE* datafile = fopen(sUpgradeFile.toLocal8Bit().data() , "rb");
  // Buffer to hold the calculated digest
  unsigned char digest[SHA256_DIGEST_LENGTH];
  SHA256_CTX ctx;
  SHA256_Init(&ctx);

  // Read data in chunks and feed it to OpenSSL SHA256
  // Don't read the last 1024 bytes, which contains the
  // signature at the end of the file
  iCounter=0;
  while((bytes = fread(buffer, 1, 1, datafile)))
  {
      SHA256_Update(&ctx, buffer, bytes);      

      //Assign 2 bytes for the version code:
      if (iCounter == (iSize-1026))
      {
        *pcFileCommsVersion=(uint8_t)buffer[0];
      }
      if (iCounter == (iSize-1025))
      {
        *pcFileAppVersion=(uint8_t)buffer[0];
        //The last byte in the file is the application version.
        break;
      }
      iCounter++;
  }
  SHA256_Final(digest, &ctx);

  //Read last 1024 bytes for the signature:
  memset(&cSignature[0],0x00,sizeof(cSignature));
  bytes = fread(&cSignature[0], 1, 1024, datafile);

  fclose(datafile);


  // Verify that calculated digest and signature match
  // Note: Only QT classes can access 'resources':
  //       https://stackoverflow.com/questions/9465727/convert-qfile-to-file  

  // Can wrap the data in a BIO buffer and still access it via the file I/O functions that
  // openssl relies on:
  QFile qFile(":/keys/upgrade_key.pub");
  if (!qFile.open(QIODevice::ReadOnly))
  {
    return -3;
  }
  QByteArray qbaData = qFile.readAll();
  qFile.close();
  if (qbaData.size()==0)
  {
    return -3;
  }

  BIO *bufio;
  RSA* pRSA_pubkey;
  bufio = BIO_new_mem_buf((void*)qbaData.constData(), qbaData.size());
  //Should the bufio memory be cleared/freed?

  EVP_PKEY* evp_key = PEM_read_bio_PUBKEY(bufio, NULL, NULL, NULL);
  pRSA_pubkey = EVP_PKEY_get1_RSA(evp_key);

  // Decrypt signature (in buffer) and verify it matches
  // with the digest calculated from data file.
  ret = RSA_verify(NID_sha256, digest, SHA256_DIGEST_LENGTH,
                   cSignature, (unsigned int)bytes, pRSA_pubkey);

  RSA_free(pRSA_pubkey);

  if(ret == 1)
  {      
    //Signature valid
    return 0;
  }
  else
  {
    //Signature invalid
    return -4;
  }
}

void MainWindow::btDownload_Browse_clicked()
{
  try
  {
    int8_t cReturnCode;
    QString sTmp;
    QString sFilename;

    ui->btDownload_Start->setEnabled(false);
    ui->lbDownload_FileCommsVersion->setVisible(false);
    ui->lbDownload_FileVersion->setVisible(false);

//On Cygwin the windows default system file dialog is not availabe.
//The Qt file dialog under Cygwin, however, does not correctly set the initial path.
//The only way I could get the root path to be set already when the dialog opens was 
//to use '/'.

//The file dialog inherits the stylesheet settings. In our case this causes the 
//background to be black, with black text.
//A work around was to create a new, unused widget (pageDownloadNoStyle), and set
//its background to white. The dialog appears correctly now
#ifdef __CYGWIN__
    printf("CYGWIN\n");
    sFilename = QFileDialog::getOpenFileName(ui->pageDownloadNoStyle, ("Open File"),
                                                 "/",
                                                 ("Data container (*.dat)"));
#else
    printf("LIN\n");
    sFilename = QFileDialog::getOpenFileName(ui->pageDownloadNoStyle, ("Open File"),
                                                 ".",
                                                 ("Data container (*.dat)"));                                                 
#endif                                                 
    if (sFilename.length()==0)
    {
      ui->leDownload_Filename->clear();
      return;
    }
    sCompleteFilename=sFilename;
    sTmp = QString(sCompleteFilename.split("/").last());

    //Verify the file signature first:
    cReturnCode = Verify_Upgrade_Signature(sCompleteFilename,&cUpgradeVersionCommunication, &cUpgradeVersionApplication);
    if (cReturnCode==0) //success
    {
      ui->leDownload_Filename->setText(sTmp);

      sTmp = sTmp.asprintf("%u",cUpgradeVersionCommunication);
      ui->lbDownload_FileCommsVersion->setText(sTmp);
      ui->lbDownload_FileCommsVersion->setVisible(true);

      sTmp = sTmp.asprintf("%u",cUpgradeVersionApplication);
      ui->lbDownload_FileVersion->setText(sTmp);
      ui->lbDownload_FileVersion->setVisible(true);


      if (cUpgradeVersionCommunication<cWalletVersionCommunication)
      {
        ui->lbDownload_Result->setText("The communication version of the file is less than the h/w wallet");
        return;
      }
      else if (cUpgradeVersionCommunication>cWalletVersionCommunication)
      {
        //Ready to start the transfer
        ui->lbDownload_Result->setText(" ");
        ui->btDownload_Start->setEnabled(true);
        return;
      }
      else
      {
        //Comms versions are equal. Is the app version larger?
        if (cUpgradeVersionApplication<cWalletVersionApplication)
        {
          ui->lbDownload_Result->setText("The application version of the file is less than the h/w wallet");
          return;
        }
      }
    }
    else
    {
      switch(cReturnCode)
      {
        case -1:
          ui->lbDownload_Result->setText("Error: Could not read the file");
          break;
        case -2:
          ui->lbDownload_Result->setText("Error: File structure invalid");
          break;
        case -3:
          ui->lbDownload_Result->setText("Error: Cannot access the signature file");
          break;
        case -4:
          ui->lbDownload_Result->setText("Error: Signature failed");
          break;
        default:
          ui->lbDownload_Result->setText("Error: Unknown error");
          break;
      }
    }
  }
  catch (...)
  {   
    Exception();
  }
}
void MainWindow::Switchto_pageSign()
{
  try
  {
    ui->teSign_Input->clear();
    ui->teSign_Output->clear();

    ui->statusbar->showMessage(" ");

    ui->btSign_Sign->setFocus();
    ui->stackedWidget->setCurrentWidget(ui->pageSign);
    ui->stackwidget_Sign->setCurrentWidget(ui->pageSign_MainControl);
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}

void MainWindow::Switchto_pageAddress()
{
  try
  {
    //Transaction in progress?
    if (cMessageQueued>0)
    {
      ui->statusbar->showMessage("Sign request in progress. Please wait for it to complete.");
      return;
    }

    ui->teRetrieveAddress_SA->clear();
    ui->teRetrieveAddress_EFVK->clear();

    ui->statusbar->showMessage(" ");

    ui->sbRetrieveAddress_Index->setFocus();
    ui->stackedWidget->setCurrentWidget(ui->pageAddress);
    ui->stackwidget_RetrieveAddress->setCurrentWidget(ui->pageRetrieveAddress_MainControl);
  }
  catch (...)
  {
    ui->statusbar->showMessage("An exception occurred");
    Exception();
  }
}


void MainWindow::btRetrieveAddress_clicked()
{
  try
  {
//    bool bOK;
    int8_t cReturnCode;
    uint8_t caData[2];

    ui->teRetrieveAddress_SA->setText("");
    ui->teRetrieveAddress_EFVK->setText("");

    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply

    ui->statusbar->showMessage("Request address");
    //Instruct the unit that we want to retrieve a spending & viewing key
    uint16_t iIndex = (uint16_t)ui->sbRetrieveAddress_Index->value();
    // 0..65535 is valid

    caData[0] = (iIndex   ) & 0xFF;
    caData[1] = (iIndex>>8) & 0xFF;

    cReturnCode = oMsgFrame->Pack(MSGID_RETRIEVE_ADDRESS, &caData[0], 2 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }
  }
  catch (...)
  {
      ui->statusbar->showMessage("An error occurred.");
      Exception();
  }
}

void MainWindow::btRetrieveAddressOTP_clicked()
{
  try
  {
    bool bOK;
    int8_t cReturnCode;
    uint8_t caData[4];
    QString sTmp;
    int32_t iOTP;

    //Convert text input to OTP number:
    sTmp = ui->leRetrieveAddress_OTP->text();
    if (sTmp.length()!=4)
    {
      ui->statusbar->showMessage("Please enter the 4 numbers of the OTP as it appears on the hardware wallet");
      return;
    }

    iOTP = sTmp.toInt(&bOK,10);
    if (
       (bOK==FALSE) ||
       (iOTP < 0)
       )
    {
      ui->statusbar->showMessage("Please enter the 4 numbers of the OTP as it appears on the hardware wallet");
      return;
    }

    //Split number into its parts:
    caData[0] = iOTP % 10;           // Ones
    caData[1] = (iOTP % 100)  / 10;  // Tens
    caData[2] = (iOTP % 1000) / 100; // Hundres
    caData[3] = iOTP / 1000;         // Thousands


    if (cMessageQueued>0)
    {
      //This should not happen...
      ui->statusbar->showMessage("A message was already submitted to the unit. Please wait for the response.");
      return;
    }
    cMessageQueued=3;  //X seconds for a reply
    ui->statusbar->showMessage("Request address OTP");

    cReturnCode = oMsgFrame->Pack(MSGID_RETRIEVE_ADDRESS_OTP, &caData[0], 4 );
    if (cReturnCode!=0)
    {
      ui->statusbar->showMessage("Could not send the command to the hardware unit");
    }

    //OTP send. Return to the retrieved address details page.
    //Only 1 OTP/retrieve request. Have to initiate a new address retrieve to try again.
    ui->stackwidget_RetrieveAddress->setCurrentWidget(ui->pageRetrieveAddress_MainControl);
  }
  catch (...)
  {
      ui->statusbar->showMessage("An error occurred.");
      Exception();
  }
}

//How to prevent the worker thread variables from being globals?
bool_t  bCancelTransfer=false;
bool_t  bTick=FALSE;
bool_t  bComplete=FALSE;
int     iWorker_PacketNr;     //Current packet nr
int     iWorker_PacketsTotal; //Total nr of packets
int     iWorker_Retries;      //Nr of resyncs
int     iFileSize;
bool_t  bWorker_Success;
uint8_t cWorker_error=0;      //cError: 0 - No error
                              //        1 - File doesn't exist
                              //        2 - Not a regular file
                              //        3 - File name too long
                              //        4 - Serial port string too long
                              //        5 - Serial port string too long
                              //        6 - Could not open the serial port.
                              //        7 - Could not open the file
                              //        8 - Could not send the data to the unit
                              //        9 - Unexpected response from the unit
                              //       10 - Timeout waiting for response from the unit
                              //       11 - Requested packet nr exceeds the filesize
                              //       12 - Could not seek or read within the file
                              //       13 - Checksum failed

//Worker::Worker(QObject* thingy, QObject* parent)
Worker::Worker(QString* oFileToSend,
               QString* oSerialPort,
               QObject* )
    : QObject(nullptr)    //no real parent, so we can move to a different thread
{
  try
  {
    int ret;
    struct stat st;

    bCancelTransfer=false;
    cWorker_error=0;    
    iWorker_PacketNr=0;
    iWorker_PacketsTotal=0;
    iWorker_Retries=0;
    bWorker_Success=FALSE;


    sFileToSend = sFileToSend.asprintf("%s", oFileToSend->toLocal8Bit().data() );
    sSerialPort = sSerialPort.asprintf("%s", oSerialPort->toLocal8Bit().data() );


    //FIXIT: Check if the serial port exist && that nobody else is connected to it
    if ( (oFileToSend->length()==0) || (oFileToSend->length()>=254) )
    {
      cWorker_error=1; //File name too long
      return;
    }
    ret = stat(sFileToSend.toLocal8Bit().data(), &st); //Access c-style char array of QString
    if (ret == -1)
    {
      cWorker_error=2; //File doesn't exist
      return;
    }
    else
    {
      if (S_ISDIR(st.st_mode))
      {
        cWorker_error=3; //Not a regular file
        return;
      }
      else if (!S_ISREG(st.st_mode))
      {
        cWorker_error=3; //Not a regular file
        return;
      }
    }

    if (st.st_size >= (65536 * (COMMS_BUFFER_SIZE-10) ) )
    {
        cWorker_error=4; //File too large
        return;
    }

    if ( (oSerialPort->length()==0) || (oSerialPort->length()>=254) )
    {
      cWorker_error=5; //Serial port string too long
      return;
    }

    bTick=FALSE;
    iWorker_PacketNr=0;
    iWorker_PacketsTotal = st.st_size / COMMS_BUFFER_SIZE;
    iWorker_Retries=0;
    iFileSize = st.st_size;


  }
  catch (...)
  {
    fprintf(stderr, "Worker::Worker exception");
  }
}


void Worker::doWork()
{

  uint32_t wFilePosition=0;
  uint32_t wFileSize=0;;
  struct stat st;

  uint16_t iSyncCount=0;
  uint16_t iPacketNr=0;

  size_t tRead=0;;
  int16_t iReturnCode=0;;
  int8_t cReturnCode=0;;
  char_t caPort[255];
//    char_t caPort[20]="vmodem1";
  uint8_t caData[ COMMS_BUFFER_SIZE+20 ];
  uint8_t cMsgID=0;;

  FILE *pFH=nullptr;
  uint16_t iCounter=0;
  uint8_t caMD5[33];

  char caFilename[255]; //Tested in previous function if filename <= 254


  QThread* thisthread = this->thread();
  try
  {
    //printf("thisthread starting\n");

    memset(caPort,0,sizeof(caPort));
    memcpy(&caPort[0], sSerialPort.toLocal8Bit().data(), (size_t)sSerialPort.length()  );

    memset(caFilename,0,sizeof(caFilename));
    memcpy(&caFilename[0], sFileToSend.toLocal8Bit().data(), (size_t)sFileToSend.length() );

    stat(&caFilename[0], &st);
    wFileSize = st.st_size;
    if (wFileSize==0)
    {
        printf("%s doesn't exist\n",&caFilename[0]);
        goto EXIT;
    }
    if (wFileSize >= (65536 * (COMMS_BUFFER_SIZE-10) ) )
    {
        printf("%s is too large. Max size=%d\n",&caFilename[0], (65536 * (COMMS_BUFFER_SIZE-10)) );
        goto EXIT;
    }

    iWorker_PacketsTotal = wFileSize / (COMMS_BUFFER_SIZE-10);

    cReturnCode = md5sum(&caFilename[0], &caMD5[0]);
    if (cReturnCode != 0)
    {
      printf("Failed to calculate the MD5 of the file\n");
      goto EXIT;
    }

    pFH = fopen (&caFilename[0], "rb");
    if (pFH==nullptr)
    {
        printf("Could not open file for reading\n");
        goto EXIT;
    }

    cReturnCode = CommsFiletx_Init( &caPort[0] );
    iCounter=0;
    while(1)
    {
      iReturnCode = CommsFiletx_Unpack(&caData[0], sizeof(caData), &cMsgID);
      if (iReturnCode>0)
      {
        //printf("\nFound response. MsgID=%s, Size= %d after %u ms\n",CommsFileTx_PrintMsgID(cMsgID),iReturnCode,iCounter);

        if (cMsgID==FILETX_ID_SYNC)
        {
          iSyncCount++;
          iWorker_Retries++;
          printf("Resync. Challenge=%u, resync count=%u\n",caData[0],iSyncCount);
          iReturnCode = CommsFiletx_Pack(&caData[0], 1, FILETX_ID_SYNC);
          if (iReturnCode != 1)
          {
            printf("Sync: Tx error: %d\n",iReturnCode);
          }
        }
        else if (cMsgID==FILETX_ID_START_UPGRADE)
        {
          //printf("Start transmission\n");
          if (pFH==nullptr)
          {
            printf("Error: File handle should be open\n");
            usleep(10000);
            goto EXIT;
          }
          iReturnCode = fseek(pFH,0,SEEK_SET);
          if (iReturnCode != 0)
          {
            printf("Could not seek to beginning of the file\n" );
            usleep(10000);
            fclose(pFH);
            goto EXIT;
          }

          iReturnCode = CommsFiletx_Pack(&caMD5[0], 32, FILETX_ID_START_UPGRADE);
          if (iReturnCode != 1)
          {
            printf("Start: Tx error: %d\n",iReturnCode);
          }
          else
          {
            //printf("Start: Sent MD5\n");
          }
        }
        else if (cMsgID==FILETX_ID_DATA)
        {

          iPacketNr  = (uint16_t) caData[0];
          iPacketNr |= (uint16_t)(caData[1]<<8);
          //printf("Requesting packet nr %u\n",iPacketNr);

          //Get the packet data from the file
          wFilePosition= iPacketNr * (COMMS_BUFFER_SIZE-10);
          if ( (wFilePosition+1) >= wFileSize)
          {
            printf("Requested file packet larger than the filesize\n");
            fclose(pFH);
            goto EXIT;
          }

          iReturnCode = fseek(pFH,wFilePosition,SEEK_SET);
          if (iReturnCode != 0)
          {
            printf("Could not seek to file position %u * %u\n",iPacketNr, (COMMS_BUFFER_SIZE-10) );
            fclose(pFH);
            goto EXIT;
          }

          tRead = fread(&caData[2], 1, (COMMS_BUFFER_SIZE-10), pFH);
          if (tRead==0)
          {
            printf("Could not read from the file\n");
            fclose(pFH);
            goto EXIT;
          }

          //Embed the packet nr in the data:
          caData[0] = (uint8_t)iPacketNr;
          caData[1] = (uint8_t)( iPacketNr>>8 );

          if (tRead == (COMMS_BUFFER_SIZE-10))
          {
            bTick=TRUE;
            iWorker_PacketNr = iPacketNr;
            //printf("Sending packet %u, size=%lu\n", iPacketNr, tRead);
            iReturnCode = CommsFiletx_Pack(&caData[0], (tRead+2), FILETX_ID_DATA);
          }
          else //End of file
          {            
            bComplete=TRUE;
            //printf("Sending packet %u, size=%lu -- END OF FILE\n", iPacketNr, tRead);
            iReturnCode = CommsFiletx_Pack(&caData[0], (tRead+2), FILETX_ID_END_OF_FILE);
          }
          if (iReturnCode != 1)
          {
            printf("Could not pack the data for transmission.\n");
            fclose(pFH);
            goto EXIT;
          }
        }
        else if(cMsgID==FILETX_ID_END_OF_TRANSMISSION)
        {
            if (caData[0]==1) //Success
            {
                //printf("Successfull transmission. Packets=%u, Syncs=%u\n",iPacketNr, iSyncCount);
            }
            else
            {
                printf("Transfer failed. Checksum mismatch. Packets=%u, Syncs=%u\n",iPacketNr, iSyncCount);
            }
            cReturnCode = CommsFiletx_Close( );
            goto EXIT;
        }
        else
        {
          printf("Unknown MsgID: 0x%02x\n",cMsgID);
          goto EXIT;
        }
      }

      if (iCounter>=250)
      {
        iCounter=0;
        printf("Timeout. No response from the unit after 2.5 seconds\n");
        goto EXIT;
      }
      usleep(10000);
    }
  }
  catch (...)
  {
      fprintf(stderr, "Worker::doWork() Exception");
      cWorker_error=13; //Exception occurred
  }

EXIT:
  //Worker is finished. Signal the thread to quit.
  if (thisthread!=nullptr)
  {
    thisthread->quit();
    thisthread=nullptr;
  }
  if (pFH != nullptr)
  {
    fclose(pFH);
  }
}

MyController::MyController(QString* oFileToSend, QString* oSerialPort, QObject* parent)
{
  iI=0;

  try
  {
    if (poWorker==nullptr)
    {
      poWorker = new  Worker(oFileToSend, oSerialPort, parent);
      poWorker->moveToThread(&workerThread);
      // provide meaningful debug output
      workerThread.setObjectName("workerThread");
      workerThread.start();
      // the thread starts the event loop and blocks waiting for events

      //Note: Old style connect(x, SIGNAL(), y, SLOT()) doesn't work
      //      to emit a signal to a slot between threads...
      //QObject::connect(poWorker, &Worker::TICK    , this, &MyController::RxTICK,     Qt::QueuedConnection );
      //QObject::connect(poWorker, &Worker::COMPLETE, this, &MyController::RxCOMPLETE, Qt::QueuedConnection );

      timerThread = new QTimer(parent);
      connect ( timerThread, SIGNAL(timeout()),
                this,        SLOT(timerThread_timeout())
              );
      timerThread->start(250);
    }
  }
  catch (...)
  {
    fprintf(stderr, "MyController::MyController() Exception");
  }
}

MyController::~MyController()
{
  workerThread.quit();
  workerThread.wait();
}

void MyController::operate()
{
  // Qt::QueuedConnection ensures that the slot is invoked in its own thread
  QMetaObject::invokeMethod(poWorker, "doWork", Qt::QueuedConnection);
}

void MyController::CancelTransfer()
{
  bCancelTransfer=true;
}

void MyController::timerThread_timeout()
{
  if (bTick==TRUE)
  {
    bTick=FALSE;
    emit TxTICK(iWorker_PacketNr, iWorker_PacketsTotal, iWorker_Retries);
  }
  if (bComplete==TRUE)
  {
    emit TxCOMPLETE(cWorker_error);
    timerThread->stop();
  }

}
