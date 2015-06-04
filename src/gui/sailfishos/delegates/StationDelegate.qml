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
    id: root
    property ListView listView: ListView.view
    width: listView.width
    property bool menuOpen: contextMenu.parent === root
    signal stationSelected()
    height: menuOpen ? contextMenu.height + contentItem.height : contentItem.height

    BackgroundItem {
        id: contentItem
        width: parent.width

        Label {
            id: lbl_name
            anchors {
                left: parent.left
                leftMargin: Theme.paddingMedium
                rightMargin: Theme.paddingLarge
                right: lbl_miscinfo.left
                verticalCenter: parent.verticalCenter
            }
            text: model.name
        }
        Label {
            id: lbl_miscinfo
            anchors {
                right: parent.right
                rightMargin: Theme.paddingMedium
                verticalCenter: parent.verticalCenter
            }
            color: Theme.secondaryColor
            text: model.miscInfo
        }

        onClicked: {
            listView.model.selectStation(stationSelect.type, model.index);
            root.stationSelected();
        }
        onPressAndHold: {
            contextMenu.show(root);
        }
    }

    RemorsePopup {
        id: remorse
    }

    ContextMenu {
        id: contextMenu
        MenuItem {
            text: model.isFavorite ? qsTr("Remove from favorites") : qsTr("Add to favorites")
            onClicked: {
                if (model.isFavorite) {
                    remorse.execute(qsTr("Removing favorite..."), function() {
                        listView.model.removeFromFavorites(model.index);
                    });

                } else {
                    listView.model.addToFavorites(model.index);
                }
            }
        }
    }
}
