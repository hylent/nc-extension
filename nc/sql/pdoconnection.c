zend_class_entry *Nc_Sql_PdoConnection_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_PdoConnection)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\PdoConnection", Nc_Sql_PdoConnection_methods);
    Nc_Sql_PdoConnection_ce = zend_register_internal_class_ex(&ce, Nc_Sql_Connection_ce, NULL TSRMLS_CC);

    zend_declare_property_null(Nc_Sql_PdoConnection_ce, ZEND_STRL("pdo"), ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_PdoConnection___construct_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, dsn)
    ZEND_ARG_INFO(0, user)
    ZEND_ARG_INFO(0, passwd)
    ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_PdoConnection_begin_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_PdoConnection_commit_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_PdoConnection_rollback_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_PdoConnection_query_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, sql)
    ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_PdoConnection, __construct)
{
    zval *p1, *p2 = NULL, *p3 = NULL, *p4 = NULL;
    uint argc = 1;
    zend_class_entry *ce;
    zval *pdo;
    zval **argv[4] = {&p1, &p2, &p3, &p4};
    zval m;
    zval *r = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|zza", &p1, &p2, &p3, &p4) == FAILURE)
        return;

    if (p2)
    {
        ++argc;
        if (p3)
        {
            ++argc;
            if (p4)
                ++argc;
        }
    }

    ce = zend_fetch_class(ZEND_STRL("pdo"), ZEND_FETCH_CLASS_DEFAULT TSRMLS_CC);
    if (!ce)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Missing PDO support.", 0 TSRMLS_CC);
        return;
    }

    ALLOC_INIT_ZVAL(pdo);
    object_init_ex(pdo, ce);
    zend_update_property(Nc_Sql_PdoConnection_ce, getThis(), ZEND_STRL("pdo"), pdo TSRMLS_CC);
    Z_DELREF_P(pdo);

    ZVAL_STRING(&m, "__construct", 0);

    if (call_user_function_ex(NULL, &pdo, &m, &r, argc, argv, 0, NULL TSRMLS_CC) == FAILURE || !r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke constructor of class #PDO#.", 0 TSRMLS_CC);
        return;
    }

    zval_ptr_dtor(&r);
}

ZEND_METHOD(Nc_Sql_PdoConnection, begin)
{
    zval *pdo;
    zval *r = NULL;

    pdo = zend_read_property(Nc_Sql_PdoConnection_ce, getThis(), ZEND_STRL("pdo"), 0 TSRMLS_CC);

    r = zend_call_method_with_0_params(&pdo, Z_OBJCE_P(pdo), NULL, "begintransaction", &r);
    if (!r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke method #PDO::beginTransaction()#.", 0 TSRMLS_CC);
        return;
    }

    zval_ptr_dtor(&r);
}

ZEND_METHOD(Nc_Sql_PdoConnection, commit)
{
    zval *pdo;
    zval *r = NULL;

    pdo = zend_read_property(Nc_Sql_PdoConnection_ce, getThis(), ZEND_STRL("pdo"), 0 TSRMLS_CC);

    r = zend_call_method_with_0_params(&pdo, Z_OBJCE_P(pdo), NULL, "commit", &r);
    if (!r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke method #PDO::commit()#.", 0 TSRMLS_CC);
        return;
    }

    zval_ptr_dtor(&r);
}

ZEND_METHOD(Nc_Sql_PdoConnection, rollback)
{
    zval *pdo;
    zval *r = NULL;

    pdo = zend_read_property(Nc_Sql_PdoConnection_ce, getThis(), ZEND_STRL("pdo"), 0 TSRMLS_CC);

    r = zend_call_method_with_0_params(&pdo, Z_OBJCE_P(pdo), NULL, "rollback", &r);
    if (!r)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke method #PDO::rollBack()#.", 0 TSRMLS_CC);
        return;
    }

    zval_ptr_dtor(&r);
}

ZEND_METHOD(Nc_Sql_PdoConnection, query)
{
    zval *sql;
    long flag;
    zval *pdo;
    zval *tmp;
    zval *stmt;
    zval *success;
    zval *data = NULL;
    zval *pz, **ppz = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zl", &sql, &flag) == FAILURE)
        return;

    if (Z_TYPE_P(sql) != IS_STRING)
        convert_to_string(sql);

    pdo = zend_read_property(Nc_Sql_PdoConnection_ce, getThis(), ZEND_STRL("pdo"), 0 TSRMLS_CC);

    tmp = zend_call_method_with_1_params(NULL, Nc_Sql_Connection_ce, NULL, "addquery", &tmp, sql);
    if (!tmp)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot add query.", 0 TSRMLS_CC);
        return;
    }
    zval_ptr_dtor(&tmp);

    stmt = zend_call_method_with_1_params(&pdo, Z_OBJCE_P(pdo), NULL, "prepare", &stmt, sql);
    if (!stmt)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke #PDO::prepare()#.", 0 TSRMLS_CC);
        return;
    }

    success = zend_call_method_with_0_params(&stmt, Z_OBJCE_P(stmt), NULL, "execute", &success);
    if (!success || Z_TYPE_P(success) != IS_BOOL || !Z_BVAL_P(success))
    {
        char *err = "";
        zval *pz, **ppz;
        pz = zend_call_method_with_0_params(&stmt, Z_OBJCE_P(stmt), NULL, "errorinfo", &pz);
        if (pz && Z_TYPE_P(pz) == IS_ARRAY && zend_hash_index_find(Z_ARRVAL_P(pz), 2, (void **) &ppz) == SUCCESS && Z_TYPE_PP(ppz) == IS_STRING)
            err = Z_STRVAL_PP(ppz);
        zend_throw_exception_ex(Nc_Sql_QueryException_ce, 0 TSRMLS_CC, "%s [SQL: %s]", err, Z_STRVAL_P(sql));
        if (pz)
            zval_ptr_dtor(&pz);
        zval_ptr_dtor(&stmt);
        if (success)
            zval_ptr_dtor(&success);
        return;
    }
    zval_ptr_dtor(&success);

    switch (flag & NC_SQL_CONNECTION_FETCH)
    {
    case NC_SQL_CONNECTION_ONE:
        if (zend_hash_find(&Z_OBJCE_P(pdo)->constants_table, ZEND_STRS("FETCH_NUM"), (void **) &ppz) != SUCCESS)
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch class constant PDO::FETCH_NUM.", 0 TSRMLS_CC);
            break;
        }
        data = zend_call_method_with_1_params(&stmt, Z_OBJCE_P(stmt), NULL, "fetch", &data, *ppz);
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
        if (zend_hash_find(&Z_OBJCE_P(pdo)->constants_table, ZEND_STRS("FETCH_ASSOC"), (void **) &ppz) != SUCCESS)
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch class constant PDO::FETCH_ASSOC.", 0 TSRMLS_CC);
            break;
        }
        data = zend_call_method_with_1_params(&stmt, Z_OBJCE_P(stmt), NULL, "fetch", &data, *ppz);
        if (Z_TYPE_P(data) != IS_ARRAY)
        {
            zval_ptr_dtor(&data);
            ALLOC_INIT_ZVAL(data);
            array_init(data);
        }
        RETVAL_ZVAL(data, 0, 0)
        efree(data);
        break;

    case NC_SQL_CONNECTION_ALL:
        if (zend_hash_find(&Z_OBJCE_P(pdo)->constants_table, ZEND_STRS("FETCH_ASSOC"), (void **) &ppz) != SUCCESS)
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch class constant PDO::FETCH_ASSOC.", 0 TSRMLS_CC);
            break;
        }
        data = zend_call_method_with_1_params(&stmt, Z_OBJCE_P(stmt), NULL, "fetchall", &data, *ppz);
        RETVAL_ZVAL(data, 0, 0)
        efree(data);
        break;

    case NC_SQL_CONNECTION_COLUMN:
        ALLOC_INIT_ZVAL(data);
        array_init(data);
        do
        {
            pz = NULL;
            pz = zend_call_method_with_0_params(&stmt, Z_OBJCE_P(stmt), NULL, "fetchcolumn", &pz);
            if (!pz)
                break;
            if (Z_TYPE_P(pz) != IS_STRING)
            {
                zval_ptr_dtor(&pz);
                break;
            }
            add_next_index_zval(data, pz);
        }
        while (1);
        RETVAL_ZVAL(data, 0, 0)
        efree(data);
        break;

    default:
        RETVAL_TRUE;
        break;
    }

    zval_ptr_dtor(&stmt);
}

zend_function_entry Nc_Sql_PdoConnection_methods[] = {
    ZEND_ME(Nc_Sql_PdoConnection, __construct, Nc_Sql_PdoConnection___construct_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(Nc_Sql_PdoConnection, begin, Nc_Sql_PdoConnection_begin_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_PdoConnection, commit, Nc_Sql_PdoConnection_commit_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_PdoConnection, rollback, Nc_Sql_PdoConnection_rollback_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_PdoConnection, query, Nc_Sql_PdoConnection_query_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
