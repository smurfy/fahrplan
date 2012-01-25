import Fahrplan 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "components"

Page {
    property alias timetableTitleText: timetableTitle.text
    property alias searchIndicatorVisible: searchIndicator.visible

    property int selMode : 0

    id: searchResultsPage

    tools: timetableResultsToolbar
    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        Item {
            id: titleBar

            height: timetableTitle.height + 30
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

        ListView {
            id: listView
            anchors {
                top: titleBar.bottom
                topMargin: 10
            }
            height: parent.height - titleBar.height - 20
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

                Grid {
                    columns: 2
                    spacing: 10

                    anchors {
                        leftMargin: 10
                        left: parent.left
                    }

                    width: parent.width
                    height: parent.height

                    Label {
                        id: lbl_time
                        text: time
                        font.bold: true
                        width: (parent.width  - 40) / 3
                    }

                    Label {
                        id: lbl_destination
                        text: destination
                        width: ((parent.width  - 40) / 3) * 2
                    }

                    Label {
                        id: lbl_type
                        text: trainType
                        font.bold: true
                        width: (parent.width  - 40) / 3
                    }

                    Label {
                        id: lbl_station
                        text: stationplatform
                        width: ((parent.width  - 40) / 3) * 2
                    }
                }


            }
        }
    }

    ListModel {
        id: timetableResultModel
    }

    FahrplanBackend {
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
                    stationplatform = "Pl. " + item.platform + " / " + item.stationName;
                }

                var dirlabel = "";
                if (selMode == 1) {
                    dirlabel = "to "
                }

                if (selMode == 0) {
                    dirlabel = "from "
                }

                timetableResultModel.append({
                    "time": Qt.formatTime( item.time,"hh:mm"),
                    "trainType": item.trainType,
                    "destination": dirlabel + item.destinationName,
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
