import QtQuick 1.1
import com.nokia.meego 1.1
import Fahrplan 1.0
import "components"

Page {
    id: root

    Rectangle {
        id: header

        height: appWindow.inPortrait ? UiConstants.HeaderDefaultHeightPortrait : UiConstants.HeaderDefaultHeightLandscape
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
            font: UiConstants.HeaderFont
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: UiConstants.DefaultMargin
            }
        }
    }

    Column {
        spacing: UiConstants.DefaultMargin
        anchors {
            top: header.bottom
            topMargin: UiConstants.DefaultMargin
            left: parent.left
            leftMargin: UiConstants.DefaultMargin
            right: parent.right
            rightMargin: UiConstants.DefaultMargin
        }

        SwitchLabel {
            title: qsTr("GPS location support")
            subtitle: checked ? qsTr("Opted-in") : qsTr("Opted-out")

            onCheckedChanged: {
                fahrplanBackend.storeSettingsValue("enableGps", checked);
            }
            Component.onCompleted: {
                checked = fahrplanBackend.getSettingsValue("enableGps", true);
            }
        }
        SwitchLabel {
            title: qsTr("Inverted style")
            subtitle: qsTr("Use dark color scheme")

            onCheckedChanged: {
                theme.inverted = checked;
                fahrplanBackend.storeSettingsValue("invertedStyle", theme.inverted);
            }
            Component.onCompleted: {
                checked = theme.inverted;
            }
        }
    }

    Button {
        id: aboutButton

        text: qsTr("About")
        anchors {
            bottom: parent.bottom
            bottomMargin: UiConstants.DefaultMargin
            horizontalCenter: parent.horizontalCenter
        }

        onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"));
    }

    FahrplanBackend {
        id: fahrplanBackend
    }

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: pageStack.pop();
        }
    }
}
