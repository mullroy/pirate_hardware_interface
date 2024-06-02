# Overview
The upgrade procedure can be triggered in two scenarios. You can decide from the connection screen to go directly to the upgrade screen, by setting the radio button to 'Upgrade'. If you've selected the 'Standard' operating mode, and there's a mismatch between the communication version of the wallet and
GUI you'll be taken to the upgrade screen too.<br>
Note: The communication version between the hardware wallet and GUI must match to use the standard functions of the hardware wallet. The application versions may differ. The application version indicate cosmetic changes in the applications themselves, but will not cause an incompatibility between the GUI and hardware wallet.<br>
<b>NOTE:</b> Up to firmware version v3.6 the upgrade procedure erased the device configuration, i.e. your mnemonic seed phrase and password. Make sure you have your mnemonic seed phrase on hand, since you'll need to set up the wallet again after the upgrade. After hardware wallet firmware v3.6 the configuration isn't erased, but the upgrade procedure will not allow downgrades. You'll only be able to load newer firmware.<br>

# Compatible versions
The version numbers of three applications must match for the system to work: Treasure chest, the Qt GUI control application and the hardware wallet software.<br>
The list of version numbers are maintained in the main README.md file<br>

# Revert GUI to earlier versions
If you'd like to revert your GUI application to an earlier version, you can do so by checking out an earlier version in the GIT source repository.
* To see the status of your local repository:<br>
<i>$ git status</i><br>
&nbsp;&nbsp;On branch master<br>
&nbsp;&nbsp;Your branch is up to date with 'origin/master'.<br>

* To get the latest code from GitHUB:<br>
<i>$ git pull</i><br>
&nbsp;&nbsp;Pull the latest updates from the internet into your local copy.<br>
  
* View the tags in the repository:<br>
<i>$ git tag</i><br>
&nbsp;&nbsp;V3.9<br>
&nbsp;&nbsp;V3.8<br>
&nbsp;&nbsp;V2.7<br>
&nbsp;&nbsp;V2.6<br>
&nbsp;&nbsp;V2.5<br>
&nbsp;&nbsp;V2.3<br>
&nbsp;&nbsp;v1.2<br>
  
* Revert to an earlier release:<br>
<i>$ git checkout v3.8</i><br>
&nbsp;&nbsp;Note: switching to 'V3.8'.<br>
&nbsp;&nbsp;You are in 'detached HEAD' state.<br>
Only the source code was altered. You have to rebuild the project (qmake -makefile ; make clean ; make) to get an executable that matches the source code.

* To return to the newest commit (HEAD) of the repository:
<br>$ git switch -</i><br>
&nbsp;&nbsp;Switched to branch 'master'<br>
&nbsp;&nbsp;Your branch is up to date with 'origin/master'.<br>
&nbsp;&nbsp;Remember to do a 'make clean' each time you switch branches or pull new code from the internet.<br>
  
# Perform an upgrade
You will need four items to perform an upgrade of the hardware wallet firmware:<br>
1) Latest version of the GUI application<br> 
2) A hardware wallet<br>
3) The upgrade file, which matches your hardware wallet serial number. Steps to obtain it detailed below.<br>
4) Your mnemonic seed phrase. For wallet firmware prior to v3.6 you'll need your mnemonic seed phrase to set up the wallet again after the upgrade. For later firmware releases the existing configuration isn't erased. It is still good practice to confirm that you can still access your mnemonic from where you've stored it, to have peace of mind that you can restore your funds at any time.<br>

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
If the GUI and wallet versions do match, the wallet will show that a connection was established.<br>
**![Version match](screenshots/upgrade/06_software_version_match.jpg?raw=true "Version mismatch")**<br>

The GUI screen will change to the download page. On the download page there are 2 columns. One for communication version and one for the application version. The communication column with the GUI and wallet versions is important.
**![GUI upgrade page](screenshots/upgrade/08_gui_upgrade.jpg?raw=true "GUI upgrade page")**<br>

Browse to the upgrade file. If you run on Windows/Cygwin, the file dialog shows the directory root of the Cygwin environment. Your home directory is located under /home/your_account. Cygwin maps your native hard drives under /cygdrive, i.e. C: will be at /cygrive/c.<br> Select the upgrade file you've received from the development team. If the security features of the file checks out the version numbers (communication+application) will be displayed on the GUI next to 'Upgrade file version'.<br>
**![GUI upgrade page](screenshots/upgrade/09_gui_upgrade.jpg?raw=true "GUI upgrade page")**<br>

You can initiate the upgrade procedure by clicking on Program. The GUI application will instruct the hardware wallet to initiate the upgrade.<br>
**![GUI upgrade ready](screenshots/upgrade/10_gui_ready.jpg?raw=true "GUI upgrade ready")**<br>
The hardware wallet LCD display will indicate that it's ready to receive the file<br>
**![Upgrade ready](screenshots/upgrade/11_upgrade_ready.jpg?raw=true "Upgrade ready")**<br>

The data transfer will start automatically on the second, high speed, serial port. (The USB cable simulates the serial connections.) The packet transfer progress is displayed on the GUI.<br>
**![GUI packet counter](screenshots/upgrade/12_gui_transferring.jpg?raw=true "GUI packet counter")**<br>
The hardware wallet will acknowledge that the transfer is in progress<br>
**![Transferring](screenshots/upgrade/13_upgrade_transferring.jpg?raw=true "Transferring")**<br>

The GUI status message will show when the transfer completed. From here on, no further interaction with the GUI is required.<br>
**![GUI transfer complete](screenshots/upgrade/14_gui_transfer_complete.jpg?raw=true "GUI transfer complete")**<br>
At this point the upgrade file resides on the hardware wallet. The LCD will prompt you to power cycle the unit, so that the unit can continue with the upgrade process.<br>
**![Transfer complete](screenshots/upgrade/15_transfer_complete.jpg?raw=true "Transfer complete")**<br>

When the hardware wallet powers up, it will show on the LCD that an upgrade is detected. The upgrade runs automatically. No user input is required.<br>
**![Upgrade detected](screenshots/upgrade/16_unit_boot_upgrade.jpg?raw=true "Upgrade detected")**<br>
The wallet verifies the integrity of the file.<br>
**![Verify integrity](screenshots/upgrade/17_unit_boot_upgrade.jpg?raw=true "Verify integrity")**<br>
The wallet extracts the archive.<br>
**![Extract archive](screenshots/upgrade/18_unit_boot_upgrade.jpg?raw=true "Extract archive")**<br>
The wallet deploys the archive.<br>
**![Deploy archive](screenshots/upgrade/19_unit_boot_upgrade.jpg?raw=true "Deploy archive")**<br>
After the upgrade is done the wallet will prompt you to power cycle it.<br>
**![Upgrade complete](screenshots/upgrade/20_unit_boot_upgrade.jpg?raw=true "Upgrade complete")**<br>
Prior to v3.6 the wallet will prompt you to configure your mnemonic seed phrase and password again after the upgrade.<br>
**![Operational](screenshots/upgrade/22_operational-2.jpg?raw=true "Operational")**<br>
