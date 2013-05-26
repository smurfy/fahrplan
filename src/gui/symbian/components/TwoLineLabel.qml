import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: root

    property alias title: lblTitle.text
    property alias subtitle: lblSubtitle.text
    property bool platformInverted: false

    implicitWidth: Math.max(lblTitle.implicitWidth, lblSubtitle.implicitWidth)
    height: childrenRect.height

    ListItemText {
        id: lblTitle
        role: "Title"
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        platformInverted: root.platformInverted
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }
    ListItemText {
        id: lblSubtitle
        role: "Subtitle"
        visible: text != ""
        height: visible ? implicitHeight : 0
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        platformInverted: root.platformInverted
        anchors {
            top: lblTitle.bottom
            topMargin: platformStyle.paddingSmall
            left: parent.left
            right: parent.right
        }
    }
}
