# Linux
# Install Packages
Install the packages through a package manager. On a Ubuntu Linux 22.04 the commands are:<br>
$ sudo apt-get update<br>
$ sudo apt-get install build-essential qt5-qmake qtbase5-dev qtbase5-dev-tools libqt5serialport5 libssl-dev git<br>

# Obtain Pirate hardware wallet control software
Git repository:<br>
$ cd<br>
Change directory back to your home directory<br>
$ git clone https://github.com/mullroy/pirate_hardware_interface.git<br>
Clone the repository into pirate_hardware_interface<br>
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
Output: ARRR

# Dependencies
The wallet uses 2 serial ports emulated over the USB port. The brand is: Silicon Labs CP210x<br>
The kernel module is called cp210x.

On modern Linux the driver should auto-load once you attache the hardware wallet to the USB port. You can verify it is loaded with the command:<br> 
$ lsmod|grep cp210x<br>
cp210x                 40960  0<br>
usbserial              57344  1 cp210x<br>

To load a module you can do the following:<br>
$ sudo depmod -a<br>
  Have to run this only once to build up the module dependency database.<br>
$ sudo modprobe cp210x<br>

Setup
-----
On Linux the default user:group permissions for the serial ports are root:dialout. Your user account doesn't automatically belong to group 'dialout'.

Add your user to /etc/group, editing the file as user 'root':<br>
  dialout: x :20:<your_user_name><br>
  i.e.: dialout: x :20:developer<br>

You have to log out of your account and back in before changes to groups are applied. You can't just close & reopen the terminal window while
staying logged in in your X-windows GUI session.
