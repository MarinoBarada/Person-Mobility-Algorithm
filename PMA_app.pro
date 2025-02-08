QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customlabel.cpp \
    globals.cpp \
    imagedialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    customlabel.h \
    globals.h \
    imagedialog.h \
    mainwindow.h

FORMS += \
    imagedialog.ui \
    mainwindow.ui

LIBS += "/Program Files (x86)/OCL_SDK_Light/lib/x86/opencl.lib"
LIBS += "/Program Files (x86)/OCL_SDK_Light/lib/x86_64/opencl.lib"

INCLUDEPATH += "/Program Files (x86)/OCL_SDK_Light/include"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
