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

import QtQuick 2.0
import Ubuntu.Components 0.1
import Fahrplan 1.0

Page {
    id: aboutPage

    Flickable {
        id: flickable

        width: aboutPage.width
        height: aboutPage.height - units.gu(4)
        flickableDirection: Flickable.VerticalFlick
        clip: true
        contentWidth: aboutPage.width -  units.gu(6)
        contentHeight: aboutContainer.height + moreText.height +  units.gu(6)

        anchors {
            left: parent.left
            leftMargin:  units.gu(2)
            rightMargin:  units.gu(2)
            right: parent.right
            top: parent.top
            topMargin:  units.gu(2)
        }

        Row {
            id: aboutContainer
            width: parent.width
            height: titleColumn.height
            spacing: units.gu(1)

            UbuntuShape {
                id: logoImg
                height: titleColumn.height
                width: height
                radius: "medium"
                image: Image {
                    source: "qrc:/fahrplan2.svg"
                }
            }

            Column {
                id: titleColumn
                width: parent.width - x
                Label {
                    id: aboutText
                    text: qsTr("About Fahrplan")
                    anchors {
                        left: logoImg.right
                        leftMargin:  units.gu(3)
                    }
                    fontSize: "large"
                }

                Label {
                    textFormat: Text.RichText
                    text: "by smurfy (maemo@smurfy.de)<br>Version: " + fahrplanBackend.version
                    anchors {
                        left: logoImg.right
                    }
                }
            }
        }

        Label{
            id: moreText
            text: "This or prior versions uses code contribution by:<br><i>gri</i>, <i>lorenzph</i>, <i>CaCO3</i>, <i>hcm</i>, <i>thp</i>, <i>qwerty12</i>, <i>qbast</i>, <i>Thomas Fischer</i>, <i>Oleksii Serdiuk</li>, <i>NightShift79</i>, <i>pirast</i>, <i>Michael Zanetti</i>" +
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
                topMargin: units.gu(2)
                horizontalCenter: parent.horizontalCenter
            }
            wrapMode: Text.WordWrap
            width: parent.width
            textFormat: Text.RichText

            onLinkActivated : Qt.openUrlExternally(link);
        }
    }
}
