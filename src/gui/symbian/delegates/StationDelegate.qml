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
import "../js/style.js" as Style

Item {
    id: root

    property ListView listView: ListView.view
    property bool platformInverted: false

    signal stationSelected()

    width: ListView.view.width
    height: 2 * platformStyle.paddingLarge + lbl_stationname.height

    Rectangle {
        id: background

        color: Style.listBackgroundHighlight
        visible: mouseArea.pressed
        anchors.fill: parent
    }

    MouseArea {
        id: mouseArea

        anchors {
            top: background.top
            left: lbl_stationname.left
            bottom: background.bottom
            right: background.right
        }

        onClicked: {
            listView.model.selectStation(stationSelect.type, model.index);
            root.stationSelected();
        }
    }

    Image {
        id: img_fav

        source: Style.getIconFromTheme(root.platformInverted, model.isFavorite ? "qtg_graf_rating_rated" : "qtg_graf_rating_unrated")
        sourceSize {
            width: platformStyle.graphicSizeSmall
            height: platformStyle.graphicSizeSmall
        }
        anchors {
            left: parent.left
            leftMargin: platformStyle.paddingMedium
            verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        anchors {
            top: background.top
            left: background.left
            bottom: background.bottom
            right: lbl_stationname.left
        }

        onClicked: {
            if (model.isFavorite) {
                banner.text = qsTr("Removing '%1' from favorites").arg(lbl_stationname.text);
                banner.open();
                listView.model.removeFromFavorites(model.index);
            } else {
                banner.text = qsTr("Adding '%1' to favorites").arg(lbl_stationname.text);
                banner.open();
                listView.model.addToFavorites(model.index);
            }
        }
    }

    Label {
        id: lbl_stationname

        text: model.name
        wrapMode: Text.WordWrap
        platformInverted: root.platformInverted
        anchors {
            left: img_fav.right
            leftMargin: platformStyle.paddingMedium
            right: lbl_miscinfo.left
            rightMargin: platformStyle.paddingMedium
            verticalCenter: parent.verticalCenter
        }
    }

    Label {
        id: lbl_miscinfo

        text: model.miscInfo
        color: Style.listBackgroundHighlight
        anchors {
            right: parent.right
            rightMargin: platformStyle.paddingMedium
            verticalCenter: parent.verticalCenter
        }
    }
}
