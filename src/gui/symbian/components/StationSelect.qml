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
                listView.model = fahrplanBackend.stationSearchResults;
                favIcon.checked = false;
                searchIcon.checked = true;
                fahrplanBackend.parser.findStationsByName(searchBox.searchText);
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
                    listView.model = fahrplanBackend.stationSearchResults;
                    favIcon.checked = false;
                    searchIcon.checked = true;
                    positionSource.update();
                }
            }
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap
            height: parent.height - search.top - stationSelectToolbar.height
            text: qsTr("Click the star icon on the search results to add or remove a station as a favorite");
            color: platformInverted ? platformStyle.colorNormalMidInverted : platformStyle.colorNormalMid
            font.pixelSize: 2 * platformStyle.fontSizeMedium
            visible: (fahrplanBackend.favorites.count == 0 && listView.model === fahrplanBackend.favorites)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            platformInverted: appWindow.platformInverted
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
            platformInverted: appWindow.platformInverted
            anchors.top: search.bottom
        }

        ListView {
            id: listView
            anchors {
                top: indicator.bottom
                topMargin: platformStyle.paddingMedium
                bottom: parent.bottom
                bottomMargin: platformStyle.paddingMedium
            }
            width: parent.width
            model: fahrplanBackend.favorites
            delegate: stationsResultDelegate
            visible: (fahrplanBackend.favorites.count > 0 && listView.model === fahrplanBackend.favorites) || listView.model === fahrplanBackend.stationSearchResults
        }
    }

    Component {
        id: stationsResultDelegate

        Item {
            id: delegateItem
            width: listView.width
            height: 2 * platformStyle.paddingLarge + lbl_stationname.height

            Rectangle {
                id: background
                anchors.fill: parent
                color: Style.listBackgroundHighlight
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
                source: Style.getIconFromTheme(appWindow.platformInverted, model.isFavorite ? "qtg_graf_rating_rated" : "qtg_graf_rating_unrated")
                sourceSize {
                    width: platformStyle.graphicSizeSmall
                    height: platformStyle.graphicSizeSmall
                }
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingMedium
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
                        banner.open();
                        listView.model.removeFromFavorites(model.index);
                    } else {
                        banner.text = qsTr("Adding '%1' to favorites").arg(lbl_stationname.text);
                        banner.open();
                        listView.model.addToFavorites(model.index);
                    }
                }
            }

            Label {
                id: lbl_stationname
                anchors {
                    left: img_fav.right
                    leftMargin: platformStyle.paddingMedium
                    right: lbl_miscinfo.left
                    rightMargin: platformStyle.paddingMedium
                    verticalCenter: parent.verticalCenter
                }
                text: model.name
                platformInverted: appWindow.platformInverted
                wrapMode: Text.WordWrap
            }

            Label {
                id: lbl_miscinfo
                anchors {
                    right: parent.right
                    rightMargin: platformStyle.paddingMedium
                    verticalCenter: parent.verticalCenter
                }
                text: model.miscInfo
                color: Style.listBackgroundHighlight
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
            ToolButton {
                id: favIcon
                iconSource: Style.getIconFromTheme(platformInverted, "qtg_graf_rating_rated")
                platformInverted: appWindow.platformInverted
                onClicked: {
                    listView.model = fahrplanBackend.favorites;

                    favIcon.checked = true;
                    searchIcon.checked = false;
                }
                checkable: true
                checked: true
            }
            ToolButton {
                id: searchIcon
                iconSource: "toolbar-search"
                platformInverted: appWindow.platformInverted
                onClicked: {
                    listView.model = fahrplanBackend.stationSearchResults;

                    favIcon.checked = false;
                    searchIcon.checked = true;
                }
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
            indicator.hide();
        }
    }
}
