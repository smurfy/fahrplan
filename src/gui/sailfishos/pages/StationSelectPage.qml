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

import QtQuick 2.0
import Sailfish.Silica 1.0
import QtPositioning 5.0
import Fahrplan 1.0
import "../delegates"

Dialog {
    id: stationSelect
    canAccept: false

    property string searchString

    onSearchStringChanged: {
        fahrplanBackend.findStationsByName(searchString);
    }

    property int type: FahrplanBackend.DepartureStation
    property FahrplanBackend fahrplanBackend: null

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height
        contentWidth: parent.width

        VerticalScrollDecorator {}

        Column {
            id: column
            spacing: Theme.paddingLarge
            width: parent.width

            PageHeader {
                title: qsTr("Select station")
            }

            Row {
                width: parent.width

                SearchField {
                    id: searchField
                    width: parent.width - gpsButton.width - 10

                    Binding {
                        target: stationSelect
                        property: "searchString"
                        value: searchField.text.trim()
                    }
                }
                IconButton {
                    id: gpsButton
                    width: icon.width
                    height: parent.height
                    icon.source: "image://theme/icon-m-gps"

                    onClicked: {
                        fahrplanBackend.stationSearchResults.clear();
                        positionSource.update();
                    }
                }
            }

            Column {
                visible: (fahrplanBackend.stationSearchResults.count > 0)
                width: parent.width

                SectionHeader {
                    text: qsTr("Search")
                }

               ListView {
                    model: fahrplanBackend.stationSearchResults
                    width: parent.width
                    height: contentHeight
                    interactive: false

                    currentIndex: -1

                    delegate: StationDelegate {
                        onStationSelected:  {
                            stationSelect.close();
                        }
                    }
                }
            }

            Column {
                width: parent.width

                SectionHeader {
                    text: qsTr("Favorites")
                }

                Label {
                    id: lbl_no_favorites
                    width: parent.width - Theme.paddingLarge
                    visible: (fahrplanBackend.favorites.count === 0)
                    horizontalAlignment: Text.AlignHCenter
                    color: Theme.highlightColor
                    wrapMode: Text.WordWrap
                    text: qsTr("Click and hold in the search results to add or remove a station as a favorite")
                }

                ListView {
                    model: fahrplanBackend.favorites
                    width: parent.width
                    height: contentHeight
                    interactive: false

                    currentIndex: -1

                    delegate: StationDelegate {
                        onStationSelected:  {
                            stationSelect.close();
                        }
                    }
                }
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

    PositionSource {
        id: positionSource
        active: false

        onPositionChanged: {

            //Only do something if active
            if (positionSource.active === false) {
                return;
            }

            if (positionSource.position.latitudeValid && positionSource.position.longitudeValid) {
                console.log(qsTr("Searching for stations..."));
                fahrplanBackend.findStationsByCoordinates(positionSource.position.coordinate.longitude, positionSource.position.coordinate.latitude);
            } else {
                console.log(qsTr("Waiting for GPS lock..."));
                positionSource.update();
            }
        }
    }
}
