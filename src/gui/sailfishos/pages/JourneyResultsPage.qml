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

import QtQuick 2.0
import Sailfish.Silica 1.0
import Fahrplan 1.0
import "../delegates"

Page {
    id: journeyResultsPage

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height
        contentWidth: parent.width

        VerticalScrollDecorator {}

        PullDownMenu {
            enabled: listView.visible
            MenuItem {
                text: qsTr("Earlier")
                enabled: listView.visible
                onClicked: {
                    indicator.visible = true;
                    fahrplanBackend.parser.searchJourneyEarlier();
                }
            }
        }

        PushUpMenu {
            enabled: listView.visible
            MenuItem {
                text: qsTr("Later")
                onClicked: {
                    indicator.visible = true;
                    fahrplanBackend.parser.searchJourneyLater();
                }
            }
        }

        Column {
            id: column
            spacing: Theme.paddingLarge
            width: parent.width

            PageHeader {
                id: journeyDesc
                Label {
                    id: journeyDate
                    width: parent.width
                    horizontalAlignment: Text.AlignRight
                    color: Theme.secondaryColor
                    anchors {
                        top: parent.top
                        topMargin: Theme.paddingLarge * 3
                        right: parent.right
                        rightMargin: Theme.paddingMedium
                    }
                }
            }

            Row {
                id: listHead
                width: parent.width
                visible: listView.visible

                anchors {
                    leftMargin: Theme.paddingMedium
                    rightMargin: Theme.paddingMedium
                    left: parent.left
                    right: parent.right
                }

                Label {
                    text: qsTr("Dep.")
                    width: (parent.width - 3) / 4
                }

                Label {
                    text: qsTr("Arr.")
                    width: (parent.width - 3) / 4
                }

                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Dur.")
                    width: (parent.width - 3) / 4
                }

                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Trans.")
                    width: (parent.width - 3) / 4
                }
            }

            ListView {
                id: listView
                width: parent.width
                height: contentHeight
                interactive: false
                visible: !indicator.visible && !errorMsg.visible

                model: journeyResultModel

                delegate: JourneyDelegate {
                    onClicked: {
                        pageStack.push(detailsResultsPage);
                        fahrplanBackend.parser.getJourneyDetails(model.id);
                    }
                }
            }
        }
    }

    Label {
        id: errorMsg
        anchors {
            centerIn: parent
            left: parent.left
            right: parent.right
            leftMargin: Theme.paddingMedium
            rightMargin: Theme.paddingMedium
        }
        horizontalAlignment: Text.AlignHCenter
        visible: false
        width: parent.width
        color: Theme.highlightColor
        font.family: Theme.fontFamilyHeading
        wrapMode: Text.WordWrap
    }


    onStatusChanged: {
        switch (status) {
            case PageStatus.Activating:
                if (pageStack.depth === 2) {
                    indicator.visible = true;
                    errorMsg.visible = false;
                    journeyDesc.title = qsTr("Searching...");
                    journeyDate.text = "";
                    fahrplanBackend.searchJourney();
                }
                break;
            case PageStatus.Deactivating:
                if (pageStack.depth === 1) {
                    indicator.visible = true;
                    errorMsg.visible = false;
                    fahrplanBackend.parser.cancelRequest();
                }
                break;
        }
    }

    BusyIndicator {
        id: indicator
        anchors.centerIn: parent
        running: true
        size: BusyIndicatorSize.Large
    }

    ListModel {
        id: journeyResultModel
    }

    Connections {
        target: fahrplanBackend

        onParserJourneyResult: {
            console.log("Got results");
            console.log(result.count);
            indicator.visible = false;

            journeyDesc.title = result.viaStation.length == 0 ? qsTr("<b>%1</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation) : qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation).arg(result.viaStation)

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

        onParserErrorOccured: {
            console.log("Got error")
            console.log(msg)
            errorMsg.visible = true;
            indicator.visible = false;
            journeyDesc.title = qsTr("Error");
            errorMsg.text = msg;
        }
    }

    JourneyDetailsResultsPage {
        id: detailsResultsPage
    }
}
