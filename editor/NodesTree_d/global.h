#ifndef NODESTREE_D_GLOBAL_H
#define NODESTREE_D_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef NODESTREE_D_LIB
# define NODESTREE_D_EXPORT Q_DECL_EXPORT
#else
# define NODESTREE_D_EXPORT Q_DECL_IMPORT
#endif

#endif // NODESTREE_D_GLOBAL_H
