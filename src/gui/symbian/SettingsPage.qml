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
        anchors {
            top: header.bottom
            topMargin: platformStyle.paddingSmall
            left: parent.left
            right: parent.right
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
        SelectLabel {
            title: qsTr("Add journeys to calendar")
            subtitle: calendarManager.selectedCalendarName
            platformInverted: appWindow.platformInverted
            onClicked: {
                calendarsList.selectedIndex = calendarManager.selectedIndex;
                calendarsList.open();
            }
        }
    }

    Button {
        id: aboutButton

        text: qsTr("About")
        width: parent.width / 2
        platformInverted: appWindow.platformInverted
        anchors {
            bottom: parent.bottom
            bottomMargin: platformStyle.paddingLarge
            horizontalCenter: parent.horizontalCenter
        }

        onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"));
    }

    SelectionDialog {
        id: calendarsList

        titleText: qsTr("Select a calendar")
        selectedIndex: calendarManager.selectedIndex
        platformInverted: appWindow.platformInverted

        model: calendarManager
        delegate: MenuItem {
            text: model.name
            platformInverted: appWindow.platformInverted

            onClicked: {
                calendarManager.selectedIndex = index;
                calendarsList.accept();
            }
        }
    }

    FahrplanBackend {
        id: fahrplanBackend
    }

    CalendarManager {
        id: calendarManager
    }

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "toolbar-back"
            platformInverted: appWindow.platformInverted
            onClicked: pageStack.pop();
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Activating)
            calendarManager.reload();
    }
}
