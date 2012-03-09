# Add more folders to ship with the application, here

#Fix for Harmattan
exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"):!contains(MEEGO_EDITION,harmattan): {
  MEEGO_VERSION_MAJOR     = 1
  MEEGO_VERSION_MINOR     = 2
  MEEGO_VERSION_PATCH     = 0
  MEEGO_EDITION           = harmattan
}

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += declarative xmlpatterns network

maemo5:QT += maemo5

symbian:TARGET.UID3 = 0xE4182966

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

CONFIG += mobility
MOBILITY += location organizer

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += src/main.cpp \
    src/parser/parser_hafasxml.cpp \
    src/parser/parser_abstract.cpp \
    src/parser/parser_definitions.cpp \
    src/parser/parser_xmlsbbch.cpp \
    src/parser/parser_xmlrejseplanendk.cpp \
    src/parser/parser_xmloebbat.cpp \
    src/parser/parser_131500comau.cpp \
    src/fahrplan.cpp \
    src/fahrplan_backend_manager.cpp \
    src/parser/parser_mobilebahnde.cpp \
    src/fahrplan_favorites_manager.cpp \
    src/calendarthreadwrapper.cpp \
    src/parser/parser_xmlnri.cpp

OTHER_FILES += \
    src/gui/harmattan/MainPage.qml \
    src/gui/harmattan/JourneyResultsPage.qml \
    src/gui/harmattan/JourneyDetailsResultsPage.qml \
    src/gui/harmattan/TimeTableResultsPage.qml \
    src/gui/harmattan/main.qml \
    src/gui/harmattan/components/SubTitleButton.qml \
    src/gui/harmattan/components/StationSelect.qml \
    data/fahrplan2_fremantle.desktop \
    data/fahrplan2_harmattan.desktop \
    data/fahrplan2.svg \
    data/fahrplan2_64.png \
    data/fahrplan2_80.png \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    src/gui/fremantle/main.qml \
    src/gui/fremantle/components/SubTitleButton.qml \
    src/gui/fremantle/hildon/style.js \
    src/gui/fremantle/hildon/hildon.js \
    src/gui/fremantle/hildon/HildonWindow.qml \
    src/gui/fremantle/hildon/HildonTouchListRow.qml \
    src/gui/fremantle/hildon/HildonLabel.qml \
    src/gui/fremantle/hildon/HildonDialogButton.qml \
    src/gui/fremantle/hildon/HildonDialog.qml \
    src/gui/fremantle/hildon/HildonButton.qml \
    src/gui/harmattan/AboutPage.qml

contains(MEEGO_EDITION,harmattan) {
    RESOURCES += harmattan_res.qrc

    DEFINES += Q_WS_MAEMO_6
    DEFINES += MEEGO_EDITION_HARMATTAN
}

maemo5 {
    RESOURCES += fremantle_res.qrc
    HEADERS += src/gui/fremantle/hildon_helper.h
    SOURCES += src/gui/fremantle/hildon_helper.cpp

    DEFINES += Q_WS_MAEMO_5
}

win32 {
    SOURCES += src/gui/desktop-test/mainwindow.cpp
    HEADERS += src/gui/desktop-test/mainwindow.h
    FORMS += src/gui/desktop-test/mainwindow.ui
}

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

# enable booster
CONFIG += qdeclarative-boostable
QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic

HEADERS += \
    src/parser/parser_hafasxml.h \
    src/parser/parser_abstract.h \
    src/parser/parser_definitions.h \
    src/parser/parser_xmlsbbch.h \
    src/parser/parser_xmlrejseplanendk.h \
    src/parser/parser_xmloebbat.h \
    src/parser/parser_131500comau.h \
    src/fahrplan.h \
    src/fahrplan_backend_manager.h \
    src/parser/parser_mobilebahnde.h \
    src/fahrplan_favorites_manager.h \
    src/calendarthreadwrapper.h \
    src/parser/parser_xmlnri.h




















