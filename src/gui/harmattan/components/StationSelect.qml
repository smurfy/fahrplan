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

            height: 70

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
                anchors {
                    left: parent.left
                    leftMargin: 10
                    right: gpsButton.left
                    rightMargin: 10
                    top: parent.top
                    topMargin: 10
                }

                onTextChanged: search.findStationsByName();
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

            function findStationsByName()
            {
                if (searchBox.text == "")
                    return;

                indicator.show(qsTr("Searching ..."));
                listView.model = fahrplanBackend.stationSearchResults;
                favIcon.checked = false;
                searchIcon.checked = true;
                fahrplanBackend.parser.findStationsByName(searchBox.text);
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
                    listView.model = fahrplanBackend.stationSearchResults;
                    favIcon.checked = false;
                    searchIcon.checked = true;
                    positionSource.start();
                }
            }
        }

        Text {
            width: parent.width
            wrapMode: Text.WordWrap
            height: parent.height - search.top - stationSelectToolbar.height
            text: qsTr("Click the star icon on the search results to add or remove a station as a favorite");
            color: "DarkGrey"
            font.pixelSize: 50
            visible: (fahrplanBackend.favorites.count == 0 && listView.model == fahrplanBackend.favorites)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors {
                top: search.bottom
                left: parent.left
                leftMargin: 20
                right: parent.right
                rightMargin: 20
            }
        }

        BusyLabel {
            id: indicator
            anchors.top: search.bottom
        }

        ListView {
            id: listView
            anchors {
                top: indicator.bottom
                topMargin: 10
                bottom: parent.bottom
                bottomMargin: 10
            }
            width: parent.width
            model: fahrplanBackend.favorites
            delegate: stationsResultDelegate
            clip: true
            visible: (fahrplanBackend.favorites.count > 0 && listView.model === fahrplanBackend.favorites) || listView.model === fahrplanBackend.stationSearchResults
        }
    }

    Component {
        id: stationsResultDelegate

        Item {
            id: delegateItem
            width: listView.width
            height: 30 + lbl_stationname.height

            Rectangle {
                id: background
                anchors.fill: parent
                color: "DarkGrey"
                visible: mouseArea.pressed
            }

            MouseArea {
                id: mouseArea
                anchors {
                    left: lbl_stationname.left
                    top: background.top
                    right: background.right
                    bottom: background.bottom
                }
                onClicked: {
                    listView.model.selectStation(stationSelect.type, model.index);
                    pageStack.pop();
                }
            }

            Image {
                id: img_fav
                source: model.isFavorite ? "image://theme/icon-s-common-favorite-mark" + inverseSuffix : "image://theme/icon-s-common-favorite-unmark" + inverseSuffix
                anchors {
                    left: parent.left
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }
            }

            MouseArea {
                id: mouseArea_fav
                anchors {
                    top: background.top
                    bottom: background.bottom
                    left: background.left
                    right: lbl_stationname.left
                }

                onClicked: {
                    if (model.isFavorite) {
                        banner.text = qsTr("Removing '%1' from favorites").arg(lbl_stationname.text);
                        banner.show();
                        listView.model.removeFromFavorites(model.index);
                    } else {
                        banner.text = qsTr("Adding '%1' to favorites").arg(lbl_stationname.text);
                        banner.show();
                        listView.model.addToFavorites(model.index);
                    }
                }
            }

            Label {
                id: lbl_stationname
                anchors {
                    left: img_fav.right
                    leftMargin: 10
                    rightMargin: 20
                    right: lbl_miscinfo.left
                    verticalCenter: parent.verticalCenter
                }
                text: model.name
            }

            Label {
                id: lbl_miscinfo
                anchors {
                    right: parent.right
                    rightMargin: 10
                    verticalCenter: parent.verticalCenter
                }
                text: model.miscInfo
                color: "DarkGrey";
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

        ButtonRow{
            ToolButton {
                id: favIcon
                platformStyle: TabButtonStyle{}
                iconSource: "image://theme/icon-m-toolbar-favorite-mark" + whiteSuffix
                onClicked: {
                    listView.model = fahrplanBackend.favorites

                    favIcon.checked = true;
                    searchIcon.checked = false;
                }
                flat: true
                checkable: true
                checked: true
            }
            ToolButton {
                    id: searchIcon
                    platformStyle: TabButtonStyle{}
                    iconSource: "image://theme/icon-m-toolbar-search" + whiteSuffix
                    onClicked: {
                        listView.model = fahrplanBackend.stationSearchResults
                        favIcon.checked = false;
                        searchIcon.checked = true;
                    }
                    flat: true
                    checkable: true
                    checked: false
                }
        }
    }

    PositionSource {
        id: positionSource
        onPositionChanged: {
            if (positionSource.position.latitudeValid && positionSource.position.longitudeValid) {
                indicator.show(qsTr("Searching for stations..."));
                positionSource.stop();

                fahrplanBackend.parser.findStationsByCoordinates(positionSource.position.coordinate.longitude, positionSource.position.coordinate.latitude);
            } else {
                indicator.show(qsTr("Waiting for GPS lock..."));
            }
        }
    }

    onStatusChanged: {
        switch (status) {
            case PageStatus.Activating:
                gpsButton.visible = fahrplanBackend.parser.supportsGps() && (fahrplanBackend.getSettingsValue("enableGps", "true") == "true");
                break;
        }
    }

    Connections {
        target: fahrplanBackend

        onParserStationsResult: {
            console.debug(fahrplanBackend.stationSearchResults.count);
            indicator.hide();
        }
    }
}
