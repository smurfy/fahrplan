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

import bb.cascades 1.3
import QtQuick 1.0 as QtQuick
import Fahrplan 1.0

import "../components"
import "../js/style.js" as UI

Page {
    actionBarVisibility: ChromeVisibility.Overlay
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll

    titleBar: Title {}

    Container {
        SegmentedControl {
            id: modeSelector

            horizontalAlignment: HorizontalAlignment.Fill
            bottomMargin: 0

            Option {
                text: qsTr("Journey Planner") + Retranslate.onLocaleOrLanguageChanged
                selected: true
            }
            Option {
                text: qsTr("Timetable") + Retranslate.onLocaleOrLanguageChanged
                enabled: fahrplan.parser.supportsTimeTable()
            }
        }
        ScrollView {
            scrollRole: ScrollRole.Main

            Container {
                bottomPadding: layoutHandler.bottomOverlayHeight

                Container {
                    id: journeyControlsContainer

                    visible: modeSelector.selectedIndex == 0

                    StationSelect {
                        id: departureStation

                        title: qsTr("Departure Station") + Retranslate.onLocaleOrLanguageChanged
                        text: fahrplan.departureStationName
                        stationType: FahrplanBackend.DepartureStation
                    }
                    StationSelect {
                        id: viaStation

                        title: qsTr("Via Station") + Retranslate.onLocaleOrLanguageChanged
                        text: fahrplan.viaStationName
                        stationType: FahrplanBackend.ViaStation
                        visible: fahrplan.parser.supportsVia()
                    }
                    StationSelect {
                        id: arrivalStation

                        title: qsTr("Arrival Station") + Retranslate.onLocaleOrLanguageChanged
                        text: fahrplan.arrivalStationName
                        stationType: FahrplanBackend.ArrivalStation
                    }
                }
                Container {
                    id: timetableControlsContainer

                    visible: modeSelector.selectedIndex == 1

                    StationSelect {
                        id: currentStation

                        title: qsTr("Station") + Retranslate.onLocaleOrLanguageChanged
                        text: fahrplan.currentStationName
                        stationType: FahrplanBackend.CurrentStation
                    }
                    StationSelect {
                        id: directionStation

                        title: qsTr("Direction") + Retranslate.onLocaleOrLanguageChanged
                        text: fahrplan.directionStationName
                        stationType: FahrplanBackend.DirectionStation
                        visible: fahrplan.parser.supportsTimeTableDirection()
                    }
                }
                Container {
                    topPadding: ui.sdu(UI.paddingTop)
                    leftPadding: ui.sdu(UI.paddingLeft)
                    rightPadding: ui.sdu(UI.paddingRight)

                    SegmentedControl {
                        id: timetableMode

                        selectedIndex: fahrplan.mode
                        horizontalAlignment: HorizontalAlignment.Fill

                        Option {
                            text: qsTr("Departure") + Retranslate.onLocaleOrLanguageChanged
                            value: FahrplanBackend.DepartureMode
                        }
                        Option {
                            text: qsTr("Arrival") + Retranslate.onLocaleOrLanguageChanged
                            value: FahrplanBackend.ArrivalMode
                        }
                        Option {
                            text: qsTr("Now") + Retranslate.onLocaleOrLanguageChanged
                            value: FahrplanBackend.NowMode
                        }

                        onSelectedIndexChanged: {
                            if (typeof selectedValue != "undefined")
                                fahrplan.mode = selectedValue;
                        }
                    }
                    Container {
                        layout: StackLayout {
                            orientation: datePicker.expanded || timePicker.expanded
                                         ? LayoutOrientation.TopToBottom
                                         : LayoutOrientation.LeftToRight
                        }
                        visible: timetableMode.selectedIndex != 2

                        DateTimePicker {
                            id: datePicker

                            title: qsTr("Date") + Retranslate.onLocaleOrLanguageChanged
                            mode: DateTimePickerMode.Date

                            onValueChanged: {
                                var date = fahrplan.dateTime;
                                date.setDate(value.getDate());
                                date.setMonth(value.getMonth());
                                date.setFullYear(value.getFullYear());
                                fahrplan.dateTime = date;
                            }
                        }
                        DateTimePicker {
                            id: timePicker

                            title: qsTr("Time") + Retranslate.onLocaleOrLanguageChanged
                            mode: DateTimePickerMode.Time
//                            minuteInterval: 1

                            onValueChanged: {
                                var date = fahrplan.dateTime;
                                date.setHours(value.getHours());
                                date.setMinutes(value.getMinutes());
//                                date.setSeconds(value.getSeconds());
                                fahrplan.dateTime = date;
                            }
                        }
                    }
                    DropDown {
                        id: trains

                        title: qsTr("Trains") + Retranslate.onLocaleOrLanguageChanged
                        visible: trainRestrictions.model.length > 0

                        attachedObjects: [
                            Repeater {
                                id: trainRestrictions
                                model: fahrplan.parser.getTrainRestrictions()
                                delegate: Option {
                                    text: modelData
                                }

                                onModelChanged: {
                                    trains.selectedIndex = 0;
                                }
                            }
                        ]
                    }
                }
            }
        }
    }

    actions: [
        ActionItem {
            title: (modeSelector.selectedIndex == 0 ? qsTr("Get Journeys") : qsTr("Show Timetable"))
                   + Retranslate.onLocaleOrLanguageChanged
            imageSource: modeSelector.selectedIndex == 0 ? Qt.resolvedUrl("../icons/journey.png")
                                                         : Qt.resolvedUrl("../icons/timetable.png")
            ActionBar.placement: ActionBarPlacement.Signature

            onTriggered: {
                if (modeSelector.selectedIndex == 0)
                    fahrplan.searchJourney();
                else
                    fahrplan.getTimeTable();
            }
        }
    ]

    attachedObjects: [
        PageLayoutUpdateHandler {
            id: layoutHandler
        },
        ComponentDefinition {
            id: stationSelectPageDefinition

            StationSelectPage {}
        },
        ComponentDefinition {
            id: actionItemDefinition

            ActionItem {
                property string swapWith
                property int swapSource
                property int swapTarget

                title: qsTr("Swap with %1").arg(swapWith) + Retranslate.onLocaleOrLanguageChanged
                imageSource: Qt.resolvedUrl("../icons/swap.png")

                onTriggered: {
                    fahrplan.swapStations(swapSource, swapTarget);
                }
            }
        },
        QtQuick.Connections {
            target: fahrplan

            onParserChanged: {
                // Switch to Journey Planner tab if
                // timetable is not supported
                if (!fahrplan.parser.supportsTimeTable())
                    modeSelector.selectedIndex = 0;
            }

            onDateTimeChanged: {
                if (fahrplan.dateTime.getFullYear() !== datePicker.value.getFullYear()
                        || fahrplan.dateTime.getMonth() !== datePicker.value.getMonth()
                        || fahrplan.dateTime.getDate() !== datePicker.value.getDate()) {
                    datePicker.value = fahrplan.dateTime;
                }
                var roundedMinutes = timePicker.minuteInterval
                                     * Math.round(fahrplan.dateTime.getMinutes()
                                                  / timePicker.minuteInterval)
                                     % 60;
                if (fahrplan.dateTime.getHours() !== timePicker.value.getHours()
                        || roundedMinutes !== timePicker.value.getMinutes()
                        /*|| fahrplan.dateTime.getSeconds() !== timePicker.value.getSeconds()*/) {
                    timePicker.value = fahrplan.dateTime;
                }
            }

            onParserJourneyResult: {
                toast.body = "onParserJourneyResult: " + result.count;
                toast.show();
            }

            onParserTimeTableResult: {
                toast.body = "onParserTimeTableResult";
                toast.show();
            }
        }
    ]

    onCreationCompleted: {
        addContextActions(departureStation);
        addContextActions(viaStation);
        addContextActions(arrivalStation);
        addContextActions(currentStation);
        addContextActions(directionStation);
    }

    function addContextActions(control)
    {
        var actions = new Array;
        if (control.stationType === FahrplanBackend.DepartureStation
                || control.stationType === FahrplanBackend.ViaStation
                || control.stationType === FahrplanBackend.ArrivalStation) {

            actions.push(actionItemDefinition.createObject(control));
            actions.push(actionItemDefinition.createObject(control));
            var pos = 0;

            switch (control.stationType) {
            case FahrplanBackend.DepartureStation:
                actions[0].swapSource = actions[1].swapSource = FahrplanBackend.DepartureStation;
                break;
            case FahrplanBackend.ViaStation:
                actions[0].swapSource = actions[1].swapSource = FahrplanBackend.ViaStation;
                break;
            case FahrplanBackend.ArrivalStation:
                actions[0].swapSource = actions[1].swapSource = FahrplanBackend.ArrivalStation;
                break;
            }

            if (control.stationType !== FahrplanBackend.DepartureStation) {
                actions[pos].swapWith = qsTr("Departure Station");
                actions[pos].swapTarget = FahrplanBackend.DepartureStation;
                control.actionSet.insert(pos + 1, actions[pos]);
                pos++;
            }
            if (control.stationType !== FahrplanBackend.ViaStation
                    && fahrplan.parser.supportsVia()) {
                actions[pos].swapWith = qsTr("Via Station");
                actions[pos].swapTarget = FahrplanBackend.ViaStation;
                control.actionSet.insert(pos + 1, actions[pos]);
                pos++;
            }
            if (control.stationType !== FahrplanBackend.ArrivalStation) {
                actions[pos].swapWith = qsTr("Arrival Station");
                actions[pos].swapTarget = FahrplanBackend.ArrivalStation;
                control.actionSet.insert(pos + 1, actions[pos]);
                pos++;
            }
        } else {
            if (control.stationType === FahrplanBackend.CurrentStation
                    && fahrplan.parser.supportsTimeTableDirection()) {
                actions.push(actionItemDefinition.createObject(control));
                actions[0].swapWith = qsTr("Direction");
                actions[0].swapSource = FahrplanBackend.CurrentStation;
                actions[0].swapTarget = FahrplanBackend.DirectionStation;
                control.actionSet.insert(1, actions[0]);
            } else if (control.stationType === FahrplanBackend.DirectionStation) {
                actions.push(actionItemDefinition.createObject(control));
                actions[0].swapWith = qsTr("Station");
                actions[0].swapSource = FahrplanBackend.DirectionStation;
                actions[0].swapTarget = FahrplanBackend.CurrentStation;
                control.actionSet.insert(1, actions[0]);
            }
        }
    }
}
