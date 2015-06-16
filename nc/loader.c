#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_nc.h"

#include "Zend/zend_exceptions.h"
#include "standard/php_filestat.h"

#include "loader/exception.c"
#include "loader/abstractloader.c"
#include "loader/classpathloader.c"
#include "loader/namespacedirectoryloader.c"
