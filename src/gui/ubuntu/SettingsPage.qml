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

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItems
import Fahrplan 1.0
import "components"

Page {
    id: root

    Rectangle {
        id: header

        height: units.gu(6)
        gradient: Gradient {
            GradientStop {
                position: 0.00
                color: "#0d70c5"
            }
            GradientStop {
                position: 1.0
                color: "#0a50a5"
            }
        }
        anchors {
            left: parent.left
            right: parent.right
        }

        Label {
            text: qsTr("Settings")
            color: "white"
            fontSize: "large"
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: units.gu(1)
            }
        }
    }

    Column {
        spacing: units.gu(1)
        anchors {
            top: header.bottom
            topMargin: units.gu(1)
            left: parent.left
            leftMargin: units.gu(1)
            right: parent.right
            rightMargin: units.gu(1)
        }

        ListItems.Standard {
            text: qsTr("Enable GPS location support")

            control: Switch {
                onCheckedChanged: {
                    fahrplanBackend.storeSettingsValue("enableGps", checked);
                }
                Component.onCompleted: {
                    checked = fahrplanBackend.getSettingsValue("enableGps", true);
                }
            }

        }
    }

    Button {
        id: aboutButton

        text: qsTr("About")
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: units.gu(1)
            horizontalCenter: parent.horizontalCenter
        }

        onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"));
    }
}
