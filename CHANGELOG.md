## [4.10] - 2024-06-02
Communication version 4, GUI application version 10<br>
<b>Note: </b> This version requires a hardware wallet running v4.x firmware.<br>
Changes:<br>
Update transaction protocol with Treasure Chest to version 3. Treasure Chest 5.8.2 is required to exchange transaction data with the GUI application. The commission is stored differently which allows the full transaction amount to be spend, leaving the account with a 0.00 balance.<br>
Addes support for hardware wallet version 4.7
* Process the commission according to the new transaction format
* When the transaction is larger than 10kb multiple packets are send to transfer the transaction input and signed output between the hardware wallet & GUI application.
## [3.9] - 2024-04-28
Communication version 3, GUI application version 9<br>
<b>Note:</b> This version requires a hardware wallet running v3.x firmware.<br>
Changes:<br>
Added support for hardware wallet version 3.6
* The unit serial number is contained in the handshake message
* Support for crypto project added: Radiant
## [3.8] - 2024-02-29
Communication version 3, GUI application version 8<br>
<b>Note:</b> This version requires a hardware wallet running v3.x firmware.<br>
Changes:<br>
Treasure Chest 5.8.0 introduced a new way that the transactions are compiled. This update supports the new transaction format.<br>
This upgrade requires that Treasure Chest, this control GUI and your wallet software are upgraded. See UPGRADE.md to request new firmware for your hardware wallet.<br>
## [2.7] - 2023-10-17
Communication version 2, GUI application version 7<br>
<b>Note:</b> This version requires a hardware wallet running v2.x firmware.<br>
Changes:<br>
Fix issues with the serial communication on newer Mac OSX release.<br>
On Mac hardware running an ARM CPU, fix build instructions to build openssl from source rather than using the version from 'ports'<br>
The build instructions for Mac uses XCode 11. A bug was introduced with XCode 15. In order to build a Qt application, like this one, we've created a patch, located inside: Qt5_patch. Follow the instructions in Qt5_patch/readme.txt on how to deploy the patch.<br>
<br>
## [2.6] - 2023-09-16
Communication version 2, GUI application version 6<br>
<b>Note:</b> This version requires a hardware wallet running v2.x firmware.<br>
Changes:<br>
Added Apple Mac OS X support. See INSTALL_mac.md for instructions to build
the application.<br>
<br>
## [2.5] - 2023-06-28
Communication version 2, GUI application version 5<br>
<b>Note:</b> This version requires a hardware wallet running v2.x firmware.<br>
Changes:<br>
Add keyboard navigation (cursor keys, return, +, /) to the mnemonic restore page.<br>
Increase serial speed of upgrade file transfer<br>
Automatically detect, via handshake, the serial port handle where the hardware<br>
wallet is registered on your computer<br>
<br>
## [2.4] - 2023-05-15
Communication version 2, GUI application version 4<br>
<b>Note:</b> This version requires a hardware wallet running v2.x firmware.<br>
<br>
Changes:
Add keyboard navigation to sign page and address retrieval pages<br>
Fix serial port buffer flooding due to invalid calculation of the wait state<br>
<br>
## [2.3] - 2023-04-26
Communication version 2, GUI application version 3<br>
<b>Note:</b> This version requires the hardware wallet to be upgraded to v2.3 (communication version 2, application version 3). The versions of the applications can differ, as long as their communcation versions match.<br>
<br>
Changes:<br>
Added OTP prompt before retrieving address or sign transaction.<br>
Step through the fields of the transaction prior to signing.<br>
Clean up build warning.<br>
Fix file dialog on Windows (when selecting the upgrade file).<br>
## [1.2] - 2023-03-14
Initial public release: Communication version 1, application version 2<br>
Added windows build option & build documentation
## [1.1] - 2023-01-13
Internal development, Initial release<br>
Reworked mnemonic and password registration<br>
Removed fingerprint reader<br>
Reset configuration if password was entered incorrectly 8x in a row<br>
## [0.6] - 2022-09-11
Internal development. Updates for TCV hardware summit
## [0.5] - 2022-06-15
Internal development. Upgrade procedure
## [0.4] - 2021-12-17
Internal development. Restore mnemonic working
## [0.3] - 2021-10-01
Internal development. Retrieve address working
## [0.2] - 2021-09-21
Internal development. Registration complete
## [0.1] - 2021-05-23
Internal development. Send/Receive of transactions working
