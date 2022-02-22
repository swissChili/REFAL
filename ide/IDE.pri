QT += quick quickcontrols2
QT -= core

CONFIG +=

QML_IMPORT_NAME = sh.swisschili.REFAL
QML_IMPORT_MAJOR_VERSION = 1
QML_IMPORT_MINOR_VERSION = 0

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

SOURCES += \
    $$PWD/IdeMain.cpp

HEADERS += \
    $$PWD/IdeMain.h

RESOURCES += \
    $$PWD/resources.qrc
