#!/bin/bash

# build inside (libertine) container on the phone

# check if we have a arm env
if [[ ! -d /usr/share/arm-linux-gnueabihf ]]
  then
  echo "this script must be run inside a armhf container."
  exit 1
fi

# find qmake
if [[ -n $(which qt5-qmake-arm-linux-gnueabihf) ]]
  then
  QMAKE=qt5-qmake-arm-linux-gnueabihf
  else
  QMAKE=qmake
fi

buildDirName="click_build"
buildDir=$(readlink -f "./$buildDirName")
echo "builddir is $buildDir"
mkdir -p "$buildDir"
$QMAKE . CONFIG+=ubuntu clickBuildFolder=$buildDirName
make clean
make install
cd "$buildDir"
click build "$buildDir" 2>/dev/null
