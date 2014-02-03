import QtQuick 2.0
import Sailfish.Silica 1.0
import Fahrplan 1.0

Item {
    id: root
    property ListView listView: ListView.view
    width: listView.width
    height: contentItem.height

    BackgroundItem {
        id: contentItem
        width: parent.width

        Column {
            id: labels

            height: childrenRect.height
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                leftMargin: 10
                rightMargin: 10
            }

            Row {
                width: parent.width
                height: childrenRect.height

                Label {
                    text: model.departureTime
                    width: (parent.width - 3 * 1) / 4
                }

                Label {
                    text: model.arrivalTime
                    width: (parent.width - 3 * 1) / 4
                }

                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: model.duration
                    width: (parent.width - 3 * 1) / 4
                }

                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: model.transfers
                    width: (parent.width - 3 * 1) / 4
                }
            }

            Label {
                text: model.trainType
                width: parent.width
            }

            Label {
                visible: model.miscInfo !== ""
                text: model.miscInfo
                width: parent.width
                font.bold: true
            }
        }

        onClicked: {
            console.log("Mooo");
        }
    }
}
