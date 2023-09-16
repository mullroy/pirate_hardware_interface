# Serial/USB driver
Navigate with a web browser to https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers<br>
**![Silabs home page](screenshots/01_silabs.jpg?raw=true "Silabs home page")**
Select the 'DOWNLOADS' tab<br>
  Select: CP210x VCP Mac OSX driverr<br>

Launch Finder and navigate to the Downloads directory. Click on the ZIP file to extract it<br>
**![Archive](screenshots/mac/02_driver_install.jpg?raw=true "Driver archive")**

Enter the directory to reveal the dmg archive. Click on SiLabsUSBDriverDisk.dmg to open the installer<br>
**![Archive](screenshots/mac/03_driver_install.jpg?raw=true "Driver archive2")**

Click 'Install CP210x VCP Driver' to launch the installer<br>
**![Install driver](screenshots/mac/04_driver_install.jpg?raw=true "Install driver")**

Accept all the prompts that are presented for steps: Introduction, License, Info and Install<br>
**![Install driver](screenshots/mac/05_driver_install.jpg?raw=true "Install driver")**

On the Guidance page, Mac OSX will inform you: 'System Extension Blocked'.
Click the 'Open Security Preferences' button
**![System Extension](screenshots/mac/06_driver_install.jpg?raw=true "System extension")**

On the Security&Privacy page, click the 'Allow' button to install system
software from: Silicon Laboratories Inc. Close the Security&Privacy window.
**![System Extension](screenshots/mac/07_driver_install.jpg?raw=true "System extension")**

Wait for the driver cache to be rebuild
**![System Extension](screenshots/mac/08_driver_install.jpg?raw=true "Driver cache")**

The driver installation is complete.
**![System Extension](screenshots/mac/09_driver_install.jpg?raw=true "Complete")**
<br><br>
Attach the Pirate Hardware wallet via a micro USB cable to your Mac machine<br>
<br>
Launch the terminal emulator from LaunchPad->Other->Terminal
**![Terminal](screenshots/mac/10_driver_install.jpg?raw=true "Menu system")**<br>

Verify that the device handles to the serial ports are registered:
Enter in the terminal: ls /dev/tty.S*
There should be two entries, with filenames like /dev/tty.SLAB_USBtoUART
**![Serial ports](screenshots/mac/11_driver_install.jpg?raw=true "Serial ports")**

# Development tools
# XCode
Note: OS X plus the development environment takes up approx 44GB of hard drive space<br>

XCode:
For this development OSX Mojave (10.14.6) was used. The latest version of
the XCode development environment for Apple Mac OSX Mojave was 11.3.1.
You'll need an Apple ID to download content from the Apple developer
website. The original URL for the Xcode 11.3.1 is:
https://download.developer.apple.com/Developer_Tools/Xcode_11.3.1/Xcode_11.3.1.xip 

Alternatively, this version of XCode is available on The Wayback Machine at archive.org. 
Browse to archive.org. In the search windows, search for: xcode 11.3.1.
**![Xcode](screenshots/mac/12_xcode.jpg?raw=true "Xcode")**

Select the archived entry. Download the torrent file. Use a torrent
application to download XCode 11.3.1, using the torrent definition file.
**![Xcode](screenshots/mac/13_xcode.jpg?raw=true "Xcode")**

Launch Finder. Navigate to where you've downloaded XCode. Click on Xcode_11.3.1.xip
to extract it. XCode will extract in the current directory as Xcode.app, sized approx. 8.4GB
**![Xcode](screenshots/mac/14_xcode.jpg?raw=true "Xcode")**

Move Xcode.app into your 'Documents' directory. Click on the Xcode icon to launch it.
The license agreement is shown. Accept it. Enter your account password to continue with the installation.
The installation is finished when the 'Welcome to Xcode' screen is displayed.
**![Xcode](screenshots/mac/15_xcode.jpg?raw=true "Xcode")**
     
4) Make a symlink to the 'Applications' directory, so XCode is visible in the menu system:
Launch the terminal emulator
$ cd /Applications
$ ln -s /Users/<your_username>/Documents/Xcode.app .
If your 'Launchpad' application page is very full, you may need to swipe left/right to 
see more of your installed applications.

Make the system aware where Xcode can be located:
$ sudo xcode-select -switch /Applications/Xcode.app
Verify the path: 
$ xcode-select -p
  /Applications/Xcode.app/Contents/Developer

Install the Xcode command line tools:
**![Xcode](screenshots/mac/16_xcode.jpg?raw=true "Xcode")**
$ sudo xcode-select --install
  Select 'Install' in the popup window
  Accept the license agreement

MacPorts:
MacPorts are ports of popular projects to Apple/Mac. We're using some
of the tools for convenience, while others install required dependencies.
**![Xcode](screenshots/mac/17_macports.jpg?raw=true "Xcode")**

With Safari, browse to macports.org
Select 'Installing MacPorts' tab. 
From the menu, select Older OS. Select Mojave. This will download MacPorts-2.8.1-10.14-Mojave.pkg
    
Use Finder to navigate to where you've downloaded the archive.
Click on the filename to start the installer
From the installer, continue through the prompts

The project installs into /opt/local
The main application is: /opt/local/bin/port

Install ports:
The ports are installed system wide. Use your 'root' account to install them. Launch terminal. 
$ sudo /opt/local/bin/port install wget
$ sudo /opt/local/bin/port install joe
$ sudo /opt/local/bin/port install git
$ sudo /opt/local/bin/port install minicom

Qt:
**![Xcode](screenshots/mac/18_qt.jpg?raw=true "Xcode")**
Qt is a cross platform graphic development environment.

With Safari, browse to https://www.qt.io/offline-installers
Select '5.12.x Offline installers' from the menu. Select 
Qt 5.12.12 fro macOS to start the download of the archive.
This will download qt-opensource-mac-x64-5.12.12.dmg

**![Xcode](screenshots/mac/19_qt.jpg?raw=true "Xcode")**
Use Finder to navigate to where you've downloaded the archive.
Click on qt-opensource-mac-x64-5.12.12.dmg to start the installer.
From the installer, continue through the prompts.

The installer expects you to open a Qt developer account. Use protonmail
or skiff.com to set up an annonymous e-mail account.

Specify the installation directory: /Users/<your account>/Documents/Qt5

**![Xcode](screenshots/mac/20_qt.jpg?raw=true "Xcode")**
Components: Qt->Qt 5.12.12->macOS
Developer and Designer Tools
  Qt Creator 5.0.2

Accept license
Start the installation

**![Xcode](screenshots/mac/21_qt.jpg?raw=true "Xcode")**
Make a symlink to the 'Applications' directory, so Qt Creator is visible in the menu system:
Launch the terminal emulator
$ cd /Applications
$ ln -s /Users/<your account>/Documents/Qt5/Qt\ Creator.app /Applications/QtCreator.app


# Environmental variables
The path to the development tools are not set up automatically in your environmental variables.<br>
**![Path](screenshots/mac/22_qt.jpg?raw=true "path")**
Launch the terminal emulator from LaunchPad->Other->Terminal

<br>
Make a backup copy of the file:<br>
$ cp .profile .profile.O<br>
  To restore the backup, you do the opposite: cp .profile.O .profile<br>
<br>
Edit the file with the joe editor.<br>
$ joe .profile<br>
  Use the arrow keys to navigate all the way to the bottom of the file, or jump there <br>
  with the command: ctrl+k followed by v <br>
  To jump to the top of the file, press: ctrl+k followed by u<br>
  <br>
  At the bottom of the file, add the updated PATH. <br>
  <br>
  export PATH="/opt/local/bin:/opt/local/sbin:$PATH"<br>
  export PATH="/Users/<your account>/Documents/Qt5/5.12.12/clang_64/bin:$PATH"<br>
  <br>
  Save the file by pressing: ctrl+k followed by x<br>
  If you want to exit the file without saving, simply press: ctrl+c to return to the shell<br>
  <br>
The config file needs to be reread. There is a command called 'source' to do this:<br>
$ source .profile<br>
The easiest way is to log out of the desktop session and to log back in.<br>

# Verify the environment
All these outputs must be detected, otherwise the build steps for the control software will not work.<br>
**![Environment](screenshots/08_environment.jpg?raw=true "Environment")**<br>
 $ which qmake<br>
     /Users/<your account>/Documents/Qt5/5.12.12/clang_64/bin/qmake
 $ which clang<br>
     /usr/bin/clang<br>


# Obtain Pirate hardware wallet control software
Git repository:<br>
$ cd<br>
    -- Change directory back to your home directory<br>
$ cd Documents    
$ git clone https://github.com/mullroy/pirate_hardware_interface.git<br>
   -- Clone the repository into pirate_hardware_interface<br>
**![GIT clone](screenshots/10.jpg?raw=true "GIT clone")**<br>   
<br>
If there are new software updates published, you can incorporate it into your source tree copy by running:<br>
$ cd ~/pirate_hardware_interface<br>
$ git pull<br>
You will have to rebuild the project again after a source code update

# Build the software
$ cd pirate_hardware_interface<br>
$ qmake -makefile<br>
   -- Let Qt create a Makefile for the project<br>
$ make -j 2<br>
   -- Compile the project, using 2 processor cores. You can increse the number, but the resources<br>
      used will also increase. Do not overload your machine by specifying a number higher than twice <br>
      the number of CPU cores in your machine or the compile will grind to a halt. You can press <br>
      ctrl+c to interrupt the build<br>
      <br>
Output: ARRR.app<br>

Make a symlink to the 'Applications' directory, so ARRR is visible in the menu system:
$ ln -s ${pwd}/ARRR.app /Applications/ARRR.app


# Run the software
From Finder, select ARRR
**![ARRR](screenshots/12.jpg?raw=true "ARRR")**<br>
<br>
The application will try to automatically detect the serial port where the hardware unit is attached. The port (/dev/tty.SLAB_USBtoUARTx) will already be filled in when the application is launched.<br>
Troubleshooting:<br>
If the serial port cannot be detected, see if the operating system detected the ports:
Launch the terminal emulator
$ ls -sal /dev/tty.S*<br>
  /dev/tty.SLAB_USBtoUART /dev/tty.SLAB_USBtoUARTx<br> 


