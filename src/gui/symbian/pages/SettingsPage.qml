/****************************************************************************
**
**  This file is a part of Fahrplan.
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

import QtQuick 1.1
import com.nokia.symbian 1.1
import Fahrplan 1.0
import "../components"

Page {
    id: settingsPage

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

    Flickable {
        id: flickable

        width: settingsPage.width
        height: settingsPage.height - header.height - 20
        flickableDirection: Flickable.VerticalFlick
        clip: true
        contentWidth: settingsPage.width
        contentHeight: settingsContainer.height

        anchors {
            top: header.bottom
            topMargin: 10
        }

        Item {
            id: settingsContainer
            width: parent.width
            height: settingsList.height + aboutButton.height + platformStyle.paddingLarge

            Column {
                id: settingsList
                anchors {
                    left: parent.left
                }
                width: parent.width

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
                    title: qsTr("Compact calendar entries")
                    subtitle: qsTr("Use shorter text format in the calendar event description")
                    platformInverted: appWindow.platformInverted

                    onCheckedChanged: {
                        fahrplanBackend.storeSettingsValue("compactCalendarEntries", checked);
                    }
                    Component.onCompleted: {
                        checked = fahrplanBackend.getSettingsValue("compactCalendarEntries", false);
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
                SwitchLabel {
                    title: qsTr("Favorites star position (requires restart)")
                    subtitle: checked ? qsTr("Right") : qsTr("Left")
                    platformInverted: appWindow.platformInverted

                    onCheckedChanged: {
                        fahrplanBackend.storeSettingsValue("favStarIconPos", checked);
                    }

                    Component.onCompleted: {
                        checked = fahrplanBackend.getSettingsValue("favStarIconPos", false);
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

                onClicked: pageStack.push(aboutPage);
            }
        }
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

    CalendarManager {
        id: calendarManager
    }

    ScrollDecorator {
        flickableItem: flickable
        platformInverted: appWindow.platformInverted
    }

    Component {
        id: aboutPage
        AboutPage {}
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
