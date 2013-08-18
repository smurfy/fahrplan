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
import com.nokia.meego 1.1

Item {
    id: root

    property ListView listView: ListView.view

    signal stationSelected()

    width: ListView.view.width
    height: 30 + lbl_stationname.height

    Rectangle {
        id: background
        anchors.fill: parent
        color: "DarkGrey"
        visible: mouseArea.pressed
    }

    MouseArea {
        id: mouseArea
        anchors {
            left: lbl_stationname.left
            top: background.top
            right: background.right
            bottom: background.bottom
        }
        onClicked: {
            listView.model.selectStation(stationSelect.type, model.index);
            root.stationSelected();
        }
    }

    Image {
        id: img_fav
        source: model.isFavorite ? "image://theme/icon-s-common-favorite-mark" + inverseSuffix : "image://theme/icon-s-common-favorite-unmark" + inverseSuffix
        anchors {
            left: parent.left
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        id: mouseArea_fav
        anchors {
            top: background.top
            bottom: background.bottom
            left: background.left
            right: lbl_stationname.left
        }

        onClicked: {
            if (model.isFavorite) {
                banner.text = qsTr("Removing '%1' from favorites").arg(lbl_stationname.text);
                banner.show();
                listView.model.removeFromFavorites(model.index);
            } else {
                banner.text = qsTr("Adding '%1' to favorites").arg(lbl_stationname.text);
                banner.show();
                listView.model.addToFavorites(model.index);
            }
        }
    }

    Label {
        id: lbl_stationname
        anchors {
            left: img_fav.right
            leftMargin: 10
            rightMargin: 20
            right: lbl_miscinfo.left
            verticalCenter: parent.verticalCenter
        }
        text: model.name
    }

    Label {
        id: lbl_miscinfo
        anchors {
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
        text: model.miscInfo
        color: "DarkGrey";
    }
}
