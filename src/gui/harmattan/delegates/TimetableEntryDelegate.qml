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

Rectangle {
    id: delegateItem

    property ListView listView: ListView.view

    width: ListView.view.width
    height: 3 * UiConstants.ButtonSpacing + lbl_destination.height + Math.max(lbl_station.height, lbl_type.height) + (lbl_miscinfo.visible ? UiConstants.ButtonSpacing + lbl_miscinfo.height : 0)
    color: {
        if (theme.inverted)
            return model.index % 2 ? "#111" : "#333"
        else
            return model.index % 2 ? "White" : "LightGrey"
    }

    Grid {
        height: parent.height
        columns: 2
        spacing: UiConstants.ButtonSpacing / 2
        anchors {
            top: parent.top
            topMargin: UiConstants.ButtonSpacing
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
        }

        Label {
            id: lbl_time
            text: Qt.formatTime(model.time, qsTr("hh:mm"))
            font.bold: true
            width: (parent.width - 4 * UiConstants.ButtonSpacing) / 3
        }
        Label {
            id: lbl_destination
            text: "%1 <b>%2</b>".arg(fahrplanBackend.mode === FahrplanBackend.ArrivalMode ? qsTr("from") : qsTr("to")).arg(model.destinationStation)
            width: ((parent.width - 4 * UiConstants.ButtonSpacing) / 3) * 2
        }
        Label {
            id: lbl_type
            text: model.trainType
            font.bold: true
            width: (parent.width - 4 * UiConstants.ButtonSpacing) / 3
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
            width: ((parent.width - 4 * UiConstants.ButtonSpacing) / 3) * 2
        }
        Item {
            id: lbl_miscinfo_title
            visible: (model.miscInfo !== "")
            width: (parent.width - 4 * UiConstants.ButtonSpacing) / 3
            height: lbl_miscinfo.height
        }
        Label {
            id: lbl_miscinfo
            visible: (model.miscInfo !== "")
            text: model.miscInfo
            width: ((parent.width - 4 * UiConstants.ButtonSpacing) / 3)  * 2
            font.bold: true
        }
    }
}
