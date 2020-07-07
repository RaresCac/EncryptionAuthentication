QT += quick

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        src/encryptor.cpp \
        src/keyretriever.cpp \
        src/main.cpp \
        src/userfile.cpp \
        src/userfilecreator.cpp

RESOURCES += qml.qrc

PKGCONFIG += openssl

win32 {
    LIBS += -LC:/Libraries/openssl-1.1.1g-win64-mingw/lib
    INCLUDEPATH += C:/Libraries/OpenSSL-Win64/include
}

#Libraries for OpenSSL
LIBS += -lssl -lcrypto

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/encryptor.h \
    src/keyretriever.h \
    src/userfile.h \
    src/userfilecreator.h
