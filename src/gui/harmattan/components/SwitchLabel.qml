import QtQuick 1.1
import com.nokia.meego 1.1

Item {
    property alias title: labels.title
    property alias subtitle: labels.subtitle
    property alias checked: checkbox.checked

    height: 2 * UiConstants.ButtonSpacing + Math.max(labels.height, checkbox.height)
    anchors {
        left: parent.left
        right: parent.right
    }

    TwoLineLabel {
        id: labels
        anchors {
            top: parent.top
            topMargin: UiConstants.ButtonSpacing
            left: parent.left
            leftMargin: UiConstants.DefaultMargin
            right: checkbox.left
            rightMargin: UiConstants.DefaultMargin
        }
    }

    Switch {
        id: checkbox
        anchors {
            right: parent.right
            rightMargin: UiConstants.DefaultMargin
            verticalCenter: labels.verticalCenter
        }
    }
}
