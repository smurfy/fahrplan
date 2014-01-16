import QtQuick 2.0
import Sailfish.Silica 1.0
//import QtMobility.location 1.1
import Fahrplan 1.0
import "../delegates"

Dialog {
    id: stationSelect
    canAccept: false

    property string searchString

    onSearchStringChanged: {
        fahrplanBackend.findStationsByName(searchString);
    }

    property int type: FahrplanBackend.DepartureStation
    property FahrplanBackend fahrplanBackend: null

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height
        contentWidth: parent.width

        VerticalScrollDecorator {}

        Column {
            id: column
            spacing: Theme.paddingLarge
            width: parent.width

            PageHeader {
                title: qsTr("Select station")
            }

            Row {
                width: parent.width

                SearchField {
                    id: searchField
                    width: parent.width - gpsButton.width - 10

                    Binding {
                        target: stationSelect
                        property: "searchString"
                        value: searchField.text.trim()
                    }
                }
                IconButton {
                    id: gpsButton
                    width: icon.width
                    height: parent.height
                    icon.source: "image://theme/icon-m-gps"

                    onClicked: {
                        fahrplanBackend.stationSearchResults.clear();
                        positionSource.start();
                    }
                }
            }

            Column {
                visible: (fahrplanBackend.stationSearchResults.count > 0)
                width: parent.width

                SectionHeader {
                    text: qsTr("Search")
                }

               ListView {
                    model: fahrplanBackend.stationSearchResults
                    width: parent.width
                    height: contentHeight
                    interactive: false

                    currentIndex: -1

                    delegate: StationDelegate {
                        onStationSelected:  {
                            stationSelect.close();
                        }
                    }
                }
            }

            Column {
                width: parent.width
                visible: (fahrplanBackend.favorites.count > 0)

                SectionHeader {
                    text: qsTr("Favorites")
                }

                ListView {
                    model: fahrplanBackend.favorites
                    width: parent.width
                    height: contentHeight
                    interactive: false

                    currentIndex: -1

                    delegate: StationDelegate {
                        onStationSelected:  {
                            stationSelect.close();
                        }
                    }
                }
            }
        }
    }

    onStatusChanged: {
        switch (status) {
            case PageStatus.Activating:
                gpsButton.visible = fahrplanBackend.parser.supportsGps() && (fahrplanBackend.getSettingsValue("enableGps", "true") === "true");
                break;
        }
    }

    /*
    PositionSource {
        id: positionSource
        onPositionChanged: {
            if (positionSource.position.latitudeValid && positionSource.position.longitudeValid) {
                console.log(qsTr("Searching for stations..."));
                positionSource.stop();

                fahrplanBackend.findStationsByCoordinates(positionSource.position.coordinate.longitude, positionSource.position.coordinate.latitude);
            } else {
                console.log(qsTr("Waiting for GPS lock..."));
            }
        }
    }*/
}
