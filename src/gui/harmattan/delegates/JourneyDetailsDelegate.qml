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

Item {
    id: delegateItem

    property ListView listView: ListView.view
    property int timeWidth

    width: ListView.view.width
    height: model.isStation ? model.isTrain ? item_train.height + item_station.height : item_station.height : model.isTrain ? item_train.height : 0

    Item {
        width: parent.width
        height: parent.height

        Item {
             id: item_station

             width: parent.width
             height: visible ? lbl_station.height + 20 : 0
             visible: model.isStation

             Rectangle {
                 color: theme.inverted ? "#111" : "White"
                 anchors.fill: parent
             }

             Rectangle {
                 width: 8
                 height: (model.isStart || model.isStop) ? parent.height / 2  : parent.height
                 color: "#0d70c5"
                 anchors {
                     left: parent.left
                     leftMargin: 81
                     top: parent.top
                     topMargin: (model.isStart) ? parent.height / 2 : 0
                 }
             }

             Rectangle {
                 width: 30
                 height: 30
                 radius: 15
                 anchors {
                     left: parent.left
                     leftMargin: timeWidth ? timeWidth : 70
                     verticalCenter: parent.verticalCenter
                 }

                 gradient: Gradient {
                     GradientStop {
                         position: 0.00;
                         color: "#0d70c5";
                     }
                     GradientStop {
                         position: 0.38;
                         color: "#0d70c5";
                     }
                     GradientStop {
                         position: 0.39;
                         color: theme.inverted ? "#111" : "White"
                     }
                     GradientStop {
                         position: 0.50;
                         color: theme.inverted ? "#111" : "White"
                     }
                     GradientStop {
                         position: 0.61;
                         color: theme.inverted ? "#111" : "White"
                     }
                     GradientStop {
                         position: 0.62;
                         color: "#0d70c5";
                     }
                     GradientStop {
                         position: 1.0;
                         color: "#0d70c5";
                     }
                 }
             }

             Label {
                 text: model.arrivalTime
                 width: parent.width - 10
                 anchors {
                     left: parent.left
                     leftMargin: 8
                     top: parent.top
                 }
             }

             Label {
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

                width: (parent.width - 110)
                height: lbl_station_name.height + lbl_station_info.height
                anchors {
                    left: parent.left
                    leftMargin: 110
                    verticalCenter: parent.verticalCenter
                }

                Label {
                    id: lbl_station_name

                    text: model.stationName
                    width: parent.width
                }

                Label {
                    id: lbl_station_info

                    text: model.stationInfo
                    width: parent.width
                    color: "DarkGrey"
                    anchors {
                        top: lbl_station_name.bottom
                    }
                }
             }
        }

        Item {
            id: item_train

            width: parent.width
            height: visible ? lbl_train.height + 30 : 0
            visible: model.isTrain
            anchors {
                top: item_station.bottom
            }

            Rectangle {
                color: theme.inverted ? "#333" : "LightGrey"
                anchors.fill: parent
            }

            Rectangle {
                width: 8
                height: parent.height
                color: "#0d70c5"
                anchors {
                    left: parent.left
                    leftMargin: 81
                }
            }

            Label {
                id: lbl_train

                width: (parent.width  - 110)
                text: {
                    var result;
                    if (model.trainDirection.length > 0) {
                        //: As in "%1 in direction %2"
                        result = qsTr("%1 to %2", "Direction").arg("<b>" + model.trainName + "</b>")
                                                              .arg(model.trainDirection);
                    } else {
                        result = "<b>" + model.trainName + "</b>";
                    }
                    if (model.trainInfo)
                        result += "<br /><i>" + model.trainInfo + "</i>";

                    return result;
                }
                anchors {
                    left: parent.left
                    leftMargin: 110
                    verticalCenter: parent.verticalCenter
                }
                onLinkActivated : Qt.openUrlExternally(link)
            }
        }
    }
}
