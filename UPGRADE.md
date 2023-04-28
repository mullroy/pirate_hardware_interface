# Overview
The upgrade procedure can be triggered in two scenarios. You can decide from the connection screen to go directly to the upgrade screen, by setting the radio button to 'Upgrade'. If you've selected the 'Standard' operating mode, and there's a mismatch between the communication version of the wallet and
GUI you'll be taken to the upgrade screen too.<br>
Note: The communication version between the hardware wallet and GUI must match to use the standard functions of the hardware wallet. The application versions may differ. The application version indicate cosmetic changes in the applications themselves, but will not cause an incompatibility between the GUI and hardware wallet.

# Revert to earlier versions
If you'd like to revert your GUI application to an earlier version, you can do so by checking out an earlier version in the GIT source repository.
* To see the status of your local repository:<br>
<i>$ git status</i><br>
&nbsp;&nbsp;On branch master<br>
&nbsp;&nbsp;Your branch is up to date with 'origin/master'.<br>
  
* View the tags in the repository:<br>
<i>$ git tag</i><br>
&nbsp;&nbsp;v2.3
&nbsp;&nbsp;v1.2
  
* Revert to an earlier release:<br>
<i>$ git checkout v1.2</i><br>
&nbsp;&nbsp;Note: switching to 'v1.2'.<br>
&nbsp;&nbsp;You are in 'detached HEAD' state.<br>
Only the source code was altered. You have to rebuild the project to get an executable that matches the latest code.

* To return to the top of the repository:<br>
<br>$ git switch -</i><br>
&nbsp;&nbsp;Switched to branch 'master'<br>
&nbsp;&nbsp;Your branch is up to date with 'origin/master'.<br>
  
# Perform an upgrade
You will need four items to perform an upgrade of the hardware wallet firmware:<br>
1) Latest version of the GUI application<br> 
2) The hardware wallet<br>
3) The upgrade file, which matches your hardware wallet serial number. Steps to obtain it detailed below.<br>
4) The upgrade erases its configuration. You'll need your mnemonic seed phrase to set up the wallet after the upgrade.<br>

# Obtain the firmware file
Power up your device through the USB connection. The Pirate logo appears.
**![Install driver](screenshots/upgrade/00_unit_boot_logo.jpg?raw=true "Boot logo")**
The serial number appears briefly on the screen. Take a photo of it with your smartphone or record it in a text editor.
[1]
You'll need to send your device serial number to the private Pirate address you've used to purchase your hardware wallet.
The firmware file will be created by the development team. They'll
correspond with you on how to deliver the upgrade file to you.

Perform the upgrade
-------------------
NOTE: The upgrade procedure will erase the device configuration,
i.e. your mnemonic seed phrase and password. Make sure you have 
your mnemonic available, since you'll need to set up the wallet 
again after the upgrade.

Power up the device and let it boot through to the main application. If your mnemonic seed phrase is not yet configured, it will print: "Unit needs to be configured", otherwise it will say "Ready".
At this point you can establish a connection to the hardware wallet with the GUI application.
Note: On newer versions of the hardware wallet firmware it will show the version number on the LCD display.
[2]
Launch the GUI application, with the hardware wallet already attached to your computer. This will ensure that the PC has already identified the serial ports of the hardware wallet. The GUI application attempts to detect the communication ports and already fill in the port number to use on the GUI.
Select the 'Upgrade' radio button and press Connect to continue.
[3]
If the hardware wallet detect the communication, it will show a 
session key on the display
[4]
Enter this session key in the GUI
[5]
The LCD of the hardware wallet will show a warning message if the communication version differs from that of the GUI. This will happen when you're for instance running the latest GUI (v2.3) and the wallet is still running v1.1 or v1.2
[6]
Likewise, the GUI screen will change to the download page. On the download page there are 2 columns. One for communication version and one for the application version. The column containing the communication version of the GUI application and wallet is important.
[7]
Browse to the upgrade file. This is the file you've requested from the development team. If the security features of the file checks out the version numbers (communication+application) will be displayed on the GUI next to 'Upgrade file version'.
[8]
You can initiate the upgrade procedure by clicking on Program.
The GUI application will instruct the hardware wallet to initiate the upgrade. The hardware wallet LCD display will indicate that it's ready to receive the file
[9][10]
If the upgrade can be started on the second serial port the progress of the packets send will be displayed on the GUI, while
the hardware wallet will acknowledge that the transfer is in progress:
[11 - kort packet counter][12]
Once the transfer is completed, the status message on the GUI will say the transfer completed. At this point the upgrade file resides on the unit. The LCD will prompt you to power cycle the unit, so that the unit can continue with the upgrade process.
No further interaction with the GUI is required.
[13][14]
When the device power up, it will show on the LCD that an upgrade file was detected:
[15]
The LCD will show that the wallet has entered upgrade mode. It will verify the integrity of the file
[16]
Extract the upgrade file
[17]
Deploy it onto the internal filesystem of the wallet
[18]
After the upgrade is comlete it will prompt you to power cycle/reboot the unit.
[19]
The unit will boot up in operational mode and prompt you to reconfigure your mnemonic seed phrase and password into the unit.
The newer software release will show the firmware version number on the LCD display.
[21]
