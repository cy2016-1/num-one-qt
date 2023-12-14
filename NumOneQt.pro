QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts websockets opengl

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Camera.cpp \
    MPU6050Display.cpp \
    MyCube.cpp \
    PointCloudWidget.cpp \
    PointsDrawWindow.cpp \
    Scene.cpp \
    Shader.cpp \
    SingleDataDisplay.cpp \
    WeSocketClient.cpp \
    interactiveacts.cpp \
    main.cpp \
    QtNumOne.cpp

HEADERS += \
    Camera.h \
    MPU6050Display.h \
    MainGUi.h \
    MyCube.h \
    PointCloudWidget.h \
    PointsDrawWindow.h \
    QtNumOne.h \
    Scene.h \
    Shader.h \
    SingleDataDisplay.h \
    WeSocketClient.h \
    interactiveacts.h

FORMS += \
    QtNumOne.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
