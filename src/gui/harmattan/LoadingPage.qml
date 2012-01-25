import Fahrplan 1.0
import QtQuick 1.1
import com.nokia.meego 1.0
import "components" as MyComponents

Page {
    property alias searchLoading: searchLoading

    tools: loadingToolbar

    Item {
        id: searchLoading
        width:  parent.width
        height: parent.height

        Text {
            id: searchLoadingText
            text: qsTr("Searching...")
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: (parent.height / 2) - searchLoadingText.height - searchIndicator.height
            }
            font {
                pointSize: 24
            }
        }

        BusyIndicator {
            id: searchIndicator
            anchors {
                top: searchLoadingText.bottom;
                topMargin: 20;
                horizontalCenter: parent.horizontalCenter
            }
            running: true
            visible: true

            platformStyle: BusyIndicatorStyle { size: "large" }
        }


        ToolBarLayout {
            id: loadingToolbar

            ToolIcon {
                id : backIcon;
                iconId: "toolbar-back"
                onClicked: {
                    pageStack.pop();
                    fahrplanBackend.parser.cancelRequest();
                }
            }
        }

        FahrplanBackend {
            id: fahrplanBackend
        }

    }
}
