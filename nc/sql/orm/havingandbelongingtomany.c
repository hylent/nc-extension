zend_class_entry *Nc_Sql_Orm_HavingAndBelongingToMany_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Orm_HavingAndBelongingToMany)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Orm\\HavingAndBelongingToMany", Nc_Sql_Orm_HavingAndBelongingToMany_methods);
    Nc_Sql_Orm_HavingAndBelongingToMany_ce = zend_register_internal_class_ex(&ce, Nc_Sql_Orm_Relation_ce, NULL TSRMLS_CC);

    return SUCCESS;
}

zend_function_entry Nc_Sql_Orm_HavingAndBelongingToMany_methods[] = {
    ZEND_FE_END
};
