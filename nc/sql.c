#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_nc.h"

#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_constants.h"
#include "standard/php_string.h"

#include "sql/exception.c"
#include "sql/queryexception.c"
#include "sql/connection.c"
#include "sql/pdoconnection.c"
#include "sql/mysqliconnection.c"
#include "sql/ociconnection.c"
#include "sql/masterslaveconnection.c"
#include "sql/adapter.c"
#include "sql/mysql.c"
#include "sql/pgsql.c"
#include "sql/oci.c"
#include "sql/predicate.c"
#include "sql/select.c"

#include "sql/orm/exception.c"
#include "sql/orm/model.c"
#include "sql/orm/modelfactory.c"
#include "sql/orm/directorymodelfactory.c"
#include "sql/orm/entity.c"
#include "sql/orm/relation.c"
#include "sql/orm/relationcontainer.c"
#include "sql/orm/belongingto.c"
#include "sql/orm/havingone.c"
#include "sql/orm/havingmany.c"
#include "sql/orm/havingandbelongingtomany.c"
