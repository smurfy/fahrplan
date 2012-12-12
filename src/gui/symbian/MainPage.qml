import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1
import "components"
import "js/style.js" as Style
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

        if (fromNowSwitch.checked ==true) {
            datePickerButton.visible = false;
            timePickerButton.visible = false;
            modeDep.checked = true;
            setToday();
        } else {
            datePickerButton.visible = true;
            timePickerButton.visible = true;

    }

    }

    Item {
        id: titleBar

        width: parent.width
        height: 60

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
            font.pixelSize: 26
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
            source: Style.getIconFromTheme(false, "qtg_graf_choice_list_indicator")
        }
    }

    Flickable {
        id: flickable
        anchors {
            topMargin: platformStyle.paddingMedium
            top: titleBar.bottom
        }
        width: mainPage.width
        height: mainPage.height - titleBar.height - (2 * platformStyle.paddingMedium)
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
                icon: "toolbar-next"
                platformInverted: appWindow.platformInverted
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
                icon: "toolbar-next"
                platformInverted: appWindow.platformInverted
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
                icon: "toolbar-next"
                platformInverted: appWindow.platformInverted
            }
            SubTitleButton {
                id: stationButton
                titleText: qsTr("Station")
                subTitleText: qsTr("please select")
                width: parent.width
                onClicked: {
                    pageStack.push(stationStationSelect)
                }
                icon: "toolbar-next"
                platformInverted: appWindow.platformInverted
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
                platformInverted: appWindow.platformInverted
                icon: "toolbar-next"
            }
            SubTitleButton {
                titleText: qsTr("Departure: Now")
                iconVisible: false
                subTitleHeight: 0
                platformInverted: appWindow.platformInverted
                Switch {
                    id: fromNowSwitch
                    signal clicked
                    Component.onCompleted: {
                        if(fahrplanBackend.getSettingsValue("fromNow", false)=="true") {
                            fromNowSwitch.checked = true
                        }
                        else {
                            fromNowSwitch.checked = false
                        }
                        updateButtonVisibility()
                    }
                    anchors {
                        right: parent.right
                        rightMargin: platformStyle.paddingMedium
                        verticalCenter: parent.verticalCenter

                    }
                    onClicked: {
                        updateButtonVisibility()
                        if (checked == true)
                            {
                            fahrplanBackend.storeSettingsValue("fromNow", true)
                        } else
                        {
                            fahrplanBackend.storeSettingsValue("fromNow", false)
                        }

                    }
            }
       }



            SubTitleButton {
                id: datePickerButton
                titleText: qsTr("Date")
                subTitleText: qsTr("please select")
                width: parent.width
                platformInverted: appWindow.platformInverted
                onClicked: {
                    datePicker.open();
                }
            }

            SubTitleButton {
                id: timePickerButton
                titleText: qsTr("Time")
                subTitleText: qsTr("please select")
                width: parent.width
                platformInverted: appWindow.platformInverted
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
                        platformInverted: appWindow.platformInverted
                    }
                    Button {
                        id: modeArr
                        text: qsTr("Arrival")
                        platformInverted: appWindow.platformInverted
                    }
                }
            }

            SubTitleButton {
                id: trainrestrictionsButton
                titleText: qsTr("Trains")
                subTitleText: selectTrainrestrictionsDialog.selectedIndex >= 0 ? selectTrainrestrictionsDialog.model.get(selectTrainrestrictionsDialog.selectedIndex).name : "None"
                width: parent.width
                platformInverted: appWindow.platformInverted
                onClicked: {
                    selectTrainrestrictionsDialog.open();
                }
            }

            Button {
                id: timetableSearch
                width: parent.width * 2 / 3
                text: modeDep.checked ? qsTr("Show departures") : qsTr("Show arrivals")
                anchors {
                    topMargin: 20
                    horizontalCenter: parent.horizontalCenter
                }
                platformInverted: appWindow.platformInverted

                onClicked: {
                    fahrplanBackend.storeSettingsValue("stationStation", stationButton.subTitleText);
                    fahrplanBackend.storeSettingsValue("directionStation", directionButton.subTitleText);

                    //Validation
                    if (stationButton.subTitleText == qsTr("please select")) {
                        banner.text = qsTr("Please select a Station");
                        banner.open();
                        return;
                    }

                    var directionStation = directionButton.subTitleText;
                    if (directionStation == qsTr("please select") || !fahrplanBackend.parser.supportsTimeTableDirection()) {
                        directionStation = "";
                    }

                    if (fromNowSwitch.checked == true) {
                        setToday();
                    }
                    var selDate = new Date(datePicker.year, datePicker.month - 1, datePicker.day);
                    var selTime = new Date(1970, 2, 1, timePicker.hour, timePicker.minute, timePicker.second);
                    var selMode = ParserAbstract.Arrival;
                    if (modeDep.checked) {
                        selMode = ParserAbstract.Departure;
                        timetablePage.timetableTitleText = qsTr("Departures");
                    }
                    if (modeArr.checked) {
                        selMode = ParserAbstract.Arrival;
                        timetablePage.timetableTitleText = qsTr("Arrivals")
                    }

                    timetablePage.searchIndicatorVisible = true;
                    timetablePage.selMode = selMode

                    pageStack.push(timetablePage);


                    fahrplanBackend.parser.getTimeTableForStation(stationButton.subTitleText, directionStation, selDate, selTime, selMode,  selectTrainrestrictionsDialog.selectedIndex);
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

                    fahrplanBackend.storeSettingsValue("viaStation", viaButton.subTitleText);
                    fahrplanBackend.storeSettingsValue("departureStation", departureButton.subTitleText);
                    fahrplanBackend.storeSettingsValue("arrivalStation", arrivalButton.subTitleText);

                    //Validation
                    if (departureButton.subTitleText == qsTr("please select") || arrivalButton.subTitleText == qsTr("please select")) {
                        banner.text = qsTr("Please select a departure and arrival station.");
                        banner.open();
                        return;
                    }

                    var viaStation = viaButton.subTitleText;
                    if (viaStation == qsTr("please select") || !fahrplanBackend.parser.supportsVia()) {
                        viaStation = "";
                    }

                    resultsPage.journeyStationsTitleText = viaStation.length == 0 ? qsTr("<b>%1</b> to <b>%2</b>").arg(departureButton.subTitleText).arg(arrivalButton.subTitleText) : qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(departureButton.subTitleText).arg(arrivalButton.subTitleText).arg(viaStation);
                    resultsPage.searchIndicatorVisible = true;
                    pageStack.push(resultsPage)
                    if (fromNowSwitch.checked ==true) {
                        setToday();
                    }
                    var selDate = new Date(datePicker.year, datePicker.month - 1, datePicker.day);
                    var selTime = new Date(1970, 2, 1, timePicker.hour, timePicker.minute, timePicker.second);
                    var selMode = ParserAbstract.Arrival;
                    if (modeDep.checked) {
                        selMode = ParserAbstract.Departure;
                    }
                    if (modeArr.checked) {
                        selMode = ParserAbstract.Arrival;
                    }
                    fahrplanBackend.parser.searchJourney(departureButton.subTitleText, arrivalButton.subTitleText, viaStation, selDate, selTime, selMode, selectTrainrestrictionsDialog.selectedIndex);
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
        platformInverted: appWindow.platformInverted
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
        platformInverted: appWindow.platformInverted
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
        platformInverted: appWindow.platformInverted
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

    AboutPage {
        id: aboutPage
    }

    DatePickerDialog {
        id: datePicker
        titleText: qsTr("Date")
        acceptButtonText: qsTr("Ok")
        rejectButtonText: qsTr("Cancel")
        platformInverted: appWindow.platformInverted
        onAccepted: {
            var selDate = new Date(datePicker.year, datePicker.month - 1, datePicker.day);
            datePickerButton.subTitleText = Qt.formatDate(selDate);
        }
    }

    TimePickerDialog {
        id: timePicker
        titleText: qsTr("Time")
        acceptButtonText: qsTr("Ok")
        rejectButtonText: qsTr("Cancel")
        fields: DateTime.Hours | DateTime.Minutes
        hourMode: DateTime.TwentyFourHours // FIXME should set through i18n
        platformInverted: appWindow.platformInverted
        onAccepted: {
            var selTime = new Date(1970, 2, 1, timePicker.hour, timePicker.minute, timePicker.second);
            timePickerButton.subTitleText = Qt.formatTime(selTime, "hh:mm");
        }
        Component.onCompleted: {
            //setToday sets both the time and the date to today, so only needed here (seems to work)
            setToday();
        }
    }

    ToolBarLayout {
        id: mainToolbar

        ToolButton {
            id: exitIcon
            iconSource: Style.getIconFromQrc(platformInverted, "icon-m-toolbar-close", ".png")
            platformInverted: appWindow.platformInverted
            onClicked: {
                Qt.quit();
            }
        }
        ButtonRow{
            ToolButton {
                id: searchMode0Toggle
                iconSource: Style.getIconFromQrc(platformInverted, "icon-m-toolbar-train", ".svg")
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
                iconSource: Style.getIconFromQrc(platformInverted, "icon-m-toolbar-clock", ".svg");
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
            id : aboutIcon
            iconSource: "toolbar-menu"
            platformInverted: appWindow.platformInverted
            onClicked: {
                mainMenu.open();
            }
        }
    }

    Menu {
        id: mainMenu
        platformInverted: appWindow.platformInverted
        content: MenuLayout {
            MenuItem {
                text: qsTr("Toggle Inverted Style")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    appWindow.platformInverted = !appWindow.platformInverted;
                }
            }
            MenuItem {
                text: qsTr("About")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    pageStack.push(aboutPage);
                }
            }
        }
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
            if (selectTrainrestrictionsDialog.selectedIndex >= 0 &&  selectTrainrestrictionsDialog.model) {
                var selObj = selectTrainrestrictionsDialog.model.get(selectTrainrestrictionsDialog.selectedIndex)
                if (selObj) {
                    trainrestrictionsButton.subTitleText = selObj.name
                }
            }
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

    function setToday() {
        var d = new Date();
        timePicker.hour = d.getHours();
        timePicker.minute = d.getMinutes();
        timePicker.second = d.getSeconds();
        timePickerButton.subTitleText = Qt.formatTime(d, "hh:mm");
        datePicker.year = d.getFullYear();
        datePicker.month = d.getMonth() + 1; // month is 0 based in Date()
        datePicker.day = d.getDate();
        datePickerButton.subTitleText = Qt.formatDate(d);
    }

}
