VERSION = 2.0.12

MOC_DIR = tmp
UI_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp

# Make the Version available in the C++ source too
symbian {
    DEFINES += FAHRPLAN_VERSION=\"$$VERSION\"
} else {
    DEFINES += FAHRPLAN_VERSION=\\\"$$VERSION\\\"
}

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

CONFIG += debug
!maemo5: CONFIG += mobility
maemo5: CONFIG += mobility11
MOBILITY += location organizer

TRANSLATIONS += \
    translations/fahrplan_de.ts \
    translations/fahrplan_ru.ts \
    translations/fahrplan_uk.ts

OTHER_FILES += \
    data/fahrplan2.svg \
    data/fahrplan2_64.png \
    data/fahrplan2_80.png \

RESOURCES += \
    translations_res.qrc

# Zlib todo for other systems ugly hack
!unix: INCLUDEPATH += C:/QtSDK/QtSources/4.8.0/src/3rdparty/zlib C:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib
unix:!symbian: LIBS += -lz

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
    src/parser/parser_xmlnri.h \
    src/parser/parser_hafasbinary.h

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
    src/parser/parser_xmlnri.cpp \
    src/parser/parser_hafasbinary.cpp

symbian|simulator {
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
        qtc_packaging/debian_harmattan/manifest.aegis

    DEFINES += Q_WS_MAEMO_6
    DEFINES += MEEGO_EDITION_HARMATTAN
    CONFIG += qt-boostable qdeclarative-boostable
    LIBS += -lmdeclarativecache
    INCLUDEPATH += /usr/include/applauncherd
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

win32|unix:!simulator:!maemo5:!contains(MEEGO_EDITION,harmattan):!symbian {
    SOURCES += src/gui/desktop-test/mainwindow.cpp
    HEADERS += src/gui/desktop-test/mainwindow.h
    FORMS += src/gui/desktop-test/mainwindow.ui
}

# enable booster
symbian {
    #appbooster on symbian is not as great.
    #so we disabled it see https://github.com/smurfy/fahrplan/issues/27
    #CONFIG += symbian_appbooster
}

maemo5|contains(MEEGO_EDITION,harmattan) {
    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic
}

# This hack is needed for lupdate to pick up texts from QML files
translate_hack {
    SOURCES += \
        src/gui/fremantle/*.qml \
        src/gui/fremantle/components/*.qml \
        src/gui/fremantle/hildon/*.qml \
        src/gui/harmattan/*.qml \
        src/gui/harmattan/components/*.qml \
        src/gui/symbian/*.qml \
        src/gui/symbian/components/*.qml
}

# Symbian Deployment Settings
symbian {
    # Do not move this settings to deployment.pri, it seams that it need to be defined in
    # the pro file

    # This UID is from my (smurfy's) offical nokia symbian package range. So
    # if you decide to fork my project and publish the app to the store make sure
    # you request your own uid's or your publish will fail.
#    TARGET.UID3 = 0x2006D175

    # Use this UID for development purposes.
    TARGET.UID3 = 0xE4182966

    # Smart Installer package's UID
    # This UID is from the protected range and therefore the package will
    # fail to install if self-signed. By default qmake uses the unprotected
    # range value if unprotected UID is defined for the application and
    # 0x2002CCCF value if protected UID is given to the application
    DEPLOYMENT.installer_header = 0x2002CCCF

    QMAKE_TARGET_COMPANY = smurfy <maemo@smurfy.de>
    QMAKE_TARGET_PRODUCT = Fahrplan
    QMAKE_TARGET_DESCRIPTION = A Journey planner/Railway Time table for many train lines in Europe and Australia
    QMAKE_TARGET_COPYRIGHT = smurfy <maemo@smurfy.de>

    DEPLOYMENT.display_name = $$QMAKE_TARGET_PRODUCT
    ICON = data/fahrplan2.svg
    CONFIG += qt-components

    TARGET.EPOCHEAPSIZE = 0x20000 0x2000000

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

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()
