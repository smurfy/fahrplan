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
import QtQuick 2.4
import QtLocation 5.0
import QtPositioning 5.2
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
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
        },

        Action {
            iconName: "location"
            onTriggered: {
                stationSelect.showFavorites = false;
                positionSource.start();
                indicator.running = true;
            }
        }
    ]

    TextField {
        id: search

        anchors { left: parent.left; right: parent.right; top: parent.top; margins: units.gu(2) }
        inputMethodHints: Qt.ImhNoPredictiveText

        onTextChanged: searchTimer.restart();
        Keys.onReturnPressed: search.findStationsByName();
        Keys.onEnterPressed: search.findStationsByName();

        placeholderText: qsTr("Search for Station...")
        secondaryItem: Icon {
            width: height
            height: parent.height - units.gu(2)
            name: "find"
            MouseArea {
                anchors.fill: parent
                onClicked: search.findStationsByName();
            }
        }

        Timer {
            id: searchTimer
            interval: 500
            onTriggered: search.findStationsByName();
        }

        function findStationsByName()
        {
            if (search.text == "") {
                stationSelect.showFavorites = true;
                return;
            }

            if (searchTimer.running)
                searchTimer.stop();

            indicator.running = true;
            stationSelect.showFavorites = false;
            fahrplanBackend.findStationsByName(search.text);
        }
    }

    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        color: "DarkGrey"
        fontSize: "large"
        horizontalAlignment: Text.AlignHCenter
        visible: fahrplanBackend.favorites.count == 0 && listView.model == fahrplanBackend.favorites
        text: qsTr("Click the star icon on the search results to add or remove a station as a favorite")
        anchors { top: search.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; margins: units.gu(2) }
    }

    UbuntuListView {
        id: listView

        anchors { top: search.bottom; bottom: parent.bottom; margins: units.gu(1) }
        clip: true
        width: parent.width
        model: stationSelect.showFavorites ? fahrplanBackend.favorites : fahrplanBackend.stationSearchResults

        delegate: ListItem {
            id: delegateItem

            RowLayout {
                id: rowLayout
                spacing: units.gu(2)
                anchors { verticalCenter: parent.verticalCenter; left: parent.left; right: parent.right; margins: units.gu(2) }

                Icon {
                    id: favoriteIcon
                    name: model.isFavorite ? "starred" : "non-starred"
                    Layout.preferredHeight: units.gu(4)
                    Layout.preferredWidth: height

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (model.isFavorite) {
                                listView.model.removeFromFavorites(index);
                                favoriteIcon.name = "non-starred";
                            } else {
                                listView.model.addToFavorites(index);
                                favoriteIcon.name = "starred";
                            }
                        }
                    }
                }

                Label {
                    id: stationName
                    text: name
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                Label {
                    id: info
                    visible: miscInfo !== ""
                    text: miscInfo
                }
            }

            onClicked: {
                listView.model.selectStation(stationSelect.type, model.index);
                stationSelect.stationSelected()
                mainStack.pop()
            }
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
