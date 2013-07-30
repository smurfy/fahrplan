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
import Ubuntu.Components.ListItems 0.1 as ListItems

Item {
    id: root
    property int min: 0
    property int max: 10
    property variant value: model.get(listView.currentIndex) !== undefined ? model.get(listView.currentIndex).modelData : ""
    property alias model: listView.model
    property alias labelText: label.text
    property alias currentIndex: listView.currentIndex

    ListModel {
        id: defaultModel
    }

    Component.onCompleted: {
        var oldIndex = currentIndex
        for (var i = 0; i < (max+1)-min; ++i) {
            defaultModel.append({modelData: root.min + i})
        }
        listView.highlightMoveDuration = 0
        currentIndex = oldIndex
        listView.highlightMoveDuration = 300
    }

    onMinChanged: {
        if (defaultModel.get(0) === undefined) {
            return;
        }

        var oldMin = defaultModel.get(0).modelData

        while (oldMin > min) {
            defaultModel.insert(0, {modelData: --oldMin })
        }
        while (oldMin < min) {
            defaultModel.remove(0)
            ++oldMin
        }
    }

    onMaxChanged: {
        if (defaultModel.get(defaultModel.count - 1) === undefined) {
            return;
        }

        var oldMax = defaultModel.get(defaultModel.count - 1).modelData

        while (max < oldMax) {
            defaultModel.remove(defaultModel.count - 1);
            --oldMax;
        }
        while (max > oldMax) {
            defaultModel.insert(defaultModel.count, {modelData: ++oldMax})
        }
    }

    Item {
        id: labelRect
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
        }
        height: units.gu(5)

        Label {
            id: label
            anchors.centerIn: parent
        }
        ListItems.Divider {
            anchors {
                left: parent.left
                bottom: parent.bottom
                right: parent.right
            }
        }
    }

    PathView {
        id: listView
        model: defaultModel
        anchors.fill: parent
        anchors.topMargin: labelRect.height
        pathItemCount: listView.height / highlightItem.height + 1
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        clip: true

        delegate: ListItems.Standard {
            width: parent.width
            highlightWhenPressed: false
            Label {
                anchors.centerIn: parent
                text: modelData
            }
            onClicked: listView.currentIndex = index
        }
        property int contentHeight: pathItemCount * highlightItem.height
        path: Path {
            startX: listView.width / 2; startY: -(listView.contentHeight - listView.height) / 2
            PathLine { x: listView.width / 2; y: listView.height + (listView.contentHeight - listView.height) / 2 }
        }
        highlight: Rectangle {
            width: parent.width
            height: units.gu(6)
            property color baseColor: "#dd4814"
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: Qt.lighter(baseColor, 1.3);
                }
                GradientStop {
                    position: 1.0;
                    color: baseColor;
                }
            }
        }
        ListItems.Divider {
            anchors {
                left: parent.left
                bottom: parent.bottom
                right: parent.right
            }
        }
    }
}
