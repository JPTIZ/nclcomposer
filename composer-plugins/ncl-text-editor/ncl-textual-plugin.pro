TARGET      =   ncl_textual_plugin
include (../plugins-common.pri)

QT          +=  xml gui

# Required to link against QSCINTILLA on Windows
DEFINES     += QSCINTILLA_DLL

INCLUDEPATH +=  include \
                ../outline-view/include \
                ../ncl-layout-view \
                deps/QScintilla-gpl-2.5.1/Qt4

release {
LIBS      +=    -Ldeps/QScintilla-gpl-2.5.1/Qt4/release \
                -lqscintilla2
}else:debug {
LIBS      +=    -L/deps/QScintilla-gpl-2.5.1/Qt4/debug \
                -lqscintilla2
}

macx:LIBS += -L/Library/Frameworks \
            -L$$quote(/Library/Application Support/Composer) \
            -lNCLLanguageProfile

unix:LIBS += -Ldeps/QScintilla-gpl-2.5.1/Qt4

win32:LIBS += -lNCLLanguageProfile

HEADERS     =   include/NCLTextEditorMainWindow.h \
                include/NCLTextEditor.h \
                include/QsciLexerNCL.h \
                include/QsciNCLAPIs.h \
                include/MyLexer.h \
                include/NCLValidator.h \
                include/NCLProblemsView.h \
                ../outline-view/include/NCLParser.h \
                ../outline-view/include/NCLTreeWidget.h \
                include/NCLTextualViewPlugin.h \
                include/NCLTextualViewPluginFactory.h \
                include/NCLTextualViewPlugin_global.h

SOURCES     =   src/main.cpp \
                src/NCLTextEditorMainWindow.cpp \
                src/NCLTextEditor.cpp \
                src/QsciLexerNCL.cpp \
                src/QsciNCLAPIs.cpp \
                src/MyLexer.cpp \
                src/NCLValidator.cpp \
                src/NCLProblemsView.cpp \
                ../outline-view/src/NCLParser.cpp \
                ../outline-view/src/NCLTreeWidget.cpp \
                src/NCLTextualViewPlugin.cpp \
                src/NCLTextualViewPluginFactory.cpp

OTHER_FILES +=  TODO \
                config/NCL_STRUCTURE \
                NEWS \
                config/NCL_STRUCTURE_OLD.txt

RESOURCES   =   ncl_textual_plugin.qrc
FORMS       +=  ui/TextualPluginPreferencesWidget.ui

INSTALLS += target
