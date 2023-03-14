# Serial/USB driver
https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers<br>
Downloads tab<br>
  Select: CP210x Universal Windows driver<br>
  Extract the zip archive on your local machine<br>
  From Windows Explorer, right-click silabser.inf, select 'Install'  from the menu<br>
Attach the Pirate Hardware wallet via a USB cable to the PC<br>
Open Control Panel->Device Manager<br>
Under Ports (COM&LPT), the USB/Uart driver should be registered:<br>
* Silicon Labs Dual CP2105 USB to UART Bridge Enhanced COM Port (COMx)<br>
Note: The /dev/ttySx inside Cygwin is one nr less than what is shown in Windows Device Manager, i.e. COM5 will be /dev/ttyS4<br>

# Cygwin
https://cygwin.com/install.html<br>
Installing and Updating Cygwin Packages:<br>
Create a local directory on your hard drive to keep the files, i.e. c:\installs<br>
Right click on " setup-x86_64.exe"  and save it into c:\installs<br>
Run the file from windows File explorer<br>
Options:<br> 
* Download source: Install from internal<br>
* Root install directory: c:\cygwin64<br>
   (Will install approx 2.2GB onto the drive)<br>
    Install for all users<br>
* Local package directory: c:\installs\cygwin<br>
   The director will be created.<br>
* Your internet connection: Direct Connection<br>
* Download site: Choose one of the sites. mirrors.kernel.org is a good site.<br>
* Select packages:<br>
   View: Category<br>
   Expand All, Devel<br>
    mingw64-i686-qt5-base  : Change dropdown 'Skip'  to ' 5.9.4-1'<br>
    mingw64-i686-qt5-tools  : Change dropdown 'Skip'  to ' 5.9.4-1'<br>
    mingw64-i686-qt5-serialport  : Change dropdown 'Skip' to ' 5.9.4-1'<br> 
    gcc-g++ : Change dropdown 'Skip'  to '11.3.0-1'<br>
    git  : Change dropdown 'Skip' to '2.39.0-1'<br>
   Expand All, Libs<br>
     libGL-devel : Change dropdown 'Skip'  to '19.1.6-1'<br>
     libssl1.0-devel : Change dropdown 'Skip'  to '1.0.2u-1'<br>
   Expand All, X11<br>
       xorg-server : Change dropdown 'Skip'  to '21.1.4-2'<br>
       xorg-x11-fonts-dpi75 : Change dropdown 'Skip'  to '7.5-4'<br>
       xinit : Change dropdown 'Skip'  to '1.4.2-2'<br>
 <br>
  (Note: The dependency packages will be automatically selected, like 'git'as part of 'gcc-g++' and 'make' as part of 'git')<br>
    Click Next, continue<br>
    Wait for download to complete - approx 340mb<br>
    Installation status and create icons:<br>
       Choose icons to create. Defaults are OK.<br>

# Environmental variables
The path to QT is not in the Cygwin bash shell config file.<br>
Navigate to the installation directory (i.e. c:\cygwin64) -> home -> your_windows_username<br>
Open the .bashrc file with Notepad (Right-click->Open with..., More apps -> Notepad)<br>
At the bottom of the file, add:<br>
PATH=$PATH:/lib/qt5/bin<br>
export DISPLAY=:0.0

# Run cygwin shell
From the start menu or Desktop, launch ' Cygwin64 Terminal'<br>
The bash shell offers a Unix style terminal. Some usefull commands, if you' re not used to this kind of terminal:<br>
$ ls<br>
&nbsp;&nbsp;Directory listing, same as ' dir' <br>
$ pwd<br>
&nbsp;&nbsp;Print working (current) directory, i.e. where are you in the file system<br>
$ cd -path-<br>
&nbsp;&nbsp;Change directory. If you type only 'cd' without a path, you're returned to your home directory<br>
$ which -file-<br>
&nbsp;&nbsp;Shows you where this application is in the filesystem. Only the directories in the PATH variable are searched.<br>

# Verify the PATH
 $  which qmake<br>
      /lib/qt5/bin/qmake<br>
$ which gcc<br>
     /usr/bin/gcc<br>

# Obtain Pirate hardware wallet control software
Branch the git repository:<br>
$ cd<br>
    -- Change directory back to your home directory<br>
$ git clone https://github.com/mullroy/pirate_hardware_interface.git<br>
   -- Clone the repository into pirate_hardware_interface<br>

# Build the software
$ cd pirate_hardware_interface<br>
$ qmake -makefile<br>
   -- Let Qt create a Makefile for the project<br>
$ make<br>
   -- Compile the project<br>
Output: Arrr.exe<br>

# Run the software
Launch the X-windows (xorg, X11) windowing interface for Cygwin to draw into:<br>
$ startxwin &<br>
The DISPLAY variable was already configured in 'Environmental variables' above<br>
With the hardware wallet plugged into the PC, see under which serial port nr it was registered:<br>
$ ls /dev/ttyS*<br>
  /dev/ttyS0  /dev/ttyS3  /dev/ttyS4  <br>
$ cd pirate_hw_interface<br>
$ ./ARRR.exe<br>
    Enter the serial number to use. Use the higher of the 2 values, i.e. /dev/ttyS4 in this example<br>
