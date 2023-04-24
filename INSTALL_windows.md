# Serial/USB driver
Navigate with a web browser to https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers<br>
**![Silabs home page](screenshots/01_silabs.jpg?raw=true "Silabs home page")**
Select the 'DOWNLOADS' tab<br>
  Select: CP210x Universal Windows driver<br>
  (Note: Some Windows installations won't recognise the universal driver. In that case, download the 'CP210x Windows Drivers' link)<br>
  Extract the downloaded zip archive on your local machine<br>
  From Windows Explorer, right-click on silabser.inf and select 'Install' from the menu<br>
  **![Install driver](screenshots/02_driver_install.jpg?raw=true "Install driver")**
  <br>
  Accept all the prompts that are presented to you:<br>
  **![Install driver](screenshots/03_driver_install.jpg?raw=true "Install driver")**
  <br><br>
Attach the Pirate Hardware wallet via a micro USB cable to the PC<br>
<br>
From the Start menu, select Control Panel<br>
**![Start menu](screenshots/04_step1.jpg?raw=true "Start menu")**<br>
Under section: Hardware and Sound, Heading: Devices and Printers, Select Device Manager<br>
**![Control Panel](screenshots/04_step2.jpg?raw=true "Control Panel")**<br>
Under Ports (COM&LPT), two USB/UART entries should be shown, each with a unique COMx number, i.e. COM3 and COM4<br>
  * Silicon Labs Dual CP2105 USB to UART Bridge Enhanced COM Port (COMx)<br>
Note: Inside Cygwin, the device handle (/dev/ttySx) is one number less than what is shown in Windows Device Manager, i.e. COM3 will be /dev/ttyS2<br>
**![Device Manager](screenshots/04_step3.jpg?raw=true "Device Manager")**<br>
# Cygwin
Note: The installation takes up approx 2.2GB of hard drive space<br>

# Installing (Update) Cygwin Packages
Create a local directory on your hard drive to keep the files, i.e. c:\installs<br>
Navigate with a web browser to https://cygwin.com/install.html<br>
**![Cygwin home page](screenshots/05_step1.jpg?raw=true "Cygwin home page")**<br>
Under the heading 'Installing and updating cygwin...', right click on 'setup-x86_64.exe' and select 'Save target as'<br>
Save it into c:\installs. The file is 1.3MB<br>
**![Cygwin installer](screenshots/05_step2.jpg?raw=true "Cygwin installer")**<br>
<br>
Launch setup-x86_64.exe from Windows File Explorer.<br>
Accept the prompt about the unknown publisher<br>
**![Cygwin installer](screenshots/06_step1.jpg?raw=true "Cygwin installer")**<br>
Options: <br>
* Download source: Install from Internet<br>
**![Cygwin options](screenshots/06_step2.jpg?raw=true "Cygwin options")**<br>
* Root install directory: c:\cygwin64<br>
    Install for all users  <br>
**![Cygwin options](screenshots/06_step3.jpg?raw=true "Cygwin options")**<br>
* Local package directory: Change the default entry to:  c:\installs\cygwin<br>
  Click 'Next' to continue. A popup will ask if the directory should be created<br>
**![Cygwin options](screenshots/06_step4.jpg?raw=true "Cygwin options")**<br>
* Your internet connection: Direct Connection<br>
**![Cygwin options](screenshots/06_step5.jpg?raw=true "Cygwin options")**<br>
The application will download the sites list from the internet.<br>
* Choose Download Site: Choose one of the sites. mirrors.kernel.org is a reliable site.<br>
**![Cygwin options](screenshots/06_step7.jpg?raw=true "Cygwin options")**<br>
The application will download the package list. This takes a while the first time your run the application.<br>
* Select packages:<br>
   View: Category<br>
   Expand All, Devel<br>
    Search: Type 'mingw64-i686-qt5' into the text box<br>
    mingw64-i686-qt5-base  : Change dropdown 'Skip'  to ' 5.9.4-1' <br>
    mingw64-i686-qt5-tools  : Change dropdown 'Skip'  to ' 5.9.4-1' <br>
    mingw64-i686-qt5-serialport  : Change dropdown 'Skip'  to ' 5.9.4-1' <br>
**![Cygwin options](screenshots/06_step8.jpg?raw=true "Cygwin options")**<br>
    Search: Press the 'Clear' button, type 'gcc-g++' into the text box<br>
    gcc-g++ : Change dropdown 'Skip'  to '11.3.0-1'<br>
    Search: Press the 'Clear' button, type 'git' into the text box<br>
    git  : Change dropdown 'Skip'  to '2.39.0-1'<br>
    Search: Press the 'Clear' button, type 'make' into the text box<br>
    make  : Change dropdown 'Skip'  to '4.4.1-1'<br>
  -Search: Press the 'Clear' button<br>
  -Expand All, Libs<br>
    Search: Type 'libGL' into the text box<br>
    libGL-devel : Change dropdown 'Skip'  to '19.1.6-1' <br>
    Search: Press the 'Clear' button, type 'libssl' into the text box<br>
    libssl1.0-devel : Change dropdown 'Skip'  to '1.0.2u-1' <br>
  -Search: Press the 'Clear' button<br>
  -Expand All, X11<br>
    Search: Type 'xorg-' into the text box<br>
    xorg-server : Change dropdown 'Skip'  to '21.1.4-2' <br>
    xorg-x11-fonts-dpi75 : Change dropdown 'Skip'  to '7.5-4' <br>
    Search: Press the 'Clear' button, type 'xinit' into the text box  <br>
    xinit : Change dropdown 'Skip'  to '1.4.2-2' <br>
  -Search: Press the 'Clear' button<br>
  -Expand All, Editors<br>
    Search: Type 'joe' into the text box<br>
    joe : Change dropdown 'Skip'  to '4.6-1' <br>
  Note: The dependency packages will be selected automatically
  Click Next, continue <br>
* Wait for the download to complete - approx 330mb. The files are cached in c:\installs\cygwin<br>
During the installation some files may not have downloaded. When presented with the list, press the retry button to download those files again.<br>
**![Cygwin options](screenshots/06_stepa.jpg?raw=true "Cygwin options")**<br>
* Installation status and create icons:<br>
Choose icons to create. Defaults are OK.<br>
**![Cygwin options](screenshots/06_stepb.jpg?raw=true "Cygwin options")**<br>
Note: If you'd like to add more packages later, run the setup application again and go through<br>
      all the menus as described above. Select your additional packages during the 'Select packages'<br>
      section<br>
It appears that after the initial install completed, there may still be packages pending.<br>
Launch the installer again, navigate through all the options untill you get to 'Select Packages'<br>
Change the View dropdown list to 'Pending'.<br>
It should say 'Nothing to install or update' in the main window. If there are packages listed,<br>
select Next to continue. A window will show which package versions will be uninstalled and which <br>
new packages will be installed in their place. Continue and wait for the installation to complete.<br>
**![Cygwin pending](screenshots/08.jpg?raw=true "Cygwin pending")**<br>
Note: To make sure the packages specified above were all installed, relaunch the installer again, <br>
      navigate through all the options untill you get to 'Select Packages'. Change the View dropdown<br>
      list to 'Picked'.<br>
      The list should contain, apart from other packages you may have chosen to explore the ecosystem:<br>
        gcc-g++, git, joe, libGL-devel, libssl1.0-devel, make, mingw64-i686-gcc-g++, mingw64-i686-qt5-base,<br>
        mingw64-i686-qt5-serialport, mingw64-i686-qt5-tools, xinit, xorg-server-devel, xorg-x11-fonts-dpi75<br>
# Run cygwin shell:
From the start menu or Desktop, launch 'Cygwin64 Terminal'<br>
On the first run, your home directory will be created in c:\cygwin64\home\ <your_windows_username><br>
The BASH shell offers a Unix style terminal. <br>
<br>
Some usefull commands, if you' re not used to this kind of terminal:<br>
$ ls<br>
&nbsp;&nbsp;Directory listing, same as 'dir' under MS Dos<br>
&nbsp;&nbsp;Note: In Unix clones, like cygwin, filenames starting with a '.' are hidden. To see <br>
&nbsp;&nbsp;all the files in a directory, type: ls -sal <br>
$ pwd<br>
&nbsp;&nbsp;Print working (current) directory, i.e. where are you in the file system<br>
$ cd &lt;path&gt;<br>
&nbsp;&nbsp;Change directory. If you type only 'cd' without a path, you're returned to your<br>
&nbsp;&nbsp;home directory<br>
$ which &lt;file&gt;<br>
&nbsp;&nbsp;Shows you where this application is in the filesystem. Only the directories in <br>
&nbsp;&nbsp;the PATH variable are searched.<br>

# Environmental variables
The path to QT is not by default included in the Cygwin environmental variables.<br>
The BASH shell config file (.bashrc) is text based with UNIX style newlines. <br>
If you edit this file using Notepad or Wordpad, it will use the Microsoft newline<br>
style (\r\n) and break the file.<br>
<br>
You have to edit this file inside the Cygwin BASH shell, or get a proper editor for <br>
Windows that doesn't break the newline style. The next steps will use the joe editor that <br>
you've installed:<br>
$ cd<br>
  This shoudl return you to your home directory<br>
$ ls -a <br>
  All the hidden files will be displayed, i.e. .bashrc, .profile, ....<br>
<br>
Make a backup copy:<br>
$ cp .bashrc .bashrc.O<br>
  To restore the backup, you do the opposite: cp .bashrc.O .bashrc<br>
<br>
Edit the file with the joe editor.<br>
$ joe .bashrc<br>
  Use the arrow keys to navigate all the way to the bottom of the file, or jump there <br>
  with the command: ctrl+k followed by v <br>
  To jump to the top of the file, press: ctrl+k followed by u<br>
  <br>
  At the bottom of the file, add the updated PATH. Also set the default X11 display path. <br>
  This is used by the Xwindows display subsystem, which is required by GUI apps:<br>
  <br>
  export PATH=$PATH:/lib/qt5/bin<br>
  export DISPLAY=:0.0<br>
  <br>
  Save the file by pressing: ctrl+k followed by x<br>
  If you want to exit the file without saving, simply press: ctrl+c to return to the shell<br>
  <br>
The config file needs to be reread. There is a command called 'source' to do this, but the <br>
easiest is to exit the shell and to relaunch it.<br>

# Verify the environment
 $ which qmake<br>
     /lib/qt5/bin/qmake<br>
 $ which gcc<br>
     /usr/bin/gcc <br>
 $ which g++<br>
    /usr/bin/g++<br>
 $ which make<br>
    /usr/bin/make<br>
 $ env | grep DISPLAY<br>
   DISPLAY=:0.0<br>

# Obtain Pirate hardware wallet control software
Git repository:<br>
$ cd<br>
    -- Change directory back to your home directory<br>
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
Output: Arrr.exe<br>

# Run the software
Launch the X-windows (xorg, X11) windowing interface for Cygwin to draw into:<br>
Select Start->Cygwin-X->XWin Server <br>
**![Xwindows](screenshots/11.jpg?raw=true "Xwindows")**<br>
The xwindows sytem runs in the background and will will reside under the hidden icons on the taskbar.<br>
**![Xwindows icon](screenshots/09_step2.jpg?raw=true "Xwindows icon")**<br>
<br>
With the hardware wallet plugged into the PC, see under which serial port nr it was registered:<br>
$ ls /dev/ttyS*<br>
  /dev/ttyS0  /dev/ttyS3  /dev/ttyS4<br>
<br>
ttyS0 will match COM1, if there is a physical serial port on the motherboard of your PC. <br>
The other 2 should represent the hardware wallet serial ports.<br>
  <br>
$ cd pirate_hw_interface<br>
$ ./ARRR.exe<br>
    Enter the serial port to use. Its usually the higher of the 2 values, i.e. /dev/ttyS4 in this <br>
    example, but try the other one too if communication isn't established.<br>
**![ARRR](screenshots/12.jpg?raw=true "ARRR")**<br>
