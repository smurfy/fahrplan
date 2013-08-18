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
            text: "This or prior versions uses code contribution by:<br><i>gri</i>, <i>lorenzph</i>, <i>CaCO3</i>, <i>hcm</i>, <i>thp</i>, <i>qwerty12</i>, <i>qbast</i>, <i>Thomas Fischer</i>, <i>Oleksii Serdiuk</li>, <i>NightShift79</i>, <i>pirast</i>" +
                  "<br><br>" +
                  "If you like fahrplan, i would appreciate a donation.<br><br><a href=\"https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DW3LGJ6VEKFVS\">Donate using PayPal</a>" +
                  "<br><br>" +
                  "If you encounter bugs or have feature requests visit the <br><br><a href=\"https://github.com/smurfy/fahrplan/issues\">Fahrplan issue tracker on GitHub</a>" +
                  "<br><br><b>Privacy Policy</b><br>" +
                  "<a href=\"https://github.com/smurfy/fahrplan/wiki/Privacy-Policy-for-fahrplan\">View online</a>" +
                  "<br><br><b>License</b><br>" +
                  "This program is free software; you can redistribute it and/or modify
            it under the terms of the GNU General Public License as published by
            the Free Software Foundation; either version 2 of the License, or
            (at your option) any later version.<br>
            <br>
            This program is distributed in the hope that it will be useful,
            but WITHOUT ANY WARRANTY; without even the implied warranty of
            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
            GNU General Public License for more details.<br>
            <br>
            You should have received a copy of the GNU General Public License
            along with this program. If not, see <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>."

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
