import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1

Dialog {
    id: root
    title: qsTr("Time")
    height: units.gu(100)

    property alias hour: hourScroller.currentIndex
    property alias minute: minuteScroller.currentIndex

    signal accepted(int hours, int minutes)
    signal rejected()

    QtObject {
        id: priv

        property date now: new Date()
    }

    contents: [
        Row {
            height: units.gu(24)
            Scroller {
                id: hourScroller
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                }
                width: parent.width / 2
                labelText: qsTr("Hour")
                min: 00
                max: 23
                currentIndex: priv.now.getHours()
            }
            Scroller {
                id: minuteScroller
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                }
                width: parent.width / 2
                labelText: qsTr("Minute")

                min: 00
                max: 59
                currentIndex: priv.now.getMinutes()
            }
        },
        Row {
            spacing: units.gu(1)
            Button {
                text: "Cancel"
                onClicked: {
                    root.rejected()
                    PopupUtils.close(root)
                }
                width: (parent.width - parent.spacing) / 2
            }
            Button {
                text: "OK"
                color: "#dd4814"

                onClicked: {
                    root.accepted(root.hour, root.minute)
                    PopupUtils.close(root)
                }
                width: (parent.width - parent.spacing) / 2
            }
        }

    ]
}
