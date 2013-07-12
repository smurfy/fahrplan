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

import Qt 4.7

import "../hildon/hildon.js" as Hildon
import "../hildon/style.js" as Style
import "../hildon"

import HildonHelper 1.0 as HildonHelper

BorderImage {
    id: subTitleButton

    property alias titleText: title.text
    property alias subTitleText: subTitle.text
    property string icon: ""
    property bool sensitive: true
    property bool pressed: mouseArea.pressed
    property bool thumb: false

    signal clicked

    source: Hildon.themedButton((thumb?'Thumb':'Finger') + 'Button', sensitive, pressed)

    //horizontalTileMode: BorderImage.Repeat
    height: thumb?105:70
    width: title.paintedWidth + subTitle.paintedWidth + 20
    border {
        top: 20
        bottom: 20
        left: 20
        right: 20
    }

    Item {
        anchors.centerIn: parent
        opacity: sensitive?1:.3
        height: parent.height
        width: parent.width

        HildonLabel {
            id: title
            height: subTitleButton.height/2 - 20
            width: parent.width - 20
            anchors {
                left: parent.left
                leftMargin: 20
                rightMargin: 20
                topMargin: 16
                top: parent.top
            }
            font: hildonHelper.standardFont("SystemFont")
            horizontalAlignment: Text.AlignHCenter
        }

        HildonLabel {
            id: subTitle
            height: subTitleButton.height/2 - 20
            width: parent.width - 20
            anchors {
                left: parent.left
                leftMargin: 20
                rightMargin: 20
                top:  title.bottom
                topMargin: 8
            }
            color: hildonHelper.standardColor("ActiveTextColor")
            font: hildonHelper.standardFont("SmallSystemFont")
            horizontalAlignment: Text.AlignHCenter
        }
    }

    HildonHelper.HildonHelper {
        id: hildonHelper
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: { if (sensitive) parent.clicked() }
    }
}
