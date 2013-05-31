import QtQuick 2.0
import Ubuntu.Components 0.1

MainView {
    id: appWindow
    width: units.gu(40)
    height: units.gu(71)

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
                    id: timeTablePage
                    searchmode: 1
                }
            }
        }
    }
}
