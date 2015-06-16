zend_class_entry *Nc_Sql_Select_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Select)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Select", Nc_Sql_Select_methods);
    Nc_Sql_Select_ce = zend_register_internal_class_ex(&ce, Nc_Kernel_ce, NULL TSRMLS_CC);

    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("paginationQuery"), ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("table"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("tables"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("field"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("join"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("where"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("group"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("having"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("order"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_bool(Nc_Sql_Select_ce, ZEND_STRL("pagination"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("pageSize"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("page"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("countingRows"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("rowOffset"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("numRows"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Sql_Select_ce, ZEND_STRL("numPages"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_long(Nc_Sql_Select_ce, ZEND_STRL("flag"), (long) NC_SQL_CONNECTION_ALL, ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_table_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, table)
    ZEND_ARG_INFO(0, alias)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_field_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, alias)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_join_arginfo, 0, 0, 3)
    ZEND_ARG_INFO(0, table)
    ZEND_ARG_INFO(0, alias)
    ZEND_ARG_INFO(0, on)
    ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_where_arginfo, 0, 0, 0)
    ZEND_ARG_INFO(0, where)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_group_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, field)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_having_arginfo, 0, 0, 0)
    ZEND_ARG_INFO(0, having)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_order_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, asc)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_paginate_arginfo, 0, 0, 0)
    ZEND_ARG_INFO(0, pageSize)
    ZEND_ARG_INFO(0, page)
    ZEND_ARG_INFO(0, countingRows)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_setNumRows_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, numRows)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select_flag_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select___get_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select___toString_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Select___invoke_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_Select, table)
{
    char *table, *alias = NULL;
    uint tableLen, aliasLen = 0;
    zval *tables;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &table, &tableLen, &alias, &aliasLen) == FAILURE)
        return;

    tables = nc_array_property(getThis(), "tables");
    if (!tables)
        return;

    if (aliasLen < 1)
    {
        alias = table;
        aliasLen = tableLen;
    }

    zend_update_property_stringl(Nc_Sql_Select_ce, getThis(), ZEND_STRL("table"), table, tableLen TSRMLS_CC);

    add_assoc_stringl_ex(tables, alias, aliasLen + 1, table, tableLen, 1);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Select, field)
{
    char *field, *alias = NULL;
    uint fieldLen, aliasLen = 0;
    zval *arr;
    zval *fields;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &field, &fieldLen, &alias, &aliasLen) == FAILURE)
        return;

    fields = nc_array_property(getThis(), "field");
    if (!fields)
        return;

    ALLOC_INIT_ZVAL(arr);
    array_init_size(arr, 2);

    add_next_index_stringl(arr, field, fieldLen, 1);

    if (alias && aliasLen > 0)
        add_next_index_stringl(arr, alias, aliasLen, 1);
    else
        add_next_index_null(arr);

    add_next_index_zval(fields, arr);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Select, join)
{
    char *table, *alias = NULL, *on, *type = NULL;
    uint tableLen, aliasLen = 0, onLen, typeLen = 0;
    zval *arr;
    zval *joins;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss!s|s", &table, &tableLen, &alias, &aliasLen, &on, &onLen, &type, &typeLen) == FAILURE)
        return;

    joins = nc_array_property(getThis(), "join");
    if (!joins)
        return;

    ALLOC_INIT_ZVAL(arr);
    array_init_size(arr, 4);

    add_next_index_stringl(arr, table, tableLen, 1);

    if (alias && aliasLen > 0)
        add_next_index_stringl(arr, alias, aliasLen, 1);
    else
        add_next_index_stringl(arr, table, tableLen, 1);

    add_next_index_stringl(arr, on, onLen, 1);

    if (type && typeLen > 0)
        add_next_index_stringl(arr, type, typeLen, 1);
    else
        add_next_index_string(arr, "left", 1);

    add_next_index_zval(joins, arr);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Select, where)
{
    zval *where;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &where) == FAILURE)
        return;

    zend_update_property(Nc_Sql_Select_ce, getThis(), ZEND_STRL("where"), where TSRMLS_CC);
    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Select, group)
{
    char *field;
    uint fieldLen;
    zval *groups;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &field, &fieldLen) == FAILURE)
        return;

    groups = nc_array_property(getThis(), "group");
    if (!groups)
        return;

    add_next_index_stringl(groups, field, fieldLen, 1);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Select, having)
{
    zval *having;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &having) == FAILURE)
        return;

    zend_update_property(Nc_Sql_Select_ce, getThis(), ZEND_STRL("having"), having TSRMLS_CC);
    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Select, order)
{
    char *field;
    uint fieldLen;
    zend_bool asc = 0;
    zval *arr;
    zval *orders;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &field, &fieldLen, &asc) == FAILURE)
        return;

    orders = nc_array_property(getThis(), "order");
    if (!orders)
        return;

    ALLOC_INIT_ZVAL(arr);
    array_init_size(arr, 2);

    add_next_index_stringl(arr, field, fieldLen, 1);
    add_next_index_bool(arr, asc);

    add_next_index_zval(orders, arr);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Select, paginate)
{
    long pageSize = 15L, page = 1L;
    zend_bool countingRows = 0;
    long rowOffset;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|llb", &pageSize, &page, &countingRows) == FAILURE)
        return;

    pageSize = MAX(2L, pageSize);
    page = MAX(1L, page);
    rowOffset = pageSize * (page - 1);

    zend_update_property_bool(Nc_Sql_Select_ce, getThis(), ZEND_STRL("pagination"), 1 TSRMLS_CC);
    zend_update_property_long(Nc_Sql_Select_ce, getThis(), ZEND_STRL("pageSize"), pageSize TSRMLS_CC);
    zend_update_property_long(Nc_Sql_Select_ce, getThis(), ZEND_STRL("page"), page TSRMLS_CC);
    zend_update_property_bool(Nc_Sql_Select_ce, getThis(), ZEND_STRL("countingRows"), countingRows TSRMLS_CC);
    zend_update_property_long(Nc_Sql_Select_ce, getThis(), ZEND_STRL("rowOffset"), rowOffset TSRMLS_CC);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Select, setNumRows)
{
    long numRows = 0L;
    long numPages;
    zval *pageSize;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &numRows) == FAILURE)
        return;

    pageSize = zend_read_property(Nc_Sql_Select_ce, getThis(), ZEND_STRL("pageSize"), 0 TSRMLS_CC);
    if (Z_TYPE_P(pageSize) != IS_LONG || Z_LVAL_P(pageSize) < 1L)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Invalid pageSize property.", 0 TSRMLS_CC);
        return;
    }

    numRows = MAX(0L, numRows);
    numPages = 1L + (long) floor((-.5 + numRows) / Z_LVAL_P(pageSize));

    zend_update_property_long(Nc_Sql_Select_ce, getThis(), ZEND_STRL("numRows"), numRows TSRMLS_CC);
    zend_update_property_long(Nc_Sql_Select_ce, getThis(), ZEND_STRL("numPages"), numPages TSRMLS_CC);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Select, flag)
{
    long flag;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &flag) == FAILURE)
        return;

    zend_update_property_long(Nc_Sql_Select_ce, getThis(), ZEND_STRL("flag"), flag TSRMLS_CC);

    RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_METHOD(Nc_Sql_Select, __get)
{
    char *name;
    uint nameLen;
    zval *pz;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &nameLen) == FAILURE)
        return;

    pz = zend_read_property(Z_OBJCE_P(getThis()), getThis(), name, nameLen, 1 TSRMLS_CC);
    Z_ADDREF_P(pz);

    RETURN_ZVAL(pz, 0, 0);
}

ZEND_METHOD(Nc_Sql_Select, __toString)
{
    zval *adapter, *retval;

    adapter = nc_kernel_injection_property(getThis(), "adapter", Nc_Sql_Adapter_ce);
    if (!adapter)
        return;

    retval = zend_call_method_with_1_params(&adapter, Z_OBJCE_P(adapter), NULL, "buildselectquery", &retval, getThis());
    zval_ptr_dtor(&adapter);

    if (!retval || Z_TYPE_P(retval) != IS_STRING)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke $adapter->buildSelectQuery($this) or invalid value returned.", 0 TSRMLS_CC);
        if (retval)
            zval_ptr_dtor(&retval);
        return;
    }

    RETVAL_ZVAL(retval, 0, 0);
    efree(retval);
}

ZEND_METHOD(Nc_Sql_Select, __invoke)
{
    zval *adapter, *retval;

    adapter = nc_kernel_injection_property(getThis(), "adapter", Nc_Sql_Adapter_ce);
    if (!adapter)
        return;

    retval = zend_call_method_with_1_params(&adapter, Z_OBJCE_P(adapter), NULL, "invokeselect", &retval, getThis());
    zval_ptr_dtor(&adapter);

    if (!retval)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot invoke $adapter->invokeSelect($this).", 0 TSRMLS_CC);
        return;
    }

    RETVAL_ZVAL(retval, 0, 0);
    efree(retval);
}

zend_function_entry Nc_Sql_Select_methods[] = {
    ZEND_ME(Nc_Sql_Select, table, Nc_Sql_Select_table_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, field, Nc_Sql_Select_field_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, join, Nc_Sql_Select_join_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, where, Nc_Sql_Select_where_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, group, Nc_Sql_Select_group_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, having, Nc_Sql_Select_having_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, order, Nc_Sql_Select_order_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, paginate, Nc_Sql_Select_paginate_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, setNumRows, Nc_Sql_Select_setNumRows_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, flag, Nc_Sql_Select_flag_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, __get, Nc_Sql_Select___get_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, __toString, Nc_Sql_Select___toString_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Select, __invoke, Nc_Sql_Select___invoke_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
