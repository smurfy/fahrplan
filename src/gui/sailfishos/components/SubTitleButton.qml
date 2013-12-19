import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    id: root

    property alias titleText: title.text
    property alias subTitleText: subTitle.text

    width: parent ? parent.width : screen.width
    implicitHeight: childrenRect.height

    Label {
        id: title
        width: parent.width - (theme.paddingMedium * 2)
        anchors.left: parent.left
        anchors.leftMargin: theme.paddingMedium
        color: theme.primaryColor
    }

    Label {
        id: subTitle
        width: parent.width - (theme.paddingMedium * 2)
        height: text.length ? (implicitHeight + theme.paddingMedium) : 0
        anchors.top: title.bottom
        anchors.left: title.left
        wrapMode: Text.Wrap
        font.pixelSize: theme.fontSizeExtraSmall
        color: theme.highlightColor
    }
}
