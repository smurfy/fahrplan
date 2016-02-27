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
import QtQuick 2.4
import Ubuntu.Components 1.3

Page {
    id: searchResultsPage

    title: qsTr("Journey alternatives")

    property string journeyStationsTitleText
    property string journeryDateTitleText
    property alias searchIndicatorVisible: searchIndicator.visible

    head.backAction: Action {
        iconName: "back"
        onTriggered: {
            mainStack.pop();
            fahrplanBackend.parser.cancelRequest();
        }
    }

    ActivityIndicator {
        id: searchIndicator
        anchors.centerIn: parent
        running: true
        visible: false
    }

    ListView {
        id: listView

        anchors.fill: parent
        delegate: journeyResultDelegate
        model: journeyResultModel
        visible: !searchIndicator.visible

        header: Column {
            id: headerColumn
            width: parent.width
            spacing: units.gu(1)
            Item {
                id: titleBar

                height: journeyStations.height + journeyDate.height + units.gu(2)
                anchors { left: parent.left; right: parent.right; margins: units.gu(2) }

                Label {
                    id: journeyStations
                    text: journeyStationsTitleText
                    fontSize: "large"
                    wrapMode: Text.WordWrap
                    width: parent.width
                    anchors { top: parent.top; topMargin: units.gu(1) }
                }

                Label {
                    id: journeyDate
                    color: "Grey"
                    fontSize: "small"
                    text: journeryDateTitleText
                    width: parent.width
                    anchors { top: journeyStations.bottom }
                }
            }

            Rectangle {
                width: parent.width
                height: headerLabel.implicitHeight + units.gu (3)
                color: mouseArea.pressed ? "DarkGrey" : "#F5F5F5"
                Label {
                    id: headerLabel
                    width: parent.width
                    font.bold: true
                    text: qsTr("↥ View earlier options")
                    horizontalAlignment: Text.AlignHCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        searchIndicatorVisible = true
                        fahrplanBackend.parser.searchJourneyEarlier()
                    }
                }
            }
        }

        footer: Rectangle {
            width: parent.width
            height: footerLabel.implicitHeight + units.gu (3)
            color: footerMouseArea.pressed ? "DarkGrey" : "#F5F5F5"
            Label {
                id: footerLabel
                width: parent.width
                font.bold: true
                text: qsTr("↧ View later options")
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea {
                id: footerMouseArea
                anchors.fill: parent
                onClicked: {
                    searchIndicatorVisible = true
                    fahrplanBackend.parser.searchJourneyLater()
                }
            }
        }
    }

    Scrollbar {
        flickableItem: listView
    }

    Component {
        id: journeyResultDelegate

        Item {
            id: delegateItem

            width: listView.width
            height: detailsColumn.height + units.gu(2)

            Rectangle {
                id: background
                anchors.fill: parent
                color: mouseArea.pressed ? "DarkGrey" : itemNum % 2 ? "#F5F5F5" : "#ECECEC"
            }

            MouseArea {
                id: mouseArea
                anchors.fill: background
                onClicked: {
                    var component = Qt.createComponent("JourneyDetailsResultsPage.qml")
                    mainStack.push(component,
                                   {titleText: qsTr("Loading details"), subTitleText: qsTr("please wait..."), searchIndicatorVisible: true});
                    fahrplanBackend.parser.getJourneyDetails(id);
                }
            }

            Column {
                id: detailsColumn

                anchors { left: parent.left; right: parent.right; margins: units.gu(2); verticalCenter: parent.verticalCenter }

                Label {
                    id: lbl_time
                    width: parent.width
                    font.bold: true
                    text: departureTime + " ↦ " + arrivalTime
                }

                Label {
                    id: lbl_detail
                    width: parent.width
                    elide: Text.ElideRight
                    fontSize: "x-small"
                    color: "Grey"
                    text: qsTr("Duration: %1 | Transfer: %2").arg(duration).arg(transfers)
                }

                Label {
                    id: lbl_trainType
                    width: parent.width
                    elide: Text.ElideRight
                    fontSize: "x-small"
                    color: "Grey"
                    text: trainType
                }

                Label {
                    id: lbl_miscInfo
                    width: parent.width
                    wrapMode: Text.WordWrap
                    visible: (miscInfo == "") ? false : true
                    text: miscInfo
                    fontSize: "small"
                    color: UbuntuColors.red
                    font.italic: true
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

            journeyStationsTitleText = result.viaStation.length == 0 ? qsTr("<b>%1</b> ↦ <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation) : qsTr("<b>%1</b> ↦ <b>%3</b> ↦ <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation).arg(result.viaStation)

            journeryDateTitleText = result.timeInfo;

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
}
