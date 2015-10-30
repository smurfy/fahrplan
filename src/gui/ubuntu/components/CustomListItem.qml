import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3

ListItem {
    id: customListItem

    property alias text: _title.text
    property alias value: _value.text

    RowLayout {
        spacing: units.gu(1)
        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter; margins: units.gu(2) }

        Label {
            id: _title
            Layout.fillWidth: true
            font.bold: true
        }

        Label {
            id: _value
            elide: Text.ElideMiddle
            horizontalAlignment: Text.AlignRight
            Layout.maximumWidth: parent.width - _title.implicitWidth - _progression.width - units.gu(2)
        }

        Icon {
            id: _progression
            name: "go-next"
            Layout.preferredWidth: units.gu(2)
            Layout.preferredHeight: units.gu(2)
        }
    }
}
