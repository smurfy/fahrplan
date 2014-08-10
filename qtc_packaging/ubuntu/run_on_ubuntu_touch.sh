#!/bin/sh
CODE_DIR=fahrplan
BUILD_DIR=builddir
USER=phablet
USER_ID=32011
PASSWORD=phablet
PACKAGE=fahrplan2
BINARY=fahrplan2
TARGET_IP=${TARGET_IP-127.0.0.1}
TARGET_SSH_PORT=${TARGET_SSH_PORT-2222}
TARGET_DEBUG_PORT=3768
RUN_OPTIONS=-qmljsdebugger=port:$TARGET_DEBUG_PORT
SETUP=false
GDB=false
CLICK=false
SUDO="echo $PASSWORD | sudo -S"
NUM_JOBS='$(( `grep -c ^processor /proc/cpuinfo` + 1 ))'
FLIPPED=false

usage() {
    echo "usage: run_on_device [OPTIONS]\n"
    echo "Script to setup a build environment and sync build and run it on the device\n"
    echo "OPTIONS:"
    echo "  -s, --setup   Setup the build environment"
    echo "  -c, --click   Build a click package"
    echo ""
    echo "IMPORTANT:"
    echo " * Make sure to have networking setup on the device beforehand."
    echo " * Execute that script from a directory containing $PACKAGE code."
    exit 1
}

exec_with_ssh() {
    ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -t $USER@$TARGET_IP -p $TARGET_SSH_PORT sudo -u $USER -i bash -ic \"$@\"
}

exec_with_adb() {
    adb shell $@
}

adb_root() {
    adb root
    adb wait-for-device
}

install_ssh_key() {
    ssh-keygen -R $TARGET_IP
    HOME_DIR="/home/phablet"
    adb push ~/.ssh/id_rsa.pub $HOME_DIR/.ssh/authorized_keys
    adb shell chown $USER_ID:$USER_ID $HOME_DIR/.ssh
    adb shell chown $USER_ID:$USER_ID $HOME_DIR/.ssh/authorized_keys
    adb shell chmod 700 $HOME_DIR/.ssh
    adb shell chmod 600 $HOME_DIR/.ssh/authorized_keys
}

setup_adb_forwarding() {
    adb forward tcp:$TARGET_SSH_PORT tcp:22
    adb forward tcp:$TARGET_DEBUG_PORT tcp:$TARGET_DEBUG_PORT
}

install_dependencies() {
    exec_with_adb apt-get update
    exec_with_adb apt-get -y install openssh-server
    exec_with_ssh $SUDO apt-get -y install build-essential rsync bzr ccache gdb ninja-build devscripts equivs qttools5-dev-tools
}

sync_code() {
    WORK_DIR=`pwd`
    [ -e .bzr ] && bzr export --uncommitted --format=dir /tmp/$CODE_DIR
    [ -e .git ] && cd /tmp && cp -r $WORK_DIR $CODE_DIR && cd $CODE_DIR && git clean -f -x && rm .git -rf && cd -
    rsync -crlOzv --delete --exclude builddir -e "ssh -p $TARGET_SSH_PORT -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no" /tmp/$CODE_DIR/ $USER@$TARGET_IP:$CODE_DIR/
    rm -rf /tmp/$CODE_DIR
}

build() {
    exec_with_ssh mkdir -p $CODE_DIR/$BUILD_DIR
    exec_with_ssh cp -r $CODE_DIR/qtc_packaging/ubuntu/debian $CODE_DIR
    exec_with_ssh "[ $CODE_DIR/debian/control -nt $PACKAGE-build-deps*deb ] && $SUDO mk-build-deps --tool=\\\"apt-get -y --no-install-recommends\\\" --build-dep --install $CODE_DIR/debian/control"
    exec_with_ssh PATH=/usr/lib/ccache:$PATH "cd $CODE_DIR/$BUILD_DIR && PATH=/usr/lib/ccache:$PATH qmake .. CONFIG+=ubuntu"
    # Workaround for qrc not getting rebuild when changing qml files only
    exec_with_ssh touch $CODE_DIR/ubuntu_res.qrc
    exec_with_ssh PATH=/usr/lib/ccache:$PATH "cd $CODE_DIR/$BUILD_DIR && PATH=/usr/lib/ccache:$PATH make -j2"
}

build_click_package() {
    exec_with_ssh mkdir -p $CODE_DIR/$BUILD_DIR/install
    exec_with_ssh rm -rf $CODE_DIR/$BUILD_DIR/install/*
    exec_with_ssh cp $CODE_DIR/$BUILD_DIR/fahrplan2 $CODE_DIR/$BUILD_DIR/install
    exec_with_ssh cp $CODE_DIR/qtc_packaging/ubuntu/click/* $CODE_DIR/$BUILD_DIR/install
    exec_with_ssh cp $CODE_DIR/data/fahrplan2_ubuntu.desktop $CODE_DIR/$BUILD_DIR/install
    exec_with_ssh cp $CODE_DIR/data/fahrplan2-square.svg $CODE_DIR/$BUILD_DIR/install
    exec_with_ssh click build $CODE_DIR/$BUILD_DIR/install
}

run() {
#    exec_with_ssh "cd $CODE_DIR/$BUILD_DIR && ./fahrplan2 --desktop_file_hint=/home/phablet/$CODE_DIR/data/fahrplan2_ubuntu.desktop"
    exec_with_ssh "cd $CODE_DIR/$BUILD_DIR && ./fahrplan2 --desktop_file_hint=/usr/share/applications/dialer-app.desktop"
}

set -- `getopt -n$0 -u -a --longoptions="setup,gdb,click,help" "sgch" "$@"`

# FIXME: giving incorrect arguments does not call usage and exit
while [ $# -gt 0 ]
do
    case "$1" in
       -s|--setup)   SETUP=true;;
       -g|--gdb)     GDB=true;;
       -c|--click)   CLICK=true;;
       -h|--help)    usage;;
       --)           shift;break;;
    esac
    shift
done


adb_root
[ "${TARGET_IP}" = "127.0.0.1" ] && setup_adb_forwarding

if $SETUP; then
    echo "Setting up environment for building $PACKAGE..."
    install_ssh_key
    install_dependencies
    sync_code
else
    echo "Transferring code.."
    sync_code
    echo "Building.."
    build
    if $CLICK; then
        echo "Building click package..."
        build_click_package
    else
        echo "Running.."
        run
    fi
fi
