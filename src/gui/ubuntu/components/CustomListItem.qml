import QtQuick 2.3
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.1
import Ubuntu.Components.ListItems 1.0 as ListItem

ListItem.Base {
    id: customListItem

    property alias text: _title.text
    property alias value: _value.text

    RowLayout {
        spacing: units.gu(1)
        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }

        Label {
            id: _title
            Layout.fillWidth: true
            font.bold: true
            elide: Text.ElideRight
        }

        Label {
            id: _value
            elide: Text.ElideRight
        }

        Icon {
            id: _progression
            name: "go-next"
            width: units.gu(2)
            height: width
        }
    }
}
