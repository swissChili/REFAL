QT += core

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	AstNode.cpp \
	Evaluator.cpp \
	Function.cpp \
	main.cpp \
	Matcher.cpp \
	Parser.cpp \
        PPrint.cpp \
	StdLib.cpp \
	VarContext.cpp

HEADERS += \
	AstNode.h \
	Evaluator.h \
	Function.h \
	Matcher.h \
	Parser.h \
        PPrint.h \
	StdLib.h \
	Token.h \
	VarContext.h

includeCli {
    SOURCES += Repl.cpp
    HEADERS += Repl.h
}

include(ide/IDE.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

includeCli {
    LIBS += -lreadline
    DEFINES += INCLUDE_CLI
}

TRANSLATIONS += $$files(ts/*.ts)
RESOURCES += translations.qrc
