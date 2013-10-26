---
layout: default
title: Compiling fahrplan from source
---

Compiling fahrplan from source
==============================

Test application
----------------

There is a test application available which could be compiled on
desktop systems. Its mainly for testing backends.

The test application can be compiled with either Qt4 or Qt5 by using
the respective qmake version.

    QT_SELECT=qt5 qmake fahrplan2.pro
    make

or use QtCreator, selecting the appropriate Qt version.

MeeGo 1.2 Harmattan
-------------------

Can be compiled with Qt4 (>= 4.7.2).
Requires Harmattan sysroot with MeeGo components.
Use the [Nokia Qt SDK][1] and build for MeeGo.

Symbian (Symbian^3, Anna and Belle)
-----------------------------------

Can be compiled with Qt4 (>= 4.7.2).
Requires Symbian sysroot with Symbian components.
Use the [Nokia Qt SDK][1] and build for Symbian.

Ubuntu Touch
------------

Can be compiled with Qt5. Requires [ubuntu-ui-toolkit][2].

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
<http://ppa.launchpad.net/ubuntu-sdk-team/ppa/ubuntu>)

    cp -r qtc_packaging/ubuntu debian
    pbuilder -p raring-armhf


BlackBerry 10
-------------

You will need to install [BlackBerry 10 Native SDK][3]. BlackBerry 10
port currently depends on Symbian Qt Components rebuild for BlackBerry.
Please, consult 3rdparty/README.txt file for instructions about how to
compile these components.

These instructions assume that you use Qt Creator for compilation so
you need to configure BlackBerry 10 target in it. See "Getting the
BlackBerry code signing keys" section of [Qt Creator with BlackBerry
10][4] wiki page for instructions.

After this, open fahrplan2.pro in Qt Creator, select BlackBerry 10 as a
target and compile it. If you want to deploy to a real device, you will
need to request and setup signing keys. Please, consult [Qt Creator with
BlackBerry 10][4] wiki page for more information.

Sailfish OS
-----------

[Not yet supported.][5]


[1]: http://www.developer.nokia.com/info/sw.nokia.com/id/da8df288-e615-443d-be5c-00c8a72435f8/Qt_SDK.html
[2]: http://developer.ubuntu.com/get-started/
[3]: https://developer.blackberry.com/native/download/
[4]: http://qt-project.org/wiki/Qt-Creator-with-BlackBerry-10
[5]: https://sailfishos.org/develop.html
