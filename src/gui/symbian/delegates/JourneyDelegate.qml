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
import com.nokia.symbian 1.1
import "../js/style.js" as Style

Rectangle {
    id: root

    property ListView listView: ListView.view

    signal clicked()

    width: ListView.view.width
    height: labels.height + 2 * platformStyle.paddingMedium
    color: {
        if (appWindow.platformInverted)
            return model.index % 2 ? Style.listBackgroundOddInverted : Style.listBackgroundEvenInverted;
        else
            return model.index % 2 ? Style.listBackgroundOdd : Style.listBackgroundEven;
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.listBackgroundHighlight
        visible: mouseArea.pressed
    }

    MouseArea {
        id: mouseArea
        anchors.fill: background
        onClicked: {
            root.clicked();
        }
    }

    Column {
        id: labels

        height: childrenRect.height
        spacing: platformStyle.paddingSmall
        anchors {
            top: parent.top
            topMargin: platformStyle.paddingMedium
            left: parent.left
            leftMargin: platformStyle.paddingMedium
            right: parent.right
            rightMargin: platformStyle.paddingMedium
        }

        Row {
            width: parent.width
            height: childrenRect.height
            spacing: platformStyle.paddingMedium

            Label {
                text: model.departureTime
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }

            Label {
                text: model.arrivalTime
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }

            Label {
                horizontalAlignment: Text.AlignHCenter
                text: model.duration
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }

            Label {
                horizontalAlignment: Text.AlignHCenter
                text: model.transfers
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }
        }

        Label {
            text: model.trainType
            width: parent.width
            platformInverted: appWindow.platformInverted
        }

        Label {
            visible: model.miscInfo !== ""
            text: model.miscInfo
            width: parent.width
            wrapMode: Text.WordWrap
            font.bold: true
            platformInverted: appWindow.platformInverted
        }
    }
}
