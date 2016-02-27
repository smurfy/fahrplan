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
    id: timetablePage

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height
        contentWidth: parent.width

        VerticalScrollDecorator {}
        Column {
            id: column
            spacing: Theme.paddingLarge
            width: parent.width

            PageHeader {
                id: timetableDesc
            }

            ListView {
                id: listView
                width: parent.width
                height: contentHeight
                interactive: false
                visible: !indicator.visible && !errorMsg.visible

                model:  fahrplanBackend.timetable

                delegate: TimetableEntryDelegate {

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
                indicator.visible = true;
                errorMsg.visible = false;
                timetableDesc.title = qsTr("Searching...");
                fahrplanBackend.getTimeTable();
                break;
            case PageStatus.Deactivating:
                errorMsg.visible = false;
                fahrplanBackend.parser.cancelRequest();
                break;
        }
    }

    BusyIndicator {
        id: indicator
        anchors.centerIn: parent
        running: true
        size: BusyIndicatorSize.Large
    }


    Connections {
        target: fahrplanBackend

        onParserTimeTableResult: {
            indicator.visible = false;
            errorMsg.visible = false;
            timetableDesc.title = fahrplanBackend.mode === FahrplanBackend.ArrivalMode ? qsTr("Arrivals") : qsTr("Departures")
        }

        onParserErrorOccured: {
            console.log("Got error")
            console.log(msg)
            errorMsg.visible = true;
            indicator.visible = false;
            timetableDesc.title = qsTr("Error");
            errorMsg.text = msg;
        }
    }
}
