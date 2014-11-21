#-------------------------------------------------
#
# Project created by QtCreator 2014-08-19T15:38:43
#
#-------------------------------------------------

QT       += core quick gui concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PersonReid
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += link_prl
CONFIG += c++11

TEMPLATE = app

HEADERS += \
    PersonReid.h \
    Descriptor.h \
    JointHistograms.h \
    DistancesFeature.h \
    RegionDescriptor.h \
    DescriptorSet.h

SOURCES += main.cpp \
    PersonReid.cpp \
    Descriptor.cpp \
    DistancesFeature.cpp \
    RegionDescriptor.cpp \
    DescriptorSet.cpp


unix:!macx {
    # CoreLib
    LIBS += -L$$OUT_PWD/../CoreLib/ -lCoreLib
    PRE_TARGETDEPS += $$OUT_PWD/../CoreLib/libCoreLib.a
    INCLUDEPATH += $$PWD/../CoreLib
    DEPENDPATH += $$PWD/../CoreLib

    # OpenCV2
    #INCLUDEPATH += $$(OPENCV2_INCLUDE)
    #DEPENDPATH += $$(OPENCV2_INCLUDE)
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_photo -lopencv_features2d -lopencv_nonfree -lopencv_flann
}

win32 {
    # ensure QMAKE_MOC contains the moc executable path
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

    # Boost
    BOOSTDIR = $$(BOOST_INCLUDEDIR)
    BOOSTLIB = $$(BOOST_LIBRARYDIR)
    !isEmpty(BOOSTDIR) {
        INCLUDEPATH += $$BOOSTDIR
        win32-g++:CONFIG(release, debug|release):LIBS += -L$$BOOSTLIB -lboost_date_time-mgw48-mt-1_56 -lboost_thread-mgw48-mt-1_56
        else:win32-g++:CONFIG(debug, debug|release):LIBS += -L$$BOOSTLIB -lboost_date_time-mgw48-mt-d-1_56 -lboost_thread-mgw48-mt-d-1_56
        else:CONFIG(release, debug|release):LIBS += -L$$BOOSTLIB -lboost_date_time-vc120-mt-1_56 -lboost_thread-vc120-mt-1_56
        else:CONFIG(debug, debug|release):LIBS += -L$$BOOSTLIB -lboost_date_time-vc120-mt-gd-1_56 -lboost_thread-vc120-mt-gd-1_56
    }

    # OpenNI2
    #LIBS += -L$$(OPENNI2_LIB) -lOpenNI2
    INCLUDEPATH += $$(OPENNI2_INCLUDE)
    DEPENDPATH += $$(OPENNI2_INCLUDE)

    # NiTE2
    #LIBS += -L$$(NITE2_LIB) -lNiTE2
    INCLUDEPATH += $$(NITE2_INCLUDE)
    DEPENDPATH += $$(NITE2_INCLUDE)

    # OpenCV2
    INCLUDEPATH += $$(OPENCV2_INCLUDE)
    DEPENDPATH += $$(OPENCV2_INCLUDE)
    CONFIG(release, debug|release):LIBS += -L$$(OPENCV2_LIB) -lopencv_core2410 -lopencv_imgproc2410 -lopencv_highgui2410 -lopencv_objdetect2410 -lopencv_photo2410 -lopencv_features2d2410 -lopencv_nonfree2410 -lopencv_flann2410
    else:CONFIG(debug, debug|release):LIBS += -L$$(OPENCV2_LIB) -lopencv_core2410d -lopencv_imgproc2410d -lopencv_highgui2410d -lopencv_objdetect2410d -lopencv_photo2410d -lopencv_features2d2410d -lopencv_nonfree2410d -lopencv_flann2410d
}

CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release
else:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug
