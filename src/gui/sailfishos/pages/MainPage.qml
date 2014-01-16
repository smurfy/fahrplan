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
            }
            MenuItem {
                text: qsTr("Journey")
                onClicked: {
                    searchmode = 0;
                    updateButtonVisibility();
                }
            }
            MenuItem {
                text: qsTr("Time Table")
                onClicked: {
                    searchmode = 1;
                    updateButtonVisibility();
                }
            }
            MenuItem {
                text: fahrplanBackend.parserName
                enabled: false
            }
        }

        model: VisualItemModel {

            ValueButton {
                id: departureButton
                label: qsTr("Departure Station")
                value: fahrplanBackend.departureStationName
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("StationSelectPage.qml"), {type: FahrplanBackend.DepartureStation, fahrplanBackend: fahrplanBackend})
                }
            }
            ValueButton {
                id: viaButton
                label: qsTr("Via Station")
                value: fahrplanBackend.viaStationName
                onClicked: {
                      pageStack.push(Qt.resolvedUrl("StationSelectPage.qml"), {type: FahrplanBackend.ViaStation, fahrplanBackend: fahrplanBackend})

                }
            }
            ValueButton {
                id: arrivalButton
                label: qsTr("Arrival Station")
                value: fahrplanBackend.arrivalStationName
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("StationSelectPage.qml"), {type: FahrplanBackend.ArrivalStation, fahrplanBackend: fahrplanBackend})

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
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("StationSelectPage.qml"), {type: FahrplanBackend.DirectionStation, fahrplanBackend: fahrplanBackend})
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
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog")

                    dialog.accepted.connect(function() {
                        value = dialog.dateText
                    })
                }
            }
            ValueButton {
                id: timePickerButton
                label: qsTr("Time")
                value: Qt.formatTime(fahrplanBackend.dateTime, Qt.DefaultLocaleShortDate)
                onClicked: {
                    var dialog = pageStack.push("Sailfish.Silica.TimePickerDialog")

                    dialog.accepted.connect(function() {
                        value = dialog.timeText
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

    FahrplanBackend {
        id: fahrplanBackend
        onParserChanged: {
            console.log("Switching to " + name);

            updateButtonVisibility();
        }

        onModeChanged: {
            updateModeCheckboxes();
        }

    }

    Component.onCompleted: {
        updateModeCheckboxes();
    }
}
