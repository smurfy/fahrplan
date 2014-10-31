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
                        title: qsTr("Departure Station") + Retranslate.onLocaleOrLanguageChanged
                        text: fahrplan.departureStationName
                        stationType: FahrplanBackend.DepartureStation
                    }
                    StationSelect {
                        title: qsTr("Via Station") + Retranslate.onLocaleOrLanguageChanged
                        text: fahrplan.viaStationName
                        stationType: FahrplanBackend.ViaStation
                        visible: fahrplan.parser.supportsVia()
                    }
                    StationSelect {
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

                        contextActions: [
                            ActionSet {
                                title: parent.title

                                ActionItem {
                                    title: qsTr("Search") + Retranslate.onLocaleOrLanguageChanged
                                    imageSource: Qt.resolvedUrl("../icons/ic_search.png")

                                    onTriggered: {
                                        currentStation.pushStationSelectPage();
                                    }
                                }
                                ActionItem {
                                    title: qsTr("Swap with Direction")
                                           + Retranslate.onLocaleOrLanguageChanged
                                    imageSource: Qt.resolvedUrl("../icons/swap.png")
                                    enabled: fahrplan.parser.supportsTimeTableDirection()

                                    onTriggered: {
                                        fahrplan.swapStations(FahrplanBackend.CurrentStation,
                                                              FahrplanBackend.DirectionStation);
                                    }
                                }
                                ActionItem {
                                    title: qsTr("Clear") + Retranslate.onLocaleOrLanguageChanged
                                    imageSource: Qt.resolvedUrl("../icons/ic_clear.png")

                                    onTriggered: {
                                        fahrplan.resetStation(FahrplanBackend.CurrentStation);
                                    }
                                }
                            }
                        ]
                    }
                    StationSelect {
                        title: qsTr("Direction") + Retranslate.onLocaleOrLanguageChanged
                        text: fahrplan.directionStationName
                        stationType: FahrplanBackend.DirectionStation
                        visible: fahrplan.parser.supportsTimeTableDirection()
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
        QtQuick.Connections {
            target: fahrplan

            onParserChanged: {
                // Switch to Journey Planner tab if
                // timetable is not supported
                if (!fahrplan.parser.supportsTimeTable())
                    modeSelector.selectedIndex = 0;
            }
        }
    ]
}
