import QtQuick 1.1
import com.nokia.symbian 1.1
import "../js/style.js" as Style

MouseArea {
    id: root

    property alias title: labels.title
    property alias subtitle: labels.subtitle
    property bool platformInverted: false

    implicitHeight: 2 * platformStyle.paddingMedium + Math.max(labels.height, icon.height)
    anchors {
        left: parent.left
        right: parent.right
    }

    Rectangle {
        color: Style.listBackgroundHighlight
        visible: parent.pressed
        anchors.fill: parent
    }

    TwoLineLabel {
        id: labels
        platformInverted: root.platformInverted
        anchors {
            top: parent.top
            topMargin: platformStyle.paddingMedium
            left: parent.left
            leftMargin: platformStyle.paddingLarge
            right: icon.left
            rightMargin: platformStyle.paddingLarge
        }
    }

    Image {
        id: icon

        source: Style.getIconFromTheme(root.platformInverted, "qtg_graf_choice_list_indicator")
        sourceSize {
            width: platformStyle.graphicSizeSmall
            height: platformStyle.graphicSizeSmall
        }
        anchors {
            right: parent.right
            rightMargin: platformStyle.paddingLarge
            verticalCenter: parent.verticalCenter
        }
    }
}
