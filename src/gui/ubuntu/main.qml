import QtQuick 2.0
import Ubuntu.Components 0.1

MainView {
    id: appWindow
    width: units.gu(40)
    height: units.gu(71)

    Tabs {
        Tab {
            title: qsTr("Journey")
            page: PageStack {
                Component.onCompleted: push(journeyPage)
                MainPage {
                    id: journeyPage

                }

            }
        }
        Tab {
            title: qsTr("Time table")
            page: PageStack {
                id: pageStack
                Component.onCompleted: push(timeTablePage)
                MainPage {
                    id: timeTablePage
                    searchmode: 1
                }
            }
        }
    }
}
