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
import com.nokia.extras 1.1
import "../delegates"

Page {
    property alias searchIndicatorVisible: searchIndicator.visible

    id: searchResultsPage

    tools: timetableResultsToolbar
    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        Item {
            id: titleBar

            width: parent.width
            height: timetableTitle.height + platformStyle.paddingMedium

            Label {
                id: timetableTitle
                text: fahrplanBackend.mode === FahrplanBackend.ArrivalMode ? qsTr("Arrivals") : qsTr("Departures")
                font.bold: true
                font.pixelSize: privateStyle.statusBarHeight
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

        ListView {
            id: listView

            model: timetableResultModel
            clip: true
            visible: !searchIndicator.visible
            anchors {
                top: titleBar.bottom
                topMargin: platformStyle.paddingMedium
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            delegate: TimetableEntryDelegate {}
        }

        ScrollDecorator {
            flickableItem: listView
            platformInverted: appWindow.platformInverted
        }
    }

    ListModel {
        id: timetableResultModel
    }

    Connections {
        target: fahrplanBackend

        onParserErrorOccured: {
            banner.text = msg;
            banner.open();
        }

        onParserTimeTableResult: {
            console.log("Got results");
            console.log(result.count);

            searchIndicatorVisible = false;

            timetableResultModel.clear();
            for (var i = 0; i < result.count; i++) {
                var item = result.getItem(i);

                var stationplatform = item.stationName;

                if (item.platform) {
                    stationplatform = qsTr("Pl.") + " " + item.platform;
                    if (item.stationName) {
                        stationplatform += " / " + item.stationName;
                    }
                }

                var dirlabel = "";
                if (fahrplanBackend.mode === FahrplanBackend.ArrivalMode)
                    dirlabel = qsTr("from <b>%1</b>").arg(item.destinationName);
                else
                    dirlabel = qsTr("to <b>%1</b>").arg(item.destinationName);

                timetableResultModel.append({
                    "time": item.time,
                    "trainType": item.trainType,
                    "destination": dirlabel,
                    "stationplatform": stationplatform,
                    "miscInfo": item.miscInfo
                });
            }
        }
    }

    ToolBarLayout {
        id: timetableResultsToolbar

        ToolButton {
            id : backIcon;
            iconSource: "toolbar-back"
            platformInverted: appWindow.platformInverted
            onClicked: {
                pageStack.pop();
                fahrplanBackend.parser.cancelRequest();
            }
        }
    }
}
