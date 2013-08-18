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

Item {
    id: subTitleButton

    property int type
    property alias titleText: title.text
    property alias iconVisible: icon.visible
    property alias subTitleText: subTitle.text
    property string icon: "qtg_graf_choice_list_indicator"
    property bool platformInverted: false

    signal clicked
    signal pressAndHold
    signal reset

    height: title.height + subTitle.height + platformStyle.paddingMedium + platformStyle.paddingLarge
    width: parent.width

    Rectangle {
        id: background
        anchors.fill: parent
        color: Style.listBackgroundHighlight
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
            leftMargin: platformStyle.paddingMedium
            rightMargin: platformStyle.paddingMedium
            topMargin: platformStyle.paddingMedium
            top: parent.top
        }

        font.bold: true
        font.pixelSize: platformStyle.fontSizeLarge
        text: "-"
        platformInverted: subTitleButton.platformInverted
    }

    Label {
        id: subTitle
        anchors {
            top:  title.bottom
            topMargin: platformStyle.paddingSmall
            left: parent.left
            leftMargin: platformStyle.paddingMedium
            right: icon.left
            rightMargin: platformStyle.paddingMedium
        }
        text: ""
        font.pixelSize: subTitle.text != "" ?  platformStyle.fontSizeLarge + 2 : 0
        platformInverted: subTitleButton.platformInverted
        wrapMode: Text.WordWrap
        visible: subTitle.text != "" ? true : false
    }

    Image {
        id: icon

        source: Style.getIconFromTheme(subTitleButton.platformInverted, subTitleButton.icon)
        sourceSize {
            width: platformStyle.graphicSizeTiny
            height: platformStyle.graphicSizeTiny
        }
        anchors {
            right: parent.right
            rightMargin: platformStyle.paddingMedium
            verticalCenter: parent.verticalCenter
        }
    }
}
