3rd party components needed for Fahrplan.

The connys-qt-components folder contains Symbian Qt Components
adaptation for BlackBerry 10. This is needed to be able to use
Symbian-style Qt Components on BlackBerry and simplifies porting
Fahrplan to BlackBerry 10 a lot.

More information is available here:
http://kodira.de/2012/12/qt-components-on-blackberry-10/

To checkout the components you need to run the following commands from
the top level of your working tree:
> git submodule init
> git submodule update

To build the components start your terminal/command line of choice, run
the Blackberry 10 development environment setup script (bbndk-env.sh or
bbndk-env.bat, depending on your platform), change to
connys-qt-components folder and run:

> configure -symbian -no-mobility -nomake tests -nomake examples -nomake demos
> make

And to free up around 4 MB, issue:
> make clean
