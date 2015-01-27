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
    height: contentItem.height + Theme.paddingLarge

    signal clicked()

    BackgroundItem {
        id: contentItem
        width: parent.width
        height: timingsLabels.height + trainTypeLabel.height + (miscInfoLabel.visible ? miscInfoLabel.height : 0)

        Column {
            id: labels

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                leftMargin: Theme.paddingMedium
                rightMargin: Theme.paddingMedium
            }

            Row {
                id: timingsLabels

                width: parent.width

                Label {
                    text: model.departureTime
                    width: (parent.width - 3 * 1) / 4
                }

                Label {
                    text: model.arrivalTime
                    width: (parent.width - 3 * 1) / 4
                }

                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: model.duration
                    width: (parent.width - 3 * 1) / 4
                }

                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: model.transfers
                    width: (parent.width - 3 * 1) / 4
                }
            }

            Label {
                id: trainTypeLabel
                text: model.trainType
                width: parent.width
            }

            Label {
                id: miscInfoLabel
                visible: model.miscInfo !== ""
                text: model.miscInfo
                width: parent.width
                wrapMode: Text.WordWrap
                font.bold: true
            }

        }

        onClicked: {
            root.clicked();
        }
    }
}
