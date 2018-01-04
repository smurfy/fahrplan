#!/bin/bash

appName=$(basename "$PWD")
clickBuild="../../click_build"
buildDir=$(readlink -f "$clickBuild/$appName")
echo "builddir is $buildDir"
mkdir -p "$buildDir"
qmake . CONFIG+=ubuntu
make clean
make install
mv -v "/manifest.json" "$buildDir"
mv -v /fahrplan2*.* "$buildDir"
mkdir -p "$buildDir/bin"
mv "/bin/fahrplan2" "$buildDir/bin"
cd "$clickBuild"
click build "$buildDir" 2>/dev/null
