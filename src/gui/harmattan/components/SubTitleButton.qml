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
    id: subTitleButton

    property int type
    property alias titleText: title.text
    property alias iconVisible: icon.visible
    property alias subTitleText: subTitle.text
    property alias icon: icon.source

    signal clicked
    signal pressAndHold
    signal reset

    height: title.height + subTitle.height + 20
    width: parent.width

    Rectangle {
        id: background
        anchors.fill: parent
        color: "DarkGrey"
        visible: mouseArea.pressed
    }

    MouseArea {
        id: mouseArea
        anchors.fill: background
        onClicked: {
            subTitleButton.clicked();
        }
        onPressAndHold: {
            subTitleButton.pressAndHold();
        }
    }

    Label {
        id: title

        anchors {
            left: parent.left
            leftMargin: 10
            rightMargin: 10
            topMargin: 7
            top: parent.top
        }

        font.bold: true
        font.pixelSize: 26
        text: "-"
    }

    Label {
        id: subTitle
        anchors {
            top:  title.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 10
            rightMargin: 10
            right: icon.left
        }
        text: ""
        font.pixelSize: subTitle.text != "" ? 28 : 0
        visible: subTitle.text != "" ? true : false
    }

    Image {
        id: icon

        anchors {
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
        height: sourceSize.height
        width: sourceSize.width
        source: "image://theme/meegotouch-combobox-indicator" + platformStyle.__invertedString
    }
}
