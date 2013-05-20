import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1
import "components"
import Fahrplan 1.0

Page {
    id: mainPage

    tools: mainToolbar

    property int searchmode : 0
    property bool startup : true
    property string inverseSuffix: theme.inverted ? "-inverse" : ""
    property variant selectedDateTime

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
            searchMode0Toggle.checked = true;
            searchMode1Toggle.checked = false;
        }
        if (searchmode == 1) {
            viaButton.visible = false;
            departureButton.visible = false;
            arrivalButton.visible = false;
            stationButton.visible = true;
            directionButton.visible = fahrplanBackend.parser.supportsTimeTableDirection();
            timetableSearch.visible = true;
            startSearch.visible = false;
            searchMode0Toggle.checked = false;
            searchMode1Toggle.checked = true;
        }
    }

    function setToday() {
        selectedDateTime = new Date();
        datePicker.year = selectedDateTime.getFullYear();
        // JavaScript Date's month is 0 based while DatePicker's is 1 based.
        datePicker.month = selectedDateTime.getMonth() + 1;
        datePicker.day = selectedDateTime.getDate();
        timePicker.hour = selectedDateTime.getHours();
        timePicker.minute = selectedDateTime.getMinutes();
        timePicker.second = selectedDateTime.getSeconds();
    }

    Item {
        id: titleBar

        width: parent.width
        height: 70

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
            font.pixelSize: 32
            color: "White"

            text: fahrplanBackend.parserName
        }

        Image {
            id: icon

            anchors {
                right: parent.right
                rightMargin: 10
                verticalCenter: parent.verticalCenter
            }
            height: sourceSize.height
            width: sourceSize.width
            source: "image://theme/meegotouch-combobox-indicator-inverted"
        }
    }

    Flickable {
        id: flickable
        anchors {
            topMargin: 10
            top: titleBar.bottom
        }
        width: mainPage.width
        height: mainPage.height - titleBar.height - 20
        contentWidth: buttons.width
        contentHeight: buttons.height
        flickableDirection: Flickable.VerticalFlick

        onMovementStarted: {
            flickable.clip = true;
        }

        Column {
            id: buttons

            anchors {
                left: parent.left
            }

            width: mainPage.width

            SubTitleButton {
                id: departureButton
                titleText: qsTr("Departure Station")
                subTitleText: qsTr("please select")
                width: parent.width
                onClicked: {
                    pageStack.push(departureStationSelect)
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(departureButton);
                }
                icon: "image://theme/icon-m-common-drilldown-arrow" + inverseSuffix
            }
            SubTitleButton {
                id: viaButton
                titleText: qsTr("Via Station")
                subTitleText: qsTr("please select")
                width: parent.width
                onClicked: {
                    pageStack.push(viaStationSelect)
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(viaButton);
                }
                icon: "image://theme/icon-m-common-drilldown-arrow" + inverseSuffix
            }
            SubTitleButton {
                id: arrivalButton
                titleText: qsTr("Arrival Station")
                subTitleText: qsTr("please select")
                width: parent.width
                onClicked: {
                    pageStack.push(arrivalStationSelect)
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(arrivalButton);
                }
                icon: "image://theme/icon-m-common-drilldown-arrow" + inverseSuffix
            }
            SubTitleButton {
                id: stationButton
                titleText: qsTr("Station")
                subTitleText: qsTr("please select")
                width: parent.width
                onClicked: {
                    pageStack.push(stationStationSelect)
                }
                icon: "image://theme/icon-m-common-drilldown-arrow" + inverseSuffix
            }
            SubTitleButton {
                id: directionButton
                titleText: qsTr("Direction")
                subTitleText: qsTr("please select")
                width: parent.width
                onClicked: {
                    pageStack.push(directionStationSelect)
                }
                onPressAndHold: {
                    timeTableSelectContextMenu.open();
                }
                icon: "image://theme/icon-m-common-drilldown-arrow" + inverseSuffix
            }

            SubTitleButton {
                id: datePickerButton
                titleText: qsTr("Date")
                subTitleText: Qt.formatDate(selectedDateTime)
                width: parent.width
                visible: !fromNowSwitch.checked
                onClicked: {
                    datePicker.open();
                }
            }

            SubTitleButton {
                id: timePickerButton
                titleText: qsTr("Time")
                subTitleText: Qt.formatTime(selectedDateTime, qsTr("hh:mm"))
                width: parent.width
                visible: !fromNowSwitch.checked
                onClicked: {
                    timePicker.open();
                }

                ButtonRow {
                    anchors {
                        right: parent.right
                        rightMargin: 50
                        verticalCenter: parent.verticalCenter
                    }
                    width: parent.width * 3 / 5
                    Button {
                        id: modeDep
                        text: qsTr("Departure")
                    }
                    Button {
                        id: modeArr
                        text: qsTr("Arrival")
                    }
                }
            }

            SubTitleButton {
                titleText: qsTr("Departure: Now")
                iconVisible: false
                subTitleText: ""
                Switch {
                    id: fromNowSwitch
                    checked: fahrplanBackend.getSettingsValue("fromNow", false) == "true" ? true : false
                    anchors {
                        right: parent.right
                        rightMargin: 10
                        verticalCenter: parent.verticalCenter
                    }
                }
            }

            SubTitleButton {
                id: trainrestrictionsButton
                titleText: qsTr("Trains")
                subTitleText: selectTrainrestrictionsDialog.selectedIndex >= 0 ? selectTrainrestrictionsDialog.model.get(selectTrainrestrictionsDialog.selectedIndex).name : "None"
                width: parent.width
                onClicked: {
                    selectTrainrestrictionsDialog.open();
                }
            }

            Button {
                id: timetableSearch
                text: modeDep.checked || fromNowSwitch.checked ? qsTr("Show departures") : qsTr("Show arrivals")
                anchors {
                    topMargin: 20
                    horizontalCenter: parent.horizontalCenter
                }

                onClicked: {
                    fahrplanBackend.storeSettingsValue("stationStation", stationButton.subTitleText);
                    fahrplanBackend.storeSettingsValue("directionStation", directionButton.subTitleText);
                    fahrplanBackend.storeSettingsValue("fromNow", fromNowSwitch.checked)

                    //Validation
                    if (stationButton.subTitleText == qsTr("please select")) {
                        banner.text = qsTr("Please select a Station");
                        banner.show();
                        return;
                    }

                    var directionStation = directionButton.subTitleText;
                    if (directionStation == qsTr("please select") || !fahrplanBackend.parser.supportsTimeTableDirection()) {
                        directionStation = "";
                    }
                    if (fromNowSwitch.checked) {
                        setToday();
                    }
                    var selMode = ParserAbstract.Arrival;
                    if (modeDep.checked || fromNowSwitch.checked) {
                        selMode = ParserAbstract.Departure;
                        timetablePage.timetableTitleText = qsTr("Departures");
                    } else if (modeArr.checked) {
                        selMode = ParserAbstract.Arrival;
                        timetablePage.timetableTitleText = qsTr("Arrivals")
                    }

                    timetablePage.searchIndicatorVisible = true;
                    timetablePage.selMode = selMode

                    pageStack.push(timetablePage);
                    fahrplanBackend.parser.getTimeTableForStation(stationButton.subTitleText, directionStation, selectedDateTime, selectedDateTime, selMode,  selectTrainrestrictionsDialog.selectedIndex);
                }
            }

            Button {
                id: startSearch
                text: qsTr("Start search")
                anchors {
                    topMargin: 20
                    horizontalCenter: parent.horizontalCenter
                }

                onClicked: {
                    fahrplanBackend.storeSettingsValue("viaStation", viaButton.subTitleText);
                    fahrplanBackend.storeSettingsValue("departureStation", departureButton.subTitleText);
                    fahrplanBackend.storeSettingsValue("arrivalStation", arrivalButton.subTitleText);
                    fahrplanBackend.storeSettingsValue("fromNow", fromNowSwitch.checked)

                    //Validation
                    if (departureButton.subTitleText == qsTr("please select") || arrivalButton.subTitleText == qsTr("please select")) {
                        banner.text = qsTr("Please select a departure and arrival station.");
                        banner.show();
                        return;
                    }

                    var viaStation = viaButton.subTitleText;
                    if (viaStation == qsTr("please select") || !fahrplanBackend.parser.supportsVia()) {
                        viaStation = "";
                    }

                    resultsPage.journeyStationsTitleText = viaStation.length == 0 ? qsTr("<b>%1</b> to <b>%2</b>").arg(departureButton.subTitleText).arg(arrivalButton.subTitleText) : qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(departureButton.subTitleText).arg(arrivalButton.subTitleText).arg(viaStation);
                    resultsPage.searchIndicatorVisible = true;
                    pageStack.push(resultsPage)
                    if (fromNowSwitch.checked) {
                        setToday();
                    }
                    var selMode = ParserAbstract.Arrival;
                    if (modeDep.checked || fromNowSwitch.checked) {
                        selMode = ParserAbstract.Departure;
                    } else if (modeArr.checked) {
                        selMode = ParserAbstract.Arrival;
                    }
                    fahrplanBackend.parser.searchJourney(departureButton.subTitleText, arrivalButton.subTitleText, viaStation, selectedDateTime, selectedDateTime, selMode, selectTrainrestrictionsDialog.selectedIndex);
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }

    StationSelect {
        id: departureStationSelect

        onStationSelected: {
            departureButton.subTitleText = name;
            pageStack.pop();
        }
    }

    StationSelect {
        id: arrivalStationSelect

        onStationSelected: {
            arrivalButton.subTitleText = name;
            pageStack.pop();
        }
    }

    StationSelect {
        id: viaStationSelect

        onStationSelected: {
            viaButton.subTitleText = name;
            pageStack.pop();
        }
    }

    StationSelect {
        id: stationStationSelect

        onStationSelected: {
            stationButton.subTitleText = name;
            pageStack.pop();
        }
    }


    StationSelect {
        id: directionStationSelect

        onStationSelected: {
            directionButton.subTitleText = name;
            pageStack.pop();
        }
    }


    SelectionDialog {
        id: selectBackendDialog
        titleText: qsTr("Select backend")
        model: parserBackendModel
        onAccepted: {
            fahrplanBackend.setParser(selectBackendDialog.selectedIndex);
        }
    }

    ListModel {
        id: parserBackendModel
    }

    SelectionDialog {
        id: selectTrainrestrictionsDialog
        titleText: qsTr("Select train")
        model: trainrestrictionsModel
        onAccepted: {
           trainrestrictionsButton.subTitleText = selectTrainrestrictionsDialog.selectedIndex >= 0 ? selectTrainrestrictionsDialog.model.get(selectTrainrestrictionsDialog.selectedIndex).name : "None"
        }
    }

    ListModel {
        id: trainrestrictionsModel
    }

    JourneyResultsPage {
        id: resultsPage
    }

    TimeTableResultsPage {
        id: timetablePage
    }

    DatePickerDialog {
        id: datePicker
        titleText: qsTr("Date")
        acceptButtonText: qsTr("Ok")
        rejectButtonText: qsTr("Cancel")
        onAccepted: {
            // We need to re-assign to selectedDateTime to trigger bindings.
            // Editing selectedDateTime directly won't trigger bindings
            // reevaluation because its tiggered only when the property itself
            // changes, not when something inside the object it holds does.
            var dateTime = selectedDateTime;
            dateTime.setFullYear(datePicker.year);
            // JavaScript Date's month is 0 based while DatePicker's is 1 based.
            dateTime.setMonth(datePicker.month - 1);
            dateTime.setDate(datePicker.day);
            selectedDateTime = dateTime;
        }
    }

    TimePickerDialog {
        id: timePicker
        titleText: qsTr("Time")
        acceptButtonText: qsTr("Ok")
        rejectButtonText: qsTr("Cancel")
        fields: DateTime.Hours | DateTime.Minutes
        hourMode: {
            if (qsTr("hh:mm").toLowerCase().indexOf("ap") < 0)
                return DateTime.TwentyFourHours;
            else
                return DateTime.TwelveHours;
        }

        onAccepted: {
            // We need to re-assign to selectedDateTime to trigger bindings.
            // See explanation in datePicker::onAccepted.
            var dateTime = selectedDateTime;
            dateTime.setHours(timePicker.hour);
            dateTime.setMinutes(timePicker.minute);
            dateTime.setSeconds(timePicker.second);
            selectedDateTime = dateTime;
        }
    }

    ToolBarLayout {
        id: mainToolbar

        ButtonRow{
            ToolButton {
                id: searchMode0Toggle
                platformStyle: TabButtonStyle{}
                iconSource: "qrc:/src/gui/harmattan/icon/icon-m-toolbar-train" + inverseSuffix + ".svg";
                onClicked: {
                    searchmode = 0;
                    updateButtonVisibility();
                }
                flat: true
                checkable: true
                checked: true
            }
            ToolButton {
                id: searchMode1Toggle
                platformStyle: TabButtonStyle{}
                iconSource: "qrc:/src/gui/harmattan/icon/icon-m-toolbar-clock" + inverseSuffix + ".svg";
                onClicked: {
                    searchmode = 1;
                    updateButtonVisibility();
                }
                flat: false
                checkable: true
                checked: false
            }
        }
        ToolIcon {
            iconId: "toolbar-settings"
            onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"));
        }
    }

    InfoBanner {
            id: banner
            objectName: "fahrplanInfoBanner"
            text: ""
            anchors.top: parent.top
            anchors.topMargin: 10
    }

    QueryDialog {
            id: firstStartDialog;
            message: qsTr("Hello<br/>" +
                "Sorry about this dialog, but the new Nokia store regulations require it.<br/><br/>" +
                "Please read the Privacy Policy of fahrplan. (available on the about page)<br/><br/>" +
                "If you want, you can also disable the gps features of fahrplan via the about menu."
            )
            acceptButtonText: qsTr("Ok")
    }


    ContextMenu {
        id: timeTableSelectContextMenu
        MenuLayout {
            MenuItem {
                text: qsTr("Clear station")
                onClicked: {
                    directionButton.subTitleText = qsTr("please select")
                }
            }
        }
    }

    ContextMenu {
        property SubTitleButton opener
        id: stationSelectContextMenu
        MenuLayout {
            MenuItem {
                id: selectStationMenu
                text: qsTr("Select station")
                onClicked: {
                    stationSelectContextMenu.opener.clicked();
                }
            }
            MenuItem {
                id: switchWithDepartureStation
                text: qsTr("Switch with Departure station")
                onClicked: {
                    var oldVal = stationSelectContextMenu.opener.subTitleText
                    stationSelectContextMenu.opener.subTitleText = departureButton.subTitleText
                    departureButton.subTitleText = oldVal;
                }
            }
            MenuItem {
                id: switchWithArrivalStation
                text: qsTr("Switch with Arrival station")
                onClicked: {
                    var oldVal = stationSelectContextMenu.opener.subTitleText
                    stationSelectContextMenu.opener.subTitleText = arrivalButton.subTitleText
                    arrivalButton.subTitleText = oldVal;
                }
            }
            MenuItem {
                id: switchWithViaStation
                text: qsTr("Switch with Via station")
                onClicked: {
                    var oldVal = stationSelectContextMenu.opener.subTitleText
                    stationSelectContextMenu.opener.subTitleText = viaButton.subTitleText
                    viaButton.subTitleText = oldVal;
                }
            }
            MenuItem {
                text: qsTr("Clear station")
                onClicked: {
                    stationSelectContextMenu.opener.subTitleText = qsTr("please select")
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
                viaButton.subTitleText = fahrplanBackend.getSettingsValue("viaStation", qsTr("please select"));
                departureButton.subTitleText = fahrplanBackend.getSettingsValue("departureStation", qsTr("please select"));
                arrivalButton.subTitleText = fahrplanBackend.getSettingsValue("arrivalStation", qsTr("please select"));
                stationButton.subTitleText = fahrplanBackend.getSettingsValue("stationStation", qsTr("please select"));
                directionButton.subTitleText = fahrplanBackend.getSettingsValue("directionStation", qsTr("please select"));
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

                selectBackendDialog.selectedIndex = index;
            }

            trainrestrictionsModel.clear();
            items = fahrplanBackend.parser.getTrainRestrictions();
            trainrestrictionsButton.visible = items.length > 1;
            for (i = 0; i < items.length; i++) {
                trainrestrictionsModel.append({
                    "name" : items[i]
                });
            }
            selectTrainrestrictionsDialog.selectedIndex = (items.length > 0) ? 0 : -1;
            if (selectTrainrestrictionsDialog.selectedIndex >= 0) {
                var selObj = selectTrainrestrictionsDialog.model.get(selectTrainrestrictionsDialog.selectedIndex)
                if (selObj) {
                    trainrestrictionsButton.subTitleText = selObj.name
                }
            }
        }
    }

    onStatusChanged: {
        switch (status) {
            case PageStatus.Active:
                if (fahrplanBackend.getSettingsValue("firstStart", "true") == "true") {
                    firstStartDialog.open();
                    fahrplanBackend.storeSettingsValue("firstStart", "false");
                }
                break;
        }
    }

    Component.onCompleted: {
        theme.inverted = fahrplanBackend.getSettingsValue("invertedStyle", "false");
        setToday();
    }
}
