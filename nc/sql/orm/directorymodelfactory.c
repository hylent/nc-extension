zend_class_entry *Nc_Sql_Orm_DirectoryModelFactory_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Orm_DirectoryModelFactory)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Orm\\DirectoryModelFactory", Nc_Sql_Orm_DirectoryModelFactory_methods);
    Nc_Sql_Orm_DirectoryModelFactory_ce = zend_register_internal_class_ex(&ce, Nc_Sql_Orm_ModelFactory_ce, NULL TSRMLS_CC);

    return SUCCESS;
}

zend_function_entry Nc_Sql_Orm_DirectoryModelFactory_methods[] = {
    ZEND_FE_END
};
