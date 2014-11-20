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

import QtQuick 1.1
import com.nokia.meego 1.1

Rectangle {
    id: root

    property ListView listView: ListView.view

    signal clicked()

    width: ListView.view.width
    height: 3 * UiConstants.ButtonSpacing + row.height + lbl_traintyp.height + (lbl_miscinfo.visible ? UiConstants.ButtonSpacing + lbl_miscinfo.height : 0)
    color: {
        if (theme.inverted)
            return model.index % 2 ? "#111" : "#333"
        else
            return model.index % 2 ? "White" : "LightGrey"
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "DarkGrey"
        visible: mouseArea.pressed
    }

    MouseArea {
        id: mouseArea
        anchors.fill: background
        onClicked: {
            root.clicked();
        }
    }

    Item {
        height: childrenRect.height
        anchors {
            top: parent.top
            topMargin: UiConstants.ButtonSpacing
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
        }

        Row {
            id: row
            width: parent.width
            height: childrenRect.height

            Label {
                id: lbl_departuretime
                text: model.departureTime
                width: (parent.width - 3 * UiConstants.ButtonSpacing) / 4
            }

            Label {
                id: lbl_arrivaltime
                text: model.arrivalTime
                width: (parent.width - 3 * UiConstants.ButtonSpacing) / 4
            }

            Label {
                id: lbl_duration
                horizontalAlignment: Text.AlignHCenter
                text: model.duration
                width: (parent.width - 3 * UiConstants.ButtonSpacing) / 4
            }

            Label {
                id: lbl_transfers
                horizontalAlignment: Text.AlignHCenter
                text: model.transfers
                width: (parent.width - 3 * UiConstants.ButtonSpacing) / 4
            }
        }

        Label {
            id: lbl_traintyp

            text: model.trainType
            anchors {
                top: row.bottom
                topMargin: UiConstants.ButtonSpacing
                left: parent.left
                right: parent.right
            }
        }

        Label {
            id: lbl_miscinfo

            text: model.miscInfo
            visible: (model.miscInfo !== "")
            wrapMode: Text.WordWrap
            font.bold: true
            anchors {
                top: lbl_traintyp.bottom
                topMargin: UiConstants.ButtonSpacing
                left: parent.left
                right: parent.right
            }
        }
    }
}
