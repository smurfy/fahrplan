import QtQuick 1.1
import com.nokia.symbian 1.1
import Fahrplan 1.0
import "components"

Page {
    id: root

    Rectangle {
        id: header

        height: appWindow.inPortrait ? privateStyle.tabBarHeightPortrait : privateStyle.tabBarHeightLandscape
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
            font.pixelSize: privateStyle.statusBarHeight
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: platformStyle.paddingMedium
            }
        }
    }

    Column {
        spacing: platformStyle.paddingMedium
        anchors {
            top: header.bottom
            topMargin: platformStyle.paddingMedium
            left: parent.left
            leftMargin: platformStyle.paddingMedium
            right: parent.right
            rightMargin: platformStyle.paddingMedium
        }

        SwitchLabel {
            title: qsTr("GPS location support")
            subtitle: checked ? qsTr("Opted-in") : qsTr("Opted-out")
            visible: appWindow.showStatusBar
            platformInverted: appWindow.platformInverted

            onCheckedChanged: {
                fahrplanBackend.storeSettingsValue("enableGps", checked);
            }
            Component.onCompleted: {
                checked = fahrplanBackend.getSettingsValue("enableGps", true);
            }
        }
        SwitchLabel {
            title: qsTr("Inverted style")
            subtitle: qsTr("Use light color scheme")
            platformInverted: appWindow.platformInverted

            onCheckedChanged: {
                appWindow.platformInverted = checked;
                fahrplanBackend.storeSettingsValue("invertedStyle", appWindow.platformInverted);
            }
            Component.onCompleted: {
                checked = appWindow.platformInverted;
            }
        }
    }

    Button {
        id: aboutButton

        text: qsTr("About")
        platformInverted: appWindow.platformInverted
        anchors {
            bottom: parent.bottom
            bottomMargin: platformStyle.paddingMedium
            horizontalCenter: parent.horizontalCenter
        }

        onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"));
    }

    FahrplanBackend {
        id: fahrplanBackend
    }

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "toolbar-back"
            platformInverted: appWindow.platformInverted
            onClicked: pageStack.pop();
        }
    }
}
