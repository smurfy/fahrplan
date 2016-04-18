import QtQuick 2.4
import Ubuntu.Components 1.3

ListItem {
    id: customListItem

    property alias title: listitemlayout.title
    property alias value: _value.text

    height: listitemlayout.height + divider.height

    ListItemLayout {
        id: listitemlayout

        title.text: ""
        title.font.bold: true

        Label {
            id: _value
            SlotsLayout.position: SlotsLayout.Trailing
        }

        ProgressionSlot {}
    }
}
