import QtQuick 2.0
import Sailfish.Silica 1.0
import Fahrplan 1.0

Page {
    property int searchmode : 0
    id: mainPage

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
                label: qsTr("Departure Station")
                value: fahrplanBackend.departureStationName
                onClicked: {
                }
            }
            ValueButton {
                label: qsTr("Via Station")
                value: fahrplanBackend.viaStationName
                onClicked: {
                }
            }
            ValueButton {
                label: qsTr("Arrival Station")
                value: fahrplanBackend.arrivalStationName
                onClicked: {
                }
            }
            ValueButton {
                label: qsTr("Station")
                value: fahrplanBackend.currentStationName
                onClicked: {
                }
            }
            ValueButton {
                label: qsTr("Direction")
                value: fahrplanBackend.directionStationName
                onClicked: {
                }
            }
            ValueButton {
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
                label: qsTr("Time")
                value: Qt.formatTime(fahrplanBackend.dateTime, Qt.DefaultLocaleShortDate)
                onClicked: {
                    var dialog = pageStack.push("Sailfish.Silica.TimePickerDialog")

                    dialog.accepted.connect(function() {
                        value = dialog.timeText
                    })
                }
            }
            TextSwitch {
                id: fromNowSwitch
                text: qsTr("Departure: Now")
                description: qsTr("Using current date and time for search")
                onCheckedChanged: {
                    if (checked)
                        fahrplanBackend.mode = FahrplanBackend.NowMode;
                    else
                        fahrplanBackend.mode = FahrplanBackend.DepartureMode;
                }
            }
        }

    }
    function updateButtonVisibility()
    {
        if (!fahrplanBackend.parser.supportsTimeTable()) {
            searchmode = 0;
        }
    }

    function updateModeCheckboxes()
    {
        if (fahrplanBackend.mode === FahrplanBackend.NowMode) {
            fromNowSwitch.checked = true;
            return;
        }
        fromNowSwitch.checked = false;
        modeDep.checked = fahrplanBackend.mode === FahrplanBackend.DepartureMode;
        modeArr.checked = fahrplanBackend.mode === FahrplanBackend.ArrivalMode;
    }

    FahrplanBackend {
        id: fahrplanBackend
        onParserChanged: {
            console.log("Switching to " + name);
        }

        onModeChanged: {
            updateModeCheckboxes();
        }

    }
}
