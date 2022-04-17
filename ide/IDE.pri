QT += quick widgets quickcontrols2
QT -= core

CONFIG += qmltypes

QML_IMPORT_NAME = sh.swisschili.REFAL
QML_IMPORT_MAJOR_VERSION = 1
QML_IMPORT_MINOR_VERSION = 0

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

SOURCES += \
    $$PWD/Cell.cpp \
    $$PWD/CellModel.cpp \
    $$PWD/IdeMain.cpp \
    $$PWD/NbRuntime.cpp \
    $$PWD/Notebook.cpp \
    $$PWD/RecentModel.cpp

HEADERS += \
    $$PWD/Cell.h \
    $$PWD/CellModel.h \
    $$PWD/IdeMain.h \
    $$PWD/NbRuntime.h \
    $$PWD/Notebook.h \
    $$PWD/RecentModel.h

RESOURCES += \
    $$PWD/resources.qrc

INCLUDEPATH += $$PWD
