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

Item {
    id: delegateItem

    property ListView listView: ListView.view
    property int timeWidth

    width: ListView.view.width
    height: loader1.height + loader2.height

    Loader {
        id: loader1
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        source: model.isStation ? "JourneyDetailsStationDelegate.qml" : ""
        asynchronous: true
        visible: status == Loader.Ready
    }

    Loader {
        id: loader2
        anchors {
            left: parent.left
            right: parent.right
            top: loader1.bottom
        }

        source: model.isTrain ? "JourneyDetailsTrainDelegate.qml" : ""
        asynchronous: true
        visible: status == Loader.Ready
    }

}
