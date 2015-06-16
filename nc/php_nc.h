#ifndef PHP_NC_H
#define PHP_NC_H

extern zend_module_entry nc_module_entry;
#define phpext_nc_ptr &nc_module_entry

#define NC_EXTNAME "nc"
#define NC_VERSION "0.7.4"

#ifdef PHP_WIN32
#define NC_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define NC_API __attribute__ ((visibility("default")))
#else
#define NC_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(nc);
PHP_MINFO_FUNCTION(nc);

#define NC_INIT_CLASS_FUNCTION(name) int nc_init_class_##name(INIT_FUNC_ARGS)
#define NC_INIT_CLASS(name) if (nc_init_class_##name(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) { return FAILURE; }
#define NC_DECLARE_CLASS(name) extern zend_function_entry name##_methods[]; extern zend_class_entry *name##_ce; NC_INIT_CLASS_FUNCTION(name);

#define nc_array_property(ce, name) _nc_array_property(ce, name TSRMLS_CC);
NC_API zval *_nc_array_property(zval *obj, const char *name TSRMLS_DC);

#define nc_kernel_injection_property(obj, name, valid_ce) _nc_kernel_injection_property(obj, name, valid_ce TSRMLS_CC);
NC_API zval *_nc_kernel_injection_property(zval *obj, const char *name, zend_class_entry *valid_ce TSRMLS_DC);

NC_DECLARE_CLASS(Nc_Exception)
NC_DECLARE_CLASS(Nc_Kernel)
NC_DECLARE_CLASS(Nc_PropertyAccess)
NC_DECLARE_CLASS(Nc_InstantStorage)

NC_DECLARE_CLASS(Nc_Loader_Exception)
NC_DECLARE_CLASS(Nc_Loader_AbstractLoader)
NC_DECLARE_CLASS(Nc_Loader_ClassPathLoader)
NC_DECLARE_CLASS(Nc_Loader_NamespaceDirectoryLoader)

#define NC_SQL_CONNECTION_READ 1
#define NC_SQL_CONNECTION_WRITE 2
#define NC_SQL_CONNECTION_SELECT 0x10
#define NC_SQL_CONNECTION_DELETE 0x20
#define NC_SQL_CONNECTION_INSERT 0x30
#define NC_SQL_CONNECTION_UPDATE 0x40
#define NC_SQL_CONNECTION_FETCH 0xf00
#define NC_SQL_CONNECTION_ALL 0x100
#define NC_SQL_CONNECTION_ROW 0x200
#define NC_SQL_CONNECTION_ONE 0x300
#define NC_SQL_CONNECTION_COLUMN 0x400

NC_DECLARE_CLASS(Nc_Sql_Exception)
NC_DECLARE_CLASS(Nc_Sql_QueryException)
NC_DECLARE_CLASS(Nc_Sql_Connection)
NC_DECLARE_CLASS(Nc_Sql_PdoConnection)
NC_DECLARE_CLASS(Nc_Sql_MysqliConnection)
NC_DECLARE_CLASS(Nc_Sql_OciConnection)
NC_DECLARE_CLASS(Nc_Sql_MasterSlaveConnection)
NC_DECLARE_CLASS(Nc_Sql_Adapter)
NC_DECLARE_CLASS(Nc_Sql_Mysql)
NC_DECLARE_CLASS(Nc_Sql_Pgsql)
NC_DECLARE_CLASS(Nc_Sql_Oci)
NC_DECLARE_CLASS(Nc_Sql_Predicate)
NC_DECLARE_CLASS(Nc_Sql_Select)

NC_DECLARE_CLASS(Nc_Sql_Orm_Exception)
NC_DECLARE_CLASS(Nc_Sql_Orm_Model)
NC_DECLARE_CLASS(Nc_Sql_Orm_ModelFactory)
NC_DECLARE_CLASS(Nc_Sql_Orm_DirectoryModelFactory)
NC_DECLARE_CLASS(Nc_Sql_Orm_Entity)
NC_DECLARE_CLASS(Nc_Sql_Orm_Relation)
NC_DECLARE_CLASS(Nc_Sql_Orm_RelationContainer)
NC_DECLARE_CLASS(Nc_Sql_Orm_BelongingTo)
NC_DECLARE_CLASS(Nc_Sql_Orm_HavingOne)
NC_DECLARE_CLASS(Nc_Sql_Orm_HavingMany)
NC_DECLARE_CLASS(Nc_Sql_Orm_HavingAndBelongingToMany)

#endif /* PHP_NC_H */
