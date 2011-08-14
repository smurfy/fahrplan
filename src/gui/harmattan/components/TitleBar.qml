import QtQuick 1.1

Item {
    id: titleBar

    property alias titleText: title.text
    height: 70

    BorderImage {
        source: "image://theme/meegotouch-list-header-background"
        width: parent.width
        height: parent.height + 14
        y: -7
    }
    Text {
        id: title
        text: ""
        font.bold: true
        font.pixelSize: 32
        style: Text.Raised
        color: "Black"
        styleColor: "Grey"
        anchors {
            left: parent.left
            leftMargin: 10
            top: parent.top
            topMargin: 20
        }
    }
}
