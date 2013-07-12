import QtQuick 1.1
import Sailfish.Silica 1.0
import Fahrplan 1.0
import "components"

Page {
    property int searchmode : 0
    id: mainPage

    SilicaListView {
        id: listView
        anchors.fill: parent

        header: PageHeader {
            title: searchmode == 0 ? qsTr("Journey") : qsTr("Time Table")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
            }
            MenuItem {
                text: qsTr("Journey")
                onClicked: {
                    searchmode = 0;
                    updateButtonVisibility();
                }
            }
            MenuItem {
                text: qsTr("Time Table")
                onClicked: {
                    searchmode = 1;
                    updateButtonVisibility();
                }
            }
        }

        model: VisualItemModel {

            SubTitleButton {
                id: departureButton
                titleText: qsTr("Departure Station")
                subTitleText: qsTr("please select")
                onClicked: {
                    console.log("clicked")
                }
            }

            SubTitleButton {
                id: viaButton
                titleText: qsTr("Via Station")
                subTitleText: qsTr("please select")
            }

            SubTitleButton {
                id: arrivalButton
                titleText: qsTr("Arrival Station")
                subTitleText: qsTr("please select")
            }

        }

    }
    function updateButtonVisibility()
    {
        if (!fahrplanBackend.parser.supportsTimeTable()) {
            searchmode = 0;
        }
    }

    FahrplanBackend {
        id: fahrplanBackend
        onParserChanged: {
            console.log("Switching to " + name);
        }
    }
}
