/****************************************************************************
**
**  This file is a part of Fahrplan.
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

import bb.cascades 1.3
import QtQuick 1.0 as QtQuick
import QtMobility.location 1.1

import "../delegates"
import "../js/style.js" as UI

Page {
    property int stationType

    actionBarVisibility: ChromeVisibility.Overlay
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll

    function findByName()
    {
        if (searchTimer.running)
            searchTimer.stop();

        if (input.text.length == 0)
            return;

        activityText.text = qsTr("Searching for stations...");
        activity.start();
        fahrplan.findStationsByName(input.text);
        tabs.selectedIndex = 1;
    }

    function findByGps()
    {
        if (searchTimer.running)
            searchTimer.stop();

        input.text = "";
        activityText.text = qsTr("Getting current location...");
        activity.start();
        fahrplan.stationSearchResults.clear();
        positionSource.update();
        tabs.selectedIndex = 1;
    }

    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
                leftPadding: ui.sdu(UI.paddingLeft)
                rightPadding: ui.sdu(UI.paddingRight)

                TextField {
                    id: input

                    hintText: qsTr("Search for Station...") + Retranslate.onLocaleOrLanguageChanged
                    layoutProperties: StackLayoutProperties { spaceQuota: 100 }
                    verticalAlignment: VerticalAlignment.Center
                    input {
                        flags: TextInputFlag.AutoCorrectionOff | TextInputFlag.WordSubstitutionOff
                        submitKey: SubmitKey.Search

                        onSubmitted: {
                            findByName();
                        }
                    }

                    onTextChanging: {
                        if (text == "") {
                            searchTimer.stop();
                            fahrplan.stationSearchResults.clear();
                        } else {
                            searchTimer.restart();
                        }
                    }
                }
            }
        }
    }

    Container {
        SegmentedControl {
            id: tabs

            selectedIndex: UI.stationSelectPageSelectedTab
            horizontalAlignment: HorizontalAlignment.Fill
            bottomMargin: 0

            Option {
                text: qsTr("Favorites") + Retranslate.onLocaleOrLanguageChanged
            }
            Option {
                text: qsTr("Search") + Retranslate.onLocaleOrLanguageChanged
            }

            onSelectedIndexChanged: {
                UI.stationSelectPageSelectedTab = selectedIndex;
            }
        }

        Container {
            layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
            topPadding: ui.sdu(UI.paddingTop)
            leftPadding: ui.sdu(UI.paddingLeft)
            rightPadding: ui.sdu(UI.paddingRight)
            horizontalAlignment: HorizontalAlignment.Center
            visible: activity.running

            ActivityIndicator {
                id: activity
            }
            Label {
                id: activityText
                verticalAlignment: VerticalAlignment.Center
            }
        }

        ListView {
            id: favorites

            property variant model: tabs.selectedIndex == 0 ? fahrplan.favorites
                                                            : fahrplan.stationSearchResults

            function selectStation(index)
            {
                model.selectStation(stationType, index);
                pop();
            }

            dataModel: model.dataModel
            bottomPadding: layoutHandler.bottomOverlayHeight
            scrollRole: ScrollRole.Main

            listItemComponents: [
                ListItemComponent {
                    StationDelegate {
                        onClicked: {
                            ListItem.view.selectStation(ListItem.indexInSection);
                        }
                    }
                }
            ]
        }
    }

    actions: [
        ActionItem {
            title: qsTr("Search") + Retranslate.onLocaleOrLanguageChanged
            imageSource: Qt.resolvedUrl("../icons/ic_search.png")
            ActionBar.placement: ActionBarPlacement.Signature

            onTriggered: {
                if (!input.focused)
                    input.requestFocus();
                findByName();
            }
        },
        ActionItem {
            title: qsTr("Find nearby") + Retranslate.onLocaleOrLanguageChanged
            imageSource: Qt.resolvedUrl("../icons/ic_location.png")
            enabled: fahrplan.parser.supportsGps()
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                findByGps();
            }
        }
    ]

    attachedObjects: [
        PageLayoutUpdateHandler {
            id: layoutHandler
        },
        PositionSource {
            id: positionSource

            onPositionChanged: {
                if (positionSource.position.latitudeValid
                        && positionSource.position.longitudeValid) {
                    activityText.text = qsTr("Searching for stations...");
                    fahrplan.findStationsByCoordinates(positionSource.position.coordinate.longitude,
                                                       positionSource.position.coordinate.latitude);
                } else {
                    // Waiting for lock
                }
            }
        },
        QtQuick.Connections {
            target: fahrplan

            onParserStationsResult: {
                activity.stop();
            }
        },
        QtQuick.Timer {
            id: searchTimer

            interval: 1000

            onTriggered: {
                findByName();
            }
        }
    ]
}
