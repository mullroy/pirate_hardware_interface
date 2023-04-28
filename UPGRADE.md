# Overview
The upgrade procedure can be triggered in two scenarios. You can decide from the connection screen to go directly to the upgrade screen, by setting the radio button to 'Upgrade'. If you've selected the 'Standard' operating mode, and there's a mismatch between the communication version of the wallet and
GUI you'll be taken to the upgrade screen too.<br>
Note: The communication version between the hardware wallet and GUI must match to use the standard functions of the hardware wallet. The application versions may differ. The application version indicate cosmetic changes in the applications themselves, but will not cause an incompatibility between the GUI and hardware wallet.<br>
<b>NOTE:</b> The upgrade procedure will erase the device configuration, i.e. your mnemonic seed phrase and password. Make sure you have your mnemonic seed phrase on hand, since you'll need to set up the wallet again after the upgrade.<br>

# Revert GUI to earlier versions
If you'd like to revert your GUI application to an earlier version, you can do so by checking out an earlier version in the GIT source repository.
* To see the status of your local repository:<br>
<i>$ git status</i><br>
&nbsp;&nbsp;On branch master<br>
&nbsp;&nbsp;Your branch is up to date with 'origin/master'.<br>
  
* View the tags in the repository:<br>
<i>$ git tag</i><br>
&nbsp;&nbsp;v2.3<br>
&nbsp;&nbsp;v1.2<br>
  
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
2) A hardware wallet<br>
3) The upgrade file, which matches your hardware wallet serial number. Steps to obtain it detailed below.<br>
4) Your mnemonic seed phrase. You'll need your mnemonic seed phrase to set up the wallet again after the upgrade.<br>

# Obtain the firmware file
Power up your device through the USB connection. The Pirate logo appears.<br>
**![Boot logo](screenshots/upgrade/00_unit_boot_logo.jpg?raw=true "Boot logo")**<br>
The serial number appears briefly on the screen. Take a photo of it with your smartphone or record it in a text editor.<br>
**![Serial number](screenshots/upgrade/01_unit_boot_serial.jpg?raw=true "Serial number")**<br>
You'll need to send your device serial number to the private Pirate address you've used to purchase your hardware wallet. The firmware file will be created by the development team. They'll correspond with you to determine the best way to send the upgrade file to you.

# Perform the upgrade
Power up the device and let it boot through to the main application. If your mnemonic seed phrase is not yet configured, it will print: "Unit needs to be configured", otherwise it will say "Ready".<br>
At this point you can establish a connection to the hardware wallet with the GUI application.<br>
Note: On newer versions the text was change to display the firmware version and the text changed to 'Please setup your mnemonic(seed phrase) and password).<br>
**![Connection promp](screenshots/upgrade/02_unit_needs_to_be_configured.jpg?raw=true "Connection prompt")**<br>

Launch the GUI application, with the hardware wallet already attached to your computer. This will ensure that the PC has already identified the serial ports of the hardware wallet. The GUI application attempts to detect the communication ports and already fill in the port number to use on the GUI.
Select the 'Upgrade' radio button and press Connect to continue.<br>
**![GUI startup](screenshots/upgrade/03_gui_startup.jpg?raw=true "GUI startup")**<br>

If the hardware wallet detect the communication, it will show a session key on the display<br>
**![Session key](screenshots/upgrade/04_connection_detected.jpg?raw=true "Session key")**<br>
Enter this session key in the GUI<br>
**![GUI session key](screenshots/upgrade/05_gui_session.jpg?raw=true "GUI session key")**<br>
The LCD of the hardware wallet will show a warning message if the communication version differs from that of the GUI. This will happen when you're for instance running the latest GUI (v2.3) and the wallet is still running v1.1 or v1.2.<br> 
**![Version mismatch](screenshots/upgrade/07_software_version_mismatch.jpg?raw=true "Version mismatch")**<br>
If the GUI and wallet versions do match, the wallet will show that the connection was established.<br>
**![Version match](screenshots/upgrade/06_software_version_match.jpg?raw=true "Version mismatch")**<br>

Likewise, the GUI screen will change to the download page. On the download page there are 2 columns. One for communication version and one for the application version. The communication column with the GUI and wallet versions is important.
**![GUI upgrade page](screenshots/upgrade/08_gui_upgrade.jpg?raw=true "GUI upgrade page")**<br>

Browse to the upgrade file. This is the file you've requested from the development team. If the security features of the file checks out the version numbers (communication+application) will be displayed on the GUI next to 'Upgrade file version'.<br>
**![GUI upgrade page](screenshots/upgrade/09_gui_upgrade.jpg?raw=true "GUI upgrade page")**<br>

You can initiate the upgrade procedure by clicking on Program. The GUI application will instruct the hardware wallet to initiate the upgrade.<br>
**![GUI upgrade ready](screenshots/upgrade/10_gui_ready.jpg?raw=true "GUI upgrade ready")**<br>
The hardware wallet LCD display will indicate that it's ready to receive the file<br>
**![Upgrade ready](screenshots/upgrade/11_upgrade_ready.jpg?raw=true "Upgrade ready")**<br>

The data transfer will occur on the second, high speed, serial port. The packet transfer progress is displayed on the GUI.<br>
**![GUI packet counter](screenshots/upgrade/12_gui_transferring.jpg?raw=true "GUI packet counter")**<br>
The hardware wallet will acknowledge that the transfer is in progress<br>
**![Transferring](screenshots/upgrade/13_upgrade_transferring.jpg?raw=true "Transferring")**<br>

The GUI status message will show when the transfer completed. From here on, no further interaction with the GUI is required.<br>
**![GUI transfer complete](screenshots/upgrade/14_gui_transfer_complete.jpg?raw=true "GUI transfer complete")**<br>
At this point the upgrade file resides on the hardware wallet. The LCD will prompt you to power cycle the unit, so that the unit can continue with the upgrade process.<br>
**![Res](screenshots/upgrade/15_transfer_complete.jpg?raw=true "GUI transfer complete")**<br>

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
