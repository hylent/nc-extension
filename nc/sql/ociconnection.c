zend_class_entry *Nc_Sql_OciConnection_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_OciConnection)
{
    long l = 0;
    zval c;

    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\OciConnection", Nc_Sql_OciConnection_methods);
    Nc_Sql_OciConnection_ce = zend_register_internal_class_ex(&ce, Nc_Sql_Connection_ce, NULL TSRMLS_CC);

    zend_declare_property_null(Nc_Sql_OciConnection_ce, ZEND_STRL("oci"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_bool(Nc_Sql_OciConnection_ce, ZEND_STRL("inTransaction"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

    if (zend_get_constant(ZEND_STRL("OCI_RETURN_NULLS"), &c TSRMLS_CC) && Z_TYPE(c) == IS_LONG)
    {
        l |= Z_LVAL(c);
        if (zend_get_constant(ZEND_STRL("OCI_RETURN_LOBS"), &c TSRMLS_CC) && Z_TYPE(c) == IS_LONG)
            l |= Z_LVAL(c);
        else
            l = 0;
    }

    zend_declare_property_long(Nc_Sql_OciConnection_ce, ZEND_STRL("ociFetchMode"), l, ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_OciConnection___construct_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, user)
    ZEND_ARG_INFO(0, passwd)
    ZEND_ARG_INFO(0, db)
    ZEND_ARG_INFO(0, charset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_OciConnection_begin_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_OciConnection_commit_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_OciConnection_rollback_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_OciConnection_query_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, sql)
    ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

static zval *callfunc(const char *fn, uint fnLen, uint pc, zval *p1, zval *p2, zval *p3, zval *p4 TSRMLS_DC)
{
    zval **ps[4] = {&p1, &p2, &p3, &p4};
    zval m;
    zval *r = NULL;

    ZVAL_STRINGL(&m, fn, fnLen, 0);

    if (call_user_function_ex(NULL, NULL, &m, &r, pc, ps, 0, NULL TSRMLS_CC) == FAILURE || !r)
    {
        zend_throw_exception_ex(Nc_Sql_Exception_ce, 0 TSRMLS_CC, "Cannot call function %s().", fn);
        return NULL;
    }

    return r;
}

ZEND_METHOD(Nc_Sql_OciConnection, __construct)
{
    zval *p1, *p2 = NULL, *p3 = NULL, *p4 = NULL;
    uint argc = 1;
    zval *oci;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|zz", &p1, &p2, &p3, &p4) == FAILURE)
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

    oci = callfunc(ZEND_STRL("oci_new_connect"), argc, p1, p2, p3, p4 TSRMLS_CC);
    if (!oci)
        return;

    zend_update_property(Nc_Sql_OciConnection_ce, getThis(), ZEND_STRL("oci"), oci TSRMLS_CC);
    Z_DELREF_P(oci);
}

ZEND_METHOD(Nc_Sql_OciConnection, begin)
{
    zend_update_property_bool(Nc_Sql_OciConnection_ce, getThis(), ZEND_STRL("inTransaction"), 1 TSRMLS_CC);
}

ZEND_METHOD(Nc_Sql_OciConnection, commit)
{
    zval *oci;
    zval *r = NULL;

    oci = zend_read_property(Nc_Sql_OciConnection_ce, getThis(), ZEND_STRL("oci"), 0 TSRMLS_CC);
    r = callfunc(ZEND_STRL("oci_commit"), 1, oci, NULL, NULL, NULL TSRMLS_CC);

    if (!r)
        return;

    if (Z_TYPE_P(r) != IS_BOOL || !Z_BVAL_P(r))
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Failed to commit a transaction.", 0 TSRMLS_CC);
        zval_ptr_dtor(&r);
        return;
    }

    zval_ptr_dtor(&r);
    zend_update_property_bool(Nc_Sql_OciConnection_ce, getThis(), ZEND_STRL("inTransaction"), 0 TSRMLS_CC);
}

ZEND_METHOD(Nc_Sql_OciConnection, rollback)
{
    zval *oci;
    zval *r = NULL;

    oci = zend_read_property(Nc_Sql_OciConnection_ce, getThis(), ZEND_STRL("oci"), 0 TSRMLS_CC);
    r = callfunc(ZEND_STRL("oci_rollback"), 1, oci, NULL, NULL, NULL TSRMLS_CC);

    if (!r)
        return;

    if (Z_TYPE_P(r) != IS_BOOL || !Z_BVAL_P(r))
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Failed to rollback a transaction.", 0 TSRMLS_CC);
        zval_ptr_dtor(&r);
        return;
    }

    zval_ptr_dtor(&r);
    zend_update_property_bool(Nc_Sql_OciConnection_ce, getThis(), ZEND_STRL("inTransaction"), 0 TSRMLS_CC);
}

ZEND_METHOD(Nc_Sql_OciConnection, query)
{
    zval *sql;
    long flag;
    zval *oci;
    zval *tmp;
    zval *stmt, *success;
    zval mode;
    zval *ociFetchMode;
    zval *data;
    zval **ppz;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zl", &sql, &flag) == FAILURE)
        return;

    if (Z_TYPE_P(sql) != IS_STRING)
        convert_to_string(sql);

    tmp = zend_call_method_with_1_params(NULL, Nc_Sql_Connection_ce, NULL, "addquery", &tmp, sql);
    if (!tmp)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot add query.", 0 TSRMLS_CC);
        return;
    }
    zval_ptr_dtor(&tmp);

    tmp = zend_read_property(Nc_Sql_OciConnection_ce, getThis(), ZEND_STRL("inTransaction"), 0 TSRMLS_CC);

    if (Z_TYPE_P(tmp) == IS_BOOL && Z_BVAL_P(tmp))
    {
        if (!zend_get_constant(ZEND_STRL("OCI_NO_AUTO_COMMIT"), &mode TSRMLS_CC))
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch constant OCI_NO_AUTO_COMMIT.", 0 TSRMLS_CC);
            return;
        }
    }
    else
    {
        if (!zend_get_constant(ZEND_STRL("OCI_COMMIT_ON_SUCCESS"), &mode TSRMLS_CC))
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch constant OCI_COMMIT_ON_SUCCESS.", 0 TSRMLS_CC);
            return;
        }
    }

    oci = zend_read_property(Nc_Sql_OciConnection_ce, getThis(), ZEND_STRL("oci"), 0 TSRMLS_CC);

    stmt = callfunc(ZEND_STRL("oci_parse"), 2, oci, sql, NULL, NULL TSRMLS_CC);
    if (!stmt)
        return;

    success = callfunc(ZEND_STRL("oci_execute"), 2, stmt, &mode, NULL, NULL TSRMLS_CC);
    if (!success)
    {
        zval_ptr_dtor(&stmt);
        return;
    }

    if (Z_TYPE_P(success) != IS_BOOL || !Z_BVAL_P(success))
    {
        zval *err, **ppz;
        char *e;

        err = callfunc(ZEND_STRL("oci_error"), 1, oci, NULL, NULL, NULL TSRMLS_CC);
        if (err && Z_TYPE_P(err) == IS_ARRAY
            && zend_hash_find(Z_ARRVAL_P(err), ZEND_STRS("message"), (void **) &ppz) == SUCCESS
            && Z_TYPE_PP(ppz) == IS_STRING)
            e = Z_STRVAL_PP(ppz);
        else
            e = "Unknown error";

        zend_throw_exception_ex(Nc_Sql_QueryException_ce, 0 TSRMLS_CC, "%s [SQL: %s]", e, Z_STRVAL_P(sql));

        zval_ptr_dtor(&stmt);
        zval_ptr_dtor(&success);
        return;
    }

    zval_ptr_dtor(&success);

    flag &= NC_SQL_CONNECTION_FETCH;
    if (!flag)
    {
        zval_ptr_dtor(&stmt);
        RETURN_TRUE;
    }

    ociFetchMode = zend_read_property(Nc_Sql_OciConnection_ce, getThis(), ZEND_STRL("ociFetchMode"), 0 TSRMLS_CC);
    if (Z_TYPE_P(ociFetchMode) != IS_LONG)
        convert_to_long(ociFetchMode);

    switch (flag)
    {
    case NC_SQL_CONNECTION_ONE:
        if (!zend_get_constant(ZEND_STRL("OCI_NUM"), &mode TSRMLS_CC) || Z_TYPE(mode) != IS_LONG)
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch oci long constant OCI_NUM.", 0 TSRMLS_CC);
            break;
        }
        ZVAL_LONG(&mode, Z_LVAL(mode) + Z_LVAL_P(ociFetchMode));
        data = callfunc(ZEND_STRL("oci_fetch_array"), 2, stmt, &mode, NULL, NULL TSRMLS_CC);
        if (!data)
            break;
        if (Z_TYPE_P(data) == IS_ARRAY && zend_hash_index_find(Z_ARRVAL_P(data), 0, (void **) &ppz) == SUCCESS)
        {
            if (Z_TYPE_PP(ppz) != IS_STRING)
                convert_to_string(*ppz);
            RETVAL_STRINGL(Z_STRVAL_PP(ppz), Z_STRLEN_PP(ppz), 1);
        }
        else
        {
            RETVAL_EMPTY_STRING();
        }
        zval_ptr_dtor(&data);
        break;

    case NC_SQL_CONNECTION_ROW:
        if (!zend_get_constant(ZEND_STRL("OCI_ASSOC"), &mode TSRMLS_CC) || Z_TYPE(mode) != IS_LONG)
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch oci long constant OCI_ASSOC.", 0 TSRMLS_CC);
            break;
        }
        ZVAL_LONG(&mode, Z_LVAL(mode) + Z_LVAL_P(ociFetchMode));
        data = callfunc(ZEND_STRL("oci_fetch_array"), 2, stmt, &mode, NULL, NULL TSRMLS_CC);
        if (!data)
            break;
        if (Z_TYPE_P(data) != IS_ARRAY)
        {
            zval_ptr_dtor(&data);
            ALLOC_INIT_ZVAL(data);
            array_init(data);
            RETVAL_ZVAL(data, 0, 0);
            efree(data);
        }
        else
        {
            zval *pz;
            pz = callfunc(ZEND_STRL("array_change_key_case"), 1, data, NULL, NULL, NULL TSRMLS_CC);
            zval_ptr_dtor(&data);
            RETVAL_ZVAL(pz, 0, 0);
            efree(pz);
        }
        break;

    case NC_SQL_CONNECTION_ALL:
        if (!zend_get_constant(ZEND_STRL("OCI_ASSOC"), &mode TSRMLS_CC) || Z_TYPE(mode) != IS_LONG)
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch oci long constant OCI_ASSOC.", 0 TSRMLS_CC);
            break;
        }
        ZVAL_LONG(&mode, Z_LVAL(mode) + Z_LVAL_P(ociFetchMode));
        ALLOC_INIT_ZVAL(data);
        array_init(data);
        do
        {
            zval *row;
            zval *pz;
            row = callfunc(ZEND_STRL("oci_fetch_array"), 2, stmt, &mode, NULL, NULL TSRMLS_CC);
            if (!row)
                break;
            if (Z_TYPE_P(row) != IS_ARRAY)
            {
                zval_ptr_dtor(&row);
                break;
            }
            pz = callfunc(ZEND_STRL("array_change_key_case"), 1, row, NULL, NULL, NULL TSRMLS_CC);
            zval_ptr_dtor(&row);
            add_next_index_zval(data, pz);
        }
        while (1);
        RETVAL_ZVAL(data, 0, 0);
        efree(data);
        break;

    case NC_SQL_CONNECTION_COLUMN:
        if (!zend_get_constant(ZEND_STRL("OCI_NUM"), &mode TSRMLS_CC) || Z_TYPE(mode) != IS_LONG)
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch oci long constant OCI_NUM.", 0 TSRMLS_CC);
            break;
        }
        ZVAL_LONG(&mode, Z_LVAL(mode) + Z_LVAL_P(ociFetchMode));
        ALLOC_INIT_ZVAL(data);
        array_init(data);
        do
        {
            zval *pz;
            pz = callfunc(ZEND_STRL("oci_fetch_array"), 2, stmt, &mode, NULL, NULL TSRMLS_CC);
            if (!pz)
                break;
            if (Z_TYPE_P(pz) != IS_ARRAY || zend_hash_index_find(Z_ARRVAL_P(pz), 0, (void **) &ppz) != SUCCESS || Z_TYPE_PP(ppz) != IS_STRING)
            {
                zval_ptr_dtor(&pz);
                break;
            }
            add_next_index_stringl(data, Z_STRVAL_PP(ppz), Z_STRLEN_PP(ppz), 1);
            zval_ptr_dtor(&pz);
        }
        while (1);
        RETVAL_ZVAL(data, 0, 0);
        efree(data);
        break;

    default:
        RETVAL_FALSE;
        break;
    }

    zval_ptr_dtor(&stmt);
}

zend_function_entry Nc_Sql_OciConnection_methods[] = {
    ZEND_ME(Nc_Sql_OciConnection, __construct, Nc_Sql_OciConnection_begin_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(Nc_Sql_OciConnection, begin, Nc_Sql_OciConnection_begin_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_OciConnection, commit, Nc_Sql_OciConnection_commit_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_OciConnection, rollback, Nc_Sql_OciConnection_rollback_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_OciConnection, query, Nc_Sql_OciConnection_query_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
