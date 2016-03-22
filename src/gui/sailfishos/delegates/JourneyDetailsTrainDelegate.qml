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
    id: item_train

    width: parent.width
    height: lbl_train.height + 2*Theme.paddingMedium

    Rectangle {
        color: Theme.secondaryHighlightColor
        anchors.fill: parent
    }

    Rectangle {
        id: separator
        width: Theme.iconSizeSmall/4
        height: parent.height
        color: Theme.highlightColor
        anchors {
            left: parent.left
            leftMargin: (timeWidth ? timeWidth : Theme.paddingLarge * 3) + Theme.iconSizeSmall/2 - width/2
        }
    }

    Label {
        id: lbl_train
        anchors {
            left: separator.right
            leftMargin: Theme.paddingMedium
            right: parent.right
            rightMargin: Theme.paddingMedium
            verticalCenter: parent.verticalCenter
        }
        wrapMode: Text.WordWrap
        text: {
            var result;
            if (model.trainDirection.length > 0) {
                //: As in "%1 in direction %2"
                result = qsTr("%1 to %2", "Direction").arg("<b>" + model.trainName + "</b>")
                                                      .arg(model.trainDirection);
            } else {
                result = "<b>" + model.trainName + "</b>";
            }
            if (model.trainInfo)
                result += "<br /><i>" + model.trainInfo + "</i>";

            return result;
        }

        onLinkActivated : Qt.openUrlExternally(link)
    }
}
