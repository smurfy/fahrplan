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

import "../components"

Page {
    actionBarVisibility: ChromeVisibility.Overlay
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll

    titleBar: Title {}

    Container {
        SegmentedControl {
            id: modeSelector

            horizontalAlignment: HorizontalAlignment.Fill
            bottomMargin: 0

            Option {
                text: qsTr("Journey Planner") + Retranslate.onLocaleOrLanguageChanged
                selected: true
            }
            Option {
                text: qsTr("Timetable") + Retranslate.onLocaleOrLanguageChanged
                enabled: fahrplan.parser.supportsTimeTable()
            }
        }
        ScrollView {
            scrollRole: ScrollRole.Main

            Container {
                bottomPadding: layoutHandler.bottomOverlayHeight

                Label {
                    text: "TODO"
                }
            }
        }
    }

    actions: [
        ActionItem {
            title: (modeSelector.selectedIndex == 0 ? qsTr("Get Journeys") : qsTr("Show Timetable"))
                   + Retranslate.onLocaleOrLanguageChanged
            imageSource: modeSelector.selectedIndex == 0 ? Qt.resolvedUrl("../icons/journey.png")
                                                         : Qt.resolvedUrl("../icons/timetable.png")
            ActionBar.placement: ActionBarPlacement.Signature
        }
    ]

    attachedObjects: [
        PageLayoutUpdateHandler {
            id: layoutHandler
        },
        QtQuick.Connections {
            target: fahrplan

            onParserChanged: {
                // Switch to Journey Planner tab if
                // timetable is not supported
                if (!fahrplan.parser.supportsTimeTable())
                    modeSelector.selectedIndex = 0;
            }
        }
    ]
}
