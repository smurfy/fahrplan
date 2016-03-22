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

Item {
    id: item_station

    width: parent.width
    height: Math.max(icon_station.height + arrivalTime.height + departureTime.height, lbl_station.height)

    Rectangle {
        width: Theme.iconSizeSmall/4
        height: parent.height
        anchors {
            //horizontalCenter: icon_station.horizontalCenter
            top: parent.top
            topMargin: 0
            left: parent.left
            leftMargin: (timeWidth ? timeWidth : Theme.paddingLarge * 3) + Theme.iconSizeSmall/2 - width/2
        }
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: (model.isStart) ? "transparent" : Theme.highlightColor;
            }
            GradientStop {
                position: 0.38;
                color: (model.isStart) ? "transparent" : Theme.highlightColor;
            }
            GradientStop {
                position: 0.39;
                color: "transparent"
            }
            GradientStop {
                position: 0.50;
                color: "transparent"
            }
            GradientStop {
                position: 0.60;
                color: "transparent"
            }
            GradientStop {
                position: 0.61;
                color: (model.isStop) ? "transparent" : Theme.highlightColor;
            }
            GradientStop {
                position: 1.0;
                color: (model.isStop) ? "transparent" : Theme.highlightColor;
            }
        }
    }

    Rectangle {
        id: icon_station
        width: Theme.iconSizeSmall
        height: Theme.iconSizeSmall
        radius: Theme.iconSizeSmall/2
        anchors {
            left: parent.left
            leftMargin: timeWidth ? timeWidth : Theme.paddingLarge * 3
            verticalCenter: parent.verticalCenter
        }

        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: Theme.highlightColor;
            }
            GradientStop {
                position: 0.38;
                color: Theme.highlightColor;
            }
            GradientStop {
                position: 0.39;
                color: "transparent"
            }
            GradientStop {
                position: 0.50;
                color: "transparent"
            }
            GradientStop {
                position: 0.61;
                color: "transparent"
            }
            GradientStop {
                position: 0.62;
                color: Theme.highlightColor;
            }
            GradientStop {
                position: 1.0;
                color: Theme.highlightColor;
            }
        }
    }

    Label {
        id: arrivalTime
        text: model.arrivalTime
        width: parent.width - 10
        anchors {
            left: parent.left
            leftMargin: 8
            top: parent.top
        }
    }

    Label {
        id: departureTime
        text: model.departureTime
        width: parent.width - 10
        anchors {
            left: parent.left
            leftMargin: 8
            bottom: parent.bottom
        }
    }

    Item {
        id: lbl_station
        anchors {
            left: icon_station.right
            leftMargin: Theme.paddingSmall
            verticalCenter: parent.verticalCenter
            right: parent.right
        }
        height: lbl_station_name.height + lbl_station_info.height

        Label {
            id: lbl_station_name
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            width: parent.width //set explicitly for wrapMode
            truncationMode: TruncationMode.None //explicitly don't truncate, it will be wrapped
            wrapMode: Text.WordWrap
            text: model.stationName
            textFormat: Text.PlainText //it's cheaper
        }

        Label {
            id: lbl_station_info
            anchors {
                left: parent.left
                right: parent.right
                top: lbl_station_name.bottom
            }
            text: model.stationInfo
            width: parent.width
            height: text ? implicitHeight : 0
            color: Theme.secondaryColor
        }
    }

}
