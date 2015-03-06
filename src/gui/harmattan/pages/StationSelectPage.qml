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

import Fahrplan 1.0
import QtQuick 1.1
import QtMobility.location 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1
import "../components"
import "../delegates"

Page {
    id: stationSelect

    property int type: FahrplanBackend.DepartureStation

    property string whiteSuffix: theme.inverted ? "-white" : ""
    property string inverseSuffix: theme.inverted ? "-inverse" : ""

    tools: stationSelectToolbar

    Item {
        width:  stationSelect.width
        height: stationSelect.height

        Item {
            id: search

            height: UiConstants.HeaderDefaultHeightPortrait
            z: 10

            anchors {
                left: parent.left
                right: parent.right
            }

            Rectangle {
                anchors.fill: parent
                color: theme.inverted ? "#222" : "LightGrey"
            }

            TextField {
                id: searchBox
                inputMethodHints: Qt.ImhNoPredictiveText
                anchors {
                    left: parent.left
                    leftMargin: 10
                    right: gpsButton.left
                    rightMargin: 10
                    top: parent.top
                    topMargin: 10
                }

                onTextChanged: searchTimer.restart();
                Keys.onReturnPressed: search.findStationsByName();
                Keys.onEnterPressed: search.findStationsByName();


                platformSipAttributes: SipAttributes { actionKeyHighlighted: true }

                placeholderText: qsTr("Search for Station...")
                platformStyle: TextFieldStyle { paddingRight: searchButton.width }
                Image {
                    id: searchButton
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    source: "image://theme/icon-m-common-search"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            search.findStationsByName();
                        }
                    }
                }
            }

            Timer {
                id: searchTimer
                interval: 1000
                onTriggered: {
                    search.findStationsByName();
                }
            }

            function findStationsByName()
            {
                if (searchBox.text == "")
                    return;

                if (searchTimer.running)
                    searchTimer.stop();

                indicator.show(qsTr("Searching ..."));
                tabs.currentTab = stationSearchResultsTab;
                fahrplanBackend.findStationsByName(searchBox.text);
            }

            Button {
                id: gpsButton
                anchors {
                    right: parent.right
                    rightMargin: visible ? 10 : 0
                    top: parent.top
                    topMargin: 10
                }
                platformStyle: ButtonStyle { inverted: !theme.inverted }
                iconSource: "image://theme/icon-s-calendar-location-picker" + (theme.inverted ? "" : "-inverse")
                width: visible ? 80 : 0

                onClicked: {
                    indicator.show(qsTr("Requesting GPS..."));
                    fahrplanBackend.stationSearchResults.clear();
                    tabs.currentTab = stationSearchResultsTab;
                    positionSource.start();
                }
            }
        }

        BusyLabel {
            id: indicator
            anchors.top: search.bottom
        }

        TabGroup {
            id: tabs

            currentTab: favoritesTab
            anchors {
                top: indicator.bottom
                left: parent.left
                bottom: parent.bottom
                right: parent.right
            }

            StationsListPage {
                id: favoritesTab

                emptyText: qsTr("Click the star icon in the search results to add or remove a station as a favorite")
                model: fahrplanBackend.favorites

                onStationSelected:  {
                    stationSelect.pageStack.pop();
                }
            }
            StationsListPage {
                id: stationSearchResultsTab

                emptyText: qsTr("Start typing station name into the search box to see a list stations")
                model: fahrplanBackend.stationSearchResults

                onStationSelected:  {
                    stationSelect.pageStack.pop();
                }
            }
        }
    }

    ToolBarLayout {
        id:stationSelectToolbar

        ToolIcon {
            id : backIcon
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }

        ButtonRow {
            id: tabIcons

            TabButton {
                id: favIcon
                tab: favoritesTab
                iconSource: "image://theme/icon-m-toolbar-favorite-mark" + whiteSuffix
            }
            TabButton {
                id: searchIcon
                tab: stationSearchResultsTab
                iconSource: "image://theme/icon-m-toolbar-search" + whiteSuffix
            }
        }
    }

    PositionSource {
        id: positionSource
        onPositionChanged: {
            if (positionSource.position.latitudeValid && positionSource.position.longitudeValid) {
                indicator.show(qsTr("Searching for stations..."));
                positionSource.stop();

                fahrplanBackend.findStationsByCoordinates(positionSource.position.coordinate.longitude, positionSource.position.coordinate.latitude);
            } else {
                indicator.show(qsTr("Waiting for GPS lock..."));
            }
        }
    }

    onStatusChanged: {
        switch (status) {
            case PageStatus.Activating:
                gpsButton.visible = fahrplanBackend.parser.supportsGps();
                break;
        }
    }

    Connections {
        target: fahrplanBackend

        onParserStationsResult: {
            indicator.hide();
        }
    }
}
