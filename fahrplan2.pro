# Add more folders to ship with the application, here

QMAKE_TARGET_COMPANY = smurfy <maemo@smurfy.de>
QMAKE_TARGET_PRODUCT = Fahrplan
QMAKE_TARGET_DESCRIPTION = A Journey planner/Railway Time table for many train lines in Europe and Australia
QMAKE_TARGET_COPYRIGHT = smurfy <maemo@smurfy.de>
VERSION = 2.0.9

DEFINES += FAHRPLAN_VERSION=\\\"$$VERSION\\\"

#Fix for Harmattan
exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"):!contains(MEEGO_EDITION,harmattan): {
  MEEGO_VERSION_MAJOR     = 1
  MEEGO_VERSION_MINOR     = 2
  MEEGO_VERSION_PATCH     = 0
  MEEGO_EDITION           = harmattan
}

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += declarative xmlpatterns network xml

maemo5:QT += maemo5

symbian {
    DEPLOYMENT.display_name = $$QMAKE_TARGET_PRODUCT
    ICON = data/fahrplan2.svg
    CONFIG += qt-components

    TARGET.UID3 = 0xE4182966

    # Smart Installer package's UID
    # This UID is from the protected range and therefore the package will
    # fail to install if self-signed. By default qmake uses the unprotected
    # range value if unprotected UID is defined for the application and
    # 0x2002CCCF value if protected UID is given to the application
    #DEPLOYMENT.installer_header = 0x2002CCCF

    # Set correct capabilities:
    # ReadUserData - needed for access to all SSL certificates
    #   and avoid "CSymbianCertificateRetriever: failed to retrieve a certificate, error  -46"
    # WriteUserData - needed to create calendar events
    # NetworkServices - needed for, well, network access :-)
    # Location - needed for searching the nearest station
    TARGET.CAPABILITY += ReadUserData WriteUserData NetworkServices Location

    vendor = \
        "%{\"$$QMAKE_TARGET_COMPANY\"}" \
        ":\"$$QMAKE_TARGET_COMPANY\""
    default_deployment.pkg_prerules += vendor

    # Next lines replace automatic addition of Qt Components dependency to .sis file
    # with the manual one. For some reason, minimal required version is set to 1.0
    # instead of 1.1 so we want to replace it with the correct dependency.
    CONFIG += qt-components_build
    qt-components = \
        "; Default dependency to Qt Quick Components for Symbian library" \
        "(0x200346DE), 1, 1, 0, {\"Qt Quick components for Symbian\"}"
    default_deployment.pkg_prerules += qt-components
}

CONFIG += debug

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
    src/parser/parser_xmlvasttrafikse.cpp \
    src/parser/parser_131500comau.cpp \
    src/fahrplan.cpp \
    src/fahrplan_backend_manager.cpp \
    src/parser/parser_mobilebahnde.cpp \
    src/fahrplan_favorites_manager.cpp \
    src/calendarthreadwrapper.cpp \
    src/parser/parser_xmlnri.cpp

TRANSLATIONS += \
    translations/fahrplan_de.ts

OTHER_FILES += \
    data/fahrplan2.svg \
    data/fahrplan2_64.png \
    data/fahrplan2_80.png

symbian || simulator {
    RESOURCES += symbian_res.qrc

    OTHER_FILES += \
        src/gui/symbian/TimeTableResultsPage.qml \
        src/gui/symbian/MainPage.qml \
        src/gui/symbian/main.qml \
        src/gui/symbian/JourneyResultsPage.qml \
        src/gui/symbian/JourneyDetailsResultsPage.qml \
        src/gui/symbian/AboutPage.qml \
        src/gui/symbian/components/SubTitleButton.qml \
        src/gui/symbian/components/StationSelect.qml \
        src/gui/symbian/js/style.js \
        src/gui/symbian/icon/icon-m-blank.png \
        src/gui/symbian/icon/icon-m-content-calendar.png \
        src/gui/symbian/icon/icon-m-content-calendar-inverse.png \
        src/gui/symbian/icon/icon-m-toolbar-train.svg \
        src/gui/symbian/icon/icon-m-toolbar-train-inverse.svg \
        src/gui/symbian/icon/icon-m-toolbar-help.svg \
        src/gui/symbian/icon/icon-m-toolbar-help-inverse.svg \
        src/gui/symbian/icon/icon-m-toolbar-clock.svg \
        src/gui/symbian/icon/icon-m-toolbar-clock-inverse.svg \
        src/gui/symbian/icon/icon-m-toolbar-close.png \
        src/gui/symbian/icon/icon-m-toolbar-close-inverse.png \
        src/gui/symbian/icon/icon-s-calendar-location-picker.png \
        src/gui/symbian/icon/icon-s-calendar-location-picker-inverse.png
}

contains(MEEGO_EDITION,harmattan) {
    RESOURCES += harmattan_res.qrc

    OTHER_FILES += \
        src/gui/harmattan/MainPage.qml \
        src/gui/harmattan/JourneyResultsPage.qml \
        src/gui/harmattan/JourneyDetailsResultsPage.qml \
        src/gui/harmattan/TimeTableResultsPage.qml \
        src/gui/harmattan/main.qml \
        src/gui/harmattan/components/SubTitleButton.qml \
        src/gui/harmattan/components/StationSelect.qml \
        src/gui/harmattan/AboutPage.qml \
        data/fahrplan2_harmattan.desktop \
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
        qtc_packaging/debian_fremantle/changelog

    DEFINES += Q_WS_MAEMO_6
    DEFINES += MEEGO_EDITION_HARMATTAN
}

maemo5 {
    RESOURCES += fremantle_res.qrc
    HEADERS += src/gui/fremantle/hildon_helper.h
    SOURCES += src/gui/fremantle/hildon_helper.cpp

    OTHER_FILES += \
        data/fahrplan2_fremantle.desktop \
        src/gui/fremantle/main.qml \
        src/gui/fremantle/components/SubTitleButton.qml \
        src/gui/fremantle/hildon/style.js \
        src/gui/fremantle/hildon/hildon.js \
        src/gui/fremantle/hildon/HildonWindow.qml \
        src/gui/fremantle/hildon/HildonTouchListRow.qml \
        src/gui/fremantle/hildon/HildonLabel.qml \
        src/gui/fremantle/hildon/HildonDialogButton.qml \
        src/gui/fremantle/hildon/HildonDialog.qml \
        src/gui/fremantle/hildon/HildonButton.qml

    DEFINES += Q_WS_MAEMO_5
}

win32: !simulator {
    SOURCES += src/gui/desktop-test/mainwindow.cpp
    HEADERS += src/gui/desktop-test/mainwindow.h
    FORMS += src/gui/desktop-test/mainwindow.ui
}

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

# enable booster
symbian {
    CONFIG += symbian_appbooster
} else {
    CONFIG += qdeclarative-boostable
    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic
}

HEADERS += \
    src/parser/parser_hafasxml.h \
    src/parser/parser_abstract.h \
    src/parser/parser_definitions.h \
    src/parser/parser_xmlsbbch.h \
    src/parser/parser_xmlrejseplanendk.h \
    src/parser/parser_xmloebbat.h \
    src/parser/parser_xmlvasttrafikse.h \
    src/parser/parser_131500comau.h \
    src/fahrplan.h \
    src/fahrplan_backend_manager.h \
    src/parser/parser_mobilebahnde.h \
    src/fahrplan_favorites_manager.h \
    src/calendarthreadwrapper.h \
    src/parser/parser_xmlnri.h
