#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>
#include <QMainWindow>
#include <QThread>
#include <QObject>
#include "BaseTypes.h"
#include "communication/message_framing.h"
#include "communication/comms_filetx.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define COMMUNICATION_VERSION 0x03
#define APPLICATION_VERSION   0x08

// GUI:   2.3 - 2.7
// Wallet 2.4
//   Pirate  ver 1

// Treasure chest: 5.8.0
// GUI:   3.8
// Wallet 3.5
//   Pirate  ver 2
//   Dero    ver 1
//   Radiant ver 1

class Worker : public QObject
{
    Q_OBJECT
public:
    //Worker(QObject* thingy, QObject* parent = nullptr);
    Worker(QString* oFileToSend, QString* oSerialPort, QObject* parent = nullptr);
    ~Worker() override;

private:
    //QObject* mythingy;
    QString sFileToSend;
    QString sSerialPort;

    static bool tick_cb(const char *fname,long bytes_sent, long bytes_total, long last_bps, int min_left, int sec_left);
    static void complete_cb(char *filename, int result, size_t size, time_t date);

private Q_SLOTS:
    void doWork();

};

class MyController : public QObject
{
  //Note: Compile fails if slots & signals are defined and this keyword(Q_OBJECT) is not
  //      present...
  Q_OBJECT
public:
    MyController();
    ~MyController() override;
    int8_t StartTransfer(QString* oFileToSend, QString* oSerialPort, QObject* parent = nullptr);
    void CancelTransfer();
    void operate();
private:
    QThread workerThread;
    Worker  *poWorker=nullptr;
    int iI;
    QTimer *timerThread;


public Q_SLOTS:
  void timerThread_timeout();

Q_SIGNALS:
  void TxTICK(int,int,int);
  void TxCOMPLETE(uint8_t);


};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void paintEvent(QPaintEvent *event) override;
  void SetupMnemonicDisplay();
  //void populate_password_on_gui(uint8_t *pcaData);
  void SetupMenu();

  int8_t getColumn(QString sColumn);
  int8_t getRow(QString sRow);

private:
  Ui::MainWindow *ui;
  int32_t   wTimerConnectCount;
  bool_t    bConnectionCancel;
  uint8_t   iPeriodicCounter;
  bool_t    bConnectionTypeStandard;

  uint8_t   cUpgradeStatus; //0=Idle

  bool_t    bHandshake;
  bool_t    bSessionKey;
  bool_t    bLoggedIn;
  bool_t    bCapturePasswordForLogin;  //False=capture password for registration, True=Capture password for login
  QString   sPassword;
  uint8_t   caSessionID[10];
  int8_t    cMessageQueued = 0; //How many seconds remaining till timeout of the message?

  QTimer    *timerConnect;
  QTimer    *timerSendPeriodicMsgs;  
  QTimer    *timerSignProgress;  
  MsgFrame_Widget *oMsgFrame;

  uint8_t    cEmojiPosition;

  uint8_t    caMnemonicViewed[24];
  uint8_t    cFingerprintStage;     //0: Idle, 1:Request initialise, 2: Request fingerprint capture
  uint8_t    cRecordFinger;         //Which finger to record: 0..3

  QPixmap    _pixmapBg;


  QString    sUpgradePort="";
  Worker*    myworker;
  QString    sCompleteFilename;

  uint8_t    cWalletVersionApplication;
  uint8_t    cWalletVersionCommunication;
  uint8_t    cUpgradeVersionApplication=0;
  uint8_t    cUpgradeVersionCommunication=0;

  int8_t Verify_Upgrade_Signature(QString sUpgradeFile, uint8_t *pcFileCommsVersion, uint8_t *pcFileAppVersion);
  int8_t Setup_GUI_for_upgrade();
  int8_t CloseSerialPort();
  void   Exception();
  void   ResolveSerialPorts();
  bool   eventFilter(QObject *Object, QEvent *Event) override;
  void   close_connection();
  void   stylesheet();
  
  QString exec(const char* cmd);
  MyController *poMyController=nullptr;



private slots:
  void filetx_tick(int iPacketNr, int iPacketsTotal, int iRetries);
  void filetx_complete(uint8_t iSuccess);

  void btConnect_clicked();
  void btCancel_clicked(void);
  void btDisconnect_clicked();
  void Switchto_pageSign();
  void Switchto_pageAddress();
  void btDownload_Browse_clicked();
  void btDownload_Start_clicked();


  void timerConnect_timeout(void);
  void timerSendPeriodicMsgs_timeout(void);
  void timerSignProgress_timeout(void);


  void message_framedetected(uint8_t, uint8_t*, uint16_t);
  void btSign_Sign_clicked();
  void btSign_Back_clicked();
  void btSign_Next_clicked();
  void btSign_OTP_clicked();
  void btSign_Clear_clicked();


  void btSetup_clicked();

  void btSetupMnemonic2_clicked();
  void btSetupMnemonic3_clicked();
  void btSetupMnemonic3_Previous_clicked();
  void btSetupMnemonic3_Next_clicked();

  void btRestoreMnemonic_Previous_clicked();
  void btRestoreMnemonic_Next_clicked();
  void btRestoreMnemonic_Clear_clicked();
  void btRestoreMnemonic_Submit_clicked();
  void btRestoreMnemonic_Left_clicked();
  void btRestoreMnemonic_Right_clicked();

  void btSetupMnemonic4_Previous_clicked();
  void btSetupMnemonic4_Next_clicked();
  void btSetupMnemonic4_Continue_clicked();

  void btSetupPassword_Generate_clicked();
  void btSetupPassword_Continue_clicked();

  void btPassword_Up_clicked();
  void btPassword_Down_clicked();
  void btPassword_Left_clicked();
  void btPassword_Right_clicked();
  void btPassword_Select_clicked();
  void btPassword_Reset_clicked();


  void btVerifyPassword_Continue_clicked();
  void btVerifyPassword_Back_clicked();

  void btApplyConfig_Start_clicked();

//  void btLogin_Clear_clicked();
//  void btLogin_Select_clicked();

  void btRetrieveAddress_clicked();
  void btRetrieveAddressOTP_clicked();


};
#endif // MAINWINDOW_H
