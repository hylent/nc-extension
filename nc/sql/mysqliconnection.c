zend_class_entry *Nc_Sql_MysqliConnection_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_MysqliConnection)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\MysqliConnection", Nc_Sql_MysqliConnection_methods);
    Nc_Sql_MysqliConnection_ce = zend_register_internal_class_ex(&ce, Nc_Sql_Connection_ce, NULL TSRMLS_CC);

    zend_declare_property_null(Nc_Sql_MysqliConnection_ce, ZEND_STRL("mysqli"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_bool(Nc_Sql_MysqliConnection_ce, ZEND_STRL("inTransaction"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_MysqliConnection___construct_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, host)
    ZEND_ARG_INFO(0, user)
    ZEND_ARG_INFO(0, passwd)
    ZEND_ARG_INFO(0, dbname)
    ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_MysqliConnection_begin_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_MysqliConnection_commit_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_MysqliConnection_rollback_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_MysqliConnection_query_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, sql)
    ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_MysqliConnection, __construct)
{
    zval *p1, *p2 = NULL, *p3 = NULL, *p4 = NULL, *p5 = NULL;
    uint argc = 1;
    zend_class_entry *ce;
    zval *mysqli = NULL;
    zval **argv[5] = {&p1, &p2, &p3, &p4, &p5};
    zval m;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|zzzz", &p1, &p2, &p3, &p4, &p5) == FAILURE)
        return;

    if (p2)
    {
        ++argc;
        if (p3)
        {
            ++argc;
            if (p4)
            {
                ++argc;
                if (p5)
                    ++argc;
            }
        }
    }

    ce = zend_fetch_class(ZEND_STRL("mysqli"), ZEND_FETCH_CLASS_DEFAULT TSRMLS_CC);
    if (!ce)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Missing mysqli support.", 0 TSRMLS_CC);
        return;
    }

    ZVAL_STRING(&m, "mysqli_connect", 0);
    if (call_user_function_ex(NULL, NULL, &m, &mysqli, argc, argv, 0, NULL TSRMLS_CC) == FAILURE || !mysqli)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke mysqli_connect().", 0 TSRMLS_CC);
        return;
    }

    zend_update_property(Nc_Sql_MysqliConnection_ce, getThis(), ZEND_STRL("mysqli"), mysqli TSRMLS_CC);
    Z_DELREF_P(mysqli);
}

ZEND_METHOD(Nc_Sql_MysqliConnection, begin)
{
    zend_update_property_bool(Nc_Sql_MysqliConnection_ce, getThis(), ZEND_STRL("inTransaction"), 1 TSRMLS_CC);
}

ZEND_METHOD(Nc_Sql_MysqliConnection, commit)
{
    zval *mysqli;
    zval *r = NULL;

    mysqli = zend_read_property(Nc_Sql_MysqliConnection_ce, getThis(), ZEND_STRL("mysqli"), 0 TSRMLS_CC);

    r = zend_call_method_with_0_params(&mysqli, Z_OBJCE_P(mysqli), NULL, "commit", &r);
    if (!r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke $this->mysqli->commit().", 0 TSRMLS_CC);
        return;
    }

    if (Z_TYPE_P(r) != IS_BOOL || !Z_BVAL_P(r))
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Failed to commit a transaction.", 0 TSRMLS_CC);
        zval_ptr_dtor(&r);
        return;
    }

    zval_ptr_dtor(&r);
    zend_update_property_bool(Nc_Sql_MysqliConnection_ce, getThis(), ZEND_STRL("inTransaction"), 0 TSRMLS_CC);
}

ZEND_METHOD(Nc_Sql_MysqliConnection, rollback)
{
    zval *mysqli;
    zval *r = NULL;

    mysqli = zend_read_property(Nc_Sql_MysqliConnection_ce, getThis(), ZEND_STRL("mysqli"), 0 TSRMLS_CC);

    r = zend_call_method_with_0_params(&mysqli, Z_OBJCE_P(mysqli), NULL, "rollback", &r);
    if (!r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke $this->mysqli->rollback().", 0 TSRMLS_CC);
        return;
    }

    if (Z_TYPE_P(r) != IS_BOOL || !Z_BVAL_P(r))
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Failed to rollback a transaction.", 0 TSRMLS_CC);
        zval_ptr_dtor(&r);
        return;
    }

    zval_ptr_dtor(&r);
    zend_update_property_bool(Nc_Sql_MysqliConnection_ce, getThis(), ZEND_STRL("inTransaction"), 0 TSRMLS_CC);
}

ZEND_METHOD(Nc_Sql_MysqliConnection, query)
{
    zval *sql;
    long flag;
    zval *mysqli;
    zval *tmp;
    zval *autoci;
    zval *result;
    zval *data;
    zval **ppz;
    zval c;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zl", &sql, &flag) == FAILURE)
        return;

    if (Z_TYPE_P(sql) != IS_STRING)
        convert_to_string(sql);

    mysqli = zend_read_property(Nc_Sql_MysqliConnection_ce, getThis(), ZEND_STRL("mysqli"), 0 TSRMLS_CC);

    tmp = zend_call_method_with_1_params(NULL, Nc_Sql_Connection_ce, NULL, "addquery", &tmp, sql);
    if (!tmp)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot add query.", 0 TSRMLS_CC);
        return;
    }
    zval_ptr_dtor(&tmp);

    tmp = zend_read_property(Nc_Sql_MysqliConnection_ce, getThis(), ZEND_STRL("inTransaction"), 0 TSRMLS_CC);

    ALLOC_INIT_ZVAL(autoci);
    ZVAL_BOOL(autoci, Z_TYPE_P(tmp) == IS_BOOL && Z_BVAL_P(tmp) ? 0 : 1);

    tmp = NULL;
    tmp = zend_call_method_with_1_params(&mysqli, Z_OBJCE_P(mysqli), NULL, "autocommit", &tmp, autoci);

    if (!tmp || Z_TYPE_P(tmp) != IS_BOOL || !Z_BVAL_P(tmp))
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke $this->mysqli->autocommit() or invalid result returned.", 0 TSRMLS_CC);
        zval_ptr_dtor(&autoci);
        if (tmp)
            zval_ptr_dtor(&tmp);
        return;
    }

    zval_ptr_dtor(&tmp);
    efree(autoci);

    result = zend_call_method_with_1_params(&mysqli, Z_OBJCE_P(mysqli), NULL, "query", &tmp, sql);
    if (!result || (Z_TYPE_P(result) == IS_BOOL && !Z_BVAL_P(result)))
    {
        zval *err;
        err = zend_read_property(Z_OBJCE_P(mysqli), mysqli, ZEND_STRL("error"), 0 TSRMLS_CC);
        if (Z_TYPE_P(err) != IS_STRING)
            convert_to_string(err);
        zend_throw_exception_ex(Nc_Sql_QueryException_ce, 0 TSRMLS_CC, "%s [SQL: %s]", Z_STRVAL_P(err), Z_STRVAL_P(sql));
        if (result)
            zval_ptr_dtor(&result);
        return;
    }

    switch (flag & NC_SQL_CONNECTION_FETCH)
    {
    case NC_SQL_CONNECTION_ONE:
        data = zend_call_method_with_0_params(&result, Z_OBJCE_P(result), NULL, "fetch_row", &data);
        if (data && Z_TYPE_P(data) == IS_ARRAY && zend_hash_index_find(Z_ARRVAL_P(data), 0, (void **) &ppz) == SUCCESS)
        {
            if (Z_TYPE_PP(ppz) != IS_STRING)
                convert_to_string(*ppz);
            RETVAL_STRINGL(Z_STRVAL_PP(ppz), Z_STRLEN_PP(ppz), 1);
        }
        else
        {
            RETVAL_EMPTY_STRING();
        }
        if (data)
            zval_ptr_dtor(&data);
        break;

    case NC_SQL_CONNECTION_ROW:
        data = zend_call_method_with_0_params(&result, Z_OBJCE_P(result), NULL, "fetch_assoc", &data);
        if (Z_TYPE_P(data) != IS_ARRAY)
            convert_to_array(data);
        RETVAL_ZVAL(data, 0, 0)
        efree(data);
        break;

    case NC_SQL_CONNECTION_ALL:
        if (!zend_get_constant(ZEND_STRL("MYSQLI_ASSOC"), &c TSRMLS_CC))
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch constant MYSQLI_ASSOC.", 0 TSRMLS_CC);
            break;
        }
        data = zend_call_method_with_1_params(&result, Z_OBJCE_P(result), NULL, "fetch_all", &data, &c);
        RETVAL_ZVAL(data, 0, 0)
        efree(data);
        break;

    case NC_SQL_CONNECTION_COLUMN:
        ALLOC_INIT_ZVAL(data);
        array_init(data);
        do
        {
            zval *pz = NULL;
            pz = zend_call_method_with_0_params(&result, Z_OBJCE_P(result), NULL, "fetch_row", &pz);
            if (Z_TYPE_P(pz) != IS_ARRAY || zend_hash_index_find(Z_ARRVAL_P(pz), 0, (void **) &ppz) != SUCCESS || Z_TYPE_PP(ppz) != IS_STRING)
            {
                zval_ptr_dtor(&pz);
                break;
            }
            add_next_index_stringl(data, Z_STRVAL_PP(ppz), Z_STRLEN_PP(ppz), 1);
            zval_ptr_dtor(&pz);
        }
        while (1);
        RETVAL_ZVAL(data, 0, 0)
        efree(data);
        break;

    default:
        RETVAL_TRUE;
        break;
    }

    zval_ptr_dtor(&result);
}

zend_function_entry Nc_Sql_MysqliConnection_methods[] = {
    ZEND_ME(Nc_Sql_MysqliConnection, __construct, Nc_Sql_MysqliConnection_begin_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(Nc_Sql_MysqliConnection, begin, Nc_Sql_MysqliConnection_begin_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_MysqliConnection, commit, Nc_Sql_MysqliConnection_commit_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_MysqliConnection, rollback, Nc_Sql_MysqliConnection_rollback_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_MysqliConnection, query, Nc_Sql_MysqliConnection_query_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
