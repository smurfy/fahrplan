import QtQuick 2.0
import Sailfish.Silica 1.0
import Fahrplan 1.0
import "../delegates"

Page {
    id: journeyResultsPage

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
                id: journeyDesc
                Label {
                    id: journeyDate
                    width: parent.width
                    horizontalAlignment: Text.AlignRight
                    anchors {
                        top: parent.top
                        topMargin: 80
                        right: parent.right
                        rightMargin: 10
                    }
                }
            }

            Row {
                id: listHead
                width: parent.width

                anchors {
                    leftMargin: 10
                    rightMargin: 10
                    left: parent.left
                    right: parent.right
                }

                Label {
                    text: qsTr("Dep.")
                    width: (parent.width - 3) / 4
                }

                Label {
                    text: qsTr("Arr.")
                    width: (parent.width - 3) / 4
                }

                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Dur.")
                    width: (parent.width - 3) / 4
                }

                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Trans.")
                    width: (parent.width - 3) / 4
                }
            }

            ListView {
                id: listView
                width: parent.width
                height: contentHeight
                model: journeyResultModel
                delegate: JourneyDelegate {

                }
            }
        }
    }


    onStatusChanged: {
        switch (status) {
            case PageStatus.Activating:
                indicator.visible = true;
                journeyDesc.title = qsTr("Searching...");
                fahrplanBackend.searchJourney();
                break;
            case PageStatus.Deactivating:
                fahrplanBackend.cancelRequest();
                break;
        }
    }

    BusyIndicator {
        id: indicator
        anchors.centerIn: parent
    }

    ListModel {
        id: journeyResultModel
    }

    Connections {
        target: fahrplanBackend

        onParserJourneyResult: {
            console.log("Got results");
            console.log(result.count);
            indicator.visible = false;

            journeyDesc.title = result.viaStation.length == 0 ? qsTr("<b>%1</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation) : qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation).arg(result.viaStation)

            journeyDate.text = result.timeInfo;

            journeyResultModel.clear();
            for (var i = 0; i < result.count; i++) {
                var item = result.getItem(i);
                journeyResultModel.append({
                    "id": item.id,
                    "departureTime": item.departureTime,
                    "arrivalTime": item.arrivalTime,
                    "trainType": item.trainType,
                    "duration": item.duration,
                    "transfers": item.transfers,
                    "miscInfo": item.miscInfo
                });
            }
        }
    }
}
