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
import QtQuick 2.0
import Ubuntu.Components 0.1
import "components"

Page {
    title: qsTr("Results")
    property alias searchResults: searchResults
    property alias journeyStationsTitleText: journeyStations.text
    property alias searchIndicatorVisible: searchIndicator.visible

    id: searchResultsPage

    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        Item {
            id: titleBar

            height: journeyStations.height + journeyDate.height + units.gu(2)
            width: parent.width

            Label {
                id: journeyStations
                text: ""
                fontSize: "medium"
                anchors {
                    left: parent.left
                    leftMargin: units.gu(1)
                    right: parent.right
                    rightMargin: units.gu(1)
                    top: parent.top
                    topMargin: units.gu(1)
                }
                width: parent.width
            }

            Label {
                id: journeyDate
                color: "Grey"
                anchors {
                    left: parent.left
                    leftMargin: units.gu(1)
                    top: journeyStations.bottom
                    topMargin: units.gu(1)
                }
                width: parent.width
                visible: !searchIndicatorVisible
            }
        }

        ActivityIndicator {
            id: searchIndicator
            anchors {
                top: titleBar.bottom
                topMargin: units.gu(5);
                horizontalCenter: parent.horizontalCenter
            }
            running: true
            visible: false
        }

        Item {
            id: listHead

            width: parent.width;
            height: lbl_departuretime.height
            visible: !searchIndicator.visible

            anchors {
                top: titleBar.bottom
                topMargin: units.gu(1)
            }

            Label {
                id: lbl_departuretime
                anchors {
                    left: parent.left
                    leftMargin: units.gu(1)
                }
                text: qsTr("Dep.")
                width: (parent.width  - units.gu(4)) / 4
            }

            Label {
                id: lbl_arrivaltime
                anchors {
                    left: lbl_departuretime.right
                    leftMargin: units.gu(1)
                }
                text: qsTr("Arr.")
                width: (parent.width  - units.gu(4)) / 4
            }

            Label {
                id: lbl_duration
                anchors {
                    left: lbl_arrivaltime.right
                    leftMargin: units.gu(1)
                }
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Dur.")
                width: (parent.width  - units.gu(4)) / 4
            }

            Label {
                id: lbl_transfers
                anchors {
                    left: lbl_duration.right
                    leftMargin: units.gu(1)
                }
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Trans.")
                width: (parent.width  - units.gu(4)) / 4
            }
        }

        ListView {
            id: listView
            anchors {
                top: listHead.bottom
                left: parent.left
                right: parent.right
                bottom: buttonRow.top
            }
            model: journeyResultModel
            delegate:  journeyResultDelegate
            clip: true
            visible: !searchIndicator.visible
        }

        Scrollbar {
            flickableItem: listView
        }

        Row {
            id: buttonRow
            anchors {
                left: parent.left
                bottom: parent.bottom
                right: parent.right
                margins: units.gu(1)
            }
            spacing: units.gu(1)

            Button {
                text: qsTr("Earlier")
                width: (parent.width - parent.spacing) / 2
                onClicked: {
                    searchIndicatorVisible = true
                    fahrplanBackend.parser.searchJourneyEarlier()
                }
            }
            Button {
                text: qsTr("Later")
                width: (parent.width - parent.spacing) / 2
                onClicked: {
                    searchIndicatorVisible = true
                    fahrplanBackend.parser.searchJourneyLater()
                }
            }
        }
    }

    Component {
        id: journeyResultDelegate

        Item {
            id: delegateItem
            width: listView.width
            height: units.gu(3) + lbl_departuretime.height + lbl_traintyp.height + (lbl_miscinfo.visible ? lbl_miscinfo.height : 0)

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

                    var component = Qt.createComponent("JourneyDetailsResultsPage.qml")
                    pageStack.push(component,
                                   {titleText: qsTr("Loading details"), subTitleText: qsTr("please wait..."), searchIndicatorVisible: true});
                    fahrplanBackend.parser.getJourneyDetails(id);
                }
            }

            Item {
                anchors {
                    verticalCenter: parent.verticalCenter
                    top: parent.top
                    topMargin: units.gu(1)
                }

                width: parent.width
                height: parent.height

                Label {
                    id: lbl_departuretime
                    anchors {
                        left: parent.left
                        leftMargin: units.gu(1)
                    }
                    text: departureTime
                    width: (parent.width  - units.gu(4)) / 4
                }

                Label {
                    id: lbl_arrivaltime
                    anchors {
                        left: lbl_departuretime.right
                        leftMargin: units.gu(1)
                    }
                    text: arrivalTime
                    width: (parent.width  - units.gu(4)) / 4
                }

                Label {
                    id: lbl_duration
                    anchors {
                        left: lbl_arrivaltime.right
                        leftMargin: units.gu(1)
                    }
                    horizontalAlignment: Text.AlignHCenter
                    text: duration
                    width: (parent.width  - units.gu(4)) / 4
                }

                Label {
                    id: lbl_transfers
                    anchors {
                        left: lbl_duration.right
                        leftMargin: units.gu(1)
                    }
                    horizontalAlignment: Text.AlignHCenter
                    text: transfers
                    width: (parent.width  - units.gu(4)) / 4
                }

                Label {
                    id: lbl_traintyp
                    anchors {
                        left: parent.left
                        leftMargin: units.gu(1)
                        top: lbl_departuretime.bottom
                        topMargin: units.gu(0.5)
                    }
                    text: trainType
                    width: parent.width - units.gu(4)
                }

                Label {
                    id: lbl_miscinfo
                    anchors {
                        left: parent.left
                        leftMargin: units.gu(1)
                        top: lbl_traintyp.bottom
                        topMargin: units.gu(0.5)
                    }
                    visible: (miscInfo == "") ? false : true
                    text: miscInfo
                    width: parent.width - units.gu(4)
                    wrapMode: Text.WordWrap
                    font.bold: true
                }

            }
        }
    }

    ListModel {
        id: journeyResultModel
    }

    Connections {
        target: fahrplanBackend
        onParserJourneyResult: {
            console.log("Got results");
            console.log(result.count);

            searchIndicatorVisible = false;

            journeyStations.text = result.viaStation.length == 0 ? qsTr("<b>%1</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation) : qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation).arg(result.viaStation)

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



//    ToolBarLayout {
//        id: journeyResultsToolbar

//        ToolIcon {
//            id : backIcon;
//            iconId: "toolbar-back"
//            onClicked: {
//                pageStack.pop();
//                fahrplanBackend.parser.cancelRequest();
//            }
//        }

//        ToolButtonRow {
//            ToolButton {
//                text:qsTr("Earlier")
//                visible: !searchIndicatorVisible;
//                onClicked: {
//                    searchIndicatorVisible = true
//                    fahrplanBackend.parser.searchJourneyEarlier();
//                }
//            }
//            ToolButton {
//                text:qsTr("Later")
//                visible: !searchIndicatorVisible;
//                onClicked: {
//                    searchIndicatorVisible = true
//                    fahrplanBackend.parser.searchJourneyLater();
//                }
//            }
//        }
//    }
}
