/****************************************************************************
**
**  This file is a part of Fahrplan.
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

import Fahrplan 1.0
import QtQuick 1.1
import com.nokia.extras 1.1
import com.nokia.meego 1.1
import "../delegates"

Page {
    id: searchDetailResultsPage

    property alias titleText: journeyStations.text
    property alias subTitleText: lbljourneyDate.text
    property alias subTitleText2: lbljourneyDuration.text
    property alias searchIndicatorVisible: searchIndicator.visible
    property string inverseSuffix: theme.inverted ? "-inverse" : ""

    property JourneyDetailResultList currentResult;

    tools: journeyDetailResultsToolbar
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

            width: parent.width
            height: (parent.height - titleBar.height) - 20
            model: journeyDetailResultModel
            clip: true
            visible: !searchIndicator.visible
            anchors {
                top: titleBar.bottom
                topMargin: 10
            }
            delegate: JourneyDetailsDelegate {}
        }

        ScrollDecorator {
            flickableItem: listView
        }
    }

    ListModel {
        id: journeyDetailResultModel
    }

    Connections {
        target: fahrplanBackend

        onAddCalendarEntryComplete: {
            if (success)
                banner.text = qsTr("Journey has been added to your calendar.");
            else
                banner.text = qsTr("Failed to add Journey to your calendar!");
            banner.show();
            calendarIcon.enabled = true;
        }

        onParserJourneyDetailsResult: {
            currentResult = result;
            console.log("Got detail results");
            console.log(result.count);

            if (result.count > 0) {
                journeyStations.text = result.viaStation.length == 0 ? qsTr("<b>%1</b> to <b>%2</b>").arg(result.departureStation.name).arg(result.arrivalStation.name) : qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(result.departureStation.name).arg(result.arrivalStation.name).arg(result.viaStation.name);
                var departureDate = Qt.formatDate(result.departureDateTime);
                var arrivalDate = Qt.formatDate(result.arrivalDateTime);

                if (departureDate == arrivalDate) {
                    arrivalDate = "";
                }

                lbljourneyDate.text = departureDate + " " + Qt.formatTime(result.departureDateTime, qsTr("hh:mm")) + " - " +
                        arrivalDate + " " + Qt.formatTime(result.arrivalDateTime, qsTr("hh:mm"));

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
                                                            "stationName" : item.departureStation.name,
                                                            "stationInfo" : item.departureInfo,
                                                            "arrivalTime" : "",
                                                            "departureTime" : Qt.formatTime(item.departureDateTime, qsTr("hh:mm")),
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
                                                            "stationName" : item.arrivalStation.name,
                                                            "stationInfo" :  item.arrivalInfo,
                                                            "arrivalTime" :  Qt.formatTime(item.arrivalDateTime, qsTr("hh:mm")),
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
                                                            "stationName" : item.arrivalStation.name,
                                                            "stationInfo" : stationInfo,
                                                            "arrivalTime" : Qt.formatTime(item.arrivalDateTime, qsTr("hh:mm")),
                                                            "departureTime" :  Qt.formatTime(nextItem.departureDateTime, qsTr("hh:mm")),
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

        ToolIcon {
            id : backIcon;
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }

        ToolIcon {
            id : calendarIcon
            iconSource: enabled ? "qrc:/src/gui/harmattan/icon/icon-m-content-calendar" + inverseSuffix + ".svg" : "image://theme/progress_78_01"

            visible: !searchIndicator.visible

            onClicked: {
                calendarIcon.enabled = false
                fahrplanBackend.addJourneyDetailResultToCalendar(currentResult);
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
