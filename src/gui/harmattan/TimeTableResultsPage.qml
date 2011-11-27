import Fahrplan 1.0 as Fahrplan
import QtQuick 1.1
import com.meego 1.0
import "components"

Page {
    property alias timetableTitleText: timetableTitle.text
    property alias destinationTitleText: lbl_destinationTitle.text
    property alias searchIndicatorVisible: searchIndicator.visible

    id: searchResultsPage

    tools: timetableResultsToolbar
    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        Item {
            id: titleBar

            height: timetableTitle.height
            width: parent.width

            Label {
                id: timetableTitle
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
            height: lbl_time.height

            anchors {
                top: titleBar.bottom
                topMargin: 50
            }

            visible: !searchIndicator.visible

            Label {
                id: lbl_time
                anchors {
                    left: parent.left
                    leftMargin: 10
                }
                text: "Time"
                width: (parent.width  - 40) / 3
            }

            Label {
                id: lbl_train
                anchors {
                    left: lbl_time.right
                    leftMargin: 10
                }
                text: "Type"
                width: (parent.width  - 40) / 3
            }

            Label {
                id: lbl_destinationTitle
                anchors {
                    left: lbl_train.right
                    leftMargin: 10
                    rightMargin: 10
                }
                horizontalAlignment: Text.AlignHCenter
                text: "To"
                width: (parent.width  - 40) / 3
            }
        }

        ListView {
            id: listView
            anchors {
                top: listHead.bottom
                topMargin: 10
            }
            height: (parent.height - titleBar.height - listHead.height) - 50
            width: parent.width
            model: timetableResultModel
            delegate:  timetableResultDelegate
            clip: true

            visible: !searchIndicator.visible
        }

        ScrollDecorator {
            flickableItem: listView
        }
    }

    Component {
        id: timetableResultDelegate

        Item {
            id: delegateItem
            width: listView.width
            height: 40 + lbl_destination.height + lbl_station.height

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

            Item {
                anchors {
                    verticalCenter: parent.verticalCenter
                    top: parent.top
                    topMargin: 10
                }

                width: parent.width
                height: parent.height

                Label {
                    id: lbl_time
                    anchors {
                        left: parent.left
                        leftMargin: 10
                    }
                    text: time
                    width: (parent.width  - 40) / 4
                }

                Label {
                    id: lbl_type
                    anchors {
                        left: lbl_time.right
                        leftMargin: 10
                    }
                    text: trainType
                    width: (parent.width  - 40) / 4
                }

                Label {
                    id: lbl_destination
                    anchors {
                        left: lbl_type.right
                        leftMargin: 10
                        rightMargin: 10
                    }
                    horizontalAlignment: Text.AlignHCenter
                    text: destination
                    width: ((parent.width  - 40) / 4) * 2
                }

                Label {
                    id: lbl_station
                    anchors {
                        left: parent.left
                        leftMargin: 10
                        top: lbl_destination.bottom
                        topMargin: 5
                    }
                    text: stationplatform
                    width: parent.width - 40
                }
            }
        }
    }

    ListModel {
        id: timetableResultModel
    }

    Fahrplan.Backend {
        id: fahrplanBackend
        onParserTimeTableResult: {
            console.log("Got results");
            console.log(result.count);

            searchIndicatorVisible = false;

            timetableResultModel.clear();
            for (var i = 0; i < result.count; i++) {
                var item = result.getItem(i);

                var stationplatform = item.stationName;

                if (item.platform) {
                    stationplatform = "Platform: " + item.platform + " / " + item.stationName;
                }

                timetableResultModel.append({
                    "time": item.time,
                    "trainType": item.trainType,
                    "destination": item.destinationName,
                    "stationplatform": stationplatform,
                    "itemNum" : i
                });
            }
        }
    }

    ToolBarLayout {
        id: timetableResultsToolbar

        ToolIcon {
            id : backIcon;
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }
    }
}
