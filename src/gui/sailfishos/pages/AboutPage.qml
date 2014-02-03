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
import Sailfish.Silica 1.0
import Fahrplan 1.0
import "../../about.js" as About

Page {
    id: aboutPage
    property FahrplanBackend fahrplanBackend: null

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height
        contentWidth: parent.width

        VerticalScrollDecorator {}

        Item {
            id: column
            width: parent.width
            height: childrenRect.height

            PageHeader {
                id: header

                title: qsTr("About Fahrplan")

                Label {
                    width: parent.width
                    horizontalAlignment: Text.AlignRight
                    anchors {
                        top: parent.top
                        topMargin: 80
                        right: parent.right
                        rightMargin: Theme.paddingMedium
                    }
                    font.pixelSize: Theme.fontSizeTiny
                    textFormat: Text.RichText
                    text: "by smurfy (maemo@smurfy.de)<br>Version: " + fahrplanBackend.version
                }
            }

            Label {
                text: About.aboutText
                wrapMode: Text.WordWrap
                width: parent.width
                textFormat: Text.RichText
                anchors {
                    top: header.bottom
                    topMargin: Theme.paddingLarge
                    leftMargin: Theme.paddingMedium
                    rightMargin: Theme.paddingMedium
                    left: parent.left
                    right: parent.right
                }

                onLinkActivated : Qt.openUrlExternally(link);
            }
        }
    }
}
