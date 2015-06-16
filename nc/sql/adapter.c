zend_class_entry *Nc_Sql_Adapter_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Adapter)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Adapter", Nc_Sql_Adapter_methods);
    Nc_Sql_Adapter_ce = zend_register_internal_class_ex(&ce, Nc_Kernel_ce, NULL TSRMLS_CC);
    Nc_Sql_Adapter_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_invokeSelect_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildSelectQuery_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_delete_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, table)
    ZEND_ARG_INFO(0, where)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_insert_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, table)
    ZEND_ARG_ARRAY_INFO(0, data, 0)
    ZEND_ARG_INFO(0, returningId)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_update_arginfo, 0, 0, 3)
    ZEND_ARG_INFO(0, table)
    ZEND_ARG_INFO(0, where)
    ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_quote_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildSelectTable_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildSelectField_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildSelectJoin_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildSelectWhere_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildSelectGroup_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildSelectHaving_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildSelectOrder_arginfo, 0, 0, 1)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildInsertQuery_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, table)
    ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Adapter_buildUpdateExpression_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_Adapter, invokeSelect)
{
    zval *select;
    zval *flag;
    zval *sql;
    zval *connection;
    zval *data;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    flag = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("flag"), 0 TSRMLS_CC);
    if (Z_TYPE_P(flag) != IS_LONG)
        convert_to_long(flag);

    ZVAL_LONG(flag, (long) (NC_SQL_CONNECTION_READ | NC_SQL_CONNECTION_SELECT | (NC_SQL_CONNECTION_FETCH & Z_LVAL_P(flag))));

    sql = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildselectquery", &sql, select);
    if (!sql || Z_TYPE_P(sql) != IS_STRING)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Invalid query sql.", 0 TSRMLS_CC);
        if (sql)
            zval_ptr_dtor(&sql);
        return;
    }

    connection = nc_kernel_injection_property(getThis(), "connection", Nc_Sql_Connection_ce);
    if (!connection)
    {
        zval_ptr_dtor(&sql);
        return;
    }

    data = zend_call_method_with_2_params(&connection, Z_OBJCE_P(connection), NULL, "query", &data, sql, flag);
    if (data)
    {
        zval *pagination, *countingRows;
        pagination = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("pagination"), 0 TSRMLS_CC);
        countingRows = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("countingRows"), 0 TSRMLS_CC);

        if (Z_TYPE_P(pagination) == IS_BOOL && Z_BVAL_P(pagination) && Z_TYPE_P(countingRows) == IS_BOOL && Z_BVAL_P(countingRows))
        {
            zval f, *s, *r, *r2;
            ZVAL_LONG(&f, NC_SQL_CONNECTION_READ | NC_SQL_CONNECTION_SELECT | NC_SQL_CONNECTION_ONE);
            s = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("paginationQuery"), 0 TSRMLS_CC);
            r = zend_call_method_with_2_params(&connection, Z_OBJCE_P(connection), NULL, "query", &r, s, &f);
            if (r)
            {
                r2 = zend_call_method_with_1_params(&select, Z_OBJCE_P(select), NULL, "setnumrows", &r2, r);
                if (r2)
                    zval_ptr_dtor(&r2);
                zval_ptr_dtor(&r);
            }
        }

        RETVAL_ZVAL(data, 0, 0);
        efree(data);
    }

    zval_ptr_dtor(&sql);
    zval_ptr_dtor(&connection);
}

ZEND_METHOD(Nc_Sql_Adapter, delete)
{
    char *table;
    uint tableLen;
    zval *where;
    zval *connection;
    char *s;
    uint sLen;
    zval *sql;
    zval flag;
    zval *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &table, &tableLen, &where) == FAILURE)
        return;

    if (Z_TYPE_P(where) != IS_STRING)
        convert_to_string(where);

    connection = nc_kernel_injection_property(getThis(), "connection", Nc_Sql_Connection_ce);
    if (!connection)
        return;

    sLen = 12 + tableLen + 7 + Z_STRLEN_P(where);
    s = emalloc(sLen + 1);
    snprintf(s, sLen + 1, "delete from %s where %s", table, Z_STRVAL_P(where));
    ALLOC_INIT_ZVAL(sql);
    ZVAL_STRINGL(sql, s, sLen, 0);

    ZVAL_LONG(&flag, NC_SQL_CONNECTION_WRITE | NC_SQL_CONNECTION_DELETE);
    retval = zend_call_method_with_2_params(&connection, Z_OBJCE_P(connection), NULL, "query", &retval, sql, &flag);

    if (retval)
    {
        RETVAL_ZVAL(retval, 0, 0);
        efree(retval);
    }
    else
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke #Nc\\Sql\\Conneciton::query()#.", 0 TSRMLS_CC);
    }

    zval_ptr_dtor(&connection);
    zval_ptr_dtor(&sql);
}

ZEND_METHOD(Nc_Sql_Adapter, update)
{
    char *table;
    uint tableLen;
    zval *where;
    zval *data;
    zval *kvs, delim, kvss;
    char *s;
    uint sLen;
    zval *sql;
    zval *connection;
    zval flag;
    zval *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sza", &table, &tableLen, &where, &data) == FAILURE)
        return;

    if (Z_TYPE_P(where) != IS_STRING)
        convert_to_string(where);

    connection = nc_kernel_injection_property(getThis(), "connection", Nc_Sql_Connection_ce);
    if (!connection)
        return;

    ALLOC_INIT_ZVAL(kvs);
    array_init(kvs);

    for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(data));
        zend_hash_has_more_elements(Z_ARRVAL_P(data)) == SUCCESS;
        zend_hash_move_forward(Z_ARRVAL_P(data)))
    {
        char *k;
        uint kSize;
        zval **ppz, *v;
        char *kv;
        uint kvLen;

        if (zend_hash_get_current_key_ex(Z_ARRVAL_P(data), &k, &kSize, NULL, 0, NULL) != HASH_KEY_IS_STRING
            || zend_hash_get_current_data(Z_ARRVAL_P(data), (void **) &ppz) != SUCCESS)
            continue;

        if (Z_TYPE_PP(ppz) == IS_ARRAY)
        {
            zval tmpk;
            ZVAL_STRING(&tmpk, k, 0);
            v = zend_call_method_with_2_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "buildupdateexpression", &v, &tmpk, *ppz);
            if (!v)
                continue;
            if (Z_TYPE_P(v) != IS_STRING)
                convert_to_string(v);
        }
        else
        {
            if (Z_TYPE_P(*ppz) != IS_STRING)
                convert_to_string(*ppz);
            v = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "quote", &v, *ppz);
            if (!v)
                continue;
        }

        kvLen = kSize - 1 + 3 + Z_STRLEN_P(v);
        kv = emalloc(kvLen + 1);
        snprintf(kv, kvLen + 1, "%s = %s", k, Z_STRVAL_P(v));

        zval_ptr_dtor(&v);

        add_next_index_stringl(kvs, kv, kvLen, 0);
    }

    ZVAL_STRING(&delim, ", ", 0);
    php_implode(&delim, kvs, &kvss TSRMLS_CC);

    sLen = 7 + tableLen + 5 + Z_STRLEN(kvss) + 7 + Z_STRLEN_P(where);
    s = emalloc(sLen + 1);
    snprintf(s, sLen + 1, "update %s set %s where %s", table, Z_STRVAL(kvss), Z_STRVAL_P(where));

    zval_ptr_dtor(&kvs);
    zval_dtor(&kvss);

    ALLOC_INIT_ZVAL(sql);
    ZVAL_STRINGL(sql, s, sLen, 0);

    ZVAL_LONG(&flag, NC_SQL_CONNECTION_WRITE | NC_SQL_CONNECTION_UPDATE);
    retval = zend_call_method_with_2_params(&connection, Z_OBJCE_P(connection), NULL, "query", &retval, sql, &flag);

    if (retval)
    {
        RETVAL_ZVAL(retval, 0, 0);
        efree(retval);
    }
    else
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke #Nc\\Sql\\Conneciton::query()#.", 0 TSRMLS_CC);
    }

    zval_ptr_dtor(&connection);
    zval_ptr_dtor(&sql);
}

ZEND_METHOD(Nc_Sql_Adapter, buildSelectTable)
{
    zval *select;
    zval *tables, *arr;
    zval delim, str;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    tables = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("tables"), 0 TSRMLS_CC);
    if (Z_TYPE_P(tables) != IS_ARRAY)
        RETURN_EMPTY_STRING();

    ALLOC_INIT_ZVAL(arr);
    array_init(arr);

    for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(tables));
        zend_hash_has_more_elements(Z_ARRVAL_P(tables)) == SUCCESS;
        zend_hash_move_forward(Z_ARRVAL_P(tables)))
    {
        char *k;
        uint kSize;
        zval **ppz;
        char *part;
        uint partLen;

        if (zend_hash_get_current_key_ex(Z_ARRVAL_P(tables), &k, &kSize, NULL, 0, NULL) != HASH_KEY_IS_STRING
            || zend_hash_get_current_data(Z_ARRVAL_P(tables), (void **) &ppz) != SUCCESS
            || Z_TYPE_PP(ppz) != IS_STRING)
            continue;

        partLen = kSize + Z_STRLEN_PP(ppz);
        part = emalloc(partLen + 1);
        strncpy(part, Z_STRVAL_PP(ppz), Z_STRLEN_PP(ppz));
        part[Z_STRLEN_PP(ppz)] = ' ';
        strncpy(part + (Z_STRLEN_PP(ppz) + 1), k, kSize - 1);
        part[partLen] = 0;

        add_next_index_stringl(arr, part, partLen, 0);
    }

    ZVAL_STRING(&delim, ", ", 0);
    php_implode(&delim, arr, &str TSRMLS_CC);

    zval_ptr_dtor(&arr);

    RETURN_STRINGL(Z_STRVAL(str), Z_STRLEN(str), 0);
}

ZEND_METHOD(Nc_Sql_Adapter, buildSelectField)
{
    zval *select;
    zval *field, *arr;
    zval delim, str;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    field = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("field"), 0 TSRMLS_CC);
    if (Z_TYPE_P(field) != IS_ARRAY)
        RETURN_STRING("*", 1);

    ALLOC_INIT_ZVAL(arr);
    array_init(arr);

    for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(field));
        zend_hash_has_more_elements(Z_ARRVAL_P(field)) == SUCCESS;
        zend_hash_move_forward(Z_ARRVAL_P(field)))
    {
        zval **ppz, **ppz1, **ppz2;
        char *part;
        uint partLen;

        if (zend_hash_get_current_data(Z_ARRVAL_P(field), (void **) &ppz) != SUCCESS || Z_TYPE_PP(ppz) != IS_ARRAY
            || zend_hash_index_find(Z_ARRVAL_PP(ppz), 0, (void **) &ppz1) != SUCCESS || Z_TYPE_PP(ppz1) != IS_STRING
            || zend_hash_index_find(Z_ARRVAL_PP(ppz), 1, (void **) &ppz2) != SUCCESS)
            continue;

        if (Z_TYPE_PP(ppz2) == IS_NULL)
        {
            add_next_index_zval(arr, *ppz1);
            continue;
        }

        if (Z_TYPE_PP(ppz2) != IS_STRING)
            continue;

        partLen = Z_STRLEN_PP(ppz1) + 1 + Z_STRLEN_PP(ppz2);
        part = emalloc(partLen + 1);
        strncpy(part, Z_STRVAL_PP(ppz1), Z_STRLEN_PP(ppz1));
        part[Z_STRLEN_PP(ppz1)] = ' ';
        strncpy(part + (Z_STRLEN_PP(ppz1) + 1), Z_STRVAL_PP(ppz2), Z_STRLEN_PP(ppz2));
        part[partLen] = 0;

        add_next_index_stringl(arr, part, partLen, 0);
    }

    ZVAL_STRING(&delim, ", ", 0);
    php_implode(&delim, arr, &str TSRMLS_CC);

    zval_ptr_dtor(&arr);

    RETURN_STRINGL(Z_STRVAL(str), Z_STRLEN(str), 0);
}

ZEND_METHOD(Nc_Sql_Adapter, buildSelectJoin)
{
    zval *select;
    zval *join, *arr;
    zval delim, str;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    join = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("join"), 0 TSRMLS_CC);
    if (Z_TYPE_P(join) != IS_ARRAY)
        RETURN_EMPTY_STRING();

    ALLOC_INIT_ZVAL(arr);
    array_init(arr);

    for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(join));
        zend_hash_has_more_elements(Z_ARRVAL_P(join)) == SUCCESS;
        zend_hash_move_forward(Z_ARRVAL_P(join)))
    {
        zval **ppz, **ppz1, **ppz2, **ppz3, **ppz4;
        char *part;
        uint partLen;

        if (zend_hash_get_current_data(Z_ARRVAL_P(join), (void **) &ppz) != SUCCESS || Z_TYPE_PP(ppz) != IS_ARRAY
            || zend_hash_index_find(Z_ARRVAL_PP(ppz), 0, (void **) &ppz1) != SUCCESS || Z_TYPE_PP(ppz1) != IS_STRING
            || zend_hash_index_find(Z_ARRVAL_PP(ppz), 1, (void **) &ppz2) != SUCCESS || Z_TYPE_PP(ppz2) != IS_STRING
            || zend_hash_index_find(Z_ARRVAL_PP(ppz), 2, (void **) &ppz3) != SUCCESS || Z_TYPE_PP(ppz3) != IS_STRING
            || zend_hash_index_find(Z_ARRVAL_PP(ppz), 3, (void **) &ppz4) != SUCCESS || Z_TYPE_PP(ppz4) != IS_STRING)
            continue;

        partLen = 1 + Z_STRLEN_PP(ppz1) + 6 + Z_STRLEN_PP(ppz2) + 1 + Z_STRLEN_PP(ppz3) + 4 + Z_STRLEN_PP(ppz4);
        part = emalloc(partLen + 1);
        snprintf(part, partLen + 1, " %s join %s %s on %s", Z_STRVAL_PP(ppz4), Z_STRVAL_PP(ppz1), Z_STRVAL_PP(ppz2), Z_STRVAL_PP(ppz3));
        part[partLen] = 0;

        add_next_index_stringl(arr, part, partLen, 0);
    }

    ZVAL_STRING(&delim, "", 0);
    php_implode(&delim, arr, &str TSRMLS_CC);

    zval_ptr_dtor(&arr);

    RETURN_STRINGL(Z_STRVAL(str), Z_STRLEN(str), 0);
}

ZEND_METHOD(Nc_Sql_Adapter, buildSelectWhere)
{
    zval *select;
    zval *where;
    char *s;
    uint sLen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    where = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("where"), 0 TSRMLS_CC);
    if (Z_TYPE_P(where) == IS_NULL)
        RETURN_EMPTY_STRING();

    if (Z_TYPE_P(where) != IS_STRING)
        convert_to_string(where);

    if (Z_STRLEN_P(where) < 1)
        RETURN_EMPTY_STRING();

    sLen = 7 + Z_STRLEN_P(where);
    s = emalloc(sLen + 1);
    snprintf(s, sLen + 1, " where %s", Z_STRVAL_P(where));

    RETURN_STRINGL(s, sLen, 0);
}

ZEND_METHOD(Nc_Sql_Adapter, buildSelectGroup)
{
    zval *select;
    zval *group;
    zval delim, str;
    char *s;
    uint sLen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    group = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("group"), 0 TSRMLS_CC);
    if (Z_TYPE_P(group) != IS_ARRAY)
        RETURN_EMPTY_STRING();

    ZVAL_STRING(&delim, ", ", 0);
    php_implode(&delim, group, &str TSRMLS_CC);

    sLen = 10 + Z_STRLEN(str);
    s = emalloc(sLen + 1);
    snprintf(s, sLen + 1, " group by %s", Z_STRVAL(str));

    efree(Z_STRVAL(str));

    RETURN_STRINGL(s, sLen, 0);
}

ZEND_METHOD(Nc_Sql_Adapter, buildSelectHaving)
{
    zval *select;
    zval *having;
    char *s;
    uint sLen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    having = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("having"), 0 TSRMLS_CC);
    if (Z_TYPE_P(having) == IS_NULL)
        RETURN_EMPTY_STRING();

    if (Z_TYPE_P(having) != IS_STRING)
        convert_to_string(having);

    if (Z_STRLEN_P(having) < 1)
        RETURN_EMPTY_STRING();

    sLen = 7 + Z_STRLEN_P(having);
    s = emalloc(sLen + 1);
    snprintf(s, sLen + 1, " having %s", Z_STRVAL_P(having));

    RETURN_STRINGL(s, sLen, 0);
}

ZEND_METHOD(Nc_Sql_Adapter, buildSelectOrder)
{
    zval *select;
    zval *order, *arr;
    zval delim, str;
    char *s;
    uint sLen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &select) == FAILURE)
        return;

    order = zend_read_property(Z_OBJCE_P(select), select, ZEND_STRL("order"), 0 TSRMLS_CC);
    if (Z_TYPE_P(order) != IS_ARRAY)
        RETURN_EMPTY_STRING();

    ALLOC_INIT_ZVAL(arr);
    array_init(arr);

    for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(order));
        zend_hash_has_more_elements(Z_ARRVAL_P(order)) == SUCCESS;
        zend_hash_move_forward(Z_ARRVAL_P(order)))
    {
        zval **ppz, **ppz1, **ppz2;
        char *part;
        uint partLen;

        if (zend_hash_get_current_data(Z_ARRVAL_P(order), (void **) &ppz) != SUCCESS || Z_TYPE_PP(ppz) != IS_ARRAY
            || zend_hash_index_find(Z_ARRVAL_PP(ppz), 0, (void **) &ppz1) != SUCCESS || Z_TYPE_PP(ppz1) != IS_STRING
            || zend_hash_index_find(Z_ARRVAL_PP(ppz), 1, (void **) &ppz2) != SUCCESS || Z_TYPE_PP(ppz2) != IS_BOOL)
            continue;

        if (Z_BVAL_PP(ppz2))
        {
            partLen = Z_STRLEN_PP(ppz1) + 4;
            part = emalloc(partLen + 1);
            snprintf(part, partLen + 1, "%s asc", Z_STRVAL_PP(ppz1));
        }
        else
        {
            partLen = Z_STRLEN_PP(ppz1) + 5;
            part = emalloc(partLen + 1);
            snprintf(part, partLen + 1, "%s desc", Z_STRVAL_PP(ppz1));
        }

        part[partLen] = 0;
        add_next_index_stringl(arr, part, partLen, 0);
    }

    ZVAL_STRING(&delim, ", ", 0);
    php_implode(&delim, arr, &str TSRMLS_CC);

    sLen = 10 + Z_STRLEN(str);
    s = emalloc(sLen + 1);
    snprintf(s, sLen + 1, " order by %s", Z_STRVAL(str));

    zval_dtor(&str);
    zval_ptr_dtor(&arr);

    RETURN_STRINGL(s, sLen, 0);
}

ZEND_METHOD(Nc_Sql_Adapter, buildInsertQuery)
{
    char *table;
    uint tableLen;
    zval *data;
    zval *ks, *vs, delim, kss, vss;
    char *s;
    uint sLen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &table, &tableLen, &data) == FAILURE)
        return;

    ALLOC_INIT_ZVAL(ks);
    ALLOC_INIT_ZVAL(vs);
    array_init(ks);
    array_init(vs);

    for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(data));
        zend_hash_has_more_elements(Z_ARRVAL_P(data)) == SUCCESS;
        zend_hash_move_forward(Z_ARRVAL_P(data)))
    {
        char *k;
        zval **ppz;
        zval *v;

        if (zend_hash_get_current_key(Z_ARRVAL_P(data), &k, NULL, 0) != HASH_KEY_IS_STRING
            || zend_hash_get_current_data(Z_ARRVAL_P(data), (void **) &ppz) != SUCCESS)
            continue;

        if (Z_TYPE_PP(ppz) != IS_STRING)
            convert_to_string(*ppz);

        v = zend_call_method_with_1_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "quote", &v, *ppz);

        add_next_index_string(ks, k, 1);
        add_next_index_zval(vs, v);
    }

    ZVAL_STRING(&delim, ", ", 0);
    php_implode(&delim, ks, &kss TSRMLS_CC);
    php_implode(&delim, vs, &vss TSRMLS_CC);

    sLen = 12 + tableLen + 2 + Z_STRLEN(kss) + 10 + Z_STRLEN(vss) + 1;
    s = emalloc(sLen + 1);
    snprintf(s, sLen + 1, "insert into %s (%s) values (%s)", table, Z_STRVAL(kss), Z_STRVAL(vss));

    zval_ptr_dtor(&ks);
    zval_ptr_dtor(&vs);

    zval_dtor(&kss);
    zval_dtor(&vss);

    RETURN_STRINGL(s, sLen, 0);
}

zend_function_entry Nc_Sql_Adapter_methods[] = {
    ZEND_ME(Nc_Sql_Adapter, invokeSelect, Nc_Sql_Adapter_invokeSelect_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Adapter, delete, Nc_Sql_Adapter_delete_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Adapter, update, Nc_Sql_Adapter_update_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ABSTRACT_ME(Nc_Sql_Adapter, buildSelectQuery, Nc_Sql_Adapter_buildSelectQuery_arginfo)
    ZEND_ABSTRACT_ME(Nc_Sql_Adapter, insert, Nc_Sql_Adapter_insert_arginfo)
    ZEND_ABSTRACT_ME(Nc_Sql_Adapter, quote, Nc_Sql_Adapter_quote_arginfo)
    ZEND_ME(Nc_Sql_Adapter, buildSelectTable, Nc_Sql_Adapter_buildSelectTable_arginfo, ZEND_ACC_PROTECTED)
    ZEND_ME(Nc_Sql_Adapter, buildSelectField, Nc_Sql_Adapter_buildSelectField_arginfo, ZEND_ACC_PROTECTED)
    ZEND_ME(Nc_Sql_Adapter, buildSelectJoin, Nc_Sql_Adapter_buildSelectJoin_arginfo, ZEND_ACC_PROTECTED)
    ZEND_ME(Nc_Sql_Adapter, buildSelectWhere, Nc_Sql_Adapter_buildSelectWhere_arginfo, ZEND_ACC_PROTECTED)
    ZEND_ME(Nc_Sql_Adapter, buildSelectGroup, Nc_Sql_Adapter_buildSelectGroup_arginfo, ZEND_ACC_PROTECTED)
    ZEND_ME(Nc_Sql_Adapter, buildSelectHaving, Nc_Sql_Adapter_buildSelectHaving_arginfo, ZEND_ACC_PROTECTED)
    ZEND_ME(Nc_Sql_Adapter, buildSelectOrder, Nc_Sql_Adapter_buildSelectOrder_arginfo, ZEND_ACC_PROTECTED)
    ZEND_ME(Nc_Sql_Adapter, buildInsertQuery, Nc_Sql_Adapter_buildInsertQuery_arginfo, ZEND_ACC_PROTECTED)
    ZEND_FENTRY(buildUpdateExpression, NULL, Nc_Sql_Adapter_buildUpdateExpression_arginfo, ZEND_ACC_PROTECTED|ZEND_ACC_ABSTRACT)
    ZEND_FE_END
};
