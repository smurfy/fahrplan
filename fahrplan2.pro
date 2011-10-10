# Add more folders to ship with the application, here

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

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
MOBILITY += Location

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += src/main.cpp \
    src/parser/parser_hafasxml.cpp \
    src/parser/parser_abstract.cpp \
    src/parser/parser_definitions.cpp \
    src/parser/parser_xmlsbbch.cpp \
    src/parser/parser_xmlrejseplanendk.cpp \
    src/parser/parser_xmloebbat.cpp \
    src/fahrplan.cpp \
    src/fahrplan_backend_manager.cpp \
    src/parser/parser_mobilebahnde.cpp \
    src/fahrplan_favorites_manager.cpp

maemo5:SOURCES += src/gui/fremantle/hildon_helper.cpp

win32:SOURCES += src/gui/desktop-test/mainwindow.cpp

OTHER_FILES += \
    src/gui/harmattan/MainPage.qml \
    src/gui/harmattan/JourneyResultsPage.qml \
    src/gui/harmattan/JourneyDetailsResultsPage.qml \
    src/gui/harmattan/LoadingPage.qml \
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
    src/gui/fremantle/hildon/HildonButton.qml

!isEmpty(MEEGO_VERSION_MAJOR):RESOURCES += \
    harmattan_res.qrc

maemo5: RESOURCES += \
    fremantle_res.qrc

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
    src/fahrplan.h \
    src/fahrplan_backend_manager.h \
    src/parser/parser_mobilebahnde.h \
    src/fahrplan_favorites_manager.h

win32:HEADERS += src/gui/desktop-test/mainwindow.h
maemo5:HEADERS += src/gui/fremantle/hildon_helper.h

win32:FORMS += \
    src/gui/desktop-test/mainwindow.ui












