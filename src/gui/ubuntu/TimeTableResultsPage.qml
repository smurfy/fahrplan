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
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import "components"

Page {
    id: searchResultsPage

    property alias searchIndicatorVisible: searchIndicator.visible
    property int selMode : 0

    title: fahrplanBackend.mode === FahrplanBackend.ArrivalMode ? qsTr("Arrivals") : qsTr("Departures")

    head.backAction: Action {
        iconName: "back"
        onTriggered: {
            mainStack.pop()
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
        clip: true
        anchors.fill: parent
        model: fahrplanBackend.timetable
        delegate:  timetableResultDelegate
    }

    Component {
        id: timetableResultDelegate

        ListItem {
            id: delegateItem

            width: listView.width
            height: units.gu(2) + mainColumn.height
            highlightColor: "Transparent"

            Column {
                id: mainColumn

                anchors { verticalCenter: parent.verticalCenter; left: parent.left; right: parent.right; leftMargin: units.gu(2); rightMargin: units.gu(2) }

                RowLayout {
                    id: mainLayout

                    spacing: units.gu(1)
                    height: Math.max(leftColumn.height, rightColumn.height)
                    width: parent.width

                    Column {
                        id: leftColumn

                        Layout.preferredWidth: units.gu(8)
                        anchors.verticalCenter: parent.verticalCenter

                        Label {
                            id: lbl_time
                            text: Qt.formatTime(model.time, Qt.DefaultLocaleShortDate)
                            font.bold: true
                        }

                        Label {
                            id: lbl_type
                            text: model.trainType
                            fontSize: "x-small"
                        }
                    }

                    Column {
                        id: rightColumn

                        Layout.fillWidth: true
                        anchors.verticalCenter: parent.verticalCenter

                        Label {
                            id: lbl_destination
                            text: model.destinationStation
                            elide: Text.ElideRight
                            font.bold: true
                            horizontalAlignment: Text.AlignLeft
                            width: parent.width
                        }

                        Label {
                            id: lbl_station
                            text: {
                                var platform;
                                if (model.platform) {
                                    platform = qsTr("Pl. %1").arg(model.platform);
                                    if (model.currentStation) {
                                        platform += " / " + model.currentStation;
                                    }
                                } else {
                                    platform = model.currentStation;
                                }
                                return platform;
                            }
                            width: parent.width
                            fontSize: "x-small"
                            visible: text !== ""
                        }
                    }
                }

                Label {
                    id: lbl_miscinfo
                    visible: miscInfo !== ""
                    text: miscInfo
                    width: parent.width
                    fontSize: "small"
                    elide: Text.ElideRight
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    font.italic: true
                    textFormat: Text.RichText
                }
            }
        }
    }

    ListModel {
        id: timetableResultModel
    }

    Connections {
        target: fahrplanBackend

        onParserTimeTableResult: {
            console.log("Got results");
            searchIndicator.running = false;
        }
    }
}
