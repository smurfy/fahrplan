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

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3 as ListItems
import "components"
import Fahrplan 1.0

Page {
    id: mainPage

    property int searchmode : 0
    property bool timeTableEnabled: true
    property bool startup : true

    header: PageHeader {
        title: tabs.selectedTabIndex === 0 ? qsTr("Journey") : qsTr("Time table")
        flickable: flickable
    
        leadingActionBar.actions: [
            Action {
                text: qsTr("Journey")
                onTriggered: tabs.selectedTabIndex = 0
            },
            Action {
                text: qsTr("Time table")
                onTriggered: tabs.selectedTabIndex = 1
                enabled: timeTableEnabled
            }
        ]
        
        leadingActionBar.numberOfSlots: 0

        trailingActionBar.actions: Action {
            iconName: "info"
            text: qsTr("About")
            onTriggered: mainStack.push(Qt.resolvedUrl("AboutPage.qml"));
        }
    }

    // Not using settings on ubuntu yet...
    //        Action {
    //            iconSource: "file:///usr/share/icons/ubuntu-mobile/actions/scalable/settings.svg"
    //            onTriggered: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"));
    //        }

    Component.onCompleted: {
        updateButtonVisibility()
    }

    function updateButtonVisibility()
    {
        //console.log("button...");
        
        if (fahrplanBackend.parser.supportsTimeTable()) {
            timeTableEnabled = true;
        }
        else {
            timeTableEnabled = false;
            tabs.selectedTabIndex = 0
        }

        if (searchmode == 0) {
            viaButton.visible = fahrplanBackend.parser.supportsVia();
            departureButton.visible = true;
            arrivalButton.visible = true;
            stationButton.visible = false;
            directionButton.visible = false;
            timetableSearch.visible = false;
            startSearch.visible = true;
        }
        if (searchmode == 1) {
            viaButton.visible = false;
            departureButton.visible = false;
            arrivalButton.visible = false;
            stationButton.visible = true;
            directionButton.visible = fahrplanBackend.parser.supportsTimeTableDirection();
            timetableSearch.visible = true;
            startSearch.visible = false;
        }
    }

    Flickable {
        id: flickable

        anchors.fill: parent
        contentHeight: buttons.height

        Column {
            id: buttons

            anchors { left: parent.left; right: parent.right }

            Item {
                id: titleBar

                property color color: "#dd4814"

                width: parent.width; height: units.gu(6)

                Rectangle {
                    anchors.fill: parent
                    gradient: Gradient {
                        GradientStop {
                            position: 0.00;
                            color: Qt.lighter(titleBar.color, 1.3);
                        }
                        GradientStop {
                            position: 1.0;
                            color: titleBar.color;
                        }
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: titleBar.color
                    visible: mouseArea.pressed
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    enabled: parent.enabled
                    onClicked: PopupUtils.open(selectBackendComponent, titleBar)
                }

                Label {
                    id: currentParserName

                    anchors { left: parent.left; right: parent.right; margins: units.gu(2); verticalCenter: parent.verticalCenter }
                    color: "White"
                    font.bold: true;
                    textSize: Label.Large
                    elide: Text.ElideRight
                }
            }

            CustomListItem {
                id: departureButton

                property int type: FahrplanBackend.DepartureStation

                title.text: qsTr("From")
                value: fahrplanBackend.departureStationName

                leadingActions: ListItemActions {
                    actions:[
                        Action {
                            iconName: "clear"
                            onTriggered: fahrplanBackend.resetStation(FahrplanBackend.DepartureStation);
                        }
                    ]
                }
                trailingActions: ListItemActions {
                    actions: [
                        Action {
                            iconSource: "qrc:///src/gui/ubuntu/icons/swap-with-via.svg"
                            onTriggered: fahrplanBackend.swapStations(FahrplanBackend.DepartureStation, FahrplanBackend.ViaStation)
                        },
                        Action {
                            iconSource: "qrc:///src/gui/ubuntu/icons/swap-with-arrival.svg"
                            onTriggered: fahrplanBackend.swapStations(FahrplanBackend.DepartureStation, FahrplanBackend.ArrivalStation)
                        }
                    ]
                    delegate: Item {
                        width: height
                        Icon {
                            source: action.iconSource
                            width: units.gu(3)
                            height: width
                            anchors.centerIn: parent
                        }
                    }
                }

                onClicked: {
                    mainStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml", {type: type, title: title.text})
                }

                onPressAndHold: openMenu(departureButton)
            }

            CustomListItem {
                id: viaButton

                property int type: FahrplanBackend.ViaStation

                title.text: qsTr("Via")
                value: fahrplanBackend.viaStationName

                leadingActions: ListItemActions {
                    actions:[
                        Action {
                            iconName: "clear"
                            onTriggered: fahrplanBackend.resetStation(FahrplanBackend.ViaStation);
                        }
                    ]
                }
                trailingActions: ListItemActions {
                    actions: [
                        Action {
                            iconSource: "qrc:///src/gui/ubuntu/icons/swap-with-departure.svg"
                            onTriggered: fahrplanBackend.swapStations(FahrplanBackend.ViaStation, FahrplanBackend.DepartureStation)
                        },
                        Action {
                            iconSource: "qrc:///src/gui/ubuntu/icons/swap-with-arrival.svg"
                            onTriggered: fahrplanBackend.swapStations(FahrplanBackend.ViaStation, FahrplanBackend.ArrivalStation)
                        }
                    ]
                    delegate: Item {
                        width: height
                        Icon {
                            source: action.iconSource
                            width: units.gu(3)
                            height: width
                            anchors.centerIn: parent
                        }
                    }
                }

                onClicked: {
                    mainStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml", {type: type, title: title.text})
                }

                onPressAndHold: openMenu(viaButton)
            }

            CustomListItem {
                id: arrivalButton

                property int type: FahrplanBackend.ArrivalStation

                title.text: qsTr("To")
                value: fahrplanBackend.arrivalStationName

                leadingActions: ListItemActions {
                    actions:[
                        Action {
                            iconName: "clear"
                            onTriggered: fahrplanBackend.resetStation(FahrplanBackend.ArrivalStation);
                        }
                    ]
                }
                trailingActions: ListItemActions {
                    actions: [
                        Action {
                            iconSource: "qrc:///src/gui/ubuntu/icons/swap-with-departure.svg"
                            onTriggered: fahrplanBackend.swapStations(FahrplanBackend.ArrivalStation, FahrplanBackend.DepartureStation)
                        },
                        Action {
                            iconSource: "qrc:///src/gui/ubuntu/icons/swap-with-via.svg"
                            onTriggered: fahrplanBackend.swapStations(FahrplanBackend.ArrivalStation, FahrplanBackend.ViaStation)
                        }
                    ]
                    delegate: Item {
                        width: height
                        Icon {
                            source: action.iconSource
                            width: units.gu(3)
                            height: width
                            anchors.centerIn: parent
                        }
                    }
                }

                onClicked: {
                    mainStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml", {type: type, title: title.text})
                }

                onPressAndHold: openMenu(arrivalButton)
            }

            CustomListItem {
                id: stationButton

                title.text: qsTr("Station")
                value: fahrplanBackend.currentStationName

                onClicked: {
                    mainStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml", {type: FahrplanBackend.CurrentStation, title: title.text})
                }
            }

            CustomListItem {
                id: directionButton

                title.text: qsTr("Direction")
                value: fahrplanBackend.directionStationName

                onClicked: {
                    mainStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml", {type: FahrplanBackend.DirectionStation})
                }

                onPressAndHold: timeTableSelectContextMenu.open()
            }

            ListItems.ValueSelector {
                id: timeModeSelector
                text: qsTr("<b>Date and time</b>")
                values: [qsTr("Now"), qsTr("Departure"), qsTr("Arrival")]
                selectedIndex: (fahrplanBackend.mode + 1) % 3
                onSelectedIndexChanged: fahrplanBackend.mode = (timeModeSelector.selectedIndex + 2) % 3
            }

            CustomListItem {
                id: datePickerButton

                title.text: qsTr("Date")
                value: Qt.formatDate(fahrplanBackend.dateTime)
                visible: timeModeSelector.selectedIndex !== 0

                onClicked: {
                    var selectedDateTime = fahrplanBackend.dateTime
                    var popupObj = PopupUtils.open(datePicker, datePickerButton, {date: selectedDateTime})
                    popupObj.accepted.connect(function(day, month, year) {
                        var newDate = selectedDateTime
                        newDate.setFullYear(year, month, day)
                        fahrplanBackend.dateTime = newDate
                    })
                }
            }

            CustomListItem {
                id: timePickerButton

                title.text: qsTr("Time")
                value: Qt.formatTime(fahrplanBackend.dateTime, 'HH:mm')
                visible: timeModeSelector.selectedIndex !== 0

                onClicked: {
                    var selectedDateTime = fahrplanBackend.dateTime;
                    var popupObj = PopupUtils.open(timePicker, timePickerButton, {time: selectedDateTime});
                    popupObj.accepted.connect(function(hour, minute) {
                        var newDate = selectedDateTime
                        newDate.setHours(hour, minute)
                        fahrplanBackend.dateTime = newDate;
                    })
                }
            }


            CustomListItem {
                id: trainrestrictionsButton
                title.text: qsTr("Transport Options")
                value: fahrplanBackend.trainrestrictionName
                onClicked: PopupUtils.open(selectTrainrestrictionsComponent, trainrestrictionsButton)
            }


            ListItem {
                divider.visible: false
                height: units.gu(8)
                Button {
                    id: timetableSearch

                    anchors { left: parent.left; right: parent.right; margins: units.gu(2); verticalCenter: parent.verticalCenter }
                    color: enabled ? UbuntuColors.green : UbuntuColors.warmGrey
                    enabled: stationButton.value !== qsTr("please select")
                    text: timeModeSelector.selectedIndex !== 2 ? qsTr("Show departures") : qsTr("Show arrivals")

                    onClicked: {
                        mainStack.push("qrc:///src/gui/ubuntu/TimeTableResultsPage.qml", {searchIndicatorVisible: true});
                        fahrplanBackend.getTimeTable();
                    }
                }

                Button {
                    id: startSearch

                    anchors { left: parent.left; right: parent.right; margins: units.gu(2); verticalCenter: parent.verticalCenter }
                    color: enabled ? UbuntuColors.green : UbuntuColors.warmGrey
                    enabled: departureButton.value !== qsTr("please select") && arrivalButton.value !== qsTr("please select")
                    text: qsTr("Plan my journey")

                    onClicked: {
                        mainStack.push("qrc:///src/gui/ubuntu/JourneyResultsPage.qml", {searchIndicatorVisible: true })
                        fahrplanBackend.searchJourney();
                    }
                }
            }
        }
    }

    Component {
        id: selectBackendComponent
        Dialog {
            title: qsTr("<b>Select backend</b>")
            id: selectBackendDialog
            contents: [
                ListView {
                    width: parent.width
                    height: units.gu(30)
                    clip: true
                    model: fahrplanBackend.backends
                    id: selectedBackendListView
                    delegate: ListItems.Standard {
                        text: name
                        onClicked: {
                            fahrplanBackend.setParser(fahrplanBackend.backends.getParserIdForItemIndex(index))
                            PopupUtils.close(selectBackendDialog)
                        }
                    }
                },

                Button {
                    text: qsTr("Close")
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: PopupUtils.close(selectBackendDialog)
                }
            ]
        }
    }

    Component {
        id: selectTrainrestrictionsComponent

        Dialog {
            id: selectTrainrestrictionsDialog
            title: qsTr("<b>Select transport mode</b>")
            ListView {
                width: parent.width
                height: contentHeight
                model: fahrplanBackend.trainrestrictions
                delegate: ListItems.Standard {
                    text: modelData
                    onClicked: {
                        fahrplanBackend.setTrainrestriction(index)
                        PopupUtils.close(selectTrainrestrictionsDialog)
                    }
                }
            }
        }
    }

    Component {
        id: datePicker
        DatePicker {}
    }

    Component {
        id: timePicker
        TimePicker {}
    }

    Component {
        id: stationSelectContextMenu
        Popover {
            id: stationSelectPopover
            property variant opener
            ListView {
                width: parent.width
                height: contentHeight
                model: contextMenuModel
                delegate: ListItems.Standard {
                    text: modelData
                    onClicked: {
                        switch(ListView.view.model.get(index).actionID) {
                        case 0:
                            print("ffffffffff", ListView.view.model.get(index).actionID, opener, opener.type)
                            fahrplanBackend.swapStations(opener.type, FahrplanBackend.DepartureStation)
                            break;
                        case 1:
                            fahrplanBackend.swapStations(opener.type, FahrplanBackend.ArrivalStation)
                            break;
                        case 2:
                            fahrplanBackend.swapStations(opener.type, FahrplanBackend.ViaStation)
                            break;
                        case 3:
                            fahrplanBackend.resetStation(opener.type);
                            break;
                        }
                        PopupUtils.close(stationSelectPopover)
                    }
                }
            }
        }
    }


    ListModel {
        id: contextMenuModel
    }

    function openMenu(opener)
    {
        contextMenuModel.clear();
        if (opener != departureButton) {
            contextMenuModel.append({modelData: qsTr("Switch with Departure station"), actionID: 0})
        }
        if (opener != arrivalButton) {
            contextMenuModel.append({modelData: qsTr("Switch with Arrival station"), actionID: 1})
        }
        if (opener != viaButton && fahrplanBackend.parser.supportsVia()) {
            contextMenuModel.append({modelData: qsTr("Switch with Via station"), actionID: 2})
        }
        contextMenuModel.append({modelData: qsTr("Clear station"), actionID: 3})

        PopupUtils.open(stationSelectContextMenu, opener, {"opener": opener});
    }

    Connections {
        target: fahrplanBackend

        /*
           An error can occour here, if the result is returned quicker than
           the pagestack is popped so we use a timer here if the pagestack is busy.
         */
        onParserJourneyResult: {
            if (result.count <= 0) {
                banner.text = qsTr("No results found");
                banner.show();
            }
        }

        onParserJourneyDetailsResult: {
            if (result.count <= 0) {
                banner.text = qsTr("Error loading details");
                banner.show();
            }
        }

        onParserErrorOccured: {
            print("Parser error:", msg)
            banner.text = msg;
            banner.show();
        }

        onParserChanged: {
            currentParserName.text = fahrplanBackend.parserShortName;
            updateButtonVisibility();

            //selectedBackendListView.currentIndex = fahrplanBackend.backends.getItemIndexForParserId(index);
        }
    }

}
