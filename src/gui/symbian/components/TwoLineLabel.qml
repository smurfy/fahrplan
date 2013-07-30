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

    property alias title: lblTitle.text
    property alias subtitle: lblSubtitle.text
    property bool platformInverted: false

    implicitWidth: Math.max(lblTitle.implicitWidth, lblSubtitle.implicitWidth)
    height: childrenRect.height

    ListItemText {
        id: lblTitle
        role: "Title"
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        platformInverted: root.platformInverted
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }
    ListItemText {
        id: lblSubtitle
        role: "Subtitle"
        visible: text != ""
        height: visible ? implicitHeight : 0
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        platformInverted: root.platformInverted
        anchors {
            top: lblTitle.bottom
            topMargin: platformStyle.paddingSmall
            left: parent.left
            right: parent.right
        }
    }
}
