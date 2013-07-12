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

Item {
    property alias title: labels.title
    property alias subtitle: labels.subtitle
    property alias checked: checkbox.checked

    height: 2 * UiConstants.ButtonSpacing + Math.max(labels.height, checkbox.height)
    anchors {
        left: parent.left
        right: parent.right
    }

    TwoLineLabel {
        id: labels
        anchors {
            top: parent.top
            topMargin: UiConstants.ButtonSpacing
            left: parent.left
            leftMargin: UiConstants.DefaultMargin
            right: checkbox.left
            rightMargin: UiConstants.DefaultMargin
        }
    }

    Switch {
        id: checkbox
        anchors {
            right: parent.right
            rightMargin: UiConstants.DefaultMargin
            verticalCenter: labels.verticalCenter
        }
    }
}
