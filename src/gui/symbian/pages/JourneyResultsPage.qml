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
import com.nokia.symbian 1.1
import "../delegates"
import "../js/style.js" as Style

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

            height: journeyStations.height + journeyDate.height + 2 * platformStyle.paddingMedium
            width: parent.width

            Label {
                id: journeyStations

                width: parent.width
                font.pixelSize: privateStyle.statusBarHeight
                text: {
                    if (fahrplanBackend.viaStationName == qsTr("please select")) {
                        return qsTr("<b>%1</b> to <b>%2</b>").arg(fahrplanBackend.departureStationName).arg(fahrplanBackend.arrivalStationName);
                    } else {
                        return qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(fahrplanBackend.departureStationName).arg(fahrplanBackend.arrivalStationName).arg(fahrplanBackend.viaStationName);
                    }
                }
                wrapMode: Text.WordWrap
                platformInverted: appWindow.platformInverted
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingMedium
                    right: parent.right
                    rightMargin: platformStyle.paddingMedium
                    top: parent.top
                    topMargin: platformStyle.paddingMedium
                }
            }

            Label {
                id: journeyDate
                color: platformInverted ? platformStyle.colorNormalMidInverted : platformStyle.colorNormalMid
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingMedium
                    top: journeyStations.bottom
                    topMargin: platformStyle.paddingMedium
                }
                width: parent.width
                visible: !searchIndicatorVisible
                platformInverted: appWindow.platformInverted
            }
        }

        BusyIndicator {
            id: searchIndicator
            anchors.centerIn: parent
            running: true
            visible: false

            width: platformStyle.graphicSizeLarge; height: width
            platformInverted: appWindow.platformInverted
        }

        Row {
            id: listHead

            height: childrenRect.height
            visible: !searchIndicator.visible
            spacing: platformStyle.paddingMedium

            anchors {
                top: titleBar.bottom
                topMargin: platformStyle.paddingMedium
                left: parent.left
                leftMargin: platformStyle.paddingMedium
                right: parent.right
                rightMargin: platformStyle.paddingMedium
            }

            Label {
                text: qsTr("Dep.")
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }

            Label {
                text: qsTr("Arr.")
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }

            Label {
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Dur.")
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }

            Label {
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Trans.")
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }
        }

        ListView {
            id: listView

            model: journeyResultModel
            clip: true
            visible: !searchIndicator.visible
            anchors {
                top: listHead.bottom
                topMargin: platformStyle.paddingMedium
                left: parent.left
                right: parent.right
                bottom: parent.bottom
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
            platformInverted: appWindow.platformInverted
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

    JourneyDetailsResultsPage {
        id: detailsResultsPage
    }

    ToolBarLayout {
        id: journeyResultsToolbar

        ToolButton {
            id : backIcon;
            iconSource: "toolbar-back"
            platformInverted: appWindow.platformInverted
            onClicked: {
                pageStack.pop();
                fahrplanBackend.parser.cancelRequest();
            }
        }

        ToolButton {
            text:qsTr("Earlier")
            visible: !searchIndicatorVisible
            platformInverted: appWindow.platformInverted
            onClicked: {
                searchIndicatorVisible = true;
                fahrplanBackend.parser.searchJourneyEarlier();
            }
        }
        ToolButton {
            text:qsTr("Later")
            visible: !searchIndicatorVisible
            platformInverted: appWindow.platformInverted
            onClicked: {
                searchIndicatorVisible = true;
                fahrplanBackend.parser.searchJourneyLater();
            }
        }
    }
}
