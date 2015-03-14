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

import "../js/style.js" as UI

Container {
    id: root

    signal clicked();

    function toggleFavorite()
    {
        if (ListItemData.isFavorite)
            ListItem.view.model.removeFromFavorites(ListItem.indexInSection);
        else
            ListItem.view.model.addToFavorites(ListItem.indexInSection);
    }

    Container {
        layout: StackLayout { orientation: LayoutOrientation.LeftToRight }
        topPadding: ui.sdu(UI.spacingTop)
        leftPadding: ui.sdu(UI.paddingLeft)
        bottomPadding: ui.sdu(UI.spacingBottom)
        rightPadding: ui.sdu(UI.paddingRight)
        verticalAlignment: VerticalAlignment.Center

        ImageView {
            imageSource: ListItemData.isFavorite
                         ? Qt.resolvedUrl("../icons/star_full.png")
                         : Qt.resolvedUrl("../icons/star_empty.png")
            filterColor: ui.palette.text
            scalingMethod: ScalingMethod.AspectFit
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties { spaceQuota: -1 }

            gestureHandlers: [
                TapHandler {
                    onTapped: {
                        toggleFavorite();
                    }
                }
            ]
        }

        Container {
            bottomPadding: ui.sdu(UI.spacingBottom)
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties { spaceQuota: 1 }

            Label {
                text: ListItemData.name
                multiline: true
                bottomMargin: 0
                textStyle {
                    base: SystemDefaults.TextStyles.PrimaryText
                }
            }
            Label {
                text: ListItemData.miscInfo
                multiline: true
                topMargin: 0
                visible: text != ""
                textStyle {
                    fontWeight: FontWeight.W300
                }
            }

            gestureHandlers: [
                TapHandler {
                    onTapped: {
                        root.clicked();
                    }
                }
            ]
        }
    }
    Divider {
        topMargin: 0
        bottomMargin: 0
    }

    contextActions: [
        ActionSet {
            title: ListItemData.name

            ActionItem {
                title: qsTr("Select this station") + Retranslate.onLocaleOrLanguageChanged
                imageSource: Qt.resolvedUrl("../icons/ic_select.png")

                onTriggered: {
                    root.clicked();
                }
            }
            ActionItem {
                title: ListItemData.isFavorite ? qsTr("Remove from favorites")
                                                 + Retranslate.onLocaleOrLanguageChanged
                                               : qsTr("Add to favorites")
                                                 + Retranslate.onLocaleOrLanguageChanged
                imageSource: ListItemData.isFavorite
                             ? Qt.resolvedUrl("../icons/star_empty.png")
                             : Qt.resolvedUrl("../icons/star_full.png")

                onTriggered: {
                    toggleFavorite();
                }
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
