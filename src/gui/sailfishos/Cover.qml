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
import Sailfish.Silica 1.0
import Fahrplan 1.0

CoverBackground {
    id: appCover

    Image {
           source: "../../../data/sailfishos/cover.png"
           anchors.horizontalCenter: parent.horizontalCenter
           width: parent.width
           height: sourceSize.height * width / sourceSize.width
           opacity: 0.1
       }

    CoverPlaceholder {
        id: fahrplanTitle
        text: qsTr("Fahrplan: ") + fahrplanBackend.parserName
    }

    Connections {
        target: fahrplanBackend
    }
}
