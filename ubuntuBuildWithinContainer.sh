#!/bin/bash

# build inside libertine container on the phone

buildDir=$(readlink -f "./click_build")
echo "builddir is $buildDir"
mkdir -p "$buildDir"
qmake . CONFIG+=ubuntu
make clean
make install
cd "$buildDir"
click build "$buildDir" 2>/dev/null
