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

import QtQuick 1.1
import com.nokia.symbian 1.1
import Fahrplan 1.0
import "../../about.js" as About

Page {
    id: aboutPage

    tools: aboutToolbar

    Flickable {
        id: flickable

        flickableDirection: Flickable.VerticalFlick
        contentWidth: aboutPage.width - 2 * platformStyle.paddingLarge
        contentHeight: aboutContainer.height + moreText.height + 2 * platformStyle.paddingLarge

        anchors {
            fill: parent
            topMargin: platformStyle.paddingLarge
            leftMargin: platformStyle.paddingLarge
            bottomMargin: 0
            rightMargin: platformStyle.paddingLarge / 3
        }

        Item {
            width: parent.width

            id: aboutContainer

            Image {
                id: logoImg
                source: "qrc:/data/fahrplan2.svg"

                sourceSize {
                    width: platformStyle.graphicSizeLarge
                    height: platformStyle.graphicSizeLarge
                }
            }

            Label {
                text: qsTr("About Fahrplan")
                id: aboutText
                anchors {
                    left: logoImg.right
                    leftMargin: platformStyle.paddingLarge
                    top: parent.top
                }

                font.pixelSize: platformStyle.fontSizeLarge
                platformInverted: appWindow.platformInverted
            }

            Label {
                textFormat: Text.RichText
                text: "by smurfy (maemo@smurfy.de)<br>Version: " + fahrplanBackend.version
                color: platformInverted ? platformStyle.colorNormalMidInverted : platformStyle.colorNormalMid
                anchors {
                    left: logoImg.right
                    leftMargin: platformStyle.paddingLarge
                    top: aboutText.bottom
                }

                font.pixelSize: platformStyle.fontSizeSmall
                platformInverted: appWindow.platformInverted
            }

            height: logoImg.height
        }

        Label {
            id: moreText
            text: About.aboutText
            anchors {
                top: aboutContainer.bottom
                topMargin: platformStyle.paddingLarge
                horizontalCenter: parent.horizontalCenter
            }
            wrapMode: Text.WordWrap
            width: parent.width
            textFormat: Text.RichText
            platformInverted: appWindow.platformInverted

            onLinkActivated : Qt.openUrlExternally(link);
        }
    }

    ToolBarLayout {
        id: aboutToolbar

        ToolButton {
            id : backIcon;
            iconSource: "toolbar-back"
            platformInverted: appWindow.platformInverted
            onClicked: {
                pageStack.pop();
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
        platformInverted: appWindow.platformInverted
    }
}
