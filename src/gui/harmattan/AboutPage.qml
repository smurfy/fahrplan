import QtQuick 1.1
import com.nokia.meego 1.0
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
                source: "file://usr/share/icons/hicolor/scalable/apps/fahrplan2.svg"
            }

            Text {
                text: qsTr("About Fahrplan")
                id: aboutText
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
                color: "darkgrey"
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
            text: "Fahrplan is a journey planner/railway time table for many train lines in europe and australia." +
                  "<br>" +
                  "Current supported backends are <b>bahn.de</b> (germany and europe), <b>sbb.ch</b> (swiss), <b>www.131500.com.au</b> (nsw, australia), <b>rejseplanen.dk</b> (denmark), <b>oebb.at</b> (austria)" +
                  "<br><br>" +
                  "This or prior versions uses code contribution by:<br><i>gri</i>, <i>lorenzph</i>, <i>CaCO3</i>, <i>hcm</i>, <i>thp</i>, <i>qwerty12</i>, <i>qbast</i>" +
                  "<br>" +
                  "Logo by <i>NightShift79</i>" +
                  "<br><br>" +
                  "If you like fahrplan, i would appreciate a donation.<br><br><a href=\"https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DW3LGJ6VEKFVS\">Donate using PayPal</a>" +
                  "<br><br><br><b>License</b><br>" +
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

    FahrplanBackend {
        id: fahrplanBackend
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
