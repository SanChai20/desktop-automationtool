QT += quick \
    core \
    widgets \

CONFIG += c++11
CONFIG += debug_and_release

RC_ICONS = jeejeeicon.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        colorimageprovider.cpp \
        covershowwindow.cpp \
        covertransparentwindow.cpp \
        gameapplicationenginewrapper.cpp \
        gamescriptclipboard.cpp \
        gamescriptimageprocessor.cpp \
        gamescriptruninstance.cpp \
        gamescriptruninstancetimeprofiler.cpp \
        gamescriptrunningtask.cpp \
        globalsettingsprofile.cpp \
        main.cpp

RESOURCES += qml.qrc \
             font.qrc \
             js.qrc

INCLUDEPATH += $$PWD/../opencv/opencv/build/include
INCLUDEPATH += $$PWD/../opencv/opencv/build/include/opencv2
INCLUDEPATH += $$PWD/../opencv/opencv/build/install/include
INCLUDEPATH += $$PWD/../opencv/opencv/build/install/include/opencv2

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32 {
LIBS+= -luser32
}

HEADERS += \
    colorimageprovider.h \
    covershowwindow.h \
    covertransparentwindow.h \
    gameapplicationenginewrapper.h \
    gamescriptclipboard.h \
    gamescriptimageprocessor.h \
    gamescriptruninstance.h \
    gamescriptruninstancetimeprofiler.h \
    gamescriptrunningtask.h \
    globalsettingsprofile.h \
    toolheader.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../opencv/opencv/build/x64/vc16/lib/ -lopencv_world470
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../opencv/opencv/build/x64/vc16/lib/ -lopencv_world470d

INCLUDEPATH += $$PWD/../opencv/opencv/build/x64/vc16
DEPENDPATH += $$PWD/../opencv/opencv/build/x64/vc16

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../opencv/opencv/build/x64/vc16/lib/libopencv_world470.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../opencv/opencv/build/x64/vc16/lib/libopencv_world470d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../opencv/opencv/build/x64/vc16/lib/opencv_world470.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../opencv/opencv/build/x64/vc16/lib/opencv_world470d.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../opencv/opencv/build/install/x64/vc16/lib/ -lopencv_xfeatures2d470
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../opencv/opencv/build/install/x64/vc16/lib/ -lopencv_xfeatures2d470d

INCLUDEPATH += $$PWD/../opencv/opencv/build/install/x64/vc16
DEPENDPATH += $$PWD/../opencv/opencv/build/install/x64/vc16

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../opencv/opencv/build/install/x64/vc16/lib/libopencv_xfeatures2d470.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../opencv/opencv/build/install/x64/vc16/lib/libopencv_xfeatures2d470d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../opencv/opencv/build/install/x64/vc16/lib/opencv_xfeatures2d470.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../opencv/opencv/build/install/x64/vc16/lib/opencv_xfeatures2d470d.lib


