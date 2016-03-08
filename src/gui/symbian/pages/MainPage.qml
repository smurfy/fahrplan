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
import com.nokia.extras 1.1
import "../components"
import "../js/style.js" as Style
import Fahrplan 1.0

Page {
    id: mainPage

    tools: mainToolbar

    property int searchmode : 0
    property bool startup : true

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
            timetableSearch.visible = false;
            startSearch.visible = true;
            searchMode0Toggle.checked = true;
            searchMode1Toggle.checked = false;
        }
        if (searchmode == 1) {
            viaButton.visible = false;
            departureButton.visible = false;
            arrivalButton.visible = false;
            currentButton.visible = true;
            directionButton.visible = fahrplanBackend.parser.supportsTimeTableDirection();
            timetableSearch.visible = true;
            startSearch.visible = false;
            searchMode0Toggle.checked = false;
            searchMode1Toggle.checked = true;
        }
    }

    function updateModeCheckboxes()
    {
        if (fahrplanBackend.mode == FahrplanBackend.NowMode) {
            fromNowSwitch.checked = true;
            return;
        }
        fromNowSwitch.checked = false;
        if (fahrplanBackend.mode === FahrplanBackend.DepartureMode)
            modeSelect.checkedButton = modeDep;
        if (fahrplanBackend.mode === FahrplanBackend.ArrivalMode)
            modeSelect.checkedButton = modeArr;
    }

    Item {
        id: titleBar

        width: parent.width
        height: privateStyle.tabBarHeightPortrait
        z: 10

        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "#0d70c5";
                }
                GradientStop {
                    position: 1.0;
                    color: "#0a50a5";
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "#0b5795"
            visible: mouseArea.pressed
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            enabled: parent.enabled
            onClicked: {
                selectBackendDialog.open();
            }
        }

        Label {
            id: currentParserName
            anchors {
                left: parent.left
                leftMargin: 10
                verticalCenter: parent.verticalCenter
            }
            font.bold: true;
            font.pixelSize: privateStyle.statusBarHeight
            color: "White"

            text: fahrplanBackend.parserShortName
        }

        Image {
            id: icon

            source: Style.getIconFromTheme(false, "qtg_graf_choice_list_indicator")
            sourceSize {
                width: platformStyle.graphicSizeTiny
                height: platformStyle.graphicSizeTiny
            }
            anchors {
                right: parent.right
                rightMargin: 10
                verticalCenter: parent.verticalCenter
            }
        }
    }

    Flickable {
        id: flickable
        anchors {
            top: titleBar.bottom
            topMargin: platformStyle.paddingMedium
            bottom: parent.bottom
            bottomMargin: platformStyle.paddingMedium
        }
        width: mainPage.width
        contentWidth: buttons.width
        contentHeight: buttons.height
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: buttons

            anchors {
                left: parent.left
            }

            width: mainPage.width

            SubTitleButton {
                id: departureButton

                type: FahrplanBackend.DepartureStation
                titleText: qsTr("Departure Station")
                subTitleText: fahrplanBackend.departureStationName
                width: parent.width
                icon: "toolbar-next"
                platformInverted: appWindow.platformInverted

                onClicked: {
                    pageStack.push(stationSelectPage, { type: type })
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(departureButton);
                }
                onReset: {
                    fahrplanBackend.resetStation(type);
                }
            }
            SubTitleButton {
                id: viaButton

                type: FahrplanBackend.ViaStation
                titleText: qsTr("Via Station")
                subTitleText: fahrplanBackend.viaStationName
                width: parent.width
                icon: "toolbar-next"
                platformInverted: appWindow.platformInverted

                onClicked: {
                    pageStack.push(stationSelectPage, { type: type })
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(viaButton);
                }
                onReset: {
                    fahrplanBackend.resetStation(type);
                }
            }
            SubTitleButton {
                id: arrivalButton

                type: FahrplanBackend.ArrivalStation
                titleText: qsTr("Arrival Station")
                subTitleText: fahrplanBackend.arrivalStationName
                width: parent.width
                icon: "toolbar-next"
                platformInverted: appWindow.platformInverted

                onClicked: {
                    pageStack.push(stationSelectPage, { type: type })
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(arrivalButton);
                }
                onReset: {
                    fahrplanBackend.resetStation(type);
                }
            }
            SubTitleButton {
                id: currentButton

                type: FahrplanBackend.CurrentStation
                titleText: qsTr("Station")
                subTitleText: fahrplanBackend.currentStationName
                width: parent.width
                icon: "toolbar-next"
                platformInverted: appWindow.platformInverted

                onClicked: {
                    pageStack.push(stationSelectPage, { type: type })
                }
            }
            SubTitleButton {
                id: directionButton

                type: FahrplanBackend.DirectionStation
                titleText: qsTr("Direction")
                subTitleText: fahrplanBackend.directionStationName
                width: parent.width
                icon: "toolbar-next"
                platformInverted: appWindow.platformInverted

                onClicked: {
                    pageStack.push(stationSelectPage, { type: type })
                }
                onPressAndHold: {
                    timeTableSelectContextMenu.open();
                }
            }

            SubTitleButton {
                id: datePickerButton

                titleText: qsTr("Date")
                subTitleText: Qt.formatDate(fahrplanBackend.dateTime)
                width: parent.width
                visible: !fromNowSwitch.checked
                platformInverted: appWindow.platformInverted

                onClicked: {
                    datePicker.show();
                }
            }

            SubTitleButton {
                id: timePickerButton

                titleText: qsTr("Time")
                subTitleText: Qt.formatTime(fahrplanBackend.dateTime, Qt.DefaultLocaleShortDate)
                width: parent.width
                visible: !fromNowSwitch.checked
                platformInverted: appWindow.platformInverted

                onClicked: {
                    timePicker.show();
                }

                ButtonRow {
                    id: modeSelect

                    width: parent.width * 3 / 5
                    anchors {
                        right: parent.right
                        rightMargin: platformStyle.graphicSizeTiny + 2 * platformStyle.paddingMedium
                        verticalCenter: parent.verticalCenter
                    }

                    Button {
                        id: modeDep
                        text: qsTr("Departure")
                        platformInverted: appWindow.platformInverted
                        onClicked: {
                            fahrplanBackend.mode = FahrplanBackend.DepartureMode;
                        }
                    }
                    Button {
                        id: modeArr
                        text: qsTr("Arrival")
                        platformInverted: appWindow.platformInverted
                        onClicked: {
                            fahrplanBackend.mode = FahrplanBackend.ArrivalMode;
                        }
                    }
                }
            }

            SubTitleButton {
                titleText: qsTr("Departure: Now")
                iconVisible: false
                platformInverted: appWindow.platformInverted

                Switch {
                    id: fromNowSwitch

                    platformInverted: appWindow.platformInverted
                    anchors {
                        right: parent.right
                        rightMargin: platformStyle.paddingMedium
                        verticalCenter: parent.verticalCenter
                    }

                    onCheckedChanged: {
                        if (checked)
                            fahrplanBackend.mode = FahrplanBackend.NowMode;
                        else
                            fahrplanBackend.mode = FahrplanBackend.DepartureMode;
                    }
                }
            }

            SubTitleButton {
                id: trainrestrictionsButton
                titleText: qsTr("Trains")
                subTitleText: fahrplanBackend.trainrestrictionName
                width: parent.width
                platformInverted: appWindow.platformInverted
                onClicked: {
                    selectTrainrestrictionsDialog.open();
                }
                visible: selectTrainrestrictionsDialog.model.count > 0
            }

            Button {
                id: timetableSearch
                width: parent.width * 2 / 3
                text: modeDep.checked || fromNowSwitch.checked ? qsTr("Show departures") : qsTr("Show arrivals")
                anchors {
                    topMargin: 20
                    horizontalCenter: parent.horizontalCenter
                }
                platformInverted: appWindow.platformInverted

                onClicked: {
                    //Validation
                    if (currentButton.subTitleText == qsTr("please select")) {
                        banner.text = qsTr("Please select a Station");
                        banner.open();
                        return;
                    }

                    timetablePage.searchIndicatorVisible = true;
                    pageStack.push(timetablePage);

                    fahrplanBackend.getTimeTable();
                }
            }

            Button {
                id: startSearch
                width: parent.width * 2 / 3
                text: qsTr("Start search")
                anchors {
                    topMargin: 20
                    horizontalCenter: parent.horizontalCenter
                }
                platformInverted: appWindow.platformInverted

                onClicked: {
                    //Validation
                    if (departureButton.subTitleText == qsTr("please select") || arrivalButton.subTitleText == qsTr("please select")) {
                        banner.text = qsTr("Please select a departure and arrival station.");
                        banner.open();
                        return;
                    }

                    resultsPage.searchIndicatorVisible = true;
                    if (fahrplanBackend.viaStationName == qsTr("please select")) {
                       resultsPage.journeyStations.text = qsTr("<b>%1</b> to <b>%2</b>").arg(fahrplanBackend.departureStationName).arg(fahrplanBackend.arrivalStationName);
                    } else {
                       resultsPage.journeyStations.text = qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(fahrplanBackend.departureStationName).arg(fahrplanBackend.arrivalStationName).arg(fahrplanBackend.viaStationName);
                    }

                    pageStack.push(resultsPage)

                    fahrplanBackend.searchJourney();
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
        platformInverted: appWindow.platformInverted
    }

    SelectionDialog {
        id: selectBackendDialog
        titleText: qsTr("Select backend")
        platformInverted: appWindow.platformInverted
        model: fahrplanBackend.backends
        onAccepted: {
            fahrplanBackend.setParser(fahrplanBackend.backends.getParserIdForItemIndex(selectBackendDialog.selectedIndex))
        }
    }

    SelectionDialog {
        id: selectTrainrestrictionsDialog
        titleText: qsTr("Select train")
        model: fahrplanBackend.trainrestrictions
        platformInverted: appWindow.platformInverted
        selectedIndex: 0
        onAccepted: {
            fahrplanBackend.setTrainrestriction(selectTrainrestrictionsDialog.selectedIndex)
        }
    }

    DatePickerDialog {
        id: datePicker

        function show()
        {
            year = fahrplanBackend.dateTime.getFullYear();
            // JavaScript Date's month is 0 based while DatePicker's is 1 based.
            month = fahrplanBackend.dateTime.getMonth() + 1;
            day = fahrplanBackend.dateTime.getDate();
            open();
        }

        titleText: qsTr("Date")
        acceptButtonText: qsTr("Ok")
        rejectButtonText: qsTr("Cancel")
        platformInverted: appWindow.platformInverted

        onAccepted: {
            // We need to re-assign to selectedDateTime to trigger bindings.
            // Editing selectedDateTime directly won't trigger bindings
            // reevaluation because its tiggered only when the property itself
            // changes, not when something inside the object it holds does.
            var dateTime = fahrplanBackend.dateTime;
            dateTime.setFullYear(datePicker.year);
            // JavaScript Date's month is 0 based while DatePicker's is 1 based.
            dateTime.setMonth(datePicker.month - 1);
            dateTime.setDate(datePicker.day);
            fahrplanBackend.dateTime = dateTime;
        }
    }

    TimePickerDialog {
        id: timePicker

        function show ()
        {
            hour = fahrplanBackend.dateTime.getHours();
            minute = fahrplanBackend.dateTime.getMinutes();
            second = fahrplanBackend.dateTime.getSeconds();
            open();
        }

        titleText: qsTr("Time")
        acceptButtonText: qsTr("Ok")
        rejectButtonText: qsTr("Cancel")
        fields: DateTime.Hours | DateTime.Minutes
        hourMode: fahrplanBackend.timeFormat24h() ? DateTime.TwentyFourHours : DateTime.TwelveHours
        platformInverted: appWindow.platformInverted

        onAccepted: {
            // We need to re-assign to selectedDateTime to trigger bindings.
            // See explanation in datePicker::onAccepted.
            var dateTime = fahrplanBackend.dateTime;
            dateTime.setHours(timePicker.hour);
            dateTime.setMinutes(timePicker.minute);
            dateTime.setSeconds(timePicker.second);
            fahrplanBackend.dateTime = dateTime;
        }
    }

    ToolBarLayout {
        id: mainToolbar

        ToolButton {
            id: exitIcon
            iconSource: Style.getIconFromQrc(platformInverted, "icon-m-toolbar-close")
            platformInverted: appWindow.platformInverted
            onClicked: {
                Qt.quit();
            }
        }
        ButtonRow{
            ToolButton {
                id: searchMode0Toggle
                iconSource: Style.getIconFromQrc(platformInverted, "icon-m-toolbar-train")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    searchmode = 0;
                    updateButtonVisibility();
                }
                checkable: true
                checked: true
            }
            ToolButton {
                id: searchMode1Toggle
                iconSource: Style.getIconFromQrc(platformInverted, "icon-m-toolbar-clock");
                platformInverted: appWindow.platformInverted
                onClicked: {
                    searchmode = 1;
                    updateButtonVisibility();
                }
                checkable: true
                checked: false
            }
        }
        ToolButton {
            iconSource: "toolbar-settings"
            platformInverted: appWindow.platformInverted
            onClicked: pageStack.push(settingsPage);
        }
    }

    ContextMenu {
        id: timeTableSelectContextMenu
        platformInverted: appWindow.platformInverted

        MenuLayout {
            MenuItem {
                text: qsTr("Clear station")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    fahrplanBackend.resetStation(FahrplanBackend.DirectionStation);
                }
            }
        }
    }

    ContextMenu {
        id: stationSelectContextMenu
        property SubTitleButton opener
        platformInverted: appWindow.platformInverted

        MenuLayout {
            MenuItem {
                id: selectStationMenu
                text: qsTr("Select station")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    stationSelectContextMenu.opener.clicked();
                }
            }
            MenuItem {
                id: switchWithDepartureStation
                text: qsTr("Switch with Departure station")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    fahrplanBackend.swapStations(stationSelectContextMenu.opener.type, FahrplanBackend.DepartureStation)
                }
            }
            MenuItem {
                id: switchWithArrivalStation
                text: qsTr("Switch with Arrival station")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    fahrplanBackend.swapStations(stationSelectContextMenu.opener.type, FahrplanBackend.ArrivalStation)
                }
            }
            MenuItem {
                id: switchWithViaStation
                text: qsTr("Switch with Via station")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    fahrplanBackend.swapStations(stationSelectContextMenu.opener.type, FahrplanBackend.ViaStation)
                }
            }
            MenuItem {
                text: qsTr("Clear station")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    stationSelectContextMenu.opener.reset();
                }
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

            stationSelectContextMenu.open();
        }
    }

    FahrplanBackend {
        id: fahrplanBackend

        /*
           An error can occour here, if the result is returned quicker than
           the pagestack is popped so we use a timer here if the pagestack is busy.
         */
        onParserJourneyResult: {
            if (result.count <= 0) {
                banner.text = qsTr("No results found");
                banner.open();
            }
        }

        onParserJourneyDetailsResult: {
            if (result.count <= 0) {
                banner.text = qsTr("Error loading details");
                banner.open();
            }
        }

        onParserErrorOccured: {
            banner.text = msg;
            banner.open();
        }

        onModeChanged: {
            updateModeCheckboxes();
        }

        onParserChanged: {
            console.log("Switching to " + name);
            currentParserName.text = fahrplanBackend.parserName;

            updateButtonVisibility();

            selectBackendDialog.selectedIndex = fahrplanBackend.backends.getItemIndexForParserId(index);
            selectTrainrestrictionsDialog.selectedIndex = 0;
        }
    }

    // Timer which enables exit icon after 1 second
    Timer {
        id: exitIconDelay
        interval: 500
        onTriggered: {
            exitIcon.enabled = true
        }
    }

    StationSelectPage {
        id: stationSelectPage
    }

    JourneyResultsPage {
        id: resultsPage
    }

    TimetablePage {
        id: timetablePage
    }

    Component {
        id: settingsPage
        SettingsPage {}
    }

    // Disable exit icon when page deactivates and start
    // the timer to enable it back when it activates.
    onStatusChanged: {
        switch (status) {
        case PageStatus.Active:
            exitIconDelay.start();
            break;
        case PageStatus.Deactivating:
            exitIcon.enabled = false;
            break;
        default:
        }
    }

    Component.onCompleted: {
        updateModeCheckboxes();
        appWindow.platformInverted = fahrplanBackend.getSettingsValue("invertedStyle", "false");
    }
}
