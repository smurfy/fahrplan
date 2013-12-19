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
import "../delegates"

Page {
    id: root

    property string emptyText
    property alias model: listView.model
    property bool platformInverted: false

    signal stationSelected()

    ListView {
        id: listView

        anchors.fill: parent
        delegate: StationDelegate {
            platformInverted: root.platformInverted

            onStationSelected: root.stationSelected()
        }
    }

    Label {
        text: root.emptyText
        color: root.platformInverted ? platformStyle.colorNormalMidInverted : platformStyle.colorNormalMid
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        opacity: (listView.count == 0) ? 1.0 : 0.0
        font.pixelSize: 2 * platformStyle.fontSizeMedium
        anchors {
            fill: parent
            margins: platformStyle.graphicSizeSmall
        }

        Behavior on opacity { NumberAnimation { duration: 100 } }
    }
}
