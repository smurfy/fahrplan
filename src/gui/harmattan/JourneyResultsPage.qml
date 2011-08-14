import Fahrplan 1.0 as Fahrplan
import QtQuick 1.1
import com.meego 1.0
import "components" as MyComponents

Page {
    property alias fahrplanBackend: fahrplanBackend
    property alias searchResults: searchResults

    id: searchResultsPage

    tools: journeyResultsToolbar
    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        MyComponents.TitleBar {
            id: titleBar
            titleText: "Search Results"
            width: parent.width * 2
            opacity: 0.9
        }

        Item {
            id: listHead

            width: parent.width;
            height: lbl_departuretime.height

            anchors {
                top: titleBar.bottom
                topMargin: 10
            }

            Label {
                id: lbl_departuretime
                anchors {
                    left: parent.left
                    leftMargin: 20
                }
                text: "Dep."
                width: (parent.width / 4)
            }

            Label {
                id: lbl_arrivaltime
                anchors {
                    left: lbl_departuretime.right
                    leftMargin: 10
                }
                text: "Arr."
                width: (parent.width / 4)
            }

            Label {
                id: lbl_duration
                anchors {
                    left: lbl_arrivaltime.right
                    leftMargin: 10
                }
                text: "Dur."
                width: (parent.width / 4)
            }

            Label {
                id: lbl_transfers
                anchors {
                    left: lbl_duration.right
                    leftMargin: 10
                }
                text: "Trans."
                width: (parent.width / 4)
            }
        }

        ListView {
            id: listView
            anchors {
                top: listHead.bottom
                topMargin: 10
                bottom: journeyResultsToolbar.top
            }
            height: (parent.height - titleBar.height - listHead.height) - 10
            width: parent.width
            model: journeyResultModel
            delegate:  journeyResultDelegate
            clip: true
        }
    }

    Component {
        id: journeyResultDelegate

        Item {
            id: delegateItem
            width: listView.width
            height: 30 + lbl_departuretime.height + lbl_traintyp.height

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

                }
            }

            Item {
                anchors {
                    verticalCenter: parent.verticalCenter
                    top: parent.top
                    topMargin: 10
                }

                width: parent.width
                height: parent.height

                Label {
                    id: lbl_departuretime
                    anchors {
                        left: parent.left
                        leftMargin: 20
                    }
                    text: departureTime
                    width: (parent.width / 4)
                }

                Label {
                    id: lbl_arrivaltime
                    anchors {
                        left: lbl_departuretime.right
                        leftMargin: 10
                    }
                    text: arrivalTime
                    width: (parent.width / 4)
                }

                Label {
                    id: lbl_duration
                    anchors {
                        left: lbl_arrivaltime.right
                        leftMargin: 10
                    }
                    text: duration
                    width: (parent.width / 4)
                }

                Label {
                    id: lbl_transfers
                    anchors {
                        left: lbl_duration.right
                        leftMargin: 10
                    }
                    text: transfers
                    width: (parent.width / 4)
                }

                Label {
                    id: lbl_traintyp
                    anchors {
                        left: parent.left
                        leftMargin: 20
                        top: lbl_departuretime.bottom
                        topMargin: 5
                    }
                    text: trainType
                    width: parent.width - 40
                }

            }
        }
    }

    ListModel {
        id: journeyResultModel
    }

    Fahrplan.Backend {
        id: fahrplanBackend
        onParserJourneyResult: {
            console.log("Got results");
            console.log(result.count);

            journeyResultModel.clear();
            for (var i = 0; i < result.count; i++) {
                var item = result.getItem(i);
                journeyResultModel.append({
                    "id": item.id,
                    "departureTime": item.departureTime,
                    "arrivalTime": item.arrivalTime,
                    "trainType": item.trainType,
                    "duration": item.duration,
                    "transfers": item.transfers,
                    "miscInfo": item.miscInfo,
                });
            }
        }
    }

    ToolBarLayout {
        id: journeyResultsToolbar

        ToolIcon {
            id : backIcon;
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop(2);
            }
        }
    }
}
