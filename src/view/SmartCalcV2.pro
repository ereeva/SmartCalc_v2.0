QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../controller/controller.cc \
    ../model/calc.cc \
    ../model/credit.cc \
    ../model/deposit.cc \
    ../libs/qcustomplot.cpp \
    dialog.cc \
    main.cc \
    mainwindow.cc

HEADERS += \
    ../controller/controller.h \
    ../model/calc.h \
    ../model/credit.h \
    ../model/deposit.h \
    ../libs/qcustomplot.h \
    dialog.h \
    mainwindow.h

FORMS += \
    dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
