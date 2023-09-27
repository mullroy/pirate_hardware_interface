This patch is required for using XCode 15 with Qt5. The patch fixes the
qmake build script to run properly. 

If you're using a new versions than XCode 11, incompatibilities with Qt 
may be introduced. A bug (https://bugreports.qt.io/browse/QTBUG-117225)
was logged on 19 September 2023 regarding an incompatibility with XCode 15
with QT 5 & QT 6. This causes qmake (Qt build script) to fail. A patched 
version of the file is included in the Qt5_patch directory. This updated
file isn't required to be deployed if you're running XCode 11 to 14. For
XCode 15 (and beyound?) please navigate to your Qt5 directory (Suggested
/Users//Documents/Qt5 in INSTALL_mac.md) and lower into the sub directories:
5.12.12/clang_64/mkspecs/features

Make a backup copy of your file: cp toolchain.prf toolchain.prf.O
Overwrite toolchain.prf with the copy located under the Qt5_patch directory.
