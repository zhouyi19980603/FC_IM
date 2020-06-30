######################################################################
# Automatically generated by qmake (3.1) Wed Jun 17 19:07:11 2020
######################################################################
TEMPLATE = app
TARGET = FC_Client_ran
INCLUDEPATH += .
QT += quick
QT += sql
QT += qml quick widgets
#CONFIG += c++11 thread
CONFIG += console c++17 thread
CONFIG -= app_bundle
include(json/json.pri)

LIBS +=/usr/local/include/boost_1_72_0/stage/lib/libboost_thread.a
DEFINES += QT_DEPRECATED_WARNINGS
# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
SOURCES += \
        fc_client.cpp \
        fc_connection.cpp \
        fc_display.cpp \
        fc_message.cpp \
        fc_message_handle.cpp \
        fc_message_instance.cpp \
        fc_message_listmodel.cpp \
        fc_profile.cpp \
        fc_thread_pool.cpp \
        main.cpp

HEADERS += \
    fc_client.h \
    fc_connection.h \
    fc_display.h \
    fc_header.h \
    fc_message.h \
    fc_message_handle.h \
    fc_message_instance.h \
    fc_message_listmodel.h \
    fc_profile.h \
    fc_thread_pool.h

DISTFILES +=

RESOURCES += \
    qml.qrc
