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
import Fahrplan 1.0

import "../js/style.js" as UI

Container {
    id: root

    property alias title: titleLabel.text
    property alias text: textLabel.text
    property int stationType
    property alias actionSet: actionSet

    function pushStationSelectPage()
    {
        var page = stationSelectPageDefinition.createObject();
        page.stationType = stationType;
        appWindow.push(page);
    }

    Container {
        layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
        topPadding: ui.sdu(UI.spacingTop)
        leftPadding: ui.sdu(UI.paddingLeft)
        bottomPadding: ui.sdu(UI.spacingBottom)
        rightPadding: ui.sdu(UI.paddingRight)

        Container {
            layoutProperties: StackLayoutProperties { spaceQuota: 1 }

            Label {
                id: titleLabel

                bottomMargin: 0
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    fontWeight: FontWeight.W500
                }
            }
            Label {
                id: textLabel

                topMargin: 0
                multiline: true
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle {
                    base: SystemDefaults.TextStyles.PrimaryText
                }
            }
        }
        ImageView {
            imageSource: Qt.resolvedUrl("../icons/ic_next.png")
            filterColor: ui.palette.text
            preferredWidth: ui.sdu(6)
            preferredHeight: ui.sdu(6)
            scalingMethod: ScalingMethod.AspectFit
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Right
            layoutProperties: StackLayoutProperties { spaceQuota: -1 }
        }
    }
    Divider {
        topMargin: 0
        bottomMargin: 0
    }

    contextActions: [
        ActionSet {
            id: actionSet

            title: title

            ActionItem {
                title: qsTr("Search") + Retranslate.onLocaleOrLanguageChanged
                imageSource: Qt.resolvedUrl("../icons/ic_search.png")

                onTriggered: {
                    pushStationSelectPage();
                }
            }
            ActionItem {
                title: qsTr("Clear") + Retranslate.onLocaleOrLanguageChanged
                imageSource: Qt.resolvedUrl("../icons/ic_clear.png")

                onTriggered: {
                    fahrplan.resetStation(stationType);
                }
            }
        }
    ]

    gestureHandlers: [
        TapHandler {
            onTapped: {
                pushStationSelectPage();
            }
        }
    ]

    onTouch: {
        if (event.touchType === TouchType.Down)
            background = ui.palette.plain;
        else if (event.touchType === TouchType.Up || event.touchType === TouchType.Cancel)
            background = ui.palette.background;
    }
}
