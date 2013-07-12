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
    title: qsTr("Date")
    height: units.gu(100)

    // 1 - 31
    property int day: priv.now.getDate()
    // 0 - 11
    property alias month: monthScroller.currentIndex
    property int year: priv.now.getFullYear()
    property alias minYear: yearScroller.min
    property alias maxYear: yearScroller.max

    signal accepted(int day, int month, int year)
    signal rejected()

    QtObject {
        id: priv

        property date now: new Date()

        function getDays(month, year) {
            switch(monthScroller.currentIndex) {
            case 1:
                if (((year % 4 === 0) && (year % 100 !== 0)) || (year % 400 === 0)) {
                    return 29;
                }
                return 28;
            case 3:
            case 5:
            case 8:
            case 10:
                return 30;
            default:
                return 31;
            }
        }
    }

    contents: [
        Row {
            height: units.gu(24)
            Scroller {
                id: dayScroller
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                }
                width: parent.width / 3
                labelText: qsTr("Day")
                min: 1
                max: priv.getDays(monthScroller.currentIndex + 1, yearScroller.value);
                currentIndex: root.day - 1
                onCurrentIndexChanged: {
                    print("currentIndex changed:", currentIndex)
                    root.day = currentIndex + 1
                }
            }
            Scroller {
                id: monthScroller
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                }
                width: parent.width / 3
                labelText: qsTr("Month")
                currentIndex: priv.now.getMonth()

                model: ListModel {
                    ListElement { modelData: "Jan" }
                    ListElement { modelData: "Feb" }
                    ListElement { modelData: "Mar" }
                    ListElement { modelData: "Apr" }
                    ListElement { modelData: "May" }
                    ListElement { modelData: "Jun" }
                    ListElement { modelData: "Jul" }
                    ListElement { modelData: "Aug" }
                    ListElement { modelData: "Sep" }
                    ListElement { modelData: "Oct" }
                    ListElement { modelData: "Nov" }
                    ListElement { modelData: "Dec" }
                }
            }
            Scroller {
                id: yearScroller
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                }
                width: parent.width / 3
                labelText: qsTr("Year")
                min: 1970
                max: 2048
                currentIndex: root.year - min
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
                    root.accepted(dayScroller.value, monthScroller.currentIndex, yearScroller.value)
                    PopupUtils.close(root)
                }
                width: (parent.width - parent.spacing) / 2
            }
        }
    ]
}
