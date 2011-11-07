import Fahrplan 1.0 as Fahrplan
import QtQuick 1.1
import com.meego 1.0
import "components"

Page {
    property alias fahrplanBackend: fahrplanBackend
    property alias searchResults: searchResults

    id: searchResultsPage

    tools: journeyResultsToolbar
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
                    right: parent.right
                    rightMargin: 10
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
                    leftMargin: 10
                }
                text: "Dep."
                width: (parent.width  - 40) / 4
            }

            Label {
                id: lbl_arrivaltime
                anchors {
                    left: lbl_departuretime.right
                    leftMargin: 10
                }
                text: "Arr."
                width: (parent.width  - 40) / 4
            }

            Label {
                id: lbl_duration
                anchors {
                    left: lbl_arrivaltime.right
                    leftMargin: 10
                }
                horizontalAlignment: Text.AlignHCenter
                text: "Dur."
                width: (parent.width  - 40) / 4
            }

            Label {
                id: lbl_transfers
                anchors {
                    left: lbl_duration.right
                    leftMargin: 10
                }
                horizontalAlignment: Text.AlignHCenter
                text: "Trans."
                width: (parent.width  - 40) / 4
            }
        }

        ListView {
            id: listView
            anchors {
                top: listHead.bottom
                topMargin: 10
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

                    detailsResultsPage.titleText = "Loading details";
                    detailsResultsPage.subTitleText = "please wait...";
                    detailsResultsPage.searchIndicatorVisible = true;
                    pageStack.push(detailsResultsPage);
                    fahrplanBackend.parser.getJourneyDetails(id);
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
                        leftMargin: 10
                    }
                    text: departureTime
                    width: (parent.width  - 40) / 4
                }

                Label {
                    id: lbl_arrivaltime
                    anchors {
                        left: lbl_departuretime.right
                        leftMargin: 10
                    }
                    text: arrivalTime
                    width: (parent.width  - 40) / 4
                }

                Label {
                    id: lbl_duration
                    anchors {
                        left: lbl_arrivaltime.right
                        leftMargin: 10
                    }
                    horizontalAlignment: Text.AlignHCenter
                    text: duration
                    width: (parent.width  - 40) / 4
                }

                Label {
                    id: lbl_transfers
                    anchors {
                        left: lbl_duration.right
                        leftMargin: 10
                    }
                    horizontalAlignment: Text.AlignHCenter
                    text: transfers
                    width: (parent.width  - 40) / 4
                }

                Label {
                    id: lbl_traintyp
                    anchors {
                        left: parent.left
                        leftMargin: 10
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

            journeyStations.text = result.departureStation +
                    " to " +
                    result.arrivalStation;

            journeyDate.text = result.timeInfo;

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


    JourneyDetailsResultsPage {
        id: detailsResultsPage
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

        ToolButtonRow {
            ToolButton {
                text:"Earlier"
                onClicked: {
                    pageStack.pop();
                    fahrplanBackend.parser.searchJourneyEarlier();
                }
            }
            ToolButton {
                text:"Later"
                onClicked: {
                    pageStack.pop();
                    fahrplanBackend.parser.searchJourneyLater();
                }
            }
        }
    }
}
