zend_class_entry *Nc_Sql_Orm_RelationContainer_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Orm_RelationContainer)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Orm\\RelationContainer", Nc_Sql_Orm_RelationContainer_methods);
    Nc_Sql_Orm_RelationContainer_ce = zend_register_internal_class_ex(&ce, Nc_Kernel_ce, NULL TSRMLS_CC);

    return SUCCESS;
}

zend_function_entry Nc_Sql_Orm_RelationContainer_methods[] = {
    ZEND_FE_END
};
