zend_class_entry *Nc_Sql_Connection_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Connection)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Connection", Nc_Sql_Connection_methods);
    Nc_Sql_Connection_ce = zend_register_internal_class_ex(&ce, Nc_Kernel_ce, NULL TSRMLS_CC);
    Nc_Sql_Connection_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("READ"), NC_SQL_CONNECTION_READ TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("WRITE"), NC_SQL_CONNECTION_WRITE TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("SELECT"), NC_SQL_CONNECTION_SELECT TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("DELETE"), NC_SQL_CONNECTION_DELETE TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("INSERT"), NC_SQL_CONNECTION_INSERT TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("UPDATE"), NC_SQL_CONNECTION_UPDATE TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("FETCH"), NC_SQL_CONNECTION_FETCH TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("ALL"), NC_SQL_CONNECTION_ALL TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("ROW"), NC_SQL_CONNECTION_ROW TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("ONE"), NC_SQL_CONNECTION_ONE TSRMLS_CC);
    zend_declare_class_constant_long(Nc_Sql_Connection_ce, ZEND_STRL("COLUMN"), NC_SQL_CONNECTION_COLUMN TSRMLS_CC);

    zend_declare_property_null(Nc_Sql_Connection_ce, ZEND_STRL("queries"), ZEND_ACC_PRIVATE | ZEND_ACC_STATIC TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Connection_getQueries_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Connection_addQuery_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Connection_begin_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Connection_commit_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Connection_rollback_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Connection_query_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, sql)
    ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_Connection, getQueries)
{
    zval *qs;
    qs = zend_read_static_property(Nc_Sql_Connection_ce, ZEND_STRL("queries"), 0 TSRMLS_CC);

    RETURN_ZVAL(qs, 1, 0);
}

ZEND_METHOD(Nc_Sql_Connection, addQuery)
{
    char *sql;
    uint sqlLen;
    zval *qs;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &sql, &sqlLen) == FAILURE)
        return;

    qs = zend_read_static_property(Nc_Sql_Connection_ce, ZEND_STRL("queries"), 0 TSRMLS_CC);

    if (Z_TYPE_P(qs) != IS_ARRAY)
        convert_to_array(qs);

    add_next_index_stringl(qs, sql, sqlLen, 1);
}

zend_function_entry Nc_Sql_Connection_methods[] = {
    ZEND_ME(Nc_Sql_Connection, getQueries, Nc_Sql_Connection_getQueries_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    ZEND_ME(Nc_Sql_Connection, addQuery, Nc_Sql_Connection_addQuery_arginfo, ZEND_ACC_PROTECTED | ZEND_ACC_STATIC)
    ZEND_ABSTRACT_ME(Nc_Sql_Connection, begin, Nc_Sql_Connection_begin_arginfo)
    ZEND_ABSTRACT_ME(Nc_Sql_Connection, commit, Nc_Sql_Connection_commit_arginfo)
    ZEND_ABSTRACT_ME(Nc_Sql_Connection, rollback, Nc_Sql_Connection_rollback_arginfo)
    ZEND_ABSTRACT_ME(Nc_Sql_Connection, query, Nc_Sql_Connection_query_arginfo)
    ZEND_FE_END
};
