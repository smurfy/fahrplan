#!/bin/bash

# build inside libertine container on the phone

buildDirName="click_build"
buildDir=$(readlink -f "./$buildDirName")
echo "builddir is $buildDir"
mkdir -p "$buildDir"
qmake . CONFIG+=ubuntu clickBuildFolder=$buildDirName
make clean
make install
cd "$buildDir"
click build "$buildDir" 2>/dev/null
