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
import com.nokia.meego 1.1
import Fahrplan 1.0
import "../../about.js" as About

Page {
    id: aboutPage

    tools: aboutToolbar

    Flickable {
        id: flickable

        width: aboutPage.width
        height: aboutPage.height - 40
        flickableDirection: Flickable.VerticalFlick
        clip: true
        contentWidth: aboutPage.width
        contentHeight: aboutContainer.height

        anchors {
            top: parent.top
            topMargin: 20
        }

        Column {
            width: parent.width
            id: aboutContainer

            LabelStyle { id: style }
            spacing: 25

            Label {
                text: qsTr("About Fahrplan")
                color: style.textColor
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                font.bold: true;
                font.pixelSize: 32
            }

            Image {
                source: "qrc:/data/fahrplan2.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Label {
                text: qsTr("Version") + " " + fahrplanBackend.version
                color: style.textColor
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
            }

            Column {
                width: parent.width - 20
                anchors {
                    leftMargin: 10
                    rightMargin: 10
                    left: parent.left
                    right: parent.right
                }

                Label {
                    text: qsTr("Maintainers")
                    font.bold: true
                }

                Repeater {
                    model: About.maintainers

                    delegate: Label {
                        text: About.maintainers[index]
                    }
                }

                Label {
                    /* spacer */
                    text: " "
                }

                Label {
                    text: qsTr("Code Contributors")
                    font.bold: true
                }

                Repeater {
                    model: About.codeContributors
                    delegate: Label {
                        text: About.codeContributors[index]
                    }
                }

                Label {
                    /* spacer */
                    text: " "
                }

                Label {
                    text: qsTr("Translators")
                    font.bold: true
                }

                Repeater {
                    model: About.translators
                    delegate: Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

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
                }

                Label {
                    wrapMode: Text.WordWrap
                    width: parent.width
                    textFormat: Text.RichText
                    onLinkActivated : Qt.openUrlExternally(link)
                    text: About.support
                }

                Label {
                    /* spacer */
                    text: " "
                }

                Label {
                    text: qsTr("License")
                    font.bold: true
                }

                Label {
                    wrapMode: Text.WordWrap
                    width: parent.width
                    textFormat: Text.RichText
                    onLinkActivated : Qt.openUrlExternally(link)
                    text: About.license
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

        ToolIcon {
            id : backIcon;
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }
}
