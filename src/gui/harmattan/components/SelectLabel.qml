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

MouseArea {
    id: root

    property alias title: labels.title
    property alias subtitle: labels.subtitle

    implicitHeight: 2 * UiConstants.ButtonSpacing + Math.max(labels.height, icon.height)
    anchors {
        left: parent.left
        right: parent.right
    }

    Rectangle {
        color: "DarkGrey"
        visible: parent.pressed
        anchors.fill: parent
    }

    TwoLineLabel {
        id: labels
        anchors {
            top: parent.top
            topMargin: UiConstants.ButtonSpacing
            left: parent.left
            leftMargin: UiConstants.DefaultMargin
            right: icon.left
            rightMargin: UiConstants.DefaultMargin
        }
    }

    Image {
        id: icon

        source: "image://theme/meegotouch-combobox-indicator" + platformStyle.__invertedString
        height: sourceSize.height
        width: sourceSize.width
        anchors {
            right: parent.right
            rightMargin: UiConstants.DefaultMargin
            verticalCenter: parent.verticalCenter
        }
    }
}
