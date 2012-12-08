import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

/*
   Class: TimePickerDialogAdv
   Dialog that shows a time picker. With another button called Now, to set the current time
*/

TimePickerDialog {
    id : root

    property alias newButtonText: nowButton.text

    buttons: Row {

        id: advButtons

        Button {
            id: nowButton
            text: "Now"
            width: root.width - 3
            style: ButtonStyle { inverted: true }
            onClicked: {
                var d = new Date();
                hour = d.getHours();
                minute = d.getMinutes();
                second = d.getSeconds();
                close();
                accepted();
            }
        }
    }

    onStatusChanged: {
       if (status == DialogStatus.Opening) {
            advButtons.anchors.top = buttons[0].bottom;
            advButtons.anchors.topMargin = 20
        }
    }
}
