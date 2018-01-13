 libcurl is already on the phone (/usr/lib/arm-linux-gnueabihf/libcurl*)
 in order to build it with Ubuntu SDK, we need the development files (e.g. headers)
 it can be installed inside he schroot of the machine with Ubuntu SDK

 > sudo schroot -c "click-ubuntu-sdk-15.04-armhf" -u root

 - it is likely you have to adapt sources.list first, because Canonical has moved vivid to a different url:
 mine for example is:
 deb [arch=armhf,amd64] http://old-releases.ubuntu.com/ubuntu vivid main restricted universe multiverse
 deb [arch=armhf,amd64] http://old-releases.ubuntu.com/ubuntu vivid-updates main restricted universe multiverse
 deb [arch=armhf,amd64] http://old-releases.ubuntu.com/ubuntu vivid-security main restricted universe multiverse

 > apt-get update

 > apt-get install libcurl4-gnutls-dev:armhf

 make sure not to forget the :armhf as arch, because otherwise you install the amd64 one, you will see errors like
 curlrules.h:143 size of array __curl_rule_01_ is negative ...
