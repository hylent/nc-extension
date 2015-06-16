#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_nc.h"

#include "standard/info.h"

zend_module_entry nc_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    NC_EXTNAME,
    NULL,
    PHP_MINIT(nc),
    NULL,
    NULL,
    NULL,
    PHP_MINFO(nc),
#if ZEND_MODULE_API_NO >= 20010901
    NC_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

PHP_MINIT_FUNCTION(nc)
{
    NC_INIT_CLASS(Nc_Exception);
    NC_INIT_CLASS(Nc_Kernel);
    NC_INIT_CLASS(Nc_PropertyAccess);
    NC_INIT_CLASS(Nc_InstantStorage);

    NC_INIT_CLASS(Nc_Loader_Exception);
    NC_INIT_CLASS(Nc_Loader_AbstractLoader);
    NC_INIT_CLASS(Nc_Loader_ClassPathLoader);
    NC_INIT_CLASS(Nc_Loader_NamespaceDirectoryLoader);

    NC_INIT_CLASS(Nc_Sql_Exception);
    NC_INIT_CLASS(Nc_Sql_QueryException);
    NC_INIT_CLASS(Nc_Sql_Connection);
    NC_INIT_CLASS(Nc_Sql_PdoConnection);
    NC_INIT_CLASS(Nc_Sql_MysqliConnection);
    NC_INIT_CLASS(Nc_Sql_OciConnection);
    NC_INIT_CLASS(Nc_Sql_MasterSlaveConnection);
    NC_INIT_CLASS(Nc_Sql_Adapter);
    NC_INIT_CLASS(Nc_Sql_Mysql);
    NC_INIT_CLASS(Nc_Sql_Pgsql);
    NC_INIT_CLASS(Nc_Sql_Oci);
    NC_INIT_CLASS(Nc_Sql_Predicate);
    NC_INIT_CLASS(Nc_Sql_Select);

    NC_INIT_CLASS(Nc_Sql_Orm_Exception);
    NC_INIT_CLASS(Nc_Sql_Orm_Model);
    NC_INIT_CLASS(Nc_Sql_Orm_ModelFactory);
    NC_INIT_CLASS(Nc_Sql_Orm_DirectoryModelFactory);
    NC_INIT_CLASS(Nc_Sql_Orm_Entity);
    NC_INIT_CLASS(Nc_Sql_Orm_Relation);
    NC_INIT_CLASS(Nc_Sql_Orm_RelationContainer);
    NC_INIT_CLASS(Nc_Sql_Orm_BelongingTo);
    NC_INIT_CLASS(Nc_Sql_Orm_HavingOne);
    NC_INIT_CLASS(Nc_Sql_Orm_HavingMany);
    NC_INIT_CLASS(Nc_Sql_Orm_HavingAndBelongingToMany);

    return SUCCESS;
}

PHP_MINFO_FUNCTION(nc)
{
    php_info_print_table_start();
    php_info_print_table_header(2, NC_EXTNAME, "enabled");
    php_info_print_table_row(2, "version", NC_VERSION);
    php_info_print_table_end();
}

#ifdef COMPILE_DL_NC
ZEND_GET_MODULE(nc)
#endif
