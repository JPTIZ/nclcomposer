#-------------------------------------------------
#
# Project created by QtCreator 2011-01-21T13:59:28
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = NCLLanguageProfile
TEMPLATE = lib
CONFIG += debug plugin


macx {
    LIBS += -L../../core \
        -F../../core \
        -framework \
        ComposerCore
    INCLUDEPATH +=../../core/include
    target.path = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L/usr/local/lib/composer \
        -lComposerCore
    INCLUDEPATH += ../../include/ \
                /usr/local/include/composer

    target.path = $$quote(/usr/local/lib/composer/extension)
}

DEFINES += NCLLANGUAGEPROFILE_LIBRARY

SOURCES += NCLLanguageProfile.cpp \
    NCLDocumentParser.cpp

HEADERS += NCLLanguageProfile.h\
        NCLLanguageProfile_global.h \
    NCLDocumentParser.h

INSTALLS += target
