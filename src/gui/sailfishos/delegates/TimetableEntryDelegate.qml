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

Item {
    id: root
    property ListView listView: ListView.view
    width: listView.width
    height: contentItem.height + seperator.height + Theme.paddingMedium

    Grid {
        id: contentItem
        height: childrenRect.height + Theme.paddingMedium

        columns: 2
        spacing: Theme.paddingMedium / 2
        anchors {
            top: parent.top
            topMargin: Theme.paddingMedium
            left: parent.left
            leftMargin: Theme.paddingMedium
            right: parent.right
            rightMargin: Theme.paddingMedium
        }

        Label {
            id: lbl_time
            text: Qt.formatTime(model.time, Qt.DefaultLocaleShortDate)
            font.bold: true
            width: (parent.width - contentItem.spacing) / 3
        }
        Label {
            id: lbl_destination
            text: "%1 <b>%2</b>".arg(fahrplanBackend.mode === FahrplanBackend.ArrivalMode ? qsTr("from") : qsTr("to")).arg(model.destinationStation)
            width: (parent.width - contentItem.spacing) * 2 / 3
            wrapMode: Text.WordWrap
        }
        Label {
            id: lbl_type
            text: model.trainType
            font.bold: true
            width: (parent.width - contentItem.spacing) / 3
            wrapMode: Text.WordWrap
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
            width: (parent.width - contentItem.spacing) * 2 / 3
            wrapMode: Text.WordWrap
        }
        Item {
            visible: model.miscInfo !== ""
            width: (parent.width - contentItem.spacing) / 3
            height: lbl_miscinfo.height
        }
        Label {
            id: lbl_miscinfo
            visible: model.miscInfo !== ""
            text: model.miscInfo
            width: (parent.width - contentItem.spacing) * 2 / 3
            font.bold: true
            wrapMode: Text.WordWrap
            onLinkActivated : Qt.openUrlExternally(link)
        }
    }

    Separator {
        id: seperator
        width: listView.width
        anchors {
            top: contentItem.bottom
        }
        color: Theme.highlightColor
    }
}
