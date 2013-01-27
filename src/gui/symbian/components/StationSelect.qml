import Fahrplan 1.0
import QtQuick 1.1
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
            visible: (stationsFavoritesModel.count == 0 && listView.model === stationsFavoritesModel)
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

        ListView {
            id: listView
            anchors {
                top: search.bottom
                topMargin: platformStyle.paddingMedium
                bottom: parent.bottom
                bottomMargin: platformStyle.paddingMedium
            }
            width: parent.width
            model: stationsFavoritesModel
            delegate: stationsResultDelegate
            visible: (stationsFavoritesModel.count > 0 && listView.model === stationsFavoritesModel) || listView.model === stationsResultModel
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
                source: Style.getIconFromTheme(appWindow.platformInverted, isfavorite ? "qtg_graf_rating_rated" : "qtg_graf_rating_unrated")
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
                    right: lbl_miscinfo.left
                    rightMargin: platformStyle.paddingMedium
                    verticalCenter: parent.verticalCenter
                }
                text: name
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
                text: miscinfo
                color: Style.listBackgroundHighlight
            }

            BusyIndicator {
                id: indicator
                anchors {
                    right: parent.right
                    rightMargin: platformStyle.graphicSizeTiny
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
                platformInverted: appWindow.platformInverted
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

    onStatusChanged: {
        switch (status) {
            case PageStatus.Activating:
                gpsButton.visible = fahrplanBackend.parser.supportsGps() && (fahrplanBackend.getSettingsValue("enableGps", "true") == "true");
                updateFavorites();
                break;
        }
    }
}
