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
        contentWidth: aboutPage.width
        contentHeight: aboutContainer.height

        anchors {
            fill: parent
            topMargin: platformStyle.paddingLarge
            bottomMargin: 0
        }

        Column {
            width: parent.width
            id: aboutContainer

            spacing: platformStyle.paddingLarge

            Label {
                text: qsTr("About Fahrplan")
                color: style.textColor
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                font.bold: true;
                font.pixelSize: platformStyle.fontSizeLarge
                platformInverted: appWindow.platformInverted
            }

            Image {
                source: "qrc:/data/fahrplan2.svg"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }

                sourceSize {
                    width: platformStyle.graphicSizeLarge
                    height: platformStyle.graphicSizeLarge
                }
            }

            Label {
                text: qsTr("Version %1").arg(fahrplanBackend.version)
                color: platformInverted ? platformStyle.colorNormalMidInverted : platformStyle.colorNormalMid
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Label {
                text: About.copyright
                color: style.textColor
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                platformInverted: appWindow.platformInverted
            }

            Column {
                width: parent.width - (platformStyle.paddingLarge * 2)
                anchors {
                    leftMargin: platformStyle.paddingLarge
                    rightMargin: platformStyle.paddingLarge
                    left: parent.left
                    right: parent.right
                }

                Label {
                    text: qsTr("Maintainers")
                    font.bold: true
                    platformInverted: appWindow.platformInverted
                }

                Repeater {
                    model: About.maintainers

                    delegate: Label {
                        text: About.maintainers[index]
                        platformInverted: appWindow.platformInverted
                    }
                }

                Label {
                    /* spacer */
                    text: " "
                }

                Label {
                    text: qsTr("Code Contributors")
                    font.bold: true
                    platformInverted: appWindow.platformInverted
                }

                Repeater {
                    model: About.codeContributors
                    delegate: Label {
                        text: About.codeContributors[index]
                        platformInverted: appWindow.platformInverted
                    }
                }

                Label {
                    /* spacer */
                    text: " "
                }

                Label {
                    text: qsTr("Translators")
                    font.bold: true
                    platformInverted: appWindow.platformInverted
                }

                Repeater {
                    model: About.translators
                    delegate: Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        platformInverted: appWindow.platformInverted
                        wrapMode: Text.WordWrap
                        text: About.translators[index]
                    }
                }

                Label {
                    /* spacer */
                    text: " "
                }

                Label {
                    text: qsTr("Support")
                    font.bold: true
                    platformInverted: appWindow.platformInverted
                }

                Label {
                    wrapMode: Text.WordWrap
                    width: parent.width
                    textFormat: Text.RichText
                    onLinkActivated : Qt.openUrlExternally(link)
                    text: About.support
                    platformInverted: appWindow.platformInverted
                }

                Label {
                    /* spacer */
                    text: " "
                }

                Label {
                    text: qsTr("License")
                    font.bold: true
                    platformInverted: appWindow.platformInverted
                }

                Label {
                    wrapMode: Text.WordWrap
                    width: parent.width
                    textFormat: Text.RichText
                    onLinkActivated : Qt.openUrlExternally(link)
                    text: About.license
                    platformInverted: appWindow.platformInverted
                }

                Label {
                    /* spacer */
                    text: " "
                }
            }
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
