import QtQuick 1.1
import com.meego 1.0
import com.nokia.extras 1.0
import "components"
import Fahrplan 1.0 as Fahrplan

Page {
    id: mainPage

    tools: mainToolbar

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
                top: parent.top
                topMargin: 20
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
            bottom: mainToolbar.top
            bottomMargin: 10
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
                titleText: "Departure Station"
                subTitleText: "please select"
                width: parent.width
                onClicked: {
                    pageStack.push(departureStationSelect)
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(departureButton);
                }
                icon: "image://theme/icon-m-common-drilldown-arrow"
            }
            SubTitleButton {
                id: viaButton
                titleText: "Via Station"
                subTitleText: "please select"
                width: parent.width
                onClicked: {
                    pageStack.push(viaStationSelect)
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(viaButton);
                }
                icon: "image://theme/icon-m-common-drilldown-arrow"
            }
            SubTitleButton {
                id: arrivalButton
                titleText: "Arrival Station"
                subTitleText: "please select"
                width: parent.width
                onClicked: {
                    pageStack.push(arrivalStationSelect)
                }
                onPressAndHold: {
                    stationSelectContextMenu.openMenu(arrivalButton);
                }
                icon: "image://theme/icon-m-common-drilldown-arrow"
            }
            SubTitleButton {
                id: datePickerButton
                titleText: "Date"
                subTitleText: "please select"
                width: parent.width
                onClicked: {
                    datePicker.open();
                }
            }

            SubTitleButton {
                id: timePickerButton
                titleText: "Time"
                subTitleText: "please select"
                width: parent.width
                onClicked: {
                    timePicker.open();
                }

                ButtonRow {
                    anchors {
                        right: parent.right
                        rightMargin: 50
                        verticalCenter: parent.verticalCenter
                    }
                    width: parent.width / 2
                    Button {
                        id: modeDep
                        text: "Departure"
                    }
                    Button {
                        id: modeArr
                        text: "Arrival"
                    }
                }
            }

            SubTitleButton {
                id: trainrestrictionsButton
                titleText: "Trains"
                subTitleText: selectTrainrestrictionsDialog.selectedIndex >= 0 ? selectTrainrestrictionsDialog.model.get(selectTrainrestrictionsDialog.selectedIndex).name : "None"
                width: parent.width
                onClicked: {
                    selectTrainrestrictionsDialog.open();
                }
            }

            Button {
                id: startSearch
                text: "Start search"
                anchors {
                    topMargin: 10
                    horizontalCenter: parent.horizontalCenter
                }

                onClicked: {
                    //Validation
                    if (departureButton.subTitleText == "please select" || arrivalButton.subTitleText == "please select") {
                        banner.text = "Please select a departure and arrival station.";
                        banner.show();
                        return;
                    }

                    var viaStation = viaButton.subTitleText;
                    if (viaStation == "please select" || !fahrplanBackend.parser.supportsVia()) {
                        viaStation = "";
                    }

                    pageStack.push(loadingPage)
                    var selDate = new Date(datePicker.year, datePicker.month - 1, datePicker.day);
                    var selTime = new Date(1970, 2, 1, timePicker.hour, timePicker.minute, timePicker.second);
                    var selMode = 0;
                    if (modeDep.checked) {
                        selMode = 1;
                    }
                    if (modeArr.checked) {
                        selMode = 0;
                    }
                    fahrplanBackend.parser.searchJourney(departureButton.subTitleText, arrivalButton.subTitleText, viaStation, selDate, selTime, selMode, selectTrainrestrictionsDialog.selectedIndex);
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

    SelectionDialog {
        id: selectBackendDialog
        titleText: "Select Backend"
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
        titleText: "Select Train"
        model: trainrestrictionsModel
        onAccepted: {
        }
    }

    ListModel {
        id: trainrestrictionsModel
    }

    JourneyResultsPage {
        id: resultsPage
    }

    LoadingPage {
        id: loadingPage
    }


    AboutPage {
        id: aboutPage
    }

    DatePickerDialog {
        id: datePicker
        titleText: "Date"
        acceptButtonText: "Ok"
        rejectButtonText: "Cancel"
        onAccepted: {
            var selDate = new Date(datePicker.year, datePicker.month - 1, datePicker.day);
            datePickerButton.subTitleText = Qt.formatDate(selDate);
        }
        Component.onCompleted: {
            var d = new Date();
            datePicker.year = d.getFullYear();
            datePicker.month = d.getMonth() + 1; // month is 0 based in Date()
            datePicker.day = d.getDate();
            datePickerButton.subTitleText = Qt.formatDate(d);
        }
    }

    TimePickerDialog {
        id: timePicker
        titleText: "Time"
        acceptButtonText: "Ok"
        rejectButtonText: "Cancel"
        fields: DateTime.Hours | DateTime.Minutes
        onAccepted: {
            var selTime = new Date(1970, 2, 1, timePicker.hour, timePicker.minute, timePicker.second);
            timePickerButton.subTitleText = Qt.formatTime(selTime, "hh:mm");
        }
        Component.onCompleted: {
            var d = new Date();
            timePicker.hour = d.getHours();
            timePicker.minute = d.getMinutes();
            timePicker.second = d.getSeconds();
            timePickerButton.subTitleText = Qt.formatTime(d, "hh:mm");
        }
    }

    ToolBarLayout {
        id: mainToolbar

        ToolIcon {
            id : aboutIcon;
            iconId: "toolbar-settings"
            onClicked: {
                pageStack.push(aboutPage);
            }
        }
    }

    InfoBanner{
            id: banner
            objectName: "fahrplanInfoBanner"
            text: ""
            anchors.top: parent.top
            anchors.topMargin: 10
    }

    Timer {
        id: showResultsTimer
        interval: 800
        running: false
        repeat: false
        onTriggered: {
            pageStack.push(resultsPage);
        }
    }

    Timer {
        id: hideLoadingTimer
        interval: 800
        running: false
        repeat: false
        onTriggered: {
            pageStack.pop();
        }
    }

    ContextMenu {
        property SubTitleButton opener
        id: stationSelectContextMenu
        MenuLayout {
            MenuItem {
                id: selectStationMenu
                text: "Select station"
                onClicked: {
                    stationSelectContextMenu.opener.clicked();

                }
            }
            MenuItem {
                id: switchWithDepartureStation
                text: "Switch with Departure station"
                onClicked: {
                    var oldVal = stationSelectContextMenu.opener.subTitleText
                    stationSelectContextMenu.opener.subTitleText = departureButton.subTitleText
                    departureButton.subTitleText = oldVal;
                }
            }
            MenuItem {
                id: switchWithArrivalStation
                text: "Switch with Arrival station"
                onClicked: {
                    var oldVal = stationSelectContextMenu.opener.subTitleText
                    stationSelectContextMenu.opener.subTitleText = arrivalButton.subTitleText
                    arrivalButton.subTitleText = oldVal;
                }
            }
            MenuItem {
                id: switchWithViaStation
                text: "Switch with Via station"
                onClicked: {
                    var oldVal = stationSelectContextMenu.opener.subTitleText
                    stationSelectContextMenu.opener.subTitleText = viaButton.subTitleText
                    viaButton.subTitleText = oldVal;
                }
            }
            MenuItem {
                id: clearStation
                text: "Clear station"
                onClicked: {
                    stationSelectContextMenu.opener.subTitleText = "please select"
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

    Fahrplan.Backend {
        id: fahrplanBackend
        /*
           An error can occour here, if the result is returned quicker than
           the pagestack is popped so we use a timer here if the pagestack is busy.
         */
        onParserJourneyResult: {
            if (pageStack.busy) {
                showResultsTimer.interval = 800
                hideLoadingTimer.interval = 800
            } else {
                showResultsTimer.interval = 1
                hideLoadingTimer.interval = 1
            }
            if (result.count > 0) {
                showResultsTimer.start();
            } else {
                hideLoadingTimer.start();
                banner.text = "No results found";
                banner.show();
            }
        }

        onParserJourneyDetailsResult: {
            if (result.count <= 0) {
                banner.text = "Error loading details";
                banner.show();
            }
        }

        onParserErrorOccured: {
            if (pageStack.busy) {
                hideLoadingTimer.interval = 800
            } else {
                hideLoadingTimer.interval = 1
            }
            hideLoadingTimer.start();
            banner.text = msg;
            banner.show();
        }

        onParserChanged: {
            console.log("Switching to " + name);
            currentParserName.text = fahrplanBackend.parserName;
            viaButton.visible = fahrplanBackend.parser.supportsVia();

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
            trainrestrictionsButton.visible = items.length > 0;
            for (i = 0; i < items.length; i++) {
                trainrestrictionsModel.append({
                    "name" : items[i]
                });
            }
            selectTrainrestrictionsDialog.selectedIndex = (items.length > 0) ? 0 : -1;
        }
    }
}
