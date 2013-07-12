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
import Ubuntu.Components.ListItems 0.1 as ListItems
import "components"

Page {
    property alias timetableTitleText: timetableTitle.text
    property alias searchIndicatorVisible: searchIndicator.visible

    property int selMode : 0

    id: searchResultsPage

    tools: timetableResultsToolbar
    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        Item {
            id: titleBar

            width: parent.width
            height: units.gu(7)

            Label {
                id: timetableTitle
                text: ""
                font.bold: true;
                fontSize: "large"
                anchors {
                    left: parent.left
                    leftMargin: units.gu(1)
                    right: parent.right
                    rightMargin: units.gu(1)
                    verticalCenter: parent.verticalCenter
                }
                width: parent.width
            }
        }

        ActivityIndicator {
            id: searchIndicator
            anchors {
                top: titleBar.bottom
                topMargin: units.gu(5)
                horizontalCenter: parent.horizontalCenter
            }
            running: true
            visible: false

        }

        ListView {
            id: listView
            anchors {
                top: titleBar.bottom
                topMargin: units.gu(1)
            }
            height: parent.height - titleBar.height - units.gu(2)
            width: parent.width
            model: timetableResultModel
            delegate:  timetableResultDelegate
            clip: true

            visible: !searchIndicator.visible
        }

        //        ScrollDecorator {
        //            flickableItem: listView
        //        }
    }

    Component {
        id: timetableResultDelegate

        ListItems.Base {
            id: delegateItem
            width: listView.width
            height: units.gu(4) + lbl_destination.height + (lbl_station.height > lbl_type.height ? lbl_station.height : lbl_type.height) + (lbl_miscinfo.visible ? lbl_miscinfo.height : 0)
            highlightWhenPressed: false


            Item {
                anchors {
                    verticalCenter: parent.verticalCenter
                    top: parent.top
                    topMargin: units.gu(1)
                }

                width: parent.width
                height: parent.height

                Grid {
                    columns: 2
                    spacing: units.gu(1)

                    anchors {
                        leftMargin: units.gu(1)
                        left: parent.left
                    }

                    width: parent.width
                    height: parent.height

                    Label {
                        id: lbl_time
                        text: time
                        font.bold: true
                        width: (parent.width  - units.gu(4)) / 4
                    }

                    Label {
                        id: lbl_destination
                        text: destination
                        width: ((parent.width  - units.gu(4)) / 4) * 3
                        elide: Text.ElideRight
                    }

                    Label {
                        id: lbl_type
                        text: trainType
                        font.bold: true
                        width: (parent.width  - units.gu(4)) / 4
                    }

                    Label {
                        id: lbl_station
                        text: stationplatform
                        width: ((parent.width  - units.gu(4)) / 4) * 3
                    }


                    Label {
                        id: lbl_miscinfo_title
                        visible: (miscInfo == "") ? false : true
                        text: ""
                        width: (parent.width  - units.gu(4)) / 4
                        elide: Text.ElideRight
                    }

                    Label {
                        id: lbl_miscinfo
                        visible: (miscInfo == "") ? false : true
                        text: miscInfo
                        width: ((parent.width  - units.gu(4)) / 4)  * 3
                        font.bold: true
                        elide: Text.ElideRight
                    }
                }


            }
        }
    }

    ListModel {
        id: timetableResultModel
    }

    //    InfoBanner{
    //            id: banner
    //            objectName: "fahrplanInfoBanner"
    //            text: ""
    //            anchors.top: parent.top
    //            anchors.topMargin: 10
    //    }

    FahrplanBackend {
        id: fahrplanBackend

        onParserErrorOccured: {
            banner.text = msg;
            banner.show();
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
                    stationplatform = qsTr("Pl. ") + item.platform;
                    if (item.stationName) {
                        stationplatform += " / " + item.stationName;
                    }
                }

                var dirlabel = "";
                if (selMode == 1) {
                    dirlabel = qsTr("to <b>%1</b>").arg(item.destinationName)
                } else if (selMode == 0) {
                    dirlabel = qsTr("from <b>%1</b>").arg(item.destinationName)
                } else {
                    dirlabel = "\u2013"
                }

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

    //    ToolBarLayout {
    //        id: timetableResultsToolbar

    //        ToolIcon {
    //            id : backIcon;
    //            iconId: "toolbar-back"
    //            onClicked: {
    //                pageStack.pop();
    //                fahrplanBackend.parser.cancelRequest();
    //            }
    //        }
    //    }
}
