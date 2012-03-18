import Fahrplan 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "components"

Page {
    property alias fahrplanBackend: fahrplanBackend
    property alias searchResults: searchResults
    property alias journeyStationsTitleText: journeyStations.text
    property alias searchIndicatorVisible: searchIndicator.visible

    id: searchResultsPage

    tools: journeyResultsToolbar
    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        Item {
            id: titleBar

            height: journeyStations.height + journeyDate.height + 20
            width: parent.width

            Label {
                id: journeyStations
                text: ""
                font.bold: true
                font.pixelSize: 30
                anchors {
                    left: parent.left
                    leftMargin: 10
                    right: parent.right
                    rightMargin: 10
                    top: parent.top
                    topMargin: 10
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
                visible: !searchIndicatorVisible
            }
        }

        BusyIndicator {
            id: searchIndicator
            anchors {
                top: titleBar.bottom
                topMargin: 50;
                horizontalCenter: parent.horizontalCenter
            }
            running: true
            visible: false

            platformStyle: BusyIndicatorStyle { size: "large" }
        }

        Item {
            id: listHead

            width: parent.width;
            height: lbl_departuretime.height
            visible: !searchIndicator.visible

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
                text: qsTr("Dep.")
                width: (parent.width  - 40) / 4
            }

            Label {
                id: lbl_arrivaltime
                anchors {
                    left: lbl_departuretime.right
                    leftMargin: 10
                }
                text: qsTr("Arr.")
                width: (parent.width  - 40) / 4
            }

            Label {
                id: lbl_duration
                anchors {
                    left: lbl_arrivaltime.right
                    leftMargin: 10
                }
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Dur.")
                width: (parent.width  - 40) / 4
            }

            Label {
                id: lbl_transfers
                anchors {
                    left: lbl_duration.right
                    leftMargin: 10
                }
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Trans.")
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
            visible: !searchIndicator.visible
        }

        ScrollDecorator {
            flickableItem: listView
        }
    }

    Component {
        id: journeyResultDelegate

        Item {
            id: delegateItem
            width: listView.width
            height: 30 + lbl_departuretime.height + lbl_traintyp.height + (lbl_miscinfo.visible ? lbl_miscinfo.height : 0)

            Rectangle {
                anchors.fill: parent
                color: itemNum % 2 ? "White" : "LightGrey"
            }

            Rectangle {
                id: background
                anchors.fill: parent
                color: "DarkGrey"
                visible: mouseArea.pressed
            }

            MouseArea {
                id: mouseArea
                anchors.fill: background
                onClicked: {

                    detailsResultsPage.titleText = qsTr("Loading details");
                    detailsResultsPage.subTitleText = qsTr("please wait...");
                    detailsResultsPage.subTitleText2 = "";
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

                Label {
                    id: lbl_miscinfo
                    anchors {
                        left: parent.left
                        leftMargin: 10
                        top: lbl_traintyp.bottom
                        topMargin: 5
                    }
                    visible: (miscInfo == "") ? false : true
                    text: miscInfo
                    width: parent.width - 40
                    font.bold: true
                }

            }
        }
    }

    ListModel {
        id: journeyResultModel
    }

    FahrplanBackend {
        id: fahrplanBackend
        onParserJourneyResult: {
            console.log("Got results");
            console.log(result.count);

            searchIndicatorVisible = false;

            journeyStations.text = result.departureStation +
                    qsTr(" to ") +
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
                    "itemNum" : i
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
                pageStack.pop();
                fahrplanBackend.parser.cancelRequest();
            }
        }

        ToolButtonRow {
            ToolButton {
                text:qsTr("Earlier")
                visible: !searchIndicatorVisible;
                onClicked: {
                    searchIndicatorVisible = true
                    fahrplanBackend.parser.searchJourneyEarlier();
                }
            }
            ToolButton {
                text:qsTr("Later")
                visible: !searchIndicatorVisible;
                onClicked: {
                    searchIndicatorVisible = true
                    fahrplanBackend.parser.searchJourneyLater();
                }
            }
        }
    }
}
