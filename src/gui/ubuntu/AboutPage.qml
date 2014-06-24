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
import "../about.js" as About

Page {
    id: aboutPage

    Flickable {
        id: flickable
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        clip: true
        contentWidth: width
        contentHeight: contentColumn.height + units.gu(2)

        Column {
            id: contentColumn
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: units.gu(1)
            height: childrenRect.height
            spacing: units.gu(1)

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
                        width: parent.width
                        fontSize: "large"
                    }

                    Label {
                        textFormat: Text.RichText
                        text: "by smurfy (maemo@smurfy.de)<br>Version: " + fahrplanBackend.version
                        width: parent.width
                    }
                }
            }

            Label{
                id: moreText
                text: About.aboutText
                anchors {
                    left: parent.left
                    right: parent.right
                }
                wrapMode: Text.WordWrap
                width: parent.width
                textFormat: Text.RichText

                onLinkActivated : Qt.openUrlExternally(link);
            }
        }
    }
}
