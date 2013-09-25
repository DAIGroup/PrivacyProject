#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T13:29:56
#
#-------------------------------------------------

QT       += core gui quick multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatasetBrowser
TEMPLATE = app

# Use C++11
QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS = -std=c++11

HEADERS  += \
    DatasetBrowser.h \
    SettingsDialog.h \
    DatasetSelector.h

SOURCES += main.cpp\
    DatasetBrowser.cpp \
    SettingsDialog.cpp \
    DatasetSelector.cpp

OTHER_FILES += \
    qml/main.qml \
    glsl/scene3d.fsh \
    glsl/scene3d.vsh \
    glsl/scene2d.fsh \
    glsl/scene2d.vsh \
    glsl/skeleton.fsh \
    glsl/skeleton.vsh \
    glsl/silhouette.fsh \
    glsl/silhouette.vsh \
    data/MSRDailyActivity3D.xml \
    data/MSRAction3D.xml

RESOURCES += \
    openglunderqml.qrc

FORMS    += \
    DatasetBrowser.ui \
    SettingsDialog.ui \
    DatasetSelector.ui

# Linux
unix:!macx {
    # CoreLib
    LIBS += -L$$OUT_PWD/../CoreLib/ -lCoreLib
    PRE_TARGETDEPS += $$OUT_PWD/../CoreLib/libCoreLib.a
    INCLUDEPATH += $$PWD/../CoreLib
    DEPENDPATH += $$PWD/../CoreLib

    # OpenCV2
    LIBS += -lopencv_core -lopencv_imgproc
    INCLUDEPATH += /usr/include/opencv/
    DEPENDPATH += /usr/include/opencv/
}

# CoreLib Dynamic
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CoreLib/release/ -lCoreLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CoreLib/debug/ -lCoreLib

# CoreLib Static
win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CoreLib/release/CoreLib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CoreLib/debug/CoreLib.lib

win32 {
    INCLUDEPATH += $$PWD

    # CoreLib
    INCLUDEPATH += $$PWD/../CoreLib
    DEPENDPATH += $$PWD/../CoreLib

    # OpenCV2
    contains(QMAKE_TARGET.arch, x86_64):LIBS += -L"C:/opencv/build/x64/vc11/lib" -lopencv_core246 -lopencv_imgproc246
    contains(QMAKE_TARGET.arch, x86):LIBS += -L"C:/opencv/build/x86/vc10/lib" -lopencv_core246 -lopencv_imgproc246
}
