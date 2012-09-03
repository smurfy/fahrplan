import Fahrplan 1.0
import QtQuick 1.1
import QtMobility.location 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import "."

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

            height: 70

            anchors {
                left: parent.left
                right: parent.right
            }

            Rectangle {
                anchors.fill: parent
                color: "LightGrey"
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

                onTextChanged: {
                    search.findStationsByName();
                }


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
                platformStyle: ButtonStyle { inverted: true }
                iconSource: "image://theme/icon-s-calendar-location-picker-inverse"
                width: visible ? 80 : 0

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
            color: "DarkGrey"
            font.pixelSize: 50
            visible: (stationsFavoritesModel.count == 0 && listView.model == stationsFavoritesModel)
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
                topMargin: 10
                top: search.bottom
                bottomMargin: 10
            }
            height: parent.height - stationSelectToolbar.height
            width: parent.width
            model: stationsFavoritesModel
            delegate: stationsResultDelegate
            clip: true
            visible: (stationsFavoritesModel.count > 0 && listView.model == stationsFavoritesModel) || listView.model == stationsResultModel
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
                source: isfavorite ? "image://theme/icon-s-common-favorite-mark" : "image://theme/icon-s-common-favorite-unmark"
                visible: showfavorite
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
                    if (showfavorite) {
                        if (fahrplanBackend.favorites.isFavorite(lbl_stationname.text)) {
                            banner.text = qsTr("Removing '%1' from favorites").arg(lbl_stationname.text)
                            banner.show();
                            fahrplanBackend.favorites.removeFavorite(lbl_stationname.text);
                        } else {
                            banner.text = qsTr("Adding '%1' to favorites").arg(lbl_stationname.text)
                            banner.show();
                            fahrplanBackend.favorites.addFavorite(lbl_stationname.text);
                        }
                    }
                }
            }

            Label {
                id: lbl_stationname
                anchors {
                    left: showfavorite ? img_fav.right : parent.left
                    leftMargin: 10
                    rightMargin: 10
                    verticalCenter: parent.verticalCenter
                }
                text: name
                width: (parent.width - 40) - lbl_miscinfo.width;
            }

            Label {
                id: lbl_miscinfo
                anchors {
                    right: parent.right
                    rightMargin: 10
                    verticalCenter: parent.verticalCenter
                }
                text: miscinfo
                color: "DarkGrey";
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

                platformStyle: BusyIndicatorStyle { size: "small" }
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
                iconSource: "image://theme/icon-m-toolbar-favorite-mark"
                onClicked: {
                    updateFavorites();
                    listView.model = stationsFavoritesModel

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
                    iconSource: "image://theme/icon-m-toolbar-search"
                    onClicked: {
                        listView.model = stationsResultModel
                        favIcon.checked = false;
                        searchIcon.checked = true;
                    }
                    flat: true
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

    InfoBanner{
            id: banner
            objectName: "fahrplanInfoBannerStationSelect"
            text: ""
            anchors.top: parent.top
            anchors.topMargin: 10
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
