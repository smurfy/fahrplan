import Fahrplan 1.0 as Fahrplan
import QtQuick 1.1
import QtMobility.location 1.1
import com.meego 1.0
import com.nokia.extras 1.0
import "." as MyComponents

Page {
    id: stationSelect

    signal stationSelected ( string name )

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
                    right: searchButtons.left
                    rightMargin: 10
                    top: parent.top
                    topMargin: 10
                }

                onTextChanged: {
                    search.findStationsByName();
                }


                platformSipAttributes: SipAttributes { actionKeyHighlighted: true }

                placeholderText: "Search for Station..."
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
                    "name": "Searching ...",
                    "process": true,
                    "internal": true,
                    "isfavorite": false,
                    "showfavorite": false,
                    "miscinfo": ""
                })
                listView.model = stationsResultModel
                fahrplanBackend.parser.findStationsByName(searchBox.text);
            }

            Button {
                id: searchButtons
                anchors {
                    right: parent.right
                    rightMargin: 10
                    top: parent.top
                    topMargin: 10
                }
                platformStyle: ButtonStyle { inverted: true }
                iconSource: "image://theme/icon-s-status-gps"
                width: 80

                onClicked: {
                    listView.model = stationsResultModel
                    stationsResultModel.clear();
                    stationsResultModel.append({
                        "name": "Requesting GPS...",
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

        ListView {
            id: listView
            anchors {
                topMargin: 10
                top: search.bottom
                bottomMargin: 10
            }
            height: parent.height - stationSelectToolbar.height
            width: parent.width
            model: stationsResultModel
            delegate: stationsResultDelegate
            clip: true
        }
    }

    Component {
        id: stationsResultDelegate

        Item {
            id: delegateItem
            width: listView.width
            height: 30 + lbl_stationname.height

            BorderImage {
                id: background
                anchors.fill: parent
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
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
                anchors.fill: img_fav
                onClicked: {
                    if (showfavorite) {
                        banner.text = "TODO: Adding " + lbl_stationname.text + " to favorites"
                        banner.show();
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
        ToolIcon {
            id : favIcon
            iconId: "toolbar-favorite-mark"
            onClicked: {
                listView.model = stationsFavoritesModel
            }
        }
        ToolIcon {
            id : searchIcon
            iconId: "toolbar-search"
            onClicked: {
                listView.model = stationsResultModel
            }
        }
    }

    Fahrplan.Backend {
        id: fahrplanBackend
        onParserStationsResult: {
            stationsResultModel.clear();
            for (var i = 0; i < result.count; i++) {
                var item = result.getItem(i);
                stationsResultModel.append({
                    "name": item.stationName,
                    "process": false,
                    "internal": false,
                    "isfavorite": false,
                    "showfavorite": true,
                    "miscinfo": item.miscInfo
                });
            }
        }
    }

    PositionSource {
        id: positionSource
        onPositionChanged: {
            stationsResultModel.clear();
            stationsResultModel.append({
                "name": "Searching for stations...",
                "process": true,
                "internal": true,
                "isfavorite": false,
                "showfavorite": false,
                "miscinfo": ""
            })
            positionSource.active = false;
            positionSource.stop();

            fahrplanBackend.parser.findStationsByCoordinates(positionSource.position.coordinate.longitude, positionSource.position.coordinate.latitude);
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
