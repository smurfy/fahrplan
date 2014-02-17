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
import com.nokia.symbian 1.1
import com.nokia.extras 1.1
import "../components"
import "../js/style.js" as Style

Page {
    id: stationSelect

    property int type: FahrplanBackend.DepartureStation

    tools: stationSelectToolbar

    Item {
        width:  stationSelect.width
        height: stationSelect.height

        Item {
            id: search

            height: privateStyle.tabBarHeightPortrait
            z: 10

            anchors {
                left: parent.left
                right: parent.right
            }

            SearchBox {
                id: searchBox
                platformInverted: appWindow.platformInverted

                anchors {
                    left: parent.left
                    right: gpsButton.left
                    verticalCenter: parent.verticalCenter
                }

                onSearchTextChanged: searchTimer.restart();
                Keys.onReturnPressed: search.findStationsByName();
                Keys.onEnterPressed: search.findStationsByName();

                placeHolderText: qsTr("Search for Station...")
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
                if (searchBox.searchText == "")
                    return;

                if (searchTimer.running)
                    searchTimer.stop();

                indicator.show(qsTr("Searching ..."));
                tabs.currentTab = stationSearchResultsTab;
                tabIcons.checkedButton = searchIcon;
                fahrplanBackend.findStationsByName(searchBox.searchText);
            }

            BorderImage {
                anchors {
                    top: parent.top
                    left: gpsButton.left
                    bottom: parent.bottom
                    right: parent.right
                }
                source: Style.getIconFromTheme(appWindow.platformInverted, "qtg_fr_tab_bar")
                border {
                    left: platformStyle.borderSizeMedium
                    top: platformStyle.borderSizeMedium
                    right: platformStyle.borderSizeMedium
                    bottom: platformStyle.borderSizeMedium
                }
            }

            Button {
                id: gpsButton
                anchors {
                    right: parent.right
                    rightMargin: visible ? platformStyle.paddingMedium : 0
                    verticalCenter: parent.verticalCenter
                }
                platformInverted: !appWindow.platformInverted
                iconSource: Style.getIconFromQrc(platformInverted, "icon-s-calendar-location-picker")
                width: visible ? height : 0

                onClicked: {
                    indicator.show(qsTr("Requesting GPS..."));
                    fahrplanBackend.stationSearchResults.clear();
                    tabs.currentTab = stationSearchResultsTab;
                    tabIcons.checkedButton = searchIcon;
                    positionSource.update();
                }
            }
        }

        BusyLabel {
            id: indicator
            platformInverted: appWindow.platformInverted
            anchors.top: search.bottom
        }

        TabGroup {
            id: tabs
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
                platformInverted: appWindow.platformInverted

                onStationSelected:  {
                    stationSelect.pageStack.pop();
                }
            }
            StationsListPage {
                id: stationSearchResultsTab

                emptyText: qsTr("Start typing station name into the search box to see a list stations")
                model: fahrplanBackend.stationSearchResults
                platformInverted: appWindow.platformInverted

                onStationSelected:  {
                    stationSelect.pageStack.pop();
                }
            }
        }
    }

    ToolBarLayout {
        id:stationSelectToolbar

        ToolButton {
            id : backIcon
            iconSource: "toolbar-back"
            platformInverted: appWindow.platformInverted
            onClicked: {
                pageStack.pop();
            }
        }

        ButtonRow {
            id: tabIcons

            TabButton {
                id: favIcon

                tab: favoritesTab
                iconSource: Style.getIconFromTheme(platformInverted, "qtg_graf_rating_rated")
                platformInverted: appWindow.platformInverted
            }
            TabButton {
                id: searchIcon

                tab: stationSearchResultsTab
                iconSource: "toolbar-search"
                platformInverted: appWindow.platformInverted
            }
        }
    }

    PositionSource {
        id: positionSource
        onPositionChanged: {
            if (positionSource.position.latitudeValid && positionSource.position.longitudeValid) {
                indicator.show(qsTr("Searching for stations..."));
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
