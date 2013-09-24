VERSION = 2.0.14

MOC_DIR = tmp
UI_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp

# Make the Version available in the C++ source too
# Also setting QSettings Vendor name
symbian {
    DEFINES += FAHRPLAN_VERSION=\"$$VERSION\"
    DEFINES += FAHRPLAN_SETTINGS_NAMESPACE=\"smurfy\"
}
ubuntu {
    DEFINES += FAHRPLAN_VERSION=\\\"$$VERSION\\\"
    DEFINES += FAHRPLAN_SETTINGS_NAMESPACE=\\\"com.ubuntu.developer.mzanetti.fahrplan2\\\"
}
!ubuntu:!symbian {
    DEFINES += FAHRPLAN_VERSION=\\\"$$VERSION\\\"
    DEFINES += FAHRPLAN_SETTINGS_NAMESPACE=\\\"smurfy\\\"
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


contains(QT_VERSION, ^5\\..\\..*) {
  QT += quick qml xmlpatterns network xml widgets concurrent
  DEFINES += BUILD_FOR_QT5
} else {
  QT += declarative xmlpatterns network xml
}

blackberry: QT += opengl
maemo5:QT += maemo5

!maemo5:!blackberry: CONFIG += mobility
maemo5: CONFIG += mobility11
!blackberry: MOBILITY += location organizer

TRANSLATIONS += \
    translations/fahrplan_de.ts \
    translations/fahrplan_ru.ts \
    translations/fahrplan_uk.ts

OTHER_FILES += \
    data/fahrplan2.svg \
    data/fahrplan2_64.png \
    data/fahrplan2_80.png \
    date/fahrplan2_150.png

RESOURCES += \
    translations_res.qrc

INCLUDEPATH += src
# Zlib todo for other systems ugly hack
!unix: INCLUDEPATH += C:/QtSDK/QtSources/4.8.0/src/3rdparty/zlib C:/QtSDK/QtSources/4.8.1/src/3rdparty/zlib G:/SDK/QTMobile/QtSources/4.8.1/src/3rdparty/zlib
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
    src/calendarthreadwrapper.h \
    src/parser/parser_xmlnri.h \
    src/parser/parser_hafasbinary.h \
    src/fahrplan_parser_thread.h \
    src/fahrplan_calendar_manager.h \
    src/models/stationslistmodel.h \
    src/models/favorites.h \
    src/models/stationsearchresults.h \
    src/models/timetable.h \
    src/models/trainrestrictions.h \
    src/parser/parser_ptvvicgovau.h \
    src/parser/parser_efa.h \
    src/parser/parser_london_efa.h \
    src/parser/parser_ireland_efa.h \
    src/parser/parser_sydney_efa.h \
    src/parser/parser_sf_bay_efa.h

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
    src/calendarthreadwrapper.cpp \
    src/parser/parser_xmlnri.cpp \
    src/parser/parser_hafasbinary.cpp \
    src/fahrplan_parser_thread.cpp \
    src/fahrplan_calendar_manager.cpp \
    src/models/stationslistmodel.cpp \
    src/models/favorites.cpp \
    src/models/stationsearchresults.cpp \
    src/models/timetable.cpp \
    src/models/trainrestrictions.cpp \
    src/parser/parser_ptvvicgovau.cpp \
    src/parser/parser_efa.cpp \
    src/parser/parser_london_efa.cpp \
    src/parser/parser_ireland_efa.cpp \
    src/parser/parser_sydney_efa.cpp \
    src/parser/parser_sf_bay_efa.cpp

# This hack is needed for lupdate to pick up texts from QML files
translate_hack {
    SOURCES += \
        src/gui/fremantle/*.qml \
        src/gui/fremantle/components/*.qml \
        src/gui/fremantle/hildon/*.qml \
        src/gui/harmattan/*.qml \
        src/gui/harmattan/components/*.qml \
        src/gui/harmattan/delegates/*.qml \
        src/gui/harmattan/pages/*.qml \
        src/gui/symbian/*.qml \
        src/gui/symbian/components/*.qml \
        src/gui/symbian/delegates/*.qml \
        src/gui/symbian/pages/*.qml \
        src/gui/ubuntu/*.qml \
        src/gui/ubuntu/components/*.qml
}

contains(MEEGO_EDITION,harmattan) {
    RESOURCES += harmattan_res.qrc

    OTHER_FILES += \
        src/gui/harmattan/main.qml \
        src/gui/harmattan/components/SubTitleButton.qml \
        src/gui/harmattan/components/StationSelect.qml \
        src/gui/harmattan/components/TwoLineLabel.qml \
        src/gui/harmattan/components/SwitchLabel.qml \
        src/gui/harmattan/components/SelectLabel.qml \
        src/gui/harmattan/components/BusyLabel.qml \
        src/gui/harmattan/delegates/StationDelegate.qml \
        src/gui/harmattan/delegates/JourneyDelegate.qml \
        src/gui/harmattan/delegates/JourneyDetailsDelegate.qml \
        src/gui/harmattan/delegates/TimetableEntryDelegate.qml \
        src/gui/harmattan/pages/AboutPage.qml \
        src/gui/harmattan/pages/JourneyDetailsResultsPage.qml \
        src/gui/harmattan/pages/JourneyResultsPage.qml \
        src/gui/harmattan/pages/MainPage.qml \
        src/gui/harmattan/pages/SettingsPage.qml \
        src/gui/harmattan/pages/StationSelectPage.qml \
        src/gui/harmattan/pages/StationsListPage.qml \
        src/gui/harmattan/pages/TimetablePage.qml \
        data/fahrplan2_harmattan.desktop \
        qtc_packaging/debian_harmattan/rules \
        qtc_packaging/debian_harmattan/README \
        qtc_packaging/debian_harmattan/copyright \
        qtc_packaging/debian_harmattan/control \
        qtc_packaging/debian_harmattan/compat \
        qtc_packaging/debian_harmattan/changelog \
        qtc_packaging/debian_harmattan/manifest.aegis

    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic
    DEFINES += BUILD_FOR_HARMATTAN HAVE_DECLARATIVE_CACHE
    CONFIG += qt-boostable qdeclarative-boostable
}

ubuntu: {
    message("Ubuntu build")
    RESOURCES += ubuntu_res.qrc

    OTHER_FILES += \
        src/gui/ubuntu/MainPage.qml \
        src/gui/ubuntu/JourneyResultsPage.qml \
        src/gui/ubuntu/JourneyDetailsResultsPage.qml \
        src/gui/ubuntu/TimeTableResultsPage.qml \
        src/gui/ubuntu/main.qml \
        src/gui/ubuntu/components/Scroller.qml \
        src/gui/ubuntu/components/StationSelect.qml \
        src/gui/ubuntu/components/DatePicker.qml \
        src/gui/ubuntu/components/TimePicker.qml \
        src/gui/ubuntu/AboutPage.qml \
        src/gui/ubuntu/SettingsPage.qml \
        data/fahrplan2_ubuntu.desktop \
        qtc_packaging/ubuntu/changelog \
        qtc_packaging/ubuntu/compat \
        qtc_packaging/ubuntu/control \
        qtc_packaging/ubuntu/copyright \
        qtc_packaging/ubuntu/rules \
        qtc_packaging/ubuntu/source/format

    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic
    DEFINES += BUILD_FOR_UBUNTU
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

    DEFINES += BUILD_FOR_MAEMO_5
    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic
}

blackberry {
    RESOURCES += blackberry_res.qrc

    HEADERS += src/blackberrypositionsource.h
    SOURCES += src/blackberrypositionsource.cpp
    LIBS += -lQtLocationSubset -lbbpim

    OTHER_FILES += \
        bar-descriptor.xml \
        src/gui/symbian/main.qml \
        src/gui/symbian/components/SubTitleButton.qml \
        src/gui/symbian/components/TwoLineLabel.qml \
        src/gui/symbian/components/SwitchLabel.qml \
        src/gui/symbian/components/SelectLabel.qml \
        src/gui/symbian/components/BusyLabel.qml \
        src/gui/symbian/delegates/StationDelegate.qml \
        src/gui/symbian/delegates/JourneyDelegate.qml \
        src/gui/symbian/delegates/JourneyDetailsDelegate.qml \
        src/gui/symbian/delegates/TimetableEntryDelegate.qml \
        src/gui/symbian/pages/AboutPage.qml \
        src/gui/symbian/pages/JourneyDetailsResultsPage.qml \
        src/gui/symbian/pages/JourneyResultsPage.qml \
        src/gui/symbian/pages/MainPage.qml \
        src/gui/symbian/pages/SettingsPage.qml \
        src/gui/symbian/pages/StationSelectPage.qml \
        src/gui/symbian/pages/StationsListPage.qml \
        src/gui/symbian/pages/TimetablePage.qml \
        src/gui/symbian/js/style.js \
        src/gui/symbian/icon/*

    DEFINES += BUILD_FOR_BLACKBERRY

    # For Qt Creator completion to work correctly
    QML_IMPORT_PATH = 3rdparty/bb10-qt-components/imports
}

win32|unix:!simulator:!maemo5:!contains(MEEGO_EDITION,harmattan):!symbian:!blackberry {
    SOURCES += src/gui/desktop-test/mainwindow.cpp
    HEADERS += src/gui/desktop-test/mainwindow.h
    FORMS += src/gui/desktop-test/mainwindow.ui

    DEFINES += BUILD_FOR_DESKTOP
}

symbian|simulator {
    RESOURCES += symbian_res.qrc

    OTHER_FILES += \
        src/gui/symbian/main.qml \
        src/gui/symbian/components/SubTitleButton.qml \
        src/gui/symbian/components/TwoLineLabel.qml \
        src/gui/symbian/components/SwitchLabel.qml \
        src/gui/symbian/components/SelectLabel.qml \
        src/gui/symbian/components/BusyLabel.qml \
        src/gui/symbian/delegates/StationDelegate.qml \
        src/gui/symbian/delegates/JourneyDelegate.qml \
        src/gui/symbian/delegates/JourneyDetailsDelegate.qml \
        src/gui/symbian/delegates/TimetableEntryDelegate.qml \
        src/gui/symbian/pages/AboutPage.qml \
        src/gui/symbian/pages/JourneyDetailsResultsPage.qml \
        src/gui/symbian/pages/JourneyResultsPage.qml \
        src/gui/symbian/pages/MainPage.qml \
        src/gui/symbian/pages/SettingsPage.qml \
        src/gui/symbian/pages/StationSelectPage.qml \
        src/gui/symbian/pages/StationsListPage.qml \
        src/gui/symbian/pages/TimetablePage.qml \
        src/gui/symbian/js/style.js \
        src/gui/symbian/icon/*

    DEFINES += BUILD_FOR_SYMBIAN

    !simulator {
        #
        # enable booster?
        #
        #appbooster on symbian is not as great.
        #so we disabled it see https://github.com/smurfy/fahrplan/issues/27
        #CONFIG += symbian_appbooster

        #
        # Symbian Deployment Settings
        #
        # Do not move this settings to deployment.pri, it seams that it need to be defined in
        # the pro file

        # This UID is from my (smurfy's) offical nokia symbian package range. So
        # if you decide to fork my project and publish the app to the store make sure
        # you request your own uid's or your publish will fail.
        #TARGET.UID3 = 0x2006D175

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
}

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()
