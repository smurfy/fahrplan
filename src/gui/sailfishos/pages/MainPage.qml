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
    property int searchmode : 0
    id: mainPage

    canNavigateForward: {
        if (searchmode == 0) {
            return departureButton.value !== qsTr("please select") &&
                   arrivalButton.value !== qsTr("please select") &&
                   departureButton.value !== arrivalButton.value;
        } else if (searchmode == 1) {
            return currentButton.value !== qsTr("please select");
        }

        //Default
        return false;
    }

    SilicaListView {
        id: listView
        anchors.fill: parent

        header: PageHeader {
            title: searchmode == 0 ? qsTr("Journey") : qsTr("Time Table")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("SettingsPage.qml"), {fahrplanBackend: fahrplanBackend})
                }
            }
            MenuItem {
                text: qsTr("Journey")
                visible: searchmode === 1
                onClicked: {
                    searchmode = 0;
                    updateButtonVisibility();
                }
            }
            MenuItem {
                text: qsTr("Time Table")
                visible: searchmode === 0
                onClicked: {
                    searchmode = 1;
                    updateButtonVisibility();
                }
            }
            MenuItem {
                id: currentBackend
                text: fahrplanBackend.parserShortName
                enabled: false
            }
        }

        model: VisualItemModel {

            ValueButton {
                id: departureButton
                label: qsTr("Departure Station")
                value: fahrplanBackend.departureStationName
                property int type: FahrplanBackend.DepartureStation
                property bool menuOpen: stationSelectContextMenu.parent === departureButton
                height: menuOpen ? stationSelectContextMenu.height + contentItem.height : contentItem.height

                onClicked: {
                    pageStack.push(Qt.resolvedUrl("StationSelectPage.qml"), {type: type, fahrplanBackend: fahrplanBackend})
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(departureButton, FahrplanBackend.DepartureStation);
                }
            }
            ValueButton {
                id: viaButton
                label: qsTr("Via Station")
                value: fahrplanBackend.viaStationName
                property int type: FahrplanBackend.ViaStation
                property bool menuOpen: stationSelectContextMenu.parent === viaButton
                height: menuOpen ? stationSelectContextMenu.height + contentItem.height : contentItem.height

                onClicked: {
                      pageStack.push(Qt.resolvedUrl("StationSelectPage.qml"), {type: type, fahrplanBackend: fahrplanBackend})
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(viaButton, FahrplanBackend.ViaStation);
                }
            }
            ValueButton {
                id: arrivalButton
                label: qsTr("Arrival Station")
                value: fahrplanBackend.arrivalStationName
                property int type: FahrplanBackend.ArrivalStation
                property bool menuOpen: stationSelectContextMenu.parent === arrivalButton
                height: menuOpen ? stationSelectContextMenu.height + contentItem.height : contentItem.height

                onClicked: {
                    pageStack.push(Qt.resolvedUrl("StationSelectPage.qml"), {type: type, fahrplanBackend: fahrplanBackend})
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(arrivalButton);
                }
            }
            ValueButton {
                id: currentButton
                label: qsTr("Station")
                value: fahrplanBackend.currentStationName
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("StationSelectPage.qml"), {type: FahrplanBackend.CurrentStation, fahrplanBackend: fahrplanBackend})

                }
            }
            ValueButton {
                id: directionButton
                label: qsTr("Direction")
                value: fahrplanBackend.directionStationName
                property bool menuOpen: timeTableSelectContextMenu.parent === directionButton
                height: menuOpen ? timeTableSelectContextMenu.height + contentItem.height : contentItem.height

                onClicked: {
                    pageStack.push(Qt.resolvedUrl("StationSelectPage.qml"), {type: FahrplanBackend.DirectionStation, fahrplanBackend: fahrplanBackend})
                }
                onPressAndHold: {
                    timeTableSelectContextMenu.show(directionButton);
                }
            }
            ComboBox {
                id: modeSelect
                label: qsTr("Mode")
                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Departure: now")
                        onClicked: {
                            fahrplanBackend.mode = FahrplanBackend.NowMode;
                        }
                    }
                    MenuItem {
                        text: qsTr("Departure")
                        onClicked: {
                            fahrplanBackend.mode = FahrplanBackend.DepartureMode;
                        }
                    }
                    MenuItem {
                        text: qsTr("Arrival")
                        onClicked: {
                            fahrplanBackend.mode = FahrplanBackend.ArrivalMode;
                        }
                    }
                }
            }
            ValueButton {
                id: datePickerButton
                label: qsTr("Date")
                value: Qt.formatDate(fahrplanBackend.dateTime)
                onClicked: {
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", {
                        date: fahrplanBackend.dateTime
                    })

                    dialog.accepted.connect(function() {
                        var dateTime = fahrplanBackend.dateTime;
                        dateTime.setFullYear(dialog.year);
                        // JavaScript Date's month is 0 based while DatePicker's is 1 based.
                        dateTime.setMonth(dialog.month - 1);
                        dateTime.setDate(dialog.day);
                        fahrplanBackend.dateTime = dateTime;
                    })
                }
            }
            ValueButton {
                id: timePickerButton
                label: qsTr("Time")
                value: Qt.formatTime(fahrplanBackend.dateTime, Qt.DefaultLocaleShortDate)
                onClicked: {
                    var dialog = pageStack.push("Sailfish.Silica.TimePickerDialog", {
                        hour: Qt.formatDateTime ( fahrplanBackend.dateTime, "hh" ),
                        minute: Qt.formatDateTime ( fahrplanBackend.dateTime, "mm" ),
                        hourMode: fahrplanBackend.timeFormat24h() ? DateTime.TwentyFourHours : DateTime.TwelveHours
                    })

                    dialog.accepted.connect(function() {
                        var dateTime = fahrplanBackend.dateTime;
                        dateTime.setHours(dialog.hour);
                        dateTime.setMinutes(dialog.minute);
                        dateTime.setSeconds(0);
                        fahrplanBackend.dateTime = dateTime;
                    })
                }
            }
            ComboBox {
                id: trainrestrictionsButton
                label: qsTr("Trains")
                value: fahrplanBackend.trainrestrictionName
                menu: ContextMenu {
                      Repeater {
                           model: fahrplanBackend.trainrestrictions
                           MenuItem {
                               text: model.name
                           }
                      }
                      onActivated: {
                          fahrplanBackend.setTrainrestriction(index)
                      }
                 }
            }
        }
    }
    function updateButtonVisibility()
    {
        if (!fahrplanBackend.parser.supportsTimeTable()) {
            searchmode = 0;
        }

        if (searchmode == 0) {
            viaButton.visible = fahrplanBackend.parser.supportsVia();
            departureButton.visible = true;
            arrivalButton.visible = true;
            currentButton.visible = false;
            directionButton.visible = false;
            pageStack.pushAttached(journeyResultsPage, {})
        }
        if (searchmode == 1) {
            viaButton.visible = false;
            departureButton.visible = false;
            arrivalButton.visible = false;
            currentButton.visible = true;
            directionButton.visible = fahrplanBackend.parser.supportsTimeTableDirection();
            pageStack.pushAttached(timetablePage, {})
        }
    }

    function updateModeCheckboxes()
    {
        if (fahrplanBackend.mode === FahrplanBackend.NowMode) {
            modeSelect.currentIndex = 0;
            datePickerButton.visible = false;
            timePickerButton.visible = false;
            return;
        }

        datePickerButton.visible = true;
        timePickerButton.visible = true;

        if (fahrplanBackend.mode === FahrplanBackend.DepartureMode) {
            modeSelect.currentIndex = 1;
            return;
        }
        if (fahrplanBackend.mode === FahrplanBackend.ArrivalMode) {
            modeSelect.currentIndex = 2;
            return;
        }
    }

    TimetablePage {
        id: timetablePage
    }

    JourneyResultsPage {
        id: journeyResultsPage
    }

    ContextMenu {
        id: stationSelectContextMenu
        property ValueButton opener

        MenuItem {
            id: switchWithDepartureStation
            text: qsTr("Switch with Departure station")
            onClicked: {
                fahrplanBackend.swapStations(stationSelectContextMenu.opener.type, FahrplanBackend.DepartureStation)
            }
        }
        MenuItem {
            id: switchWithArrivalStation
            text: qsTr("Switch with Arrival station")
            onClicked: {
                fahrplanBackend.swapStations(stationSelectContextMenu.opener.type, FahrplanBackend.ArrivalStation)
            }
        }
        MenuItem {
            id: switchWithViaStation
            text: qsTr("Switch with Via station")
            onClicked: {
                fahrplanBackend.swapStations(stationSelectContextMenu.opener.type, FahrplanBackend.ViaStation)
            }
        }
        MenuItem {
            text: qsTr("Clear station")
            onClicked: {
                fahrplanBackend.resetStation(stationSelectContextMenu.opener.type);
            }
        }

        function openMenu(opener)
        {
            stationSelectContextMenu.opener = opener;
            switchWithViaStation.visible = false;
            switchWithDepartureStation.visible = false;
            switchWithArrivalStation.visible = false;

            if (opener != viaButton && fahrplanBackend.parser.supportsVia()) {
                switchWithViaStation.visible = true;
            }
            if (opener != arrivalButton) {
                switchWithArrivalStation.visible = true;
            }
            if (opener != departureButton) {
                switchWithDepartureStation.visible = true;
            }

            stationSelectContextMenu.show(opener);
        }
    }

    ContextMenu {
        id: timeTableSelectContextMenu

        MenuItem {
            text: qsTr("Clear station")
            onClicked: {
                fahrplanBackend.resetStation(FahrplanBackend.DirectionStation);
            }
        }
    }

    Connections {
        target: fahrplanBackend

        onParserChanged: {
            console.log("Switching to " + name);
            updateButtonVisibility();
        }

        onModeChanged: {
            updateModeCheckboxes();
        }
    }

    onStatusChanged: {
        switch (status) {
            case PageStatus.Active:
                updateButtonVisibility();
                break;
        }
    }

    Component.onCompleted: {
        updateModeCheckboxes();
    }
}
