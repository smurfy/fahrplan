import Fahrplan 1.0
import QtQuick 1.1
import com.nokia.extras 1.0
import com.meego 1.0
import "components"

Page {
    id: searchDetailResultsPage

    property alias titleText: journeyStations.text
    property alias subTitleText: lbljourneyDate.text
    property alias subTitleText2: lbljourneyDuration.text
    property alias searchIndicatorVisible: searchIndicator.visible

    property JourneyDetailResultList currentResult;

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
                    right: parent.right
                    rightMargin: 10
                    top: parent.top
                    topMargin: 20
                }
                width: parent.width
            }

            Item {
                id: journeyDate
                anchors {
                    left: parent.left
                    leftMargin: 10
                    right: parent.right
                    rightMargin: 10
                    top: journeyStations.bottom
                    topMargin: 10
                }
                width: parent.width
                height: lbljourneyDate.height

                Label {
                    id: lbljourneyDate
                    anchors {
                        left: parent.left
                        top: parent.top
                    }
                    width: ((parent.width / 3) * 2) - 20
                    color: "Grey"
                }

                Label {
                    id: lbljourneyDuration
                    anchors {
                        right: parent.right
                        left: lbljourneyDate.right
                        top: parent.top
                    }
                    width: (parent.width / 3) - 20
                    color: "Grey"
                    horizontalAlignment: Text.AlignRight
                }
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
            visible: !searchIndicator.visible
            height: (parent.height - titleBar.height) - 20
            width: parent.width
            model: journeyDetailResultModel
            delegate:  journeyDetailResultDelegate
            clip: true
        }

        ScrollDecorator {
            flickableItem: listView
        }
    }

    Component {
        id: journeyDetailResultDelegate

        Item {
            id: delegateItem
            width: listView.width
            height: isStation ? isTrain ? item_train.height + item_station.height : item_station.height : isTrain ? item_train.height : 0

            Item {
                anchors {
                    verticalCenter: parent.verticalCenter
                }

                width: parent.width
                height: parent.height

                Item {
                     id: item_station

                     visible: isStation
                     height: visible ? lbl_station.height + 20 : 0

                     width: parent.width

                     Rectangle {
                         anchors {
                             fill: parent
                         }
                         color: "White"
                     }

                     Rectangle {
                         anchors {
                             left: parent.left
                             leftMargin: 81
                             top: parent.top
                             topMargin: (isStart) ? parent.height / 2 : 0
                         }
                         color: "#0d70c5"
                         height: (isStart || isStop) ? parent.height / 2  : parent.height
                         width: 8
                     }

                     Rectangle {
                         anchors {
                             left: parent.left
                             leftMargin: 70
                             verticalCenter: parent.verticalCenter
                         }

                         gradient: Gradient {
                             GradientStop {
                                 position: 0.00;
                                 color: "#0d70c5";
                             }
                             GradientStop {
                                 position: 0.38;
                                 color: "#0d70c5";
                             }
                             GradientStop {
                                 position: 0.39;
                                 color: "White";
                             }
                             GradientStop {
                                 position: 0.50;
                                 color: "White";
                             }
                             GradientStop {
                                 position: 0.61;
                                 color: "White";
                             }
                             GradientStop {
                                 position: 0.62;
                                 color: "#0d70c5";
                             }
                             GradientStop {
                                 position: 1.0;
                                 color: "#0d70c5";
                             }
                         }
                         radius: 15
                         height: 30
                         width: 30
                     }

                     Label {
                         anchors {
                             left: parent.left
                             leftMargin: 8
                             top: parent.top
                         }

                         text: arrivalTime
                         width: parent.width - 10
                     }

                     Label {

                         anchors {
                             left: parent.left
                             leftMargin: 8
                             bottom: parent.bottom
                         }

                         text: departureTime
                         width: parent.width - 10
                     }

                     Item {
                        id: lbl_station

                        height: lbl_station_name.height + lbl_station_info.height
                        width: (parent.width - 110)

                        anchors {
                            left: parent.left
                            leftMargin: 110
                            verticalCenter: parent.verticalCenter
                        }

                        Label {
                            id: lbl_station_name
                            text: stationName
                            width: parent.width
                        }

                        Label {

                            anchors {
                                top: lbl_station_name.bottom
                            }
                            color: "DarkGrey"
                            id: lbl_station_info
                            text: stationInfo
                            width: parent.width
                        }
                     }
                }

                Item {
                    id: item_train

                    anchors {
                        top: item_station.bottom
                    }

                    visible: isTrain
                    height: visible ? lbl_train.height + 30 : 0

                    width: parent.width

                    Rectangle {
                        anchors {
                            fill: parent
                        }
                        color: "LightGrey"
                    }

                    Rectangle {
                        anchors {
                            left: parent.left
                            leftMargin: 81
                        }
                        color: "#0d70c5"
                        height: parent.height
                        width: 8
                    }

                    Label {
                        id: lbl_train
                        anchors {
                            left: parent.left
                            leftMargin: 110
                            verticalCenter: parent.verticalCenter
                        }
                        text: trainName
                        font.bold: true
                        width: (parent.width  - 110)
                    }
                }
            }
        }
    }

    ListModel {
        id: journeyDetailResultModel
    }

    FahrplanBackend {
        id: fahrplanBackend

        onParserJourneyDetailsResult: {
            currentResult = result;
            console.log("Got detail results");
            console.log(result.count);

            if (result.count > 0) {
                journeyStations.text = result.departureStation + " to " + result.arrivalStation;
                var departureDate = Qt.formatDate(result.departureDateTime);
                var arrivalDate = Qt.formatDate(result.arrivalDateTime);

                if (departureDate == arrivalDate) {
                    arrivalDate = "";
                }

                lbljourneyDate.text = departureDate + " " + Qt.formatTime(result.departureDateTime,"hh:mm") + " - " +
                        arrivalDate + " " + Qt.formatTime(result.arrivalDateTime,"hh:mm");

                lbljourneyDuration.text = "Dur.: " + result.duration;

                journeyDetailResultModel.clear();
                for (var i = 0; i < result.count; i++) {
                    var item = result.getItem(i);

                    var nextItem = null;
                    if (i < result.count -1) {
                        nextItem = result.getItem(i+1);
                    }


                    var isStart = (i == 0);
                    var isStop = (i == result.count -1);

                    //Add first departure Station and the train
                    if (isStart) {
                        journeyDetailResultModel.append({
                                                            "isStart" : true,
                                                            "isStop" : false,
                                                            "trainName" : item.train + " " + item.info,
                                                            "stationName" : item.departureStation,
                                                            "stationInfo" : item.departureInfo,
                                                            "arrivalTime" : "",
                                                            "departureTime" : Qt.formatTime(item.departureDateTime,"hh:mm"),
                                                            "isStation" : true,
                                                            "isTrain" : true

                        });
                    }

                    //Add arrival station, but only if its the last item or if the next departureStation differs
                    //from arrival station
                    if (isStop || (nextItem && nextItem.departureStation != item.arrivalStation)) {
                        journeyDetailResultModel.append({
                                                            "isStart" : false,
                                                            "isStop" : isStop,
                                                            "trainName" : "",
                                                            "stationName" : item.arrivalStation,
                                                            "stationInfo" :  item.arrivalInfo,
                                                            "arrivalTime" :  Qt.formatTime(item.arrivalDateTime,"hh:mm"),
                                                            "departureTime" : "",
                                                            "isStation" : true,
                                                            "isTrain" : false

                        });
                    }

                    //Add one Station
                    if ((nextItem && nextItem.departureStation == item.arrivalStation)) {

                        var stationInfo = item.arrivalInfo;

                        if (stationInfo.length > 0 && nextItem.departureInfo) {
                            stationInfo = stationInfo + " / ";
                        }

                        stationInfo = stationInfo + nextItem.departureInfo;

                        journeyDetailResultModel.append({
                                                            "isStart" : false,
                                                            "isStop" : false,
                                                            "trainName" :  nextItem.train + " " + nextItem.info,
                                                            "stationName" : item.arrivalStation,
                                                            "stationInfo" : stationInfo,
                                                            "arrivalTime" : Qt.formatTime(item.arrivalDateTime,"hh:mm"),
                                                            "departureTime" :  Qt.formatTime(nextItem.departureDateTime,"hh:mm"),
                                                            "isStation" : true,
                                                            "isTrain" : true

                        });
                    }

                }
                searchIndicator.visible = false;
            } else {
                pageStack.pop();
            }
        }
    }

    InfoBanner {
            id: banner
            objectName: "FahrplanCalendarInfoBanner"
            text: ""
            anchors.top: parent.top
            anchors.topMargin: 10
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

        ToolIcon {
            id : calendarIcon;
            iconId: enabled ? "toolbar-list" : ""
            visible: !searchIndicator.visible

            onClicked: {
                calendarIcon.enabled = false
                fahrplanBackend.addJourneyDetailResultToCalendar(currentResult);
            }

            Connections {
                target: fahrplanBackend
                onCalendarEntryAdded: {
                                          if (success)
                                              banner.text = "Journey has been added to your calendar.";
                                          else
                                              banner.text = "Failed to add Journey to your calendar!";

                                          banner.show();
                                          calendarIcon.enabled = true
                                      }
            }

            BusyIndicator {
                visible: !calendarIcon.enabled
                running: true
                style: BusyIndicatorStyle { size: "small" }
                anchors.centerIn: parent
            }
        }
    }
}
