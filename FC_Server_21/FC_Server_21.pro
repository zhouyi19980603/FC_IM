
CONFIG += console c++11 thread


QT += sql
QT += quick

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        fc_accept.cpp \
        fc_buddylist_ctrl.cpp \
        fc_database.cpp \
        fc_message.cpp \
        fc_server.cpp \
        fc_session.cpp \
        fc_thread_pool.cpp \
        main.cpp

QML_IMPORT_PATH =
include(json/json.pri)

QML_DESIGNER_IMPORT_PATH =

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    fc_accept.h \
    fc_buddylist_ctrl.h \
    fc_database.h \
    fc_header.h \
    fc_message.h \
    fc_server.h \
    fc_session.h \
    fc_thread_pool.h
