# Serial/USB driver
Navigate with a web browser to https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers<br>
Select the 'DOWNLOADS' tab. Select: CP210x VCP Mac OSX driver to start the download of the driver.<br>
**![Silabs home page](screenshots/01_silabs.jpg?raw=true "Silabs home page")**

Launch Finder and navigate to the Downloads directory. Click on the ZIP file to extract it.<br>
**![Archive](screenshots/mac/02_driver_install.jpg?raw=true "Driver archive")**

Enter the new directory to reveal the dmg archive. Click on SiLabsUSBDriverDisk.dmg to open the installer.<br>
**![Archive](screenshots/mac/03_driver_install.jpg?raw=true "Driver archive2")**

Click 'Install CP210x VCP Driver' to launch the installer.<br>
**![Install driver](screenshots/mac/04_driver_install.jpg?raw=true "Install driver")**

Accept all the prompts that are presented for steps: Introduction, License, Info and Install.<br>
**![Install driver](screenshots/mac/05_driver_install.jpg?raw=true "Install driver")**

On the Guidance page, Mac OSX will inform you: 'System Extension Blocked'. Click the 'Open Security Preferences' button.<br>
**![System Extension](screenshots/mac/06_driver_install.jpg?raw=true "System extension")**

On the Security&Privacy page, click the 'Allow' button to install system software from: Silicon Laboratories Inc. 
Afterward, close the Security&Privacy window to return to the driver installer.<br>
**![System Extension](screenshots/mac/07_driver_install.jpg?raw=true "System extension")**

Wait for the driver cache to be rebuild.<br>
**![System Extension](screenshots/mac/08_driver_install.jpg?raw=true "Driver cache")**

The driver installation is complete.<br>
**![System Extension](screenshots/mac/09_driver_install.jpg?raw=true "Complete")**
<br>
Verify that the device handles to the serial ports are registered:<br>
Attach the Pirate Hardware wallet via the micro USB cable to your Mac machine. Launch the terminal emulator from<br>
LaunchPad->Other->Terminal<br>
**![Terminal](screenshots/mac/10_driver_install.jpg?raw=true "Menu system")**<br>

Enter at the terminal prompt:<br>
&nbsp;&nbsp;&nbsp;$<b> ls /dev/tty.S*</b><br>
There should be two entries, with filenames like /dev/tty.SLAB_USBtoUART:<br>
**![Serial ports](screenshots/mac/11_driver_install.jpg?raw=true "Serial ports")**

If the two ports are present your machine has detected the hardware wallet successfully.<br>

# Development tools
# XCode
<b>Note:</b> OS X plus the development environment takes up approx 44GB of hard drive space. Make sure you have enough space available on your hard drive before continueing with the installation.<br>

For this development OSX Mojave (10.14.6) was used. The last version of XCode released for Mac OSX Mojave was 11.3.1. <br>
<b>Note:</b> If you're using a new vervion of XCode there was an incompatibility detected with XCode 15 & QT 5. A patched file
is provided inside the Qt5_patch directory.

You'll need an Apple ID to download content from the Apple developer website. The original URL for the Xcode 11.3.1 is: https://download.developer.apple.com/Developer_Tools/Xcode_11.3.1/Xcode_11.3.1.xip 

Alternatively, Xcode 11.3.1 is available on The Wayback Machine at archive.org. Browse to archive.org. In the search windows enter: xcode 11.3.1.<br>
**![Xcode](screenshots/mac/12_xcode.jpg?raw=true "Xcode")**

Select the archived entry. Download the torrent file. Use a torrent application to download XCode 11.3.1, using the torrent definition file obtained from archive.org. The size of the install file is 7.4GB<br>
**![Xcode](screenshots/mac/13_xcode.jpg?raw=true "Xcode")**

After the download is complete, launch Finder. Navigate to where you've downloaded XCode. Click on Xcode_11.3.1.xip to extract it. XCode will extract in the current directory as Xcode.app, sized approx. 8.4GB<br>
**![Xcode](screenshots/mac/14_xcode.jpg?raw=true "Xcode")**

Move Xcode.app into your 'Documents' directory. Click on the Xcode icon to launch it. The license agreement is shown. Accept it. Enter your account password to continue with the installation. The installation is finished when the 'Welcome to Xcode' screen is displayed.<br>
**![Xcode](screenshots/mac/15_xcode.jpg?raw=true "Xcode")**
     
Make a symlink to the 'Applications' directory, so XCode is visible in the Launchpad menu system:<br>
Launch the terminal emulator.<br>
&nbsp;&nbsp;$<b> cd Documents</b><br>
&nbsp;&nbsp;$<b> ln -s $PWD/Xcode.app /Applications/Xcode.app</b><br>
If your 'Launchpad' application page is very full, you may need to swipe left/right to see more of your installed applications.<br>
Make the system aware where Xcode can be located:<br>
&nbsp;&nbsp;$<b> sudo xcode-select -switch /Applications/Xcode.app</b><br>
Verify the path: <br>
&nbsp;&nbsp;$<b> xcode-select -p</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;/Applications/Xcode.app/Contents/Developer<br>
<br>
Install the Xcode command line tools:<br>
**![Xcode](screenshots/mac/16_xcode.jpg?raw=true "Xcode")**
&nbsp;&nbsp;$<b> sudo xcode-select --install</b><br>
&nbsp;&nbsp;Select 'Install' in the popup window<br>
&nbsp;&nbsp;Accept the license agreement<br>
<br>
# MacPorts
MacPorts contain popular projects from other operating systems that were adapted to run on Apple/Mac. We're using some of the tools for convenience, while other install required dependencies.<br>
With Safari, browse to macports.org<br>
**![Xcode](screenshots/mac/17_macports.jpg?raw=true "MacPorts")**
Select 'Installing MacPorts' tab. From the menu, select Older OS. Select Mojave. This will download MacPorts-2.8.1-10.14-Mojave.pkg<br>  
Use Finder to navigate to where you've downloaded the archive. Click on the filename to start the installer. From the installer, continue through the prompts.<br>
The project installs into <b>/opt/local</b>. The main application is: <b>/opt/local/bin/port</b>
<br>
Install ports:<br>
The ports are installed system wide. Use your 'root' account via the 'sudo' command to install them.<br>
OpenSSL is available as part of the ports, but if you're running an ARM CPU in your Mac machine, then there is an issue with Intel x86 cross compiled libraries. It's better to build openssl from source and sidestep the dependency issues.<br>
Launch the terminal emulator.<br>
&nbsp;&nbsp;$<b> sudo /opt/local/bin/port install wget</b><br>
&nbsp;&nbsp;$<b> sudo /opt/local/bin/port install joe</b><br>
&nbsp;&nbsp;$<b> sudo /opt/local/bin/port install git</b><br>
&nbsp;&nbsp;$<b> sudo /opt/local/bin/port install minicom</b><br>
<br>
# OpenSSL
A precompiled openssl is available via MacPorts. However, if you have an ARM CPU in your machine there are incompatibilities where Qt still references the Intel x86 variant of the libraries, which are not installed as part of MacPorts. In the quest to have a uniform installation on all hardware, build OpenSSL from source and install it in the default /usr/local location.<br>
With Safari, browse to openssl.org<br>
**![Xcode](screenshots/mac/23_openssl.jpg?raw=true "OpenSSL")**
Select 'Download' tabl. From the list, select openssl-1.1.1w.tar.gz<br>
Use Finder to navigate to where you've downloaded the archive. Click on the filename to extract the archive.<br>
Launch the terminal emulator. Navigate with the terminal emulator to the location where you've downloaded and extracted the archive.<br>
&nbsp;&nbsp;<b>cd Downloads</b><br>
&nbsp;&nbsp;<b>cd openssl-1.1.1w</b><br>
&nbsp;&nbsp;<b>perl Configure darwin64-x86_64-cc</b><br>
&nbsp;&nbsp;<b>make -j 8</b><br>
&nbsp;&nbsp;<b>sudo make install<b><br>
<br>
# Qt development environment
Qt is a cross platform graphic development environment.<br>
With Safari, browse to https://www.qt.io/offline-installers<br>
**![Xcode](screenshots/mac/18_qt.jpg?raw=true "Qt")**
Select '5.12.x Offline installers' from the menu. Select 'Qt 5.12.12 for macOS' to start the download of the archive. This will download qt-opensource-mac-x64-5.12.12.dmg. The size of the archive is approx. 2.7GB<br>
Launch Finder and navigate to where you've downloaded the archive. Click on qt-opensource-mac-x64-5.12.12.dmg to start the installer.
From the installer, continue through the prompts.<br>
<b>Login:</b> The installer expects you to open a Qt developer account. If you don't want to provide personal information, create a Protonmail or Skiff.com account first. Use the annonymous e-mail account to register a Qt developer account.<br>
<b>Installation Folder:</b> Specify the installation directory: /Users/<your account>/Documents/Qt5<br>
<b>Select components:</b><br>
Qt->Qt 5.12.12->macOS<br>
Developer and Designer Tools->Qt Creator 5.0.2<br>
**![Xcode](screenshots/mac/19_qt.jpg?raw=true "install options")**<br>
Accept license and start the installation.<br>
**![Xcode](screenshots/mac/20_qt.jpg?raw=true "install options")**<br>

Make a symlink to the 'Applications' directory, so that Qt Creator is visible in the Launchpad menu system:<br>
Launch the terminal emulator.<br>
&nbsp;&nbsp;$<b> cd Documents/Qt5</b><br>
&nbsp;&nbsp;$<b> ln -s $PWD/Qt\ Creator.app /Applications/QtCreator.app</b><br>

# Patch
If you're using a new versions than XCode 11, incompatibilities with Qt may be introduced. A bug (https://bugreports.qt.io/browse/QTBUG-117225) was logged on 19 September 2023 regarding an incompatibility with XCode 15 with QT 5 & QT 6. This causes qmake (Qt build script) to fail. A patched version of the file is included in the <b>Qt5_patch</b> directory. This updated file isn't required to be deployed if you're running XCode 11 to 14. For XCode 15 (and beyound?) please navigate to your Qt5 directory (Suggested /Users/<your account>/Documents/Qt5 above) and lower into the sub directories: 5.12.12/clang_64/mkspecs/features<br>
Make a backup copy of your file: cp toolchain.prf toolchain.prf.O<br>
Overwrite toolchain.prf with the copy located under the Qt5_patch directory.<br>

# Environmental variables
The path to the development tools are not set up automatically in your environmental variables. You have to edit the 'profile' of your account and add it there.<br>
Launch the terminal emulator.<br>
Make a backup copy of the file:<br>
&nbsp;&nbsp;$<b> cp .profile .profile.O</b><br>
To restore the backup, you do the opposite: cp .profile.O .profile<br>
<br>
Edit the file with the joe editor.<br>
&nbsp;&nbsp;$<b> joe .profile</b><br>
**![Path](screenshots/mac/21_path.jpg?raw=true "path")**<br>
Use the arrow keys to navigate all the way to the bottom of the file, or jump there with the command: ctrl+k followed by v <br>
To jump to the top of the file, press: ctrl+k followed by u<br> 
At the bottom of the file, add the updated PATH. <br>
<b>export PATH="/opt/local/bin:/opt/local/sbin:$PATH"</b><br>
<b>export PATH="/Users/&lt;your account&gt;/Documents/Qt5/5.12.12/clang_64/bin:$PATH"</b><br>
<br>
Save the file by pressing: ctrl+k followed by x If you want to exit the file without saving, simply press: ctrl+c to return to the shell<br>
<br>
The config file needs to be reread by the system to make it active. There is a command called 'source' to do this:<br>
&nbsp;&nbsp;$<b> source .profile</b><br>
Another way is to log out of the desktop session and to log back in. The config file is read during login<br>
<br>
Verify the environment<br>
All these outputs must be detected, otherwise the build steps for the control software will not work.<br>
&nbsp;&nbsp;$<b> which qmake</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;/Users/&lt;your account&gt;/Documents/Qt5/5.12.12/clang_64/bin/qmake<br>
&nbsp;&nbsp;$<b>which clang</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;/usr/bin/clang<br>
<br>
# Pirate hardware wallet control software
Download the GIT source repository:<br>
**![GIT clone](screenshots/mac/22_git.jpg?raw=true "GIT clone")**<br>
&nbsp;&nbsp;$<b> cd</b><br>
&nbsp;&nbsp;-- Change directory back to your home directory<br>
&nbsp;&nbsp;$<b> cd Documents</b><br>
&nbsp;&nbsp;$<b> git clone https://github.com/mullroy/pirate_hardware_interface.git</b><br>
&nbsp;&nbsp;-- Clone the repository into pirate_hardware_interface<br>
<br>
If there are new software updates published, you can incorporate it into your source tree copy by running:<br>
&nbsp;&nbsp;$<b> cd ~/pirate_hardware_interface</b><br>
&nbsp;&nbsp;$<b> git pull</b><br>
You will have to rebuild the project again after a source code update<br>
<br>
Build the software<br>
&nbsp;&nbsp;$<b> cd pirate_hardware_interface</b><br>
&nbsp;&nbsp;$<b> qmake -makefile</b><br>
&nbsp;&nbsp;-- Qt creates a Makefile for the project<br>
&nbsp;&nbsp;$<b> make -j 2</b><br>
Output: ARRR.app<br>
<br>
Make a symlink to the 'Applications' directory, so that ARRR is visible in the Launchpad menu system:<br>
&nbsp;&nbsp;$<b> ln -s ${pwd}/ARRR.app /Applications/ARRR.app</b><br>
<br>
# Run the software
From Launchpad, select ARRR<br>
**![ARRR](screenshots/12.jpg?raw=true "ARRR")**<br>
<br>
The application will try to automatically detect the serial port where the hardware unit is attached. The port (/dev/tty.SLAB_USBtoUARTx) will already be filled in when the application is launched.<br>
Troubleshooting:<br>
1. If the serial port cannot be detected, make sure the hardware wallet is powered up and LCD display shows that it is ready to accept a connection.<br>
2. See if the operating system detected the ports:<br>
Launch the terminal emulator<br>
&nbsp;&nbsp;$<b> ls -sal /dev/tty.S*</b><br>
&nbsp;&nbsp;&nbsp;&nbsp;/dev/tty.SLAB_USBtoUART /dev/tty.SLAB_USBtoUARTx<br>
