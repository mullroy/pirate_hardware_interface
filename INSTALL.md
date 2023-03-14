Instructions to build from source
---------------------------------
(Linux Ubuntu (deb) example)

# apt-get update
# apt-get install build-essential qt5-qmake qtbase5-dev qtbase5-dev-tools libqt5serialport5 libssl-dev
$ qmake -makefile
$ make

Dependencies
------------
The wallet uses 2 serial ports emulated over the
USB port. The brand is: Silicon Labs CP210x 
The kernel module is called cp210x.

On modern Linux the driver should auto-load once you attache the
hardware wallet to the USB port. You can verify it is loaded 
with the command: 
$ lsmod|grep cp210x
cp210x                 40960  0
usbserial              57344  1 cp210x

To load a module you can do the following:
# depmod -a
  Have to run this only once to build up the module dependency
  database.
# modprobe cp210x  

Setup
-----
On Linux the default user:group permissions for the serial ports
are root:dialout. Your user account doesn't automatically belong
to group 'dialout'.

Add your user to /etc/group, editing the file as user 'root':
  dialout:x:20:<your_user_name>
  i.e.: dialout:x:20:developer

You have to log out of your account and back in before changes to groups
are applied. You can't just close & reopen the terminal window while
staying logged in in your X-windows GUI session.

