import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems
import Fahrplan 1.0
import "components"

Page {
    id: root

    Rectangle {
        id: header

        height: units.gu(6)
        gradient: Gradient {
            GradientStop {
                position: 0.00
                color: "#0d70c5"
            }
            GradientStop {
                position: 1.0
                color: "#0a50a5"
            }
        }
        anchors {
            left: parent.left
            right: parent.right
        }

        Label {
            text: qsTr("Settings")
            color: "white"
            fontSize: "large"
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: units.gu(1)
            }
        }
    }

    Column {
        spacing: units.gu(1)
        anchors {
            top: header.bottom
            topMargin: units.gu(1)
            left: parent.left
            leftMargin: units.gu(1)
            right: parent.right
            rightMargin: units.gu(1)
        }

        ListItems.Standard {
            text: qsTr("Enable GPS location support")

            control: Switch {
                onCheckedChanged: {
                    fahrplanBackend.storeSettingsValue("enableGps", checked);
                }
                Component.onCompleted: {
                    checked = fahrplanBackend.getSettingsValue("enableGps", true);
                }
            }

        }
    }

    Button {
        id: aboutButton

        text: qsTr("About")
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: units.gu(1)
            horizontalCenter: parent.horizontalCenter
        }

        onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"));
    }

    FahrplanBackend {
        id: fahrplanBackend
        Component.onCompleted: print("settingspage's parser:", parser)
    }
}
