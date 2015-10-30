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
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.Pickers 1.3

Dialog {
    id: root

    title: qsTr("<b>Date</b>")

    property alias date: datePicker.date

    signal accepted(int day, int month, int year)
    signal rejected()

    contents: [
        DatePicker {
            id: datePicker
            mode: "Years|Months|Days"
        },

        Row {
            spacing: units.gu(1)
            Button {
                text: qsTr("Cancel")
                color: UbuntuColors.lightGrey
                width: (parent.width - parent.spacing) / 2

                onClicked: {
                    root.rejected()
                    PopupUtils.close(root)
                }
            }

            Button {
                text: qsTr("Ok")
                color: UbuntuColors.green
                width: (parent.width - parent.spacing) / 2

                onClicked: {
                    root.accepted(datePicker.day, datePicker.month, datePicker.year)
                    PopupUtils.close(root)
                }
            }
        }
    ]
}
