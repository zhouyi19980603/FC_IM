QT += quick

CONFIG += c++11 console thread
QT += sql
include(json/json.pri)
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FC_BuddyItem.cpp \
    FC_BuddyModel.cpp \
    FC_BuddyTeam.cpp \
    database.cpp \
    fc_account_msg.cpp \
    fc_buddylist_ctrl.cpp \
    fc_client.cpp \
    fc_connection.cpp \
    fc_message.cpp \
    fc_message_handle.cpp \
    fc_profile.cpp \
    fc_profile_handle.cpp \
    fc_thread_pool.cpp \
    main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    AddUI.qml \
    Buddy.qml \
    asserts/add1.jpg \
    asserts/return.jpg

HEADERS += \
    FC_BuddyItem.h \
    FC_BuddyModel.h \
    FC_BuddyTeam.h \
    database.h \
    fc_account_msg.h \
    fc_buddylist_ctrl.h \
    fc_client.h \
    fc_connection.h \
    fc_header.h \
    fc_message.h \
    fc_message_handle.h \
    fc_profile.h \
    fc_profile_handle.h \
    fc_thread_pool.h
