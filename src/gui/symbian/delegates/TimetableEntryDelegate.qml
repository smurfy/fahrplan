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
import "../js/style.js" as Style

Rectangle {
    id: delegateItem

    property ListView listView: ListView.view

    width: ListView.view.width
    height: grid.height + 2 * platformStyle.paddingMedium
    color: {
        if (appWindow.platformInverted)
            return model.index % 2 ? Style.listBackgroundOddInverted : Style.listBackgroundEvenInverted;
        else
            return model.index % 2 ? Style.listBackgroundOdd : Style.listBackgroundEven;
    }

    Item {
        width: parent.width
        height: parent.height

        Grid {
            id: grid

            height: childrenRect.height
            columns: 2
            spacing: platformStyle.paddingMedium / 2
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
                text: Qt.formatTime(model.time, qsTr("hh:mm"))
                font.bold: true
                width: (parent.width - grid.spacing) / 3
                platformInverted: appWindow.platformInverted
            }
            Label {
                id: lbl_destination
                text: "%1 <b>%2</b>".arg(fahrplanBackend.mode === FahrplanBackend.ArrivalMode ? qsTr("from") : qsTr("to")).arg(model.destinationStation)
                width: (parent.width - grid.spacing) * 2 / 3
                wrapMode: Text.WordWrap
                platformInverted: appWindow.platformInverted
            }
            Label {
                id: lbl_type
                text: model.trainType
                font.bold: true
                width: (parent.width - grid.spacing) / 3
                wrapMode: Text.WordWrap
                platformInverted: appWindow.platformInverted
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
                width: (parent.width - grid.spacing) * 2 / 3
                wrapMode: Text.WordWrap
                platformInverted: appWindow.platformInverted
            }
            Item {
                visible: model.miscInfo !== ""
                width: (parent.width - grid.spacing) / 3
                height: lbl_miscinfo.height
            }
            Label {
                id: lbl_miscinfo
                visible: model.miscInfo !== ""
                text: model.miscInfo
                width: (parent.width - grid.spacing) * 2 / 3
                font.bold: true
                wrapMode: Text.WordWrap
                platformInverted: appWindow.platformInverted
            }
        }
    }
}
