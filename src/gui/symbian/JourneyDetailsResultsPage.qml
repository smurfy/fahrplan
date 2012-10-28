import Fahrplan 1.0
import QtQuick 1.1
import com.nokia.extras 1.1
import com.nokia.symbian 1.1
import "components"
import "js/style.js" as Style

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

            height: journeyStations.height + journeyDate.height + 2 * platformStyle.paddingMedium
            width: parent.width

            Label {
                id: journeyStations
                text: ""
                font.pixelSize: 26
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingMedium
                    right: parent.right
                    rightMargin: platformStyle.paddingMedium
                    top: parent.top
                    topMargin: platformStyle.paddingMedium
                }
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Item {
                id: journeyDate
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingMedium
                    right: parent.right
                    rightMargin: platformStyle.paddingMedium
                    top: journeyStations.bottom
                    topMargin: platformStyle.paddingMedium
                }
                width: parent.width
                height: lbljourneyDate.height

                Label {
                    id: lbljourneyDate
                    anchors {
                        top: parent.top
                        right: lbljourneyDuration.left
                        rightMargin: platformStyle.paddingMedium
                        left: parent.left
                    }
                    color: platformStyle.colorNormalMid
                    wrapMode: Text.WordWrap
                }

                Label {
                    id: lbljourneyDuration
                    anchors {
                        right: parent.right
                        top: parent.top
                    }
                    color: platformStyle.colorNormalMid
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        BusyIndicator {
            id: searchIndicator
            anchors.centerIn: parent
            running: true
            visible: false

            width: platformStyle.graphicSizeLarge; height: width
        }

        ListView {
            id: listView
            anchors {
                top: titleBar.bottom
                topMargin: platformStyle.paddingMedium
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            visible: !searchIndicator.visible
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

                Rectangle {
                     id: item_station

                     height: visible ? lbl_station.height + 2 * platformStyle.paddingMedium : 0
                     color: Style.listBackgroundOdd
                     visible: isStation

                     anchors {
                         left: parent.left
                         right: parent.right
                     }

                     Item {
                         id: times

                         width: childrenRect.width
                         height: parent.height

                         anchors {
                             left: parent.left
                             leftMargin: platformStyle.paddingMedium
                         }

                         Label {
                             anchors {
                                 top: parent.top
                                 topMargin: platformStyle.paddingSmall
                                 left: parent.left
                             }

                             // This hack is needed to have correct padding for the line
                             text: arrivalTime != "" ? arrivalTime : "00:00"
                             color: item_station.color
                             platformInverted: appWindow.platformInverted
                         }

                         Label {
                             anchors {
                                 left: parent.left
                                 bottom: parent.bottom
                                 bottomMargin: platformStyle.paddingSmall
                             }

                             text: departureTime
                         }
                     }

                     Item {
                         id: circle

                         width: childrenRect.width
                         height: parent.height

                         anchors {
                             left: times.right
                             leftMargin: platformStyle.paddingMedium
                         }

                         Rectangle {
                             anchors {
                                 left: parent.left
                                 leftMargin: 11
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
                                     color: Style.listBackgroundOdd
                                 }
                                 GradientStop {
                                     position: 0.50;
                                     color: Style.listBackgroundOdd
                                 }
                                 GradientStop {
                                     position: 0.61;
                                     color: Style.listBackgroundOdd
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
                             smooth: true
                             height: 30
                             width: 30
                         }
                     }

                     Item {
                        id: lbl_station

                        height: childrenRect.height

                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: circle.right
                            leftMargin: platformStyle.paddingMedium
                            right: parent.right
                            rightMargin: platformStyle.paddingMedium
                        }

                        Label {
                            id: lbl_station_name
                            text: stationName
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }

                        Label {
                            anchors {
                                top: lbl_station_name.bottom
                                topMargin: platformStyle.paddingSmall
                            }
                            color: platformStyle.colorNormalMid
                            id: lbl_station_info
                            text: stationInfo
                            width: parent.width
                        }
                     }
                }

                Rectangle {
                    id: item_train

                    height: visible ? lbl_train.height + 30 : 0
                    color: Style.listBackgroundEven
                    visible: isTrain

                    anchors {
                        top: item_station.bottom
                        left: parent.left
                        right: parent.right
                    }

                    Rectangle {
                        anchors {
                            left: parent.left
                            leftMargin: times.width + (circle.width - width) / 2 + 2 * platformStyle.paddingMedium
                        }
                        color: "#0d70c5"
                        height: parent.height
                        width: 8
                    }

                    Label {
                        id: lbl_train
                        anchors {
                            left: parent.left
                            leftMargin: times.width + circle.width + 3 * platformStyle.paddingMedium
                            right: parent.right
                            rightMargin: platformStyle.paddingMedium
                            verticalCenter: parent.verticalCenter
                        }
                        text: trainName
                        font.bold: true
                        wrapMode: Text.WordWrap
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

        onAddCalendarEntryComplete: {
            if (success)
                banner.text = qsTr("Journey has been added to your calendar.");
            else
                banner.text = qsTr("Failed to add Journey to your calendar!");
            banner.open();
            calendarIcon.enabled = true
        }

        onParserJourneyDetailsResult: {
            currentResult = result;
            console.log("Got detail results");
            console.log(result.count);

            if (result.count > 0) {
                journeyStations.text = result.viaStation.length == 0 ? qsTr("<b>%1</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation) : qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation).arg(result.viaStation);
                var departureDate = Qt.formatDate(result.departureDateTime);
                var arrivalDate = Qt.formatDate(result.arrivalDateTime);

                if (departureDate == arrivalDate) {
                    arrivalDate = "";
                }

                lbljourneyDate.text = departureDate + " " + Qt.formatTime(result.departureDateTime,"hh:mm") + " - " +
                        arrivalDate + " " + Qt.formatTime(result.arrivalDateTime,"hh:mm");

                lbljourneyDuration.text = qsTr("Dur.: %1").arg(result.duration);

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

    ToolBarLayout {
        id: journeyDetailResultsToolbar

        ToolButton {
            id : backIcon;
            iconSource: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }

        ToolButton {
            id : calendarIcon;
            iconSource: enabled ? "qrc:/src/gui/symbian/icon/icon-m-content-calendar.png" : "qrc:/src/gui/symbian/icon/icon-m-blank.png"

            visible: !searchIndicator.visible

            onClicked: {
                calendarIcon.enabled = false
                fahrplanBackend.addJourneyDetailResultToCalendar(currentResult);
            }

            BusyIndicator {
                visible: !calendarIcon.enabled
                running: visible
                width: platformStyle.graphicSizeSmall; height: width
                anchors.centerIn: parent
            }
        }
    }
}
