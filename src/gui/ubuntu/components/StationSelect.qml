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
import QtQuick 2.3
import QtLocation 5.0
import QtPositioning 5.2
import Ubuntu.Components 1.1
import Ubuntu.Components.ListItems 0.1 as ListItems
import "."
import ".."

Page {
    id: stationSelect

    signal stationSelected ()
    property int type: FahrplanBackend.DepartureStation
//    property QtObject fahrplanBackend

    property bool showFavorites: true

    head.actions: [
        Action {
            iconName: stationSelect.showFavorites ? "starred" : "non-starred"
            onTriggered: {
                stationSelect.showFavorites = !stationSelect.showFavorites
            }
        }

    ]

    Item {
        width:  stationSelect.width
        height: stationSelect.height

        Item {
            id: search

            height: units.gu(7)

            anchors {
                left: parent.left
                right: parent.right
            }

            TextField {
                id: searchBox
                anchors {
                    left: parent.left
                    leftMargin: units.gu(1)
                    right: gpsButton.left
                    //right: parent.right
                    rightMargin: units.gu(1)
                    top: parent.top
                    topMargin: units.gu(1)
                }
                inputMethodHints: Qt.ImhNoPredictiveText

                onTextChanged: searchTimer.restart();
                Keys.onReturnPressed: search.findStationsByName();
                Keys.onEnterPressed: search.findStationsByName();


                placeholderText: qsTr("Search for Station...")
                secondaryItem: [
                    Icon {
                        id: searchButton
                        height: parent.height - units.gu(1)
                        width: height
                        name: "find"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                search.findStationsByName();
                            }
                        }
                    }
                ]
            }

            Timer {
                id: searchTimer
                interval: 500
                onTriggered: {
                    search.findStationsByName();
                }
            }

            function findStationsByName()
            {
                if (searchBox.text == "") {
                    stationSelect.showFavorites = true;
                    return;
                }

                if (searchTimer.running)
                    searchTimer.stop();

                indicator.running = true;
                stationSelect.showFavorites = false;
                fahrplanBackend.findStationsByName(searchBox.text);
            }

            Button {
                id: gpsButton
                anchors {
                    right: parent.right
                    rightMargin: visible ? units.gu(1) : 0
                    top: parent.top
                    topMargin: units.gu(1)
                }
                iconName: "location"
                width: visible ? height : 0

                onClicked: {
                    stationSelect.showFavorites = false;
                    positionSource.start();
                    indicator.running = true;
                }
            }
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap
            height: parent.height - search.top
            text: qsTr("Click the star icon on the search results to add or remove a station as a favorite");
            color: "DarkGrey"
            fontSize: "large"
            visible: (fahrplanBackend.favorites.count == 0 && listView.model == fahrplanBackend.favorites)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors {
                top: search.bottom
                left: parent.left
                leftMargin: units.gu(2)
                right: parent.right
                rightMargin: units.gu(2)
            }
        }

        ListView {
            id: listView
            anchors {
                topMargin: units.gu(1)
                top: search.bottom
                bottomMargin: units.gu(1)
            }
            height: parent.height
            width: parent.width
            model: stationSelect.showFavorites ? fahrplanBackend.favorites : fahrplanBackend.stationSearchResults
            delegate: ListItems.Standard {
                id: delegateItem
                text: name
                iconName: model.isFavorite ? "starred" : "non-starred"

                onClicked: {
                    listView.model.selectStation(stationSelect.type, model.index);
                    stationSelect.stationSelected()
                }

                MouseArea {
                    anchors {
                        top: parent.top
                        left: parent.left
                        leftMargin: units.gu(1)
                        bottom: parent.bottom
                    }
                    width: height

                    onClicked: {
                        if (model.isFavorite) {
                            listView.model.removeFromFavorites(index);
                            delegateItem.iconName = "non-starred";
                        } else {
                            listView.model.addToFavorites(index);
                            delegateItem.iconName = "starred";
                        }
                    }
                }
            }

            clip: true
        }
    }

    ActivityIndicator {
        id: indicator
        anchors.centerIn: parent
        running: false
        visible: running
    }

    Connections {
        target: fahrplanBackend

        onParserStationsResult: {
            indicator.running = false;
        }
    }

    PositionSource {
        id: positionSource
        active: false

        onPositionChanged: {

            // For some reason the active property doesn't seem to work on Ubuntu currently.
            // It'll keep on emitting position changes despite active being set to false...
            // https://bugs.launchpad.net/location-service/+bug/1395313
            if (!positionSource.active) {
                return;
            }

            if (positionSource.position.latitudeValid && positionSource.position.longitudeValid) {
                positionSource.active = false;
                fahrplanBackend.parser.findStationsByCoordinates(positionSource.position.coordinate.longitude, positionSource.position.coordinate.latitude);
            } else {
                positionSource.active = true
            }
        }
    }
}
