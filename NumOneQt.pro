QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts websockets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MPU6050Display.cpp \
    MyCube.cpp \
    SingleDataDisplay.cpp \
    WeSocketClient.cpp \
    interactiveacts.cpp \
    main.cpp \
    QtNumOne.cpp

HEADERS += \
    MPU6050Display.h \
    MainGUi.h \
    MyCube.h \
    QtNumOne.h \
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
