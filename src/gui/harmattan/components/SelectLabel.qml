import QtQuick 1.1
import com.nokia.meego 1.1

MouseArea {
    id: root

    property alias title: labels.title
    property alias subtitle: labels.subtitle

    implicitHeight: 2 * UiConstants.ButtonSpacing + Math.max(labels.height, icon.height)
    anchors {
        left: parent.left
        right: parent.right
    }

    Rectangle {
        color: "DarkGrey"
        visible: parent.pressed
        anchors.fill: parent
    }

    TwoLineLabel {
        id: labels
        anchors {
            top: parent.top
            topMargin: UiConstants.ButtonSpacing
            left: parent.left
            leftMargin: UiConstants.DefaultMargin
            right: icon.left
            rightMargin: UiConstants.DefaultMargin
        }
    }

    Image {
        id: icon

        source: "image://theme/meegotouch-combobox-indicator" + platformStyle.__invertedString
        height: sourceSize.height
        width: sourceSize.width
        anchors {
            right: parent.right
            rightMargin: UiConstants.DefaultMargin
            verticalCenter: parent.verticalCenter
        }
    }
}
