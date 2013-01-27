import Fahrplan 1.0
import QtQuick 1.1
import com.nokia.symbian 1.1
import "components"
import "js/style.js" as Style

Page {
    property alias fahrplanBackend: fahrplanBackend
    property alias searchResults: searchResults
    property alias journeyStationsTitleText: journeyStations.text
    property alias searchIndicatorVisible: searchIndicator.visible

    id: searchResultsPage

    tools: journeyResultsToolbar
    Item {
        id: searchResults

        width:  parent.width
        height: parent.height

        Item {
            id: titleBar

            height: journeyStations.height + journeyDate.height + 2 * platformStyle.paddingMedium
            width: parent.width

            Label {
                id: journeyStations
                text: ""
                font.pixelSize: privateStyle.statusBarHeight
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingMedium
                    right: parent.right
                    rightMargin: platformStyle.paddingMedium
                    top: parent.top
                    topMargin: platformStyle.paddingMedium
                }
                width: parent.width
                wrapMode: Text.WordWrap
                platformInverted: appWindow.platformInverted
            }

            Label {
                id: journeyDate
                color: platformInverted ? platformStyle.colorNormalMidInverted : platformStyle.colorNormalMid
                anchors {
                    left: parent.left
                    leftMargin: platformStyle.paddingMedium
                    top: journeyStations.bottom
                    topMargin: platformStyle.paddingMedium
                }
                width: parent.width
                visible: !searchIndicatorVisible
                platformInverted: appWindow.platformInverted
            }
        }

        BusyIndicator {
            id: searchIndicator
            anchors.centerIn: parent
            running: true
            visible: false

            width: platformStyle.graphicSizeLarge; height: width
            platformInverted: appWindow.platformInverted
        }

        Row {
            id: listHead

            height: childrenRect.height
            visible: !searchIndicator.visible
            spacing: platformStyle.paddingMedium

            anchors {
                top: titleBar.bottom
                topMargin: platformStyle.paddingMedium
                left: parent.left
                leftMargin: platformStyle.paddingMedium
                right: parent.right
                rightMargin: platformStyle.paddingMedium
            }

            Label {
                text: qsTr("Dep.")
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }

            Label {
                text: qsTr("Arr.")
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }

            Label {
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Dur.")
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }

            Label {
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Trans.")
                width: (parent.width - 3 * listHead.spacing) / 4
                platformInverted: appWindow.platformInverted
            }
        }

        ListView {
            id: listView
            anchors {
                top: listHead.bottom
                topMargin: platformStyle.paddingMedium
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            model: journeyResultModel
            delegate: journeyResultDelegate
            clip: true
            visible: !searchIndicator.visible
        }

        ScrollDecorator {
            flickableItem: listView
            platformInverted: appWindow.platformInverted
        }
    }

    Component {
        id: journeyResultDelegate

        Rectangle {
            id: delegateItem

            width: listView.width
            height: labels.height + 2 * platformStyle.paddingMedium
            color: {
                if (appWindow.platformInverted)
                    return itemNum % 2 ? Style.listBackgroundOddInverted : Style.listBackgroundEvenInverted;
                else
                    return itemNum % 2 ? Style.listBackgroundOdd : Style.listBackgroundEven;
            }

            Rectangle {
                id: background
                anchors.fill: parent
                color: Style.listBackgroundHighlight
                visible: mouseArea.pressed
            }

            MouseArea {
                id: mouseArea
                anchors.fill: background
                onClicked: {
                    detailsResultsPage.titleText = qsTr("Loading details");
                    detailsResultsPage.subTitleText = qsTr("please wait...");
                    detailsResultsPage.subTitleText2 = "";
                    detailsResultsPage.searchIndicatorVisible = true;
                    pageStack.push(detailsResultsPage);
                    fahrplanBackend.parser.getJourneyDetails(id);
                }
            }

            Column {
                id: labels

                height: childrenRect.height
                spacing: platformStyle.paddingSmall
                anchors {
                    top: parent.top
                    topMargin: platformStyle.paddingMedium
                    left: parent.left
                    leftMargin: platformStyle.paddingMedium
                    right: parent.right
                    rightMargin: platformStyle.paddingMedium
                }

                Row {
                    width: parent.width
                    height: childrenRect.height
                    spacing: platformStyle.paddingMedium

                    Label {
                        text: departureTime
                        width: (parent.width - 3 * listHead.spacing) / 4
                        platformInverted: appWindow.platformInverted
                    }

                    Label {
                        text: arrivalTime
                        width: (parent.width - 3 * listHead.spacing) / 4
                        platformInverted: appWindow.platformInverted
                    }

                    Label {
                        horizontalAlignment: Text.AlignHCenter
                        text: duration
                        width: (parent.width - 3 * listHead.spacing) / 4
                        platformInverted: appWindow.platformInverted
                    }

                    Label {
                        horizontalAlignment: Text.AlignHCenter
                        text: transfers
                        width: (parent.width - 3 * listHead.spacing) / 4
                        platformInverted: appWindow.platformInverted
                    }
                }

                Label {
                    text: trainType
                    width: parent.width
                    platformInverted: appWindow.platformInverted
                }

                Label {
                    visible: miscInfo !== ""
                    text: miscInfo
                    width: parent.width
                    font.bold: true
                    platformInverted: appWindow.platformInverted
                }
            }
        }
    }

    ListModel {
        id: journeyResultModel
    }

    FahrplanBackend {
        id: fahrplanBackend
        onParserJourneyResult: {
            console.log("Got results");
            console.log(result.count);

            searchIndicatorVisible = false;

            journeyStations.text = result.viaStation.length == 0 ? qsTr("<b>%1</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation) : qsTr("<b>%1</b> via <b>%3</b> to <b>%2</b>").arg(result.departureStation).arg(result.arrivalStation).arg(result.viaStation)

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
                    "miscInfo": item.miscInfo,
                    "itemNum" : i
                });
            }
        }
    }

    JourneyDetailsResultsPage {
        id: detailsResultsPage
    }

    ToolBarLayout {
        id: journeyResultsToolbar

        ToolButton {
            id : backIcon;
            iconSource: "toolbar-back"
            platformInverted: appWindow.platformInverted
            onClicked: {
                pageStack.pop();
                fahrplanBackend.parser.cancelRequest();
            }
        }

        ToolButton {
            text:qsTr("Earlier")
            visible: !searchIndicatorVisible
            platformInverted: appWindow.platformInverted
            onClicked: {
                searchIndicatorVisible = true;
                fahrplanBackend.parser.searchJourneyEarlier();
            }
        }
        ToolButton {
            text:qsTr("Later")
            visible: !searchIndicatorVisible
            platformInverted: appWindow.platformInverted
            onClicked: {
                searchIndicatorVisible = true;
                fahrplanBackend.parser.searchJourneyLater();
            }
        }
    }
}
