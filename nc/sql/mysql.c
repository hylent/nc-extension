zend_class_entry *Nc_Sql_Mysql_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Mysql)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Mysql", Nc_Sql_Mysql_methods);
    Nc_Sql_Mysql_ce = zend_register_internal_class_ex(&ce, Nc_Sql_Adapter_ce, NULL TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Mysql_buildSelectQuery_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Mysql_insert_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, table)
    ZEND_ARG_ARRAY_INFO(0, data, 0)
    ZEND_ARG_INFO(0, returningId)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Mysql_quote_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Mysql_buildUpdateExpression_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_Mysql, buildSelectQuery)
{
    zval *select;

    char *paginationQuery = NULL;
    zval *pagination;
    char *parts[9] = {"", NULL, NULL, NULL, NULL, NULL, NULL, NULL, ""};
    char *part8 = NULL;
    uint part8Len = 0;
    char *s;
    uint sLen = 0;

    const char *names[7] = {"buildselectfield", "buildselecttable", "buildselectjoin", "buildselectwhere", "buildselectgroup", "buildselecthaving", "buildselectorder"};
    int ipos;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    pagination = zend_read_property(Nc_Sql_Select_ce, select, ZEND_STRL("pagination"), 0 TSRMLS_CC);

    if (pagination && Z_TYPE_P(pagination) == IS_BOOL && Z_BVAL_P(pagination))
    {
        zval *countingRows, *pageSize, *rowOffset;
        zval ps, ro;

        countingRows = zend_read_property(Nc_Sql_Select_ce, select, ZEND_STRL("countingRows"), 0 TSRMLS_CC);

        if (countingRows && Z_TYPE_P(countingRows) == IS_BOOL && Z_BVAL_P(countingRows))
        {
            parts[0] = " sql_calc_found_rows";
            sLen += 20;
            paginationQuery = "select found_rows()";
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

        part8Len = 7 + Z_STRLEN(ro) + 2 + Z_STRLEN(ps);
        part8 = emalloc(part8Len + 1);
        snprintf(part8, part8Len + 1, " limit %s, %s", Z_STRVAL(ro), Z_STRVAL(ps));

        zval_dtor(&ps);
        zval_dtor(&ro);

        parts[8] = part8;
        sLen += part8Len;
    }

    for (ipos = 0; ipos < 7; ++ipos)
    {
        zval *tmp = NULL;
        tmp = zend_call_method(&getThis(), Z_OBJCE_P(getThis()), NULL, names[ipos], strlen(names[ipos]), &tmp, 1, select, NULL TSRMLS_CC);

        if (!tmp)
        {
            zend_throw_exception_ex(Nc_Sql_Exception_ce, 0 TSRMLS_CC, "Cannot invoke $this->%s().", names[ipos]);
            break;
        }

        if (Z_TYPE_P(tmp) != IS_STRING)
            convert_to_string(tmp);

        parts[ipos + 1] = Z_STRVAL_P(tmp);
        sLen += Z_STRLEN_P(tmp);

        efree(tmp);
    }

    if (ipos >= 7)
    {
        sLen += 13;
        s = emalloc(sLen + 1);
        snprintf(s, sLen + 1, "select%s %s from %s%s%s%s%s%s%s", parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6], parts[7], parts[8]);

        RETVAL_STRINGL(s, sLen, 0);
    }

    for (ipos = 1; ipos <= 7; ++ipos)
        if (parts[ipos])
            efree(parts[ipos]);

    if (part8Len > 0 && part8)
        efree(part8);

    if (paginationQuery)
        zend_update_property_string(Z_OBJCE_P(select), select, ZEND_STRL("paginationQuery"), paginationQuery TSRMLS_CC);
    else
        zend_update_property_null(Z_OBJCE_P(select), select, ZEND_STRL("paginationQuery") TSRMLS_CC);
}

ZEND_METHOD(Nc_Sql_Mysql, insert)
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

    ZVAL_LONG(&flag, NC_SQL_CONNECTION_WRITE | NC_SQL_CONNECTION_INSERT);
    retval = zend_call_method_with_2_params(&connection, Z_OBJCE_P(connection), NULL, "query", &retval, sql, &flag);
    zval_ptr_dtor(&sql);

    if (!retval || Z_TYPE_P(retval) != IS_BOOL || !Z_BVAL_P(retval))
    {
        zval_ptr_dtor(&connection);
        if (retval)
            zval_ptr_dtor(&retval);
        RETURN_FALSE;
    }

    zval_ptr_dtor(&retval);

    if (returningIdLen > 0 && returningId)
    {
        zval returningIdSql;

        ZVAL_STRING(&returningIdSql, "select last_insert_id()", 0);
        ZVAL_LONG(&flag, NC_SQL_CONNECTION_WRITE | NC_SQL_CONNECTION_SELECT | NC_SQL_CONNECTION_ONE);
        retval = zend_call_method_with_2_params(&connection, Z_OBJCE_P(connection), NULL, "query", &retval, &returningIdSql, &flag);

        RETVAL_ZVAL(retval, 0, 0);
        efree(retval);
        zval_ptr_dtor(&connection);
        return;
    }

    RETURN_TRUE;
}

ZEND_METHOD(Nc_Sql_Mysql, quote)
{
    char *s, *r;
    uint sLen, rLen = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &s, &sLen) == FAILURE)
        return;

    s = php_addslashes(s, sLen, &rLen, 0 TSRMLS_CC);
    rLen += 2;
    r = (char *) emalloc(rLen + 1);
    r[0] = '\'';
    strncpy(r + 1, s, rLen - 2);
    r[rLen - 1] = '\'';
    r[rLen] = '\0';
    efree(s);

    RETURN_STRINGL(r, rLen, 0)
}

ZEND_METHOD(Nc_Sql_Mysql, buildUpdateExpression)
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
                rLen = 7 + fieldLen + 2 + Z_STRLEN_P(qv) + 1;
                r = emalloc(rLen + 1);
                snprintf(r, rLen + 1, "concat(%s, %s)", field, Z_STRVAL_PP(ppz), Z_STRVAL_P(qv));

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

zend_function_entry Nc_Sql_Mysql_methods[] = {
    ZEND_ME(Nc_Sql_Mysql, buildSelectQuery, Nc_Sql_Mysql_buildSelectQuery_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Mysql, insert, Nc_Sql_Mysql_insert_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Mysql, quote, Nc_Sql_Mysql_quote_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Mysql, buildUpdateExpression, Nc_Sql_Mysql_buildUpdateExpression_arginfo, ZEND_ACC_PROTECTED)
    ZEND_FE_END
};
