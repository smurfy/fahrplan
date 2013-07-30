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

import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1

Dialog {
    id: root
    title: qsTr("Time")
    height: units.gu(100)

    property alias hour: hourScroller.currentIndex
    property alias minute: minuteScroller.currentIndex

    signal accepted(int hours, int minutes)
    signal rejected()

    QtObject {
        id: priv

        property date now: new Date()
    }

    contents: [
        Row {
            height: units.gu(24)
            Scroller {
                id: hourScroller
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                }
                width: parent.width / 2
                labelText: qsTr("Hour")
                min: 00
                max: 23
                currentIndex: priv.now.getHours()
            }
            Scroller {
                id: minuteScroller
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                }
                width: parent.width / 2
                labelText: qsTr("Minute")

                min: 00
                max: 59
                currentIndex: priv.now.getMinutes()
            }
        },
        Row {
            spacing: units.gu(1)
            Button {
                text: "Cancel"
                onClicked: {
                    root.rejected()
                    PopupUtils.close(root)
                }
                width: (parent.width - parent.spacing) / 2
            }
            Button {
                text: "OK"
                color: "#dd4814"

                onClicked: {
                    root.accepted(root.hour, root.minute)
                    PopupUtils.close(root)
                }
                width: (parent.width - parent.spacing) / 2
            }
        }

    ]
}
