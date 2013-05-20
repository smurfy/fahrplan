import QtQuick 1.1
import com.nokia.meego 1.1

Item {
    property alias title: lblTitle.text
    property alias subtitle: lblSubtitle.text
    property alias checked: checkbox.checked

    height: childrenRect.height
    anchors {
        left: parent.left
        right: parent.right
    }

    Item {
        id: labels

        width: childrenRect.width
        height: childrenRect.height
        anchors.left: parent.left

        Label {
            id: lblTitle
            font: UiConstants.TitleFont
            anchors {
                top: parent.top
                left: parent.left
            }
        }
        Label {
            id: lblSubtitle
            font: UiConstants.SubtitleFont
            visible: text != ""
            height: visible ? implicitHeight : 0
            anchors {
                top: lblTitle.bottom
                left: parent.left
            }
        }
    }
    Switch {
        id: checkbox
        anchors {
            right: parent.right
            verticalCenter: labels.verticalCenter
        }
    }
}
