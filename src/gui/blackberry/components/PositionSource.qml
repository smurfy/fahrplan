import QtQuick 1.1

// This is a PositionSource dummy for BlackBerry 10.
// It will be replaced with the working one later.
Item {
    property bool active: false
    signal positionChanged()
    function start() {}
}
