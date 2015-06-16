zend_class_entry *Nc_Sql_MasterSlaveConnection_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_MasterSlaveConnection)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\MasterSlaveConnection", Nc_Sql_MasterSlaveConnection_methods);
    Nc_Sql_MasterSlaveConnection_ce = zend_register_internal_class_ex(&ce, Nc_Sql_Connection_ce, NULL TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_MasterSlaveConnection_begin_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_MasterSlaveConnection_commit_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_MasterSlaveConnection_rollback_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_MasterSlaveConnection_query_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, sql)
    ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_MasterSlaveConnection, begin)
{
    zval *masterConnection, *r = NULL;

    masterConnection = nc_kernel_injection_property(getThis(), "masterConnection", Nc_Sql_Connection_ce);
    if (!masterConnection)
        return;

    r = zend_call_method_with_0_params(&masterConnection, Z_OBJCE_P(masterConnection), NULL, "begin", &r);
    zval_ptr_dtor(&masterConnection);

    if (!r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke begin of injection property masterConnection.", 0 TSRMLS_CC);
        return;
    }

    zval_ptr_dtor(&r);
}

ZEND_METHOD(Nc_Sql_MasterSlaveConnection, commit)
{
    zval *masterConnection, *r = NULL;

    masterConnection = nc_kernel_injection_property(getThis(), "masterConnection", Nc_Sql_Connection_ce);
    if (!masterConnection)
        return;

    r = zend_call_method_with_0_params(&masterConnection, Z_OBJCE_P(masterConnection), NULL, "commit", &r);
    zval_ptr_dtor(&masterConnection);

    if (!r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke commit of injection property masterConnection.", 0 TSRMLS_CC);
        return;
    }

    zval_ptr_dtor(&r);
}

ZEND_METHOD(Nc_Sql_MasterSlaveConnection, rollback)
{
    zval *masterConnection, *r = NULL;

    masterConnection = nc_kernel_injection_property(getThis(), "masterConnection", Nc_Sql_Connection_ce);
    if (!masterConnection)
        return;

    r = zend_call_method_with_0_params(&masterConnection, Z_OBJCE_P(masterConnection), NULL, "rollback", &r);
    zval_ptr_dtor(&masterConnection);

    if (!r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke rollback of injection property masterConnection.", 0 TSRMLS_CC);
        return;
    }

    zval_ptr_dtor(&r);
}

ZEND_METHOD(Nc_Sql_MasterSlaveConnection, query)
{
    zval *sql, *flag;
    zval *connection, *r;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &sql, &flag) == FAILURE || Z_TYPE_P(flag) != IS_LONG)
        return;

    if (Z_LVAL_P(flag) & NC_SQL_CONNECTION_WRITE)
    {
        connection = nc_kernel_injection_property(getThis(), "masterConnection", Nc_Sql_Connection_ce);
    }
    else
    {
        connection = nc_kernel_injection_property(getThis(), "slaveConnection", Nc_Sql_Connection_ce);
    }

    if (!connection)
        return;

    r = zend_call_method_with_2_params(&connection, Z_OBJCE_P(connection), NULL, "query", &r, sql, flag);
    zval_ptr_dtor(&connection);

    if (!r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke query of injection property masterConnection/slaveConnection.", 0 TSRMLS_CC);
        return;
    }

    RETVAL_ZVAL(r, 0, 0);
    efree(r);
}

zend_function_entry Nc_Sql_MasterSlaveConnection_methods[] = {
    ZEND_ME(Nc_Sql_MasterSlaveConnection, begin, Nc_Sql_MasterSlaveConnection_begin_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_MasterSlaveConnection, commit, Nc_Sql_MasterSlaveConnection_commit_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_MasterSlaveConnection, rollback, Nc_Sql_MasterSlaveConnection_rollback_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_MasterSlaveConnection, query, Nc_Sql_MasterSlaveConnection_query_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
