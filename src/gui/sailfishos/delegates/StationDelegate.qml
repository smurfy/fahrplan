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

    ContextMenu {
        id: contextMenu
        MenuItem {
            text: model.isFavorite ? qsTr("Remove from favorites") : qsTr("Add to favorites")
            onClicked: {
                if (model.isFavorite) {
                    listView.model.removeFromFavorites(model.index);
                } else {
                    listView.model.addToFavorites(model.index);
                }
            }
        }
    }
}
