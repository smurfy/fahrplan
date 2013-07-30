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

MouseArea {
    id: root

    property alias title: labels.title
    property alias subtitle: labels.subtitle
    property bool platformInverted: false

    implicitHeight: 2 * platformStyle.paddingMedium + Math.max(labels.height, icon.height)
    anchors {
        left: parent.left
        right: parent.right
    }

    Rectangle {
        color: Style.listBackgroundHighlight
        visible: parent.pressed
        anchors.fill: parent
    }

    TwoLineLabel {
        id: labels
        platformInverted: root.platformInverted
        anchors {
            top: parent.top
            topMargin: platformStyle.paddingMedium
            left: parent.left
            leftMargin: platformStyle.paddingLarge
            right: icon.left
            rightMargin: platformStyle.paddingLarge
        }
    }

    Image {
        id: icon

        source: Style.getIconFromTheme(root.platformInverted, "qtg_graf_choice_list_indicator")
        sourceSize {
            width: platformStyle.graphicSizeSmall
            height: platformStyle.graphicSizeSmall
        }
        anchors {
            right: parent.right
            rightMargin: platformStyle.paddingLarge
            verticalCenter: parent.verticalCenter
        }
    }
}
