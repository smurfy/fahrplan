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
import com.nokia.meego 1.1
import "../delegates"

Page {
    property alias searchResults: searchResults
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

                width: parent.width
                font.pixelSize: 30
                text: {
                    if (fahrplanBackend.viaStationName == qsTr("please select")) {
                        return qsTr("<b>%1</b> to <b>%2</b>").arg(fahrplanBackend.departureStationName).arg(fahrplanBackend.arrivalStationName);
                    } else {
                        return qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(fahrplanBackend.departureStationName).arg(fahrplanBackend.arrivalStationName).arg(fahrplanBackend.viaStationName);
                    }
                }
                anchors {
                    left: parent.left
                    leftMargin: 10
                    right: parent.right
                    rightMargin: 10
                    top: parent.top
                    topMargin: 10
                }
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

            width: parent.width
            height: (parent.height - titleBar.height - listHead.height) - 10
            model: journeyResultModel
            clip: true
            visible: !searchIndicator.visible
            anchors {
                top: listHead.bottom
                topMargin: 10
            }
            delegate: JourneyDelegate {
                onClicked: {
                    detailsResultsPage.titleText = qsTr("Loading details");
                    detailsResultsPage.subTitleText = qsTr("please wait...");
                    detailsResultsPage.subTitleText2 = "";
                    detailsResultsPage.searchIndicatorVisible = true;
                    pageStack.push(detailsResultsPage);
                    fahrplanBackend.parser.getJourneyDetails(model.id);
                }
            }
        }

        ScrollDecorator {
            flickableItem: listView
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
                    "miscInfo": item.miscInfo
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

    JourneyDetailsResultsPage {
        id: detailsResultsPage
    }
}
