import Fahrplan 1.0 as Fahrplan
import QtQuick 1.1
import com.meego 1.0

Page {
    property alias fahrplanBackend: fahrplanBackend
    property alias searchLoading: searchLoading
    property alias searchResults: searchResults

    Item {
        id: searchLoading

        width:  parent.width
        height: parent.height

        Text {
            id: searchLoadingText
            text: "Searching..."
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
    }

    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        visible: false;

        Fahrplan.Backend {
            id: fahrplanBackend
            onParserJourneyResult: {
                searchResults.visible = true;
                searchLoading.visible = false;
                console.log("Got results");
            }
        }

        ToolBarLayout {
            id:stationSelectToolbar

            ToolIcon {
                id : backIcon;
                iconId: "toolbar-back"
                onClicked: {
                    pageStack.pop();
                }
            }
        }
    }
}
