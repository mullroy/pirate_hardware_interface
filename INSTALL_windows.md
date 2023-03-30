Serial/USB driver
=================
Navigate with a web browser to https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
Select the 'DOWNLOADS' tab
  Select: CP210x Universal Windows driver
  Extract the downloaded zip archive on your local machine
  From Windows Explorer, right-click on silabser.inf and select 'Install' from the menu
  
Attach the Pirate Hardware wallet via a micro USB cable to the PC

Open Control Panel -> Hardware and Sound -> Devices and Printers-> Device Manager
Under Ports (COM&LPT), two USB/UART entries should be shown, each with a unique COMx number, i.e. COM3 and COM4
  * Silicon Labs Dual CP2105 USB to UART Bridge Enhanced COM Port (COMx)
Note: Inside Cygwin, the device handel (/dev/ttySx) is one number less than what is shown in Windows Device Manager, i.e. COM3 will be /dev/ttyS2

CYGWIN
======
Note: The installation takes up approx 2.2GB of hard drive space

Installing (Updating) Cygwin Packages
-------------------------------------
Create a local directory on your hard drive to keep the files, i.e. c:\installs
Navigate with a web browser to https://cygwin.com/install.html
Under the heading 'Installing and updating cygwin...', right click on 'setup-x86_64.exe' and select 'Save target as'
Save it into c:\installs. The file is 1.3MB

Launch setup-x86_64.exe from Windows File Explorer
Options: 
* Download source: Install from Internet
* Root install directory: c:\cygwin64
    Install for all users  
* Local package directory: Change the default entry to:  c:\installs\cygwin
  Click 'Next' to continue. A popup will ask if the directory should be created
* Your internet connection: Direct Connection

The application will download the sites list from the internet.
* Choose Download Site: Choose one of the sites. mirrors.kernel.org is a reliable site.

The application will download the package list. This takes a while the first time your run the application.
* Select packages:
   View: Category
   
   Expand All, Devel
    Search: Type 'mingw64-i686-qt5' into the text box
    mingw64-i686-qt5-base  : Change dropdown 'Skip'  to ' 5.9.4-1' 
    mingw64-i686-qt5-tools  : Change dropdown 'Skip'  to ' 5.9.4-1' 
    mingw64-i686-qt5-serialport  : Change dropdown 'Skip'  to ' 5.9.4-1' 
    
    Search: Press the 'Clear' button, type 'gcc-g++' into the text box
    gcc-g++ : Change dropdown 'Skip'  to '11.3.0-1'
   
    Search: Press the 'Clear' button, type 'git' into the text box
    git  : Change dropdown 'Skip'  to '2.39.0-1'
    
    Search: Press the 'Clear' button, type 'make' into the text box
    make  : Change dropdown 'Skip'  to '4.4.1-1'

  Search: Press the 'Clear' button
  Expand All, Libs
    Search: Type 'libGL' into the text box
    libGL-devel : Change dropdown 'Skip'  to '19.1.6-1' 
       
    Search: Press the 'Clear' button, type 'libssl' into the text box
    libssl1.0-devel : Change dropdown 'Skip'  to '1.0.2u-1' 
  
  Search: Press the 'Clear' button
  Expand All, X11
    Search: Type 'xorg-' into the text box
    xorg-server : Change dropdown 'Skip'  to '21.1.4-2' 
    xorg-x11-fonts-dpi75 : Change dropdown 'Skip'  to '7.5-4' 
    
    Search: Press the 'Clear' button, type 'xinit' into the text box  
    xinit : Change dropdown 'Skip'  to '1.4.2-2' 
    
  Search: Press the 'Clear' button
  Expand All, Editors
    Search: Type 'joe' into the text box
    joe : Change dropdown 'Skip'  to '4.6-1' 
 
  (Note: The dependency packages will be automatically selected)
  Click Next, continue 

Wait for the download to complete - approx 330mb. The files are cached in c:\installs\cygwin
Installation status and create icons:
Choose icons to create. Defaults are OK.


Note: If you'd like to add more packages later, run the setup application again and go through
      all the menus as described above. Select your additional packages during the 'Select packages'
      section

Note: It appears that after the initial install completed, there may still be packages pending.
      Launch the installer again, navigate through all the options till you get to the 'Select Packages'
      screen. Change the View dropdown list to 'Pending'.
      It should say 'Nothing to install or update' in the main window. If there are packages listed,
      select Next to continue. A window will show which package versions will be uninstalled and which 
      new packages will be installed in their place. Continue and wait for the installation to complete.
      
Note: To make sure the packages specified above were all installed, relaunch the installer again, 
      navigate through all the options till you get to the 'Select Packages'. Change the View dropdown
      list to 'Picked'.
      The list should have, apart from other packages you may have chosen to explore the ecosystem:
        gcc-g++, git, joe, libGL-devel, libssl1.0-devel, make, mingw64-i686-gcc-g++, mingw64-i686-qt5-base,
        mingw64-i686-qt5-serialport, mingw64-i686-qt5-tools, xinit, xorg-server-devel, xorg-x11-fonts-dpi75
        
        
      
Run cygwin shell:
From the start menu or Desktop, launch 'Cygwin64 Terminal'
On the first run, your home directory will be created in c:\cygwin64\home\<your_windows_username>
The BASH shell offers a Unix style terminal. 

Some usefull commands, if you' re not used to this kind of terminal:
$ ls  -- Directory listing, same as 'dir' under MS Dos
         Note: In Unix clones, like cygwin, filenames starting with a '.' are hidden. To see 
               all the files in a directory, type: ls -sal 
$ pwd - Print working (current) directory, i.e. where are you in the file system
$ cd <path>  -- Change directory. If you type only 'cd' without a path, you're returned to your
   home directory
$ which <file> -- Shows you where this application is in the filesystem. Only the directories in 
    the PATH variable are searched.
    

Environmental variables
The path to QT is not by default included in the Cygwin environmental variables.
The BASH shell config file (.bashrc) is texed based with UNIX style newlines. 
If you edit this file using Notepad or Wordpad, it will use the Microsoft newline
styel (\r\n) and break the file.

You have to edit this file inside the Cygwin BASH shell, or get a proper editor for 
Windows that doesn't break the newline style. The next steps will use the editors that 
you've installed:
$ cd
  This shoudl return you to your home directory
$ ls -a 
  All the hidden files will be displayed, i.e. .bashrc, .profile, ....

Make a backup copy:
$ cp .bashrc .bashrc.O
  To restore the backup, you do the opposite: cp .bashrc.O .bashrc

Edit the file with the joe editor.
$ joe .bashrc
  Use the arrow keys to navigate all the way to the bottom of the file, or jump there 
  with the command: ctrl+k followed by v 
  To jump to the top of the file, press: ctrl+k followed by u
  
  At the bottom of the file, add the updated PATH. Set the default X11 display path. 
  This is used by the Xwindows display subsystem, which is required by GUI apps:
  
  export PATH=$PATH:/lib/qt5/bin
  export DISPLAY=:0.0
  
  Save the file by pressing: ctrl+k followed by x
  If you want to exit the file without saving, simply press: ctrl+c to return to the shell
  
The config file needs to be reread. There is a command called 'source' to do this, but the 
easiest is to exit the shell and to relaunch it.
    
Verify the environment:
 $ which qmake
     /lib/qt5/bin/qmake
 $ which gcc
     /usr/bin/gcc 
 $ which g++
    /usr/bin/g++
 $ which make
    /usr/bin/make
 $ env | grep DISPLAY
   DISPLAY=:0.0

Obtain Pirate hardware wallet control software
Git repository:
$ cd
    -- Change directory back to your home directory
$ git clone https://github.com/mullroy/pirate_hardware_interface.git
   -- Clone the repository into pirate_hardware_interface

Build software:
$ cd pirate_hardware_interface
$ qmake -makefile
   -- Let Qt create a Makefile for the project
$ make -j 2
   -- Compile the project, using 2 processor cores. You can increse the number, but the resources
      used will also increase. Do not overload your machine by specifying a number higher than twice 
      the number of CPU cores in your machine or the compile will grind to a halt. You can press 
      ctrl+c to interrupt the build
      
Output: Arrr.exe

Run the software:
Launch the X-windows (xorg, X11) windowing interface for Cygwin to draw into:
Select Start->Cygwin-X->XWin Server 
The xwindows sytem runs in the background and will will reside under the hidden icons on the taskbar.

With the hardware wallet plugged into the PC, see under which serial port nr it was registered:
$ ls /dev/ttyS*
  /dev/ttyS0  /dev/ttyS3  /dev/ttyS4

ttyS0 will match COM1, if there is a physical serial port on the motherboard of your PC. 
The other 2 should represent the hardware wallet serial ports.
  
$ cd pirate_hw_interface
$ ./ARRR.exe
    Enter the serial port to use. Its usually the higher of the 2 values, i.e. /dev/ttyS4 in this 
    example, but try the other one too if communication isn't established.
    
