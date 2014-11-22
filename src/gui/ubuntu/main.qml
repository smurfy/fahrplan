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

import QtQuick 2.3
import Ubuntu.Components 1.1
import Fahrplan 1.0

MainView {
    id: appWindow
    width: units.gu(40)
    height: units.gu(71)

    FahrplanBackend {
        id: fahrplanBackend
    }

    useDeprecatedToolbar: false
    Tabs {
        onCurrentPageChanged: {
            while (journeyPageStack.depth > 1) {
                journeyPageStack.pop()
            }
            while (timeTablePageStack.depth > 1) {
                timeTablePageStack.pop()
            }
        }

        Tab {
            title: qsTr("Journey")
            page: PageStack {
                id: journeyPageStack
                Component.onCompleted: push(journeyPage)
                MainPage {
                    title: qsTr("Journey")
                    id: journeyPage
                }
            }
        }
        Tab {
            title: qsTr("Time table")
            page: PageStack {
                id: timeTablePageStack
                Component.onCompleted: push(timeTablePage)
                MainPage {
                    title: qsTr("Time table")
                    id: timeTablePage
                    searchmode: 1
                }
            }
        }
    }
}
