import QtQuick 1.1
import com.meego 1.0

Button {
    id: subTitleButton

    property alias titleText: title.text
    property alias subTitleText: subTitle.text

    height: subTitle.height + 50

    Item {
        anchors {
            verticalCenter: subTitleButton.verticalCenter
            leftMargin: 10
            left: parent.left
        }

        Label {
            id: title

            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }

            font.bold: true
            text: "-"
            width: subTitleButton.width / 2 - 10
        }

        Label {
            id: subTitle
            anchors {
                left: title.right
                leftMargin: 10
                verticalCenter: parent.verticalCenter
            }

            color: "#cc6633"
            text: ""
            width: subTitleButton.width / 2 - 10
        }
    }
}
