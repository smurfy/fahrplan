import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: root

    property alias title: lblTitle.text
    property alias subtitle: lblSubtitle.text
    property alias checked: checkbox.checked
    property bool platformInverted: false

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

        ListItemText {
            id: lblTitle
            role: "Title"
            platformInverted: root.platformInverted
            anchors {
                top: parent.top
                left: parent.left
            }
        }
        ListItemText {
            id: lblSubtitle
            role: "Subtitle"
            visible: text != ""
            height: visible ? implicitHeight : 0
            platformInverted: root.platformInverted
            anchors {
                top: lblTitle.bottom
                left: parent.left
            }
        }
    }
    Switch {
        id: checkbox
        platformInverted: root.platformInverted
        anchors {
            right: parent.right
            verticalCenter: labels.verticalCenter
        }
    }
}
