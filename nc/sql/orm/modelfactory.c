zend_class_entry *Nc_Sql_Orm_ModelFactory_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Orm_ModelFactory)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Orm\\ModelFactory", Nc_Sql_Orm_ModelFactory_methods);
    Nc_Sql_Orm_ModelFactory_ce = zend_register_internal_class_ex(&ce, Nc_Kernel_ce, NULL TSRMLS_CC);

    Nc_Sql_Orm_ModelFactory_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    zend_class_implements(Nc_Sql_Orm_ModelFactory_ce TSRMLS_CC, 1, Nc_PropertyAccess_ce);
    zend_declare_property_null(Nc_Sql_Orm_ModelFactory_ce, ZEND_STRL("models"), ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Orm_ModelFactory_setModel_arginfo, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, model, Nc\\Sql\\Orm\\Model, 0)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_ARRAY_INFO(0, metadata, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_Orm_ModelFactory, setModel)
{
}

zend_function_entry Nc_Sql_Orm_ModelFactory_methods[] = {
    ZEND_ME(Nc_Sql_Orm_ModelFactory, setModel, Nc_Sql_Orm_ModelFactory_setModel_arginfo, ZEND_ACC_PROTECTED)
    ZEND_FE_END
};
