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

