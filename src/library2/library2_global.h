#ifndef LIBRARY2_GLOBAL_H
#define LIBRARY2_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBRARY2_LIBRARY)
#  define LIBRARY2SHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBRARY2SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBRARY2_GLOBAL_H
