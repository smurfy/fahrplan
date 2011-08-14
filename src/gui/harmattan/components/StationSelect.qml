import Fahrplan 1.0 as Fahrplan
import QtQuick 1.1
import QtMobility.location 1.1
import com.meego 1.0
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
                leftMargin: 10
            }

            BorderImage {
                source: "image://theme/meegotouch-list-header-background"
                width: parent.width
                height: parent.height + 14
                y: -7
            }

            TextField {
                id: searchBox
                anchors {
                    left: parent.left
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
                    "miscinfo": ""
                })
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
                    stationsResultModel.clear();
                    stationsResultModel.append({
                        "name": "Requesting GPS...",
                        "process": true,
                        "internal": true,
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
                bottom: stationSelectToolbar.top
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
                anchors.fill: background
                onClicked: {
                    if (!internal) {
                        stationSelect.stationSelected(name)
                    }
                }
            }

            Label {
                id: lbl_stationname
                anchors {
                    left: parent.left
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

    ToolBarLayout {
        id:stationSelectToolbar

        ToolIcon {
            id : backIcon;
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
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
                "miscinfo": ""
            })
            positionSource.active = false;
            positionSource.stop();

            fahrplanBackend.parser.findStationsByCoordinates(positionSource.position.coordinate.longitude, positionSource.position.coordinate.latitude);
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
