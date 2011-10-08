import Qt 4.7
import "hildon"
import "components" as MyComponents

HildonWindow {
    width: 800
    height: 480

    MyComponents.SubTitleButton {
        id: departureButton
        titleText: "Departure Station"
        subTitleText: "please select"
        width: parent.width
        onClicked: {

        }
    }
}
