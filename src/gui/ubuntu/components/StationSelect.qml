import Fahrplan 1.0
import QtQuick 2.0
import QtLocation 5.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems
import "."
import ".."

Page {
    id: stationSelect

    signal stationSelected ( string name )

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
                if (searchBox.text == "")
                    return;

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
                fahrplanBackend.parser.findStationsByName(searchBox.text);
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
//                    listView.model = stationsResultModel
//                    favIcon.checked = false;
//                    searchIcon.checked = true;

//                    stationsResultModel.clear();
//                    stationsResultModel.append({
//                        "name": qsTr("Requesting GPS..."),
//                        "process": true,
//                        "internal": true,
//                        "isfavorite": false,
//                        "showfavorite": false,
//                        "miscinfo": ""
//                    })

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
                icon: "file:///usr/share/icons/ubuntu-mobile/actions/scalable/favorite-" + (isfavorite ? "selected.svg" : "unselected.svg")

                onClicked: {
                    if (!internal) {
                        stationSelect.stationSelected(name)
                    }
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
                        if (showfavorite) {
                            if (fahrplanBackend.favorites.isFavorite(name)) {
                                fahrplanBackend.favorites.removeFavorite(name);
                                delegateItem.icon = "file:///usr/share/icons/ubuntu-mobile/actions/scalable/favorite-unselected.svg"
                            } else {
                                fahrplanBackend.favorites.addFavorite(name);
                                delegateItem.icon = "file:///usr/share/icons/ubuntu-mobile/actions/scalable/favorite-selected.svg"
                            }
                        }
                    }
                }
            }

            clip: true
            visible: (fahrplanBackend.favorites.count > 0 && listView.model == fahrplanBackend.favorites) || listView.model == stationsResultModel
        }
    }

    ListModel {
        id: stationsResultModel
    }

    FahrplanBackend {
        id: fahrplanBackend

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
}
