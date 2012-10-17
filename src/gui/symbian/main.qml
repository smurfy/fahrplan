import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    MainPage { id: mainPage }

    InfoBanner { id: banner }
}
