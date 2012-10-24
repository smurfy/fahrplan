import Fahrplan 1.0
import QtQuick 1.1
import QtMobility.location 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1
import "../js/style.js" as Style

Page {
    id: stationSelect

    signal stationSelected ( string name )

    function updateFavorites()
    {
        stationsFavoritesModel.clear();
        var favorites = fahrplanBackend.favorites.getFavorites();
        for (var i = 0; i < favorites.length; i++) {
            stationsFavoritesModel.append({
                "name": favorites[i],
                "process": false,
                "internal": false,
                "isfavorite": true,
                "showfavorite": true,
                "miscinfo": ""
            })
        }
    }

    tools: stationSelectToolbar

    Item {
        width:  stationSelect.width
        height: stationSelect.height

        Item {
            id: search

            height: 60

            anchors {
                left: parent.left
                right: parent.right
            }

            SearchBox {
                id: searchBox

                anchors {
                    left: parent.left
                    right: gpsButton.left
                    verticalCenter: parent.verticalCenter
                }

                onSearchTextChanged: {
                    search.findStationsByName();
                }

                placeHolderText: qsTr("Search for Station...")
            }

            function findStationsByName(){
                stationsResultModel.clear();
                stationsResultModel.append({
                    "name": qsTr("Searching ..."),
                    "process": true,
                    "internal": true,
                    "isfavorite": false,
                    "showfavorite": false,
                    "miscinfo": ""
                })
                listView.model = stationsResultModel
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
                source: "image://theme/qtg_fr_tab_bar"
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
                platformInverted: true
                iconSource: "qrc:/src/gui/symbian/icon/icon-s-calendar-location-picker-inverse.png"
                width: visible ? 60 : 0

                onClicked: {
                    listView.model = stationsResultModel
                    favIcon.checked = false;
                    searchIcon.checked = true;

                    stationsResultModel.clear();
                    stationsResultModel.append({
                        "name": qsTr("Requesting GPS..."),
                        "process": true,
                        "internal": true,
                        "isfavorite": false,
                        "showfavorite": false,
                        "miscinfo": ""
                    })

                    positionSource.active = true;
                    positionSource.start();
                }
            }
        }

        Text {
            width: parent.width
            wrapMode: Text.WordWrap
            height: parent.height - search.top - stationSelectToolbar.height
            text: qsTr("Click the star icon on the search results to add or remove a station as a favorite");
            color: platformStyle.colorNormalMid
            font.pixelSize: 2 * platformStyle.fontSizeMedium
            visible: (stationsFavoritesModel.count == 0 && listView.model === stationsFavoritesModel)
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

        ListView {
            id: listView
            anchors {
                topMargin: platformStyle.paddingMedium
                top: search.bottom
                bottomMargin: platformStyle.paddingMedium
            }
            height: parent.height - stationSelectToolbar.height
            width: parent.width
            model: stationsFavoritesModel
            delegate: stationsResultDelegate
            clip: true
            visible: (stationsFavoritesModel.count > 0 && listView.model === stationsFavoritesModel) || listView.model === stationsResultModel
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
                color: Style.listBackgroundHighlight
                visible: mouseArea.pressed
            }

            MouseArea {
                id: mouseArea
                enabled: !internal
                anchors {
                    left: lbl_stationname.left
                    top: background.top
                    right: background.right
                    bottom: background.bottom
                }
                onClicked: {
                    if (!internal) {
                        stationSelect.stationSelected(name)
                    }
                }
            }

            Image {
                id: img_fav
                source: isfavorite ? "image://theme/qtg_graf_rating_rated" : "image://theme/qtg_graf_rating_unrated"
                visible: showfavorite
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
                    if (showfavorite) {
                        if (fahrplanBackend.favorites.isFavorite(lbl_stationname.text)) {
                            banner.text = qsTr("Removing '%1' from favorites").arg(lbl_stationname.text)
                            banner.open();
                            fahrplanBackend.favorites.removeFavorite(lbl_stationname.text);
                        } else {
                            banner.text = qsTr("Adding '%1' to favorites").arg(lbl_stationname.text)
                            banner.open();
                            fahrplanBackend.favorites.addFavorite(lbl_stationname.text);
                        }
                    }
                }
            }

            Label {
                id: lbl_stationname
                anchors {
                    left: showfavorite ? img_fav.right : parent.left
                    leftMargin: platformStyle.paddingMedium
                    rightMargin: 20
                    right: process ? process.left : parent.right
                    verticalCenter: parent.verticalCenter
                }
                text: name
            }

            Label {
                id: lbl_miscinfo
                anchors {
                    right: parent.right
                    rightMargin: platformStyle.paddingMedium
                    verticalCenter: parent.verticalCenter
                }
                text: miscinfo
                color: Style.listBackgroundHighlight
            }

            BusyIndicator {
                id: indicator
                anchors {
                    right: parent.right
                    rightMargin: 20
                    verticalCenter: parent.verticalCenter
                }
                running: process
                visible: process

                width: platformStyle.graphicSizeSmall; height: width
            }
        }
    }

    ListModel {
        id: stationsResultModel
    }

    ListModel {
        id: stationsFavoritesModel
    }

    ToolBarLayout {
        id:stationSelectToolbar

        ToolButton {
            id : backIcon
            iconSource: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }

        ButtonRow {
            ToolButton {
                id: favIcon
                iconSource: "image://theme/qtg_graf_rating_rated"
                onClicked: {
                    updateFavorites();
                    listView.model = stationsFavoritesModel

                    favIcon.checked = true;
                    searchIcon.checked = false;
                }
                checkable: true
                checked: true
            }
            ToolButton {
                id: searchIcon
                iconSource: "toolbar-search"
                onClicked: {
                    listView.model = stationsResultModel
                    favIcon.checked = false;
                    searchIcon.checked = true;
                }
                checkable: true
                checked: false
            }
        }
    }

    FahrplanBackend {
        id: fahrplanBackend

        onParserChanged: {
            gpsButton.visible = fahrplanBackend.parser.supportsGps();
            updateFavorites();
        }

        onParserStationsResult: {
            stationsResultModel.clear();
            for (var i = 0; i < result.count; i++) {
                var item = result.getItem(i);
                stationsResultModel.append({
                    "name": item.stationName,
                    "process": false,
                    "internal": false,
                    "isfavorite": fahrplanBackend.favorites.isFavorite(item.stationName),
                    "showfavorite": true,
                    "miscinfo": item.miscInfo
                });
            }
        }
        onFavoritesChanged: {
            updateFavorites();
        }
    }

    PositionSource {
        id: positionSource
        onPositionChanged: {
            if (positionSource.position.latitudeValid && positionSource.position.longitudeValid) {
                stationsResultModel.clear();
                stationsResultModel.append({
                    "name": qsTr("Searching for stations..."),
                    "process": true,
                    "internal": true,
                    "isfavorite": false,
                    "showfavorite": false,
                    "miscinfo": ""
                })
                positionSource.active = false;
                positionSource.stop();

                fahrplanBackend.parser.findStationsByCoordinates(positionSource.position.coordinate.longitude, positionSource.position.coordinate.latitude);
            } else {
                stationsResultModel.clear();
                stationsResultModel.append({
                    "name": qsTr("Waiting for GPS lock..."),
                    "process": true,
                    "internal": true,
                    "isfavorite": false,
                    "showfavorite": false,
                    "miscinfo": ""
                })
            }
        }
    }

    function printableMethod(method) {
        if (method == PositionSource.SatellitePositioningMethod)
            return "Satellite";
        else if (method == PositionSource.NoPositioningMethod)
            return "Not available"
        else if (method == PositionSource.NonSatellitePositioningMethod)
            return "Non-satellite"
        else if (method == PositionSource.AllPositioningMethods)
            return "Multiple"
        return "source error";
    }
}
