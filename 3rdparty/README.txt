3rd party components needed for Fahrplan.

The bb10-qt-components folder contains a clone of Symbian Qt Components
adaptation for BlackBerry 10 with some patches by me (Oleksii Serdiuk).
This clone is needed to be able to use Symbian-style Qt Components on
BlackBerry 10 and simplifies porting Fahrplan to BlackBerry 10 a lot.

More information on original adaptation is available here:
http://kodira.de/2012/12/qt-components-on-blackberry-10/

To checkout the components you need to run the following commands from
the top level of your working tree:
> git submodule init
> git submodule update

To build the components start your terminal/command line of choice, run
the Blackberry 10 development environment setup script (bbndk-env.sh or
bbndk-env.bat, depending on your platform), change to bb10-qt-components
folder and run:

> configure -symbian -no-mobility -nomake tests -nomake examples -nomake demos
> make

And to free up around 4 MB, issue:
> make clean

-----------------------

gauss-kruger-cpp

This library is used for coordinate transformations in backends where the API
is using some Gauss–Krüger projection rather than latitude and longitude. No
separate building is required - only to include one header file and one cpp
file. To avoid extra work for all Fahrplan developers, it is not included as
a Git submodule but only copied from the original repository. Updating
shouldn't be too hard to do: remove the existing folder and clone the
repository. The current home of the project is
https://github.com/f03el/gauss-kruger-cpp.
