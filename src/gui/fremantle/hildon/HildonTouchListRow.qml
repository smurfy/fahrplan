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

Image {
    width: parent.width
    fillMode: Image.TileHorizontally
    source: Hildon.themeFilename('TouchListBackground'+(mouseArea.pressed?'Pressed':'Normal'))
    height: 70

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
}

