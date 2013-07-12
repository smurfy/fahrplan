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

import "hildon.js" as Hildon
import "style.js" as Style

Item {
    id: hildonDialog

    default property alias children: container.children

    property string title: "Unnamed dialog"
    property variant actions
    property int dialogHeight: 200

    function appear() {
        state = "active"
    }

    state: "default"

    states: [
        State {
            name: "default"
            AnchorChanges { target: window; anchors.top: parent.bottom }
        },
        State {
            name: "active"
            AnchorChanges { target: window; anchors.bottom: parent.bottom }
            PropertyChanges { target: shade; opacity: 0.7 }
        },
        State {
            name: "done"
            AnchorChanges { target: window; anchors.top: parent.bottom }
        }
    ]

    transitions: Transition {
        AnchorAnimation { duration: 400; easing.type: Easing.OutBack }
        PropertyAnimation { duration: 400; property: "opacity" } //; easing.type: Easing.OutBack }
    }

    z: 100
    anchors.fill: parent

    Rectangle {
        id: shade
        anchors.fill: parent
        opacity: 0
        color: "black"
        MouseArea {
            anchors.fill: parent
            onClicked: { hildonDialog.state = "done" }
        }
    }

    Rectangle {
        id: window
        anchors.left: parent.left
        anchors.right: parent.right

        height: hildonDialog.dialogHeight
        color: "black"
        Image {
            id: titlebar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            source: Hildon.themeFilename('wmDialog')
            height: 46
            fillMode: Image.TileHorizontally

            HildonLabel {
                anchors.centerIn: parent
                text: hildonDialog.title
            }
        }

        Item {
            id: container
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: titlebar.bottom
            z: 12
        }

        Item {
            id: actionArea
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 12
            anchors.bottomMargin: 12
            width: 200
            z: 13

            Repeater {
                anchors.fill: parent
                model: hildonDialog.actions
                width: parent.width

                HildonDialogButton {
                    anchors.bottom: parent.bottom
                    text: modelData
                    width: parent.width
                    anchors.bottomMargin: (hildonDialog.actions.length-index-1)*height
                    onClicked: { hildonDialog.state = "done" }
                }
            }
        }

        MouseArea { anchors.fill: parent }
    }

    Rectangle {
        id: bottomGlue
        color: "black"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: window.bottom
    }
}

