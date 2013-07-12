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

Item {
    id: root

    property alias title: labels.title
    property alias subtitle: labels.subtitle
    property alias checked: checkbox.checked
    property bool platformInverted: false

    height: 2 * platformStyle.paddingMedium + Math.max(labels.height, checkbox.height)
    anchors {
        left: parent.left
        right: parent.right
    }

    TwoLineLabel {
        id: labels
        platformInverted: root.platformInverted
        anchors {
            top: parent.top
            topMargin: platformStyle.paddingMedium
            left: parent.left
            leftMargin: platformStyle.paddingLarge
            right: checkbox.left
            rightMargin: platformStyle.paddingLarge
        }
    }

    Switch {
        id: checkbox
        platformInverted: root.platformInverted
        anchors {
            right: parent.right
            rightMargin: platformStyle.paddingLarge
            verticalCenter: labels.verticalCenter
        }
    }
}
