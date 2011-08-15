import Fahrplan 1.0 as Fahrplan
import QtQuick 1.1
import com.meego 1.0
import "components" as MyComponents

Page {
    id: searchDetailResultsPage

    tools: journeyDetailResultsToolbar
    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        Item {
            id: titleBar

            height: journeyStations.height + journeyDate.height + 30
            width: parent.width

            Label {
                id: journeyStations
                text: ""
                font.bold: true
                font.pixelSize: 32
                anchors {
                    left: parent.left
                    leftMargin: 10
                    top: parent.top
                    topMargin: 20
                }
                width: parent.width
            }

            Label {
                id: journeyDate
                color: "Grey"
                anchors {
                    left: parent.left
                    leftMargin: 10
                    top: journeyStations.bottom
                    topMargin: 10
                }
                width: parent.width
            }
        }

        ListView {
            id: listView
            anchors {
                top: titleBar.bottom
                topMargin: 10
                bottom: journeyDetailResultsToolbar.top
                bottomMargin: 10
            }
            height: (parent.height - titleBar.height) - 20
            width: parent.width
            model: journeyDetailResultModel
            delegate:  journeyDetailResultDelegate
            clip: true
        }
    }

    Component {
        id: journeyDetailResultDelegate

        Item {
            id: delegateItem
            width: listView.width
            height: 30 + item_departurestations.height + lbl_train.height + item_arrivalstations.height

            Item {
                anchors {
                    verticalCenter: parent.verticalCenter
                    top: parent.top
                    topMargin: 10
                }

                width: parent.width
                height: parent.height

                Item {

                    id: item_departurestations

                    height: lbl_departurestation.height + 30
                    width: parent.width


                    BorderImage {
                        id: item_departurestations_background
                        anchors.fill: parent
                        source: "image://theme/meegotouch-list-background-pressed-center"
                    }

                    Label {
                        id: lbl_departuretime
                        anchors {
                            left: parent.left
                            leftMargin: 10
                            top: parent.top
                            topMargin: 10
                        }
                        text: departureTime
                        width: (parent.width - 40) / 3
                    }


                    Label {
                        id: lbl_departureinfo
                        anchors {
                            left: lbl_departuretime.right
                            leftMargin: 10
                            top: parent.top
                            topMargin: 10
                        }
                        text: departureInfo
                        width: (parent.width - 40) / 3
                    }


                    Label {
                        id: lbl_departurestation
                        anchors {
                            right: parent.right
                            rightMargin: 10
                            top: parent.top
                            topMargin: 10
                        }
                        font.bold: true
                        text: departureStation
                        horizontalAlignment: Text.AlignRight
                        width: (parent.width  - 40) / 3
                    }
                }

                Label {
                    id: lbl_train
                    anchors {
                        left: parent.left
                        leftMargin: 10
                        top: item_departurestations.bottom
                        topMargin: 15
                    }
                    text: train
                    width: (parent.width  - 40)
                }

                Item {

                    id: item_arrivalstations

                    height: lbl_arrivalstation.height + 30
                    width: parent.width

                    anchors {
                        top: lbl_train.bottom
                        topMargin: 15
                    }

                    BorderImage {
                        id: item_arrivalstations_background
                        anchors.fill: parent
                        source: "image://theme/meegotouch-list-background-pressed-center"
                    }

                    Label {
                        id: lbl_arrivaltime
                        anchors {
                            left: parent.left
                            leftMargin: 10
                            top: parent.top
                            topMargin: 10
                        }
                        text: arrivalTime
                        width: (parent.width  - 40) / 3
                    }

                    Label {
                       id: lbl_arrivalinfo
                       anchors {
                           left: lbl_arrivaltime.right
                           leftMargin: 10
                           top: parent.top
                           topMargin: 10
                       }
                       text: arrivalInfo
                       width: (parent.width  - 40) / 3
                    }

                    Label {
                        id: lbl_arrivalstation
                        anchors {
                            right: parent.right
                            rightMargin: 10
                            top: parent.top
                            topMargin: 10
                        }
                        font.bold: true
                        horizontalAlignment: Text.AlignRight
                        text: arrivalStation
                        width: (parent.width  - 40) / 3
                    }
                }
            }
        }
    }

    ListModel {
        id: journeyDetailResultModel
    }

    Fahrplan.Backend {
        id: fahrplanBackend
        onParserJourneyDetailsResult: {
            console.log("Got detail results");
            console.log(result.count);
            journeyStations.text = result.departureStation + " to " + result.arrivalStation;
            journeyDate.text = "Duration: " + result.duration;
            journeyDetailResultModel.clear();
            for (var i = 0; i < result.count; i++) {
                var item = result.getItem(i);
                journeyDetailResultModel.append({
                    "departureTime": "Dep. " + Qt.formatTime(item.departureDateTime,"hh:mm"),
                    "arrivalTime": "Arr. " + Qt.formatTime(item.arrivalDateTime,"hh:mm"),
                    "departureStation": item.departureStation,
                    "arrivalStation": item.arrivalStation,
                    "departureInfo": item.departureInfo,
                    "arrivalInfo": item.arrivalInfo,
                    "train": item.train + " " + item.info
                });
            }
        }
    }

    ToolBarLayout {
        id: journeyDetailResultsToolbar

        ToolIcon {
            id : backIcon;
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }
    }
}
