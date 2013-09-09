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
import QtQuick 2.0
import QtLocation 5.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems
import "."
import ".."

Page {
    id: stationSelect

    signal stationSelected ()
    property int type: FahrplanBackend.DepartureStation
//    property QtObject fahrplanBackend

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
//                    right: gpsButton.left
                    right: parent.right
                    rightMargin: units.gu(1)
                    top: parent.top
                    topMargin: units.gu(1)
                }

                onTextChanged: search.findStationsByName();
                Keys.onReturnPressed: search.findStationsByName();
                Keys.onEnterPressed: search.findStationsByName();


                placeholderText: qsTr("Search for Station...")
                Icon {
                    id: searchButton
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    name: "find"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            search.findStationsByName();
                        }
                    }
                }
            }

            function findStationsByName()
            {
                if (searchBox.text == "") {
                    listView.model = fahrplanBackend.favorites
                    return;
                }

                indicator.running = true;
                listView.model = fahrplanBackend.stationSearchResults
                fahrplanBackend.findStationsByName(searchBox.text);
            }

            // Not using GPS on ubuntu yet
//            Button {
//                id: gpsButton
//                anchors {
//                    right: parent.right
//                    rightMargin: visible ? units.gu(1) : 0
//                    top: parent.top
//                    topMargin: units.gu(1)
//                }

//                Icon {
//                    name: "file:///usr/share/icons/ubuntu-mobile/actions/scalable/location.svg"
//                    height: parent.height - units.gu(1)
//                    width: height
//                    anchors.centerIn: parent
//                    z: 2
//                }

//                width: visible ? units.gu(8) : 0

//                onClicked: {
//                    listView.model = FahrplanBackend.stationSearchResults
//                    favIcon.checked = false;
//                    searchIcon.checked = true;

//                    positionSource.start();
//                }
//            }
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
            model: fahrplanBackend.favorites
            delegate: ListItems.Standard {
                id: delegateItem
                text: name
                icon: "file:///usr/share/icons/ubuntu-mobile/actions/scalable/favorite-" + (model.isFavorite ? "selected.svg" : "unselected.svg")

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
                            delegateItem.icon = "file:///usr/share/icons/ubuntu-mobile/actions/scalable/favorite-unselected.svg"
                        } else {
                            listView.model.addToFavorites(index);
                            delegateItem.icon = "file:///usr/share/icons/ubuntu-mobile/actions/scalable/favorite-selected.svg"
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

    // Disabling for now as Ubuntu's AppArmor config currently prevents accessing GPS
/*    PositionSource {
        id: positionSource
        active: false

        onPositionChanged: {
            if (positionSource.position.latitudeValid && positionSource.position.longitudeValid) {
                positionSource.stop();

                fahrplanBackend.parser.findStationsByCoordinates(positionSource.position.coordinate.longitude, positionSource.position.coordinate.latitude);
            } else {
                positionSource.active = true
            }
        }
    }
    */
}
