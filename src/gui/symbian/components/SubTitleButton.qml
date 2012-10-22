import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: subTitleButton

    property alias titleText: title.text
    property alias subTitleText: subTitle.text
    property alias icon: icon.source

    signal clicked
    signal pressAndHold

    height: title.height + subTitle.height + platformStyle.paddingMedium + platformStyle.paddingLarge
    width: parent.width

    Rectangle {
        id: background
        anchors.fill: parent
        color: "DarkGrey"
        visible: mouseArea.pressed
    }

    MouseArea {
        id: mouseArea
        anchors.fill: background
        onClicked: {
            subTitleButton.clicked();
        }
        onPressAndHold: {
            subTitleButton.pressAndHold();
        }
    }

    Label {
        id: title

        anchors {
            left: parent.left
            leftMargin: platformStyle.paddingMedium
            rightMargin: platformStyle.paddingMedium
            topMargin: platformStyle.paddingMedium
            top: parent.top
        }

        font.bold: true
        font.pixelSize: platformStyle.fontSizeLarge
        text: "-"
    }

    Label {
        id: subTitle
        anchors {
            top:  title.bottom
            topMargin: platformStyle.paddingSmall
            left: parent.left
            leftMargin: platformStyle.paddingMedium
            right: icon.left
            rightMargin: platformStyle.paddingMedium
        }
        text: ""
        font.pixelSize: platformStyle.fontSizeLarge + 2
        wrapMode: Text.WordWrap
    }

    Image {
        id: icon

        anchors {
            right: parent.right
            rightMargin: platformStyle.paddingMedium
            verticalCenter: parent.verticalCenter
        }
        height: sourceSize.height
        width: sourceSize.width
        source: "image://theme/qtg_graf_choice_list_indicator"
    }
}
