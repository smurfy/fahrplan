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
import "components"
import "js/style.js" as Style

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
            anchors {
                top: titleBar.bottom
                topMargin: platformStyle.paddingMedium
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            model: timetableResultModel
            delegate: timetableResultDelegate
            clip: true
            visible: !searchIndicator.visible
        }

        ScrollDecorator {
            flickableItem: listView
            platformInverted: appWindow.platformInverted
        }
    }

    Component {
        id: timetableResultDelegate

        Rectangle {
            id: delegateItem

            width: listView.width
            height: grid.height + 2 * platformStyle.paddingMedium
            color: {
                if (appWindow.platformInverted)
                    return itemNum % 2 ? Style.listBackgroundOddInverted : Style.listBackgroundEvenInverted;
                else
                    return itemNum % 2 ? Style.listBackgroundOdd : Style.listBackgroundEven;
            }

            Rectangle {
                id: background
                anchors.fill: parent
                color: Style.listBackgroundHighlight
                visible: mouseArea.pressed
            }

            Item {
                width: parent.width
                height: parent.height

                Grid {
                    id: grid

                    columns: 2
                    height: childrenRect.height
                    spacing: platformStyle.paddingMedium

                    anchors {
                        top: parent.top
                        topMargin: platformStyle.paddingMedium
                        left: parent.left
                        leftMargin: platformStyle.paddingMedium
                        right: parent.right
                        rightMargin: platformStyle.paddingMedium
                    }

                    Label {
                        id: lbl_time
                        text: time
                        font.bold: true
                        width: (parent.width - grid.spacing) / 3
                        platformInverted: appWindow.platformInverted
                    }

                    Label {
                        id: lbl_destination
                        text: destination
                        width: (parent.width - grid.spacing) * 2 / 3
                        wrapMode: Text.WordWrap
                        platformInverted: appWindow.platformInverted
                    }

                    Label {
                        id: lbl_type
                        text: trainType
                        font.bold: true
                        width: (parent.width - grid.spacing) / 3
                        wrapMode: Text.WordWrap
                        platformInverted: appWindow.platformInverted
                    }

                    Label {
                        id: lbl_station
                        text: stationplatform
                        width: (parent.width - grid.spacing) * 2 / 3
                        wrapMode: Text.WordWrap
                        platformInverted: appWindow.platformInverted
                    }

                    Item {
                        visible: miscInfo !== ""
                        width: (parent.width - grid.spacing) / 3
                        height: lbl_miscinfo.height
                    }

                    Label {
                        id: lbl_miscinfo
                        visible: miscInfo !== ""
                        text: miscInfo
                        width: (parent.width - grid.spacing) * 2 / 3
                        font.bold: true
                        wrapMode: Text.WordWrap
                        platformInverted: appWindow.platformInverted
                    }
                }
            }
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
                    "time": Qt.formatTime( item.time, qsTr("hh:mm")),
                    "trainType": item.trainType,
                    "destination": dirlabel,
                    "stationplatform": stationplatform,
                    "miscInfo": item.miscInfo,
                    "itemNum" : i
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
