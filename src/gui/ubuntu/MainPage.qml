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
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems
import "components"
import Fahrplan 1.0

Page {
    id: mainPage

    tools: mainToolbar

    property int searchmode : 0
    property bool startup : true
    property variant selectedDateTime

    Component.onCompleted: {
        setToday();
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

    function setToday() {
        selectedDateTime = new Date();
    }

    Column {
        anchors.fill: parent
        Item {
            id: titleBar
            property color color: "#dd4814"

            width: parent.width
            height: units.gu(6)

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
                onClicked: {
                    PopupUtils.open(selectBackendComponent, titleBar);
                }
            }

            Label {
                id: currentParserName
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: units.gu(2)
                    verticalCenter: parent.verticalCenter
                }
                font.bold: true;
                fontSize: "large"
                color: "White"

                text: fahrplanBackend.parserName
            }

            Image {
                id: icon

                anchors {
                    right: parent.right
                    rightMargin: units.gu(1)
                    verticalCenter: parent.verticalCenter
                }
                height: sourceSize.height
                width: sourceSize.width
                //source: "image://theme/meegotouch-combobox-indicator-inverted"
            }
        }

        Flickable {
            id: flickable
            anchors {
                left: parent.left
                right: parent.right
            }
            height: parent.height - y

            onMovementStarted: {
                flickable.clip = true;
            }

            Column {
                id: buttons

                anchors {
                    left: parent.left
                    right: parent.right
                }


                ListItems.Subtitled {
                    id: departureButton
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    progression: true

                    text: qsTr("Departure Station")
                    subText: qsTr("please select")

                    onClicked: {
                        pageStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml")
                        pageStack.currentPage.stationSelected.connect(function(name) {
                                                                          departureButton.subText = name;
                                                                          pageStack.pop();

                                                                      })
                    }
                    onPressAndHold: {
                        openMenu(departureButton);
                    }
                }
                ListItems.Subtitled {
                    id: viaButton
                    text: qsTr("Via Station")
                    subText: qsTr("please select")
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    progression: true
                    onClicked: {
                        pageStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml")
                        pageStack.currentPage.stationSelected.connect(function(name) {
                                                                          viaButton.subText = name;
                                                                          pageStack.pop();

                                                                      })
                    }
                    onPressAndHold: {
                        openMenu(viaButton);
                    }
                }
                ListItems.Subtitled {
                    id: arrivalButton
                    text: qsTr("Arrival Station")
                    subText: qsTr("please select")
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    progression: true
                    onClicked: {
                        pageStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml")
                        pageStack.currentPage.stationSelected.connect(function(name) {
                                                                          arrivalButton.subText = name;
                                                                          pageStack.pop();

                                                                      })
                    }
                    onPressAndHold: {
                        openMenu(arrivalButton);
                    }
                }
                ListItems.Subtitled {
                    id: stationButton
                    text: qsTr("Station")
                    subText: qsTr("please select")
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    progression: true
                    onClicked: {
                        pageStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml")
                        pageStack.currentPage.stationSelected.connect(function(name) {
                                                                          stationButton.subText = name;
                                                                          pageStack.pop();

                                                                      })
                    }
                }
                ListItems.Subtitled {
                    id: directionButton
                    text: qsTr("Direction")
                    subText: qsTr("please select")
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    progression: true
                    onClicked: {
                        pageStack.push("qrc:///src/gui/ubuntu/components/StationSelect.qml")
                        pageStack.currentPage.stationSelected.connect(function(name) {
                                                                          directionButton.subText = name;
                                                                          pageStack.pop();

                                                                      })
                    }
                    onPressAndHold: {
                        timeTableSelectContextMenu.open();
                    }
                }

                ListItems.ValueSelector {
                    id: timeModeSelector
                    text: qsTr("Date and time")
                    values: [qsTr("Now"), qsTr("Departure"), qsTr("Arrival")]
                }

                ListItems.Subtitled {
                    id: datePickerButton
                    text: qsTr("Date")
                    subText: Qt.formatDate(selectedDateTime)
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    visible: timeModeSelector.selectedIndex !== 0
                    onClicked: {
                        var popupObj = PopupUtils.open(datePicker, datePickerButton, {day: selectedDateTime.getDate(), month: selectedDateTime.getMonth(), year: selectedDateTime.getFullYear()})
                        popupObj.accepted.connect(function(day, month, year) {
                                                      var newDate = selectedDateTime
                                                      newDate.setFullYear(year, month, day)
                                                      selectedDateTime = newDate
                                                  })

                    }
                }

                ListItems.Subtitled {
                    id: timePickerButton
                    text: qsTr("Time")
                    subText: Qt.formatTime(selectedDateTime, qsTr("hh:mm"))
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    visible: timeModeSelector.selectedIndex !== 0
                    onClicked: {
                        var popupObj = PopupUtils.open(timePicker, timePickerButton, {hour: selectedDateTime.getHours(), minute: selectedDateTime.getMinutes()});
                        popupObj.accepted.connect(function(hour, minute) {
                                                      var newDate = selectedDateTime
                                                      newDate.setHours(hour, minute)
                                                      selectedDateTime = newDate
                                                  })
                    }
                }


                ListItems.Subtitled {
                    id: trainrestrictionsButton
                    text: qsTr("Trains")
                    subText: selectTrainrestrictionsComponent.selectedIndex >= 0 ? selectTrainrestrictionsComponent.model.get(selectTrainrestrictionsComponent.selectedIndex).name : "None"
                    width: parent.width
                    onClicked: {
                        PopupUtils.open(selectTrainrestrictionsComponent, trainrestrictionsButton);
                    }
                }


                ListItems.Standard {
                    showDivider: false
                    Button {
                        id: timetableSearch
                        text: timeModeSelector.selectedIndex !== 2 ? qsTr("Show departures") : qsTr("Show arrivals")
                        enabled: stationButton.subText !== qsTr("please select")
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: units.gu(2)
                            verticalCenter: parent.verticalCenter
                        }

                        onClicked: {
                            fahrplanBackend.storeSettingsValue("stationStation", stationButton.subText);
                            fahrplanBackend.storeSettingsValue("directionStation", directionButton.subText);
                            fahrplanBackend.storeSettingsValue("fromNow", timeModeSelector.selectedIndex === 0)

                            var directionStation = directionButton.subText;
                            if (directionStation == qsTr("please select") || !fahrplanBackend.parser.supportsTimeTableDirection()) {
                                directionStation = "";
                            }
                            if (timeModeSelector.selectedIndex === 0) {
                                setToday();
                            }
                            var selMode = ParserAbstract.Arrival;
                            var titleText;
                            if (timeModeSelector.selectedIndex !== 2) {
                                selMode = ParserAbstract.Departure;
                                titleText = qsTr("Departures");
                            } else {
                                selMode = ParserAbstract.Arrival;
                                titleText = qsTr("Arrivals")
                            }

                            pageStack.push("qrc:///src/gui/ubuntu/TimeTableResultsPage.qml", {timetableTitleText: titleText, searchIndicatorVisible: true, selMode: selMode});
                            fahrplanBackend.parser.getTimeTableForStation(stationButton.subText, directionStation, selectedDateTime, selectedDateTime, selMode,  selectTrainrestrictionsComponent.selectedIndex);
                        }
                    }
                    Button {
                        id: startSearch
                        text: qsTr("Start search")
                        enabled: departureButton.subText !== qsTr("please select") && arrivalButton.subText !== qsTr("please select")
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: units.gu(2)
                            verticalCenter: parent.verticalCenter
                        }

                        onClicked: {
                            fahrplanBackend.storeSettingsValue("viaStation", viaButton.subText);
                            fahrplanBackend.storeSettingsValue("departureStation", departureButton.subText);
                            fahrplanBackend.storeSettingsValue("arrivalStation", arrivalButton.subText);
                            fahrplanBackend.storeSettingsValue("fromNow", timeModeSelector.selectedIndex === 0)

                            //Validation
                            var viaStation = viaButton.subText;
                            if (viaStation == qsTr("please select") || !fahrplanBackend.parser.supportsVia()) {
                                viaStation = "";
                            }

                            var titleText = viaStation.length == 0 ? qsTr("<b>%1</b> to <b>%2</b>").arg(departureButton.subText).arg(arrivalButton.subText) : qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(departureButton.subText).arg(arrivalButton.subText).arg(viaStation);
                            var searchVisible = true;
                            pageStack.push("qrc:///src/gui/ubuntu/JourneyResultsPage.qml", {journeyStationsTitleText: titleText, searchIndicatorVisible: searchVisible })
                            if (timeModeSelector.selectedIndex === 0) {
                                setToday();
                            }
                            var selMode = ParserAbstract.Arrival;
                            if (timeModeSelector.selectedIndex !== 2) {
                                selMode = ParserAbstract.Departure;
                            } else {
                                selMode = ParserAbstract.Arrival;
                            }
                            fahrplanBackend.parser.searchJourney(departureButton.subText, arrivalButton.subText, viaStation, selectedDateTime, selectedDateTime, selMode, selectTrainrestrictionsComponent.selectedIndex);
                        }
                    }
                }
            }
        }
    }

    Component {
        id: selectBackendComponent
        Dialog {
            title: qsTr("Select backend")
            id: selectBackendDialog
            ListView {
                width: parent.width
                height: contentHeight
                model: parserBackendModel
                delegate: ListItems.Standard {
                    text: modelData
                    onClicked: {
                        fahrplanBackend.setParser(index);
                        PopupUtils.close(selectBackendDialog)
                    }
                }

            }

        }
    }

    ListModel {
        id: parserBackendModel
    }

    Component {
        id: selectTrainrestrictionsComponent

        Dialog {
            id: selectTrainrestrictionsDialog
            title: qsTr("Select train")
            ListView {
                width: parent.width
                height: contentHeight
                model: trainrestrictionsModel
                delegate: ListItems.Standard {
                    text: modelData
                    onClicked: {
                       trainrestrictionsButton.subText = trainrestrictionsModel.get(index).name
                       PopupUtils.close(selectTrainrestrictionsDialog)
                    }
                }
            }
        }
    }

    ListModel {
        id: trainrestrictionsModel
    }

    Component {
        id: datePicker
        DatePicker {

        }
    }

    Component {
        id: timePicker
        TimePicker {

        }
    }

    ToolbarActions {
        id: mainToolbar

        // Not using settings on ubuntu yet...
//        Action {
//            iconSource: "file:///usr/share/icons/ubuntu-mobile/actions/scalable/settings.svg"
//            onTriggered: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"));
//        }
        Action {
            iconSource: "file:///usr/share/icons/ubuntu-mobile/actions/scalable/go-to.svg"
            onTriggered: pageStack.push(Qt.resolvedUrl("AboutPage.qml"));
        }
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
                        print("ffffffffff", ListView.view.model.get(index).actionID, ListView.view.model.get(index).modelData)
                        switch(ListView.view.model.get(index).actionID) {
                        case 0:
                            var oldVal = stationSelectPopover.opener.subText
                            stationSelectPopover.opener.subText = departureButton.subText
                            departureButton.subText = oldVal;
                            break;
                        case 1:
                            var oldVal = stationSelectPopover.opener.subText
                            stationSelectPopover.opener.subText = arrivalButton.subText
                            arrivalButton.subText = oldVal;
                            break;
                        case 2:
                            var oldVal = stationSelectPopover.opener.subText
                            stationSelectPopover.opener.subText = viaButton.subText
                            viaButton.subText = oldVal;
                            break;
                        case 3:
                            stationSelectPopover.opener.subText = qsTr("please select")
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

    FahrplanBackend {
        id: fahrplanBackend

        Component.onCompleted: {
            var items = fahrplanBackend.getParserList();
            var index = 0;
            for (var i = 0; i < items.length; ++i) {
                if (items[i] == fahrplanBackend.parserName) {
                    index = i;
                }
            }

            onParserChanged(fahrplanBackend.parserName, index)
        }
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
            banner.text = msg;
            banner.show();
        }

        onParserChanged: {

            console.log("Switching to " + name);
            currentParserName.text = fahrplanBackend.parserName;

            if (startup) {
                viaButton.subText = fahrplanBackend.getSettingsValue("viaStation", qsTr("please select"));
                departureButton.subText = fahrplanBackend.getSettingsValue("departureStation", qsTr("please select"));
                arrivalButton.subText = fahrplanBackend.getSettingsValue("arrivalStation", qsTr("please select"));
                stationButton.subText = fahrplanBackend.getSettingsValue("stationStation", qsTr("please select"));
                directionButton.subText = fahrplanBackend.getSettingsValue("directionStation", qsTr("please select"));
                startup = false;
            }


            updateButtonVisibility();

            var items;
            var i;

            if (parserBackendModel.count == 0) {
                items = fahrplanBackend.getParserList();
                for (i = 0; i < items.length; i++) {
                    parserBackendModel.append({
                        "name" : items[i]
                    });
                }

            }

            trainrestrictionsModel.clear();
            items = fahrplanBackend.parser.getTrainRestrictions();
            trainrestrictionsButton.visible = items.length > 1;
            for (i = 0; i < items.length; i++) {
                trainrestrictionsModel.append({
                    "name" : items[i]
                });
            }
            if (items.length > 0) {
                trainrestrictionsButton.subText = trainrestrictionsModel.get(0).name
            }
        }
    }
}
