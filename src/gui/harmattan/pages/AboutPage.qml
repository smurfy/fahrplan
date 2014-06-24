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
        contentWidth: aboutPage.width - 60
        contentHeight: aboutContainer.height + moreText.height + 60

        anchors {
            left: parent.left
            leftMargin: 20
            rightMargin: 20
            right: parent.right
            top: parent.top
            topMargin: 20
        }

        Item {
            width: parent.width

            id: aboutContainer

            Image {
                id: logoImg
                source: "qrc:/data/fahrplan2.svg"
            }

            LabelStyle { id: style }

            Text {
                id: aboutText
                text: qsTr("About Fahrplan")
                color: style.textColor
                anchors {
                    left: logoImg.right
                    leftMargin: 30
                    top: parent.top
                }

                font {
                    pointSize: 24
                }
            }

            Text {
                textFormat: Text.RichText
                text: "by smurfy (maemo@smurfy.de)<br>Version: " + fahrplanBackend.version
                color: theme.inverted ? "lightgrey" : "darkgrey"
                anchors {
                    left: logoImg.right
                    leftMargin: 30
                    top: aboutText.bottom
                }

                font {
                    pointSize: 14
                }
            }

            height: logoImg.height
        }

        Text {
            id: moreText
            color: style.textColor
            text: About.aboutText
            anchors {
                top: aboutContainer.bottom
                topMargin: 20
                horizontalCenter: parent.horizontalCenter
            }
            wrapMode: Text.WordWrap
            width: parent.width
            textFormat: Text.RichText

            font {
                pointSize: 14
            }

            onLinkActivated : Qt.openUrlExternally(link);
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
