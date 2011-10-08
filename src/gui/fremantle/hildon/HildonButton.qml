import Qt 4.7

import "hildon.js" as Hildon
import "style.js" as Style

BorderImage {
    id: hildonButton

    property string text: ""
    property string icon: ""
    property bool sensitive: true
    property bool pressed: mouseArea.pressed
    property bool thumb: false

    signal clicked

    source: Hildon.themedButton((thumb?'Thumb':'Finger') + 'Button', sensitive, pressed)

    //horizontalTileMode: BorderImage.Repeat
    height: thumb?105:70
    width: label.paintedWidth + 20
    border {
        top: 20
        bottom: 20
        left: 20
        right: 20
    }


    Row {
        anchors.centerIn: parent
        spacing: iconImage.visible?10:0
        opacity: sensitive?1:.3

        Image {
            id: iconImage
            source: (hildonButton.icon!="")?Hildon.iconFilename(hildonButton.icon):""
            visible: source != ""
            height: hildonButton.height
            fillMode: Image.PreserveAspectFit
        }

        HildonLabel {
            id: label
            text: hildonButton.text
            height: hildonButton.height
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: { if (sensitive) parent.clicked() }
    }
}

