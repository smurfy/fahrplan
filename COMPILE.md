Compiling fahrplan from source
==============================

Test application
----------------

There is a test application available which could be compiled on desktop systems.
Its mainly for testing backends.

The test application can be compiled with either Qt4 or Qt5 by using the respective qmake version.

	QT_SELECT=qt5 qmake fahrplan2.pro
	make

or use QtCreator, selecting the appropriate Qt version.

MeeGo 1.2 Harmattan
-------------------

Can be compiled with Qt4 (>= 4.7.2).
Requires Harmattan sysroot with MeeGo components.
Use the Nokia Qt SDK [1] and build for MeeGo.

Symbian (Belle or Anna)
-----------------------

Can be compiled with Qt4 (>= 4.7.2).
Requires Symbian sysroot with Symbian components.
Use the Nokia Qt SDK [1] and build for Symbian.

Ubuntu Touch
------------

Can be compiled with Qt5. Requires ubuntu-ui-toolkit. [2]

Install the ubuntu-sdk

	sudo apt-get install ubuntu-sdk
	
Compile

	QT_SELECT=qt5 qmake fahrplan2.pro CONFIG+=ubuntu
	make

or use QtCreator and build for Qt5. For now you have to manually
add CONFIG+=ubuntu to the qmake options in the project config.

You can run the resulting binary on Ubuntu Desktop, Tablet (in 
SideStage mode) and Phone.

Packages for Ubuntu Touch Devices can be built using any debian
package builder. Example for compiling a package for Ubuntu Phones
based on arm architecture and Ubuntu 13.04 (raring) using pbuilder:

	pcreate -a armhf -d raring raring-armhf
	
(add the Ubuntu Touch SDK ppa to your build environment when asked:
http://ppa.launchpad.net/ubuntu-sdk-team/ppa/ubuntu)

	pbuilder -p raring-armhf


Blackberry 10
-------------

//TODO Blackberry

Sailfish OS
-----------

Not yet supported. [3]

References and links
--------------------

1. http://www.developer.nokia.com/info/sw.nokia.com/id/da8df288-e615-443d-be5c-00c8a72435f8/Qt_SDK.html
2. http://developer.ubuntu.com/get-started/
3. https://sailfishos.org/develop.html
