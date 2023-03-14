Serial/USB driver
https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
Downloads tab
  Select: CP210x Universal Windows driver
  Extract the zip archive on your local machine
   From Windows Explorer, right-click silabser.inf, select 'Install'  from the menu
Attach the Pirate Hardware wallet via a USB cable to the PC
Open Control Panel->Device Manager
Under Ports (COM&LPT), the USB/Uart driver should be registered:
* Silicon Labs Dual CP2105 USB to UART Bridge Enhanced COM Port (COMx)
Note: The /dev/ttySx inside Cygwin is one nr less than what is shown in Windows Device Manager, i.e. COM5 will be /dev/ttyS4

https://cygwin.com/install.html
Installing and Updating Cygwin Packages:
Create a local directory on your hard drive to keep the files, i.e. c:\installs
Right click on " setup-x86_64.exe"  and save it into c:\installs
Run the file from windows File explorer
Options: 
* Download source: Install from internal
* Root install directory: c:\cygwin64   
   (Will install approx 2.2GB onto the drive)
    Install for all users
* Local package directory: c:\installs\cygwin
   The director will be created.
* Your internet connection: Direct Connection
* Download site: Choose one of the sites. mirrors.kernel.org is a good site.
* Select packages:
   View: Category
   Expand All, Devel
    mingw64-i686-qt5-base  : Change dropdown 'Skip'  to ' 5.9.4-1' 
    mingw64-i686-qt5-tools  : Change dropdown 'Skip'  to ' 5.9.4-1' 
    mingw64-i686-qt5-serialport  : Change dropdown 'Skip'  to ' 5.9.4-1' 
    gcc-g++ : Change dropdown 'Skip'  to '11.3.0-1'  
    git  : Change dropdown 'Skip'  to '2.39.0-1'  
   Expand All, Libs
     libGL-devel : Change dropdown 'Skip'  to '19.1.6-1' 
     libssl1.0-devel : Change dropdown 'Skip'  to '1.0.2u-1' 
   Expand All, X11
       xorg-server : Change dropdown 'Skip'  to '21.1.4-2' 
       xorg-x11-fonts-dpi75 : Change dropdown 'Skip'  to '7.5-4' 
       xinit : Change dropdown 'Skip'  to '1.4.2-2' 
 
  (Note: The dependency packages will be automatically selected, like 'make'  as part of 'git' 
                 and 'git'  as part of 'gcc-g++')
    Click Next, continue 
    Wait for download to complete - approx 340mb
    Installation status and create icons:
       Choose icons to create. Defaults are OK.

Path
The path to QT is not in the Cygwin file. Navigate to the installation directory (i.e. c:\cygwin64) -> home -> your_windows_username
Open the .bashrc file with Notepad (Right-click->Open with..., More apps -> Notepad)
At the bottom of the file, add:
PATH=$PATH:/lib/qt5/bin

Run cygwin shell:
From the start menu or Desktop, launch ' Cygwin64 Terminal'
The bash shell offers a Unix style terminal. Some usefull commands, if you' re not used to this kind of terminal:
$ ls  -- Directory listing, same as ' dir' 
$ pwd - Print working (current) directory, i.e. where are you in the file system
$ cd <path>  -- Change directory. If you type only 'cd' without a path, you're returned to your    
   home directory
$ which <file> -- Shows you where this application is in the filesystem. Only the directories in 
    the PATH variable are searched.
 Verify the PATHs:
 $  which qmake
      /lib/qt5/bin/qmake
$ which gcc
     /usr/bin/gcc

Obtain Pirate hardware wallet control software
Branch the git repository:
$ cd
    -- Change directory back to your home directory
$ git clone https://github.com/mullroy/pirate_hardware_interface.git
   -- Clone the repository into pirate_hardware_interface

Build software:
$ cd pirate_hardware_interface
$ qmake -makefile
   -- Let Qt create a Makefile for the project
$ make
   -- Compile the project
Output: Arrr.exe

Run the software:
Launch the X-windows (xorg, X11) windowing interface for Cygwin to draw into:
$ startxwin &
$ export DISPLAY=:0.0
With the hardware wallet plugged into the PC, see under which serial port nr it was registered:
$ ls /dev/ttyS*
  /dev/ttyS0  /dev/ttyS3  /dev/ttyS4  
$ cd pirate_hw_interface
$ ./ARRR.exe
    Enter the serial number to use. Use the higher of the 2 values, i.e. /dev/ttyS4 in this 
    example














    