QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ban_users.cpp \
    database.cpp \
    login_moder_form.cpp \
    main.cpp \
    moderator_basement.cpp \
    see_all_users.cpp \
    see_messages.cpp \
    serverwindow.cpp

HEADERS += \
    ban_users.h \
    database.h \
    login_moder_form.h \
    moderator_basement.h \
    see_all_users.h \
    see_messages.h \
    serverwindow.h

FORMS += \
    ban_users.ui \
    login_moder_form.ui \
    moderator_basement.ui \
    see_all_users.ui \
    see_messages.ui \
    serverwindow.ui

TRANSLATIONS += \
    Server_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
