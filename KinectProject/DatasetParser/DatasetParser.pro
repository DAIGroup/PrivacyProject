#-------------------------------------------------
#
# Project created by QtCreator 2013-03-27T13:34:59
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = DatasetParser
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += link_prl
CONFIG += c++11

TEMPLATE  = app

SOURCES += main.cpp


unix:!macx {
    # CoreLib
    LIBS += -L$$OUT_PWD/../CoreLib/ -lCoreLib
    PRE_TARGETDEPS += $$OUT_PWD/../CoreLib/libCoreLib.a
    INCLUDEPATH += $$PWD/../CoreLib
    DEPENDPATH += $$PWD/../CoreLib
}

win32 {
    load(moc)
    INCLUDEPATH += $$PWD

    # CoreLib
    INCLUDEPATH += $$PWD/../CoreLib
    DEPENDPATH += $$PWD/../CoreLib

    # CoreLib Dynamic
    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CoreLib/release/ -lCoreLib
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CoreLib/debug/ -lCoreLib

    # CoreLib Static
    CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CoreLib/release/CoreLib.lib
    else:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CoreLib/debug/CoreLib.lib
}
