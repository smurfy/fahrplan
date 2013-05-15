import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: subTitleButton

    property alias titleText: title.text
    property alias iconVisible: icon.visible
    property alias subTitleText: subTitle.text
    property alias icon: icon.source

    signal clicked
    signal pressAndHold

    height: title.height + subTitle.height + 20
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
            leftMargin: 10
            rightMargin: 10
            topMargin: 7
            top: parent.top
        }

        font.bold: true
        font.pixelSize: 26
        text: "-"
    }

    Label {
        id: subTitle
        anchors {
            top:  title.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 10
            rightMargin: 10
            right: icon.left
        }
        text: ""
        font.pixelSize: subTitle.text != "" ? 28 : 0
        visible: subTitle.text != "" ? true : false
    }

    Image {
        id: icon

        anchors {
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
        height: sourceSize.height
        width: sourceSize.width
        source: "image://theme/meegotouch-combobox-indicator" + platformStyle.__invertedString
    }
}
