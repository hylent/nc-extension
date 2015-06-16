zend_class_entry *Nc_Sql_Oci_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Oci)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Oci", Nc_Sql_Oci_methods);
    Nc_Sql_Oci_ce = zend_register_internal_class_ex(&ce, Nc_Sql_Adapter_ce, NULL TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Oci_buildSelectQuery_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Oci_insert_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, table)
    ZEND_ARG_ARRAY_INFO(0, data, 0)
    ZEND_ARG_INFO(0, returningId)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Oci_quote_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Oci_buildUpdateExpression_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_Oci, buildSelectQuery)
{
    zval *select;
    char *paginationQuery = NULL;
    zval *pagination;
    char *parts[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char *s;
    uint sLen = 0;
    zval *tmp[7];
    int i;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    tmp[0] = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildselectfield", &tmp[0], select);
    tmp[1] = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildselecttable", &tmp[1], select);
    tmp[2] = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildselectjoin", &tmp[2], select);
    tmp[3] = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildselectwhere", &tmp[3], select);
    tmp[4] = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildselectgroup", &tmp[4], select);
    tmp[5] = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildselecthaving", &tmp[5], select);
    tmp[6] = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildselectorder", &tmp[6], select);

    for (i = 0; i < 7; ++i)
    {
        if (tmp[i])
        {
            if (Z_TYPE_P(tmp[i]) != IS_STRING)
                convert_to_string(tmp[i]);

            parts[i] = Z_STRVAL_P(tmp[i]);
            sLen += Z_STRLEN_P(tmp[i]);
            continue;
        }

        zend_throw_exception_ex(Nc_Sql_Exception_ce, 0 TSRMLS_CC, "Error occurred when building sql parts %d.", i);
        while (i--)
            zval_ptr_dtor(&tmp[i]);

        return;
    }

    pagination = zend_read_property(Nc_Sql_Select_ce, select, ZEND_STRL("pagination"), 0 TSRMLS_CC);

    if (pagination && Z_TYPE_P(pagination) == IS_BOOL && Z_BVAL_P(pagination))
    {
        zval *countingRows, *pageSize, *rowOffset;
        zval ps, ro;

        countingRows = zend_read_property(Nc_Sql_Select_ce, select, ZEND_STRL("countingRows"), 0 TSRMLS_CC);

        if (countingRows && Z_TYPE_P(countingRows) == IS_BOOL && Z_BVAL_P(countingRows))
        {
            uint pqLen = 21 + Z_STRLEN_P(tmp[1]) + Z_STRLEN_P(tmp[2]) + Z_STRLEN_P(tmp[3]);
            paginationQuery = emalloc(pqLen + 1);
            snprintf(paginationQuery, pqLen + 1, "select count(*) from %s%s%s", parts[1], parts[2], parts[3]);
        }

        pageSize = zend_read_property(Nc_Sql_Select_ce, select, ZEND_STRL("pageSize"), 0 TSRMLS_CC);
        rowOffset = zend_read_property(Nc_Sql_Select_ce, select, ZEND_STRL("rowOffset"), 0 TSRMLS_CC);

        if (Z_TYPE_P(pageSize) != IS_LONG || Z_TYPE_P(rowOffset) != IS_LONG)
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Invalid pagination properties type.", 0 TSRMLS_CC);
            return;
        }

        ZVAL_COPY_VALUE(&ps, pageSize);
        ZVAL_COPY_VALUE(&ro, rowOffset);
        convert_to_string(&ps);
        convert_to_string(&ro);

        sLen += 50 + 6 + 21 + Z_STRLEN(ps) + 14 + Z_STRLEN(ro);
        s = emalloc(sLen + 1);
        snprintf(s, sLen + 1, "select * from (select _a.*, rownum r from (select %s from %s%s%s%s%s%s) _a where rownum <= %s) b where r > %s",
            parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6], Z_STRVAL(ps), Z_STRVAL(ro));

        zval_dtor(&ps);
        zval_dtor(&ro);
    }
    else
    {
        sLen += 13;
        s = emalloc(sLen + 1);
        snprintf(s, sLen + 1, "select %s from %s%s%s%s%s%s", parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6]);
    }

    RETVAL_STRINGL(s, sLen, 0);

    for (i = 0; i < 7; ++i)
        zval_ptr_dtor(&tmp[i]);

    if (paginationQuery)
    {
        zend_update_property_string(Z_OBJCE_P(select), select, ZEND_STRL("paginationQuery"), paginationQuery TSRMLS_CC);
        efree(paginationQuery);
    }
    else
    {
        zend_update_property_null(Z_OBJCE_P(select), select, ZEND_STRL("paginationQuery") TSRMLS_CC);
    }
}

ZEND_METHOD(Nc_Sql_Oci, insert)
{
    char *table, *returningId = NULL;
    uint tableLen, returningIdLen = 0;
    zval *data;
    zval *connection, *sql, ztable, flag, *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa|s", &table, &tableLen, &data, &returningId, &returningIdLen) == FAILURE)
        return;

    connection = nc_kernel_injection_property(getThis(), "connection", Nc_Sql_Connection_ce);
    if (!connection)
        return;

    ZVAL_STRINGL(&ztable, table, tableLen, 0);
    sql = zend_call_method_with_2_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildinsertquery", &sql, &ztable, data);

    if (!sql)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke $this->buildInsertQuery().", 0 TSRMLS_CC);
        zval_ptr_dtor(&connection);
        return;
    }

    if (Z_TYPE_P(sql) != IS_STRING)
        convert_to_string(sql);

    if (returningIdLen > 0 && returningId)
    {
        char *s;
        uint sLen;

        sLen = Z_STRLEN_P(sql) + 11 + returningIdLen;
        s = emalloc(sLen + 1);
        strncpy(s, Z_STRVAL_P(sql), Z_STRLEN_P(sql));
        strncpy(s + Z_STRLEN_P(sql), " returning ", 11);
        strncpy(s + Z_STRLEN_P(sql) + 11, returningId, returningIdLen);
        s[sLen] = 0;

        ZVAL_STRINGL(sql, s, sLen, 0);
        ZVAL_LONG(&flag, NC_SQL_CONNECTION_WRITE | NC_SQL_CONNECTION_INSERT | NC_SQL_CONNECTION_ONE);
    }
    else
    {
        ZVAL_LONG(&flag, NC_SQL_CONNECTION_WRITE | NC_SQL_CONNECTION_INSERT);
    }

    retval = zend_call_method_with_2_params(&connection, Z_OBJCE_P(connection), NULL, "query", &retval, sql, &flag);

    zval_ptr_dtor(&sql);
    zval_ptr_dtor(&connection);

    if (!retval)
        RETURN_FALSE;

    RETVAL_ZVAL(retval, 0, 0);
    efree(retval);
}

ZEND_METHOD(Nc_Sql_Oci, quote)
{
    char *s;
    uint sLen;

    char *buf;
    uint bufp = 0;
    uint sp = 0;
    uint step = 2;
    uint bufPlus = 0;
    uint lenPlus = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &s, &sLen) == FAILURE)
        return;

    buf = emalloc(sLen + 3);
    buf[bufp++] = '\'';

    while (sp < sLen)
    {
        if (s[sp] == '\'')
        {
            ++lenPlus;
            if (lenPlus > bufPlus)
            {
                bufPlus += step;
                step *= 2;
                buf = erealloc(buf, sLen + 3 + bufPlus);
            }
            buf[bufp++] = '\'';
        }
        buf[bufp++] = s[sp++];
    }

    buf[bufp++] = '\'';
    buf[bufp] = 0;

    RETVAL_STRINGL(buf, bufp, 1);
    efree(buf);
}

ZEND_METHOD(Nc_Sql_Oci, buildUpdateExpression)
{
    char *field;
    uint fieldLen;
    zval *options;
    zval **ppz;
    char *r;
    uint rLen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &field, &fieldLen, &options) == FAILURE)
        return;

    do
    {
        if (zend_hash_index_find(Z_ARRVAL_P(options), 0, (void **) &ppz) == FAILURE || Z_TYPE_PP(ppz) != IS_STRING)
            break;

        if (Z_STRLEN_PP(ppz) == 1)
        {
            zval **ppz2, *qv;
            if (zend_hash_index_find(Z_ARRVAL_P(options), 1, (void **) &ppz2) == FAILURE)
                break;

            if (Z_TYPE_PP(ppz2) != IS_STRING)
                convert_to_string(*ppz2);

            qv = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "quote", &qv, *ppz2);

            switch (*Z_STRVAL_PP(ppz))
            {
            case '+':
            case '-':
                rLen = fieldLen + 3 + Z_STRLEN_P(qv);
                r = emalloc(rLen + 1);
                snprintf(r, rLen + 1, "%s %s %s", field, Z_STRVAL_PP(ppz), Z_STRVAL_P(qv));

                zval_ptr_dtor(&qv);
                RETURN_STRINGL(r, rLen, 0);

            case '.':
                rLen = fieldLen + 4 + Z_STRLEN_P(qv);
                r = emalloc(rLen + 1);
                snprintf(r, rLen + 1, "%s || %s", field, Z_STRVAL_PP(ppz), Z_STRVAL_P(qv));

                zval_ptr_dtor(&qv);
                RETURN_STRINGL(r, rLen, 0);
            }

            zval_ptr_dtor(&qv);
        }
        else
        {
            if (strcmp("raw", Z_STRVAL_PP(ppz)) == 0)
            {
                zval **ppz2;
                if (zend_hash_index_find(Z_ARRVAL_P(options), 1, (void **) &ppz2) == FAILURE)
                    break;

                if (Z_TYPE_PP(ppz2) != IS_STRING)
                    convert_to_string(*ppz2);

                RETURN_STRINGL(Z_STRVAL_PP(ppz2), Z_STRLEN_PP(ppz2), 1);
            }
        }
    }
    while (0);

    zend_throw_exception(Nc_Sql_Exception_ce, "Invalid update expression options.", 0 TSRMLS_CC);
}

zend_function_entry Nc_Sql_Oci_methods[] = {
    ZEND_ME(Nc_Sql_Oci, buildSelectQuery, Nc_Sql_Oci_buildSelectQuery_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Oci, insert, Nc_Sql_Oci_insert_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Oci, quote, Nc_Sql_Oci_quote_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Oci, buildUpdateExpression, Nc_Sql_Oci_buildUpdateExpression_arginfo, ZEND_ACC_PROTECTED)
    ZEND_FE_END
};
