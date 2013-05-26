import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: root

    property alias title: labels.title
    property alias subtitle: labels.subtitle
    property alias checked: checkbox.checked
    property bool platformInverted: false

    height: 2 * platformStyle.paddingMedium + Math.max(labels.height, checkbox.height)
    anchors {
        left: parent.left
        right: parent.right
    }

    TwoLineLabel {
        id: labels
        platformInverted: root.platformInverted
        anchors {
            top: parent.top
            topMargin: platformStyle.paddingMedium
            left: parent.left
            leftMargin: platformStyle.paddingLarge
            right: checkbox.left
            rightMargin: platformStyle.paddingLarge
        }
    }

    Switch {
        id: checkbox
        platformInverted: root.platformInverted
        anchors {
            right: parent.right
            rightMargin: platformStyle.paddingLarge
            verticalCenter: labels.verticalCenter
        }
    }
}
