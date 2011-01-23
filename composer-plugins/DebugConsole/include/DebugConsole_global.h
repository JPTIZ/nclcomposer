#ifndef DEBUGCONSOLE_GLOBAL_H
#define DEBUGCONSOLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DEBUGCONSOLE_LIBRARY)
#  define DEBUGCONSOLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define DEBUGCONSOLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DEBUGCONSOLE_GLOBAL_H
