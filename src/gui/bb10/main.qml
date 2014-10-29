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

import bb.cascades 1.3
import bb.system 1.3
import QtQuick 1.0
import Fahrplan 1.0

import "pages"

NavigationPane {
    id: appWindow

    MainPage {}

    Menu.definition: MenuDefinition {
        id: menuDefinition

        actions: [
            ActionItem {
                title: qsTr("About") + Retranslate.onLocaleOrLanguageChanged
                imageSource: Qt.resolvedUrl("icons/ic_info.png");

                onTriggered: {
                    Application.menuEnabled = false;
                    appWindow.push(aboutPageDefinition.createObject());
                }
            }
        ]
    }

    attachedObjects: [
        ComponentDefinition {
            id: aboutPageDefinition
            AboutPage {}
        },
        FahrplanBackend {
            id: fahrplan

            onParserErrorOccured: {
                console.debug(msg);
                toast.body = msg;
                toast.show();
            }
        },
        SystemToast {
            id: toast

            position: SystemUiPosition.TopCenter
        }
    ]

    onPopTransitionEnded: {
        // Destroy popped Page once back transition has ended
        page.destroy();
        Application.menuEnabled = true;
    }
}
