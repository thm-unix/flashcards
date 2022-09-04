QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    editor.cpp \
    export.cpp \
    main.cpp \
    choosestack.cpp \
    settings.cpp \
    singleeditor.cpp \
    stackviewer.cpp

HEADERS += \
    about.h \
    choosestack.h \
    editor.h \
    export.h \
    settings.h \
    singleeditor.h \
    stackviewer.h

FORMS += \
    about.ui \
    choosestack.ui \
    editor.ui \
    settings.ui \
    singleeditor.ui \
    stackviewer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc
