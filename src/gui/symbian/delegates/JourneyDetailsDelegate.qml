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
import "../js/style.js" as Style

Item {
    id: delegateItem

    property ListView listView: ListView.view
    property int timeWidth

    width: ListView.view.width
    height: model.isStation ? model.isTrain ? item_train.height + item_station.height : item_station.height : model.isTrain ? item_train.height : 0

    Item {
        width: parent.width
        height: parent.height
        anchors {
            verticalCenter: parent.verticalCenter
        }

        Rectangle {
             id: item_station

             height: visible ? lbl_station.height + 2 * platformStyle.paddingMedium : 0
             color: appWindow.platformInverted ? Style.listBackgroundOddInverted : Style.listBackgroundOdd
             visible: model.isStation
             anchors {
                 left: parent.left
                 right: parent.right
             }

             Item {
                 id: times

                 width: timeWidth ? timeWidth : childrenRect.width
                 height: parent.height
                 anchors {
                     left: parent.left
                     leftMargin: platformStyle.paddingMedium
                 }

                 Label {
                     // This hack is needed to have correct padding for the line
                     text: model.arrivalTime
                     platformInverted: appWindow.platformInverted
                     anchors {
                         left: parent.left
                         top: parent.top
                         topMargin: platformStyle.paddingSmall
                     }
                 }

                 Label {
                     text: model.departureTime
                     platformInverted: appWindow.platformInverted
                     anchors {
                         left: parent.left
                         bottom: parent.bottom
                         bottomMargin: platformStyle.paddingSmall
                     }
                 }
             }

             Item {
                 id: circle

                 width: childrenRect.width
                 height: parent.height
                 anchors {
                     left: times.right
                     leftMargin: platformStyle.paddingMedium
                 }

                 Rectangle {
                     width: platformStyle.graphicSizeSmall / 4
                     height: (model.isStart || model.isStop) ? parent.height / 2  : parent.height
                     color: "#0d70c5"
                     anchors {
                         left: parent.left
                         leftMargin: (platformStyle.graphicSizeSmall - width) / 2
                         top: parent.top
                         topMargin: (model.isStart) ? parent.height / 2 : 0
                     }
                 }

                 Rectangle {
                     width: height
                     height: platformStyle.graphicSizeSmall
                     radius: platformStyle.graphicSizeSmall / 2
                     smooth: true
                     anchors {
                         left: parent.left
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
                             color: appWindow.platformInverted ? Style.listBackgroundOddInverted : Style.listBackgroundOdd
                         }
                         GradientStop {
                             position: 0.50;
                             color: appWindow.platformInverted ? Style.listBackgroundOddInverted : Style.listBackgroundOdd
                         }
                         GradientStop {
                             position: 0.61;
                             color: appWindow.platformInverted ? Style.listBackgroundOddInverted : Style.listBackgroundOdd
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
             }

             Item {
                id: lbl_station

                height: childrenRect.height
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: circle.right
                    leftMargin: platformStyle.paddingMedium
                    right: parent.right
                    rightMargin: platformStyle.paddingMedium
                }

                Label {
                    id: lbl_station_name

                    text: model.stationName
                    width: parent.width
                    wrapMode: Text.WordWrap
                    platformInverted: appWindow.platformInverted
                }

                Label {
                    id: lbl_station_info

                    text: model.stationInfo
                    width: parent.width
                    color: platformInverted ? platformStyle.colorNormalMidInverted : platformStyle.colorNormalMid
                    platformInverted: appWindow.platformInverted
                    anchors {
                        top: lbl_station_name.bottom
                        topMargin: platformStyle.paddingSmall
                    }
                }
             }
        }

        Rectangle {
            id: item_train

            height: visible ? lbl_train.height + 30 : 0
            color: appWindow.platformInverted ? Style.listBackgroundEvenInverted : Style.listBackgroundEven
            visible: model.isTrain
            anchors {
                top: item_station.bottom
                left: parent.left
                right: parent.right
            }

            Rectangle {
                width: platformStyle.graphicSizeSmall / 4
                height: parent.height
                color: "#0d70c5"
                anchors {
                    left: parent.left
                    leftMargin: times.width + (circle.width - width) / 2 + 2 * platformStyle.paddingMedium
                }
            }

            Label {
                id: lbl_train

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
                wrapMode: Text.WordWrap
                platformInverted: appWindow.platformInverted
                anchors {
                    left: parent.left
                    leftMargin: times.width + circle.width + 3 * platformStyle.paddingMedium
                    right: parent.right
                    rightMargin: platformStyle.paddingMedium
                    verticalCenter: parent.verticalCenter
                }
                onLinkActivated : Qt.openUrlExternally(link)
            }
        }
    }
}
