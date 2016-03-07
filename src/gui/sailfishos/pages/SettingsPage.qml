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

import QtQuick 2.0
import Sailfish.Silica 1.0
import Fahrplan 1.0

Page {
    id: settingsPage
    property FahrplanBackend fahrplanBackend: null

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height
        contentWidth: parent.width

        VerticalScrollDecorator {}
        Column {
            id: column
            spacing: Theme.paddingLarge
            width: parent.width

            PageHeader {
                title: qsTr("Settings")
            }

            TextSwitch {
                visible: fahrplanBackend.supportsCalendar
                text: qsTr("Compact calendar entries")
                description: qsTr("Use shorter text format in the calendar event description")
                onCheckedChanged: {
                    fahrplanBackend.storeSettingsValue("compactCalendarEntries", checked);
                }
                Component.onCompleted: {
                    checked = fahrplanBackend.getSettingsValue("compactCalendarEntries", false) === "true" ? true : false;
                }
            }

            ComboBox {
                visible: fahrplanBackend.supportsCalendar
                label: qsTr("Add journeys to calendar")
                value: calendarManager.selectedCalendarName
                menu: ContextMenu {
                      Repeater {
                           model: calendarManager
                           MenuItem {
                               text: model.name
                           }
                      }
                }
                onCurrentIndexChanged: {
                    calendarManager.selectedIndex = currentIndex;
                }

            }

            ComboBox {
                id: currentBackend
                label: qsTr("Backend")
                value: fahrplanBackend.parserName
                menu: ContextMenu {
                      Repeater {
                           model: fahrplanBackend.backends
                           MenuItem {
                               text: model.name
                           }
                      }
                      Component.onCompleted: {
                          currentBackend.currentIndex = fahrplanBackend.backends.getItemIndexForParserId(fahrplanBackend.getSettingsValue("currentBackend", 0));
                      }
                }
                onCurrentIndexChanged: {
                    fahrplanBackend.setParser(fahrplanBackend.backends.getParserIdForItemIndex(currentIndex))
                }

            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("About")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AboutPage.qml"), {fahrplanBackend: fahrplanBackend})
                }
            }
        }
    }

    CalendarManager {
        id: calendarManager
    }

    onStatusChanged: {
        if (status === PageStatus.Activating)
            calendarManager.reload();
    }
}
