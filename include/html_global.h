#pragma once

#include <QtCore/qglobal.h>

#if defined(HTML_LIBRARY)
  #define HTML_SHARED_EXPORT Q_DECL_EXPORT
#else
  #define HTML_SHARED_EXPORT Q_DECL_IMPORT
#endif

