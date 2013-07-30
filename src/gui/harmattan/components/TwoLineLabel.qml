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
    id: root

    property alias title: lblTitle.text
    property alias subtitle: lblSubtitle.text
    property bool platformInverted: false

    implicitWidth: Math.max(lblTitle.implicitWidth, lblSubtitle.implicitWidth)
    height: childrenRect.height

    Label {
        id: lblTitle
        font: UiConstants.TitleFont
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }
    Label {
        id: lblSubtitle
        font: UiConstants.SubtitleFont
        visible: text != ""
        height: visible ? implicitHeight : 0
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        anchors {
            top: lblTitle.bottom
            topMargin: UiConstants.ButtonSpacing
            left: parent.left
            right: parent.right
        }
    }
}
