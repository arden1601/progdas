QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = CarRentalGUI
TEMPLATE = app

SOURCES += \
    main_gui.cpp \
    MainWindow.cpp \
    CarDialog.cpp \
    RentalDialog.cpp \
    ReturnDialog.cpp \
    Car.cpp \
    Customer.cpp \
    Date.cpp \
    RentalTransaction.cpp \
    FileManager.cpp \
    RentalSystem.cpp

HEADERS += \
    MainWindow.h \
    CarDialog.h \
    RentalDialog.h \
    ReturnDialog.h \
    Car.h \
    Customer.h \
    Date.h \
    RentalTransaction.h \
    FileManager.h \
    RentalSystem.h

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target