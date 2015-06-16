zend_class_entry *Nc_Sql_Predicate_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Predicate)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Predicate", Nc_Sql_Predicate_methods);
    Nc_Sql_Predicate_ce = zend_register_internal_class_ex(&ce, Nc_Kernel_ce, NULL TSRMLS_CC);

    zend_declare_property_null(Nc_Sql_Predicate_ce, ZEND_STRL("parts"), ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_raw_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, raw)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_eq_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_neq_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_gt_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_gte_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_lt_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_lte_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_between_arginfo, 0, 0, 3)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, v1)
    ZEND_ARG_INFO(0, v2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_in_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_ARRAY_INFO(0, values, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_inSelect_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_OBJ_INFO(0, select, Nc\\Sql\\Select, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_like_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, field)
    ZEND_ARG_INFO(0, value)
    ZEND_ARG_INFO(0, headingWildcard)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_setAnd_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate_setOr_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Predicate___toString_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_bool fieldValueOp(zval *obj, char *f, uint fLen, zval *value, const char *op, uint opLen TSRMLS_DC)
{
    char *part;
    uint partLen;
    zval *adapter, *pz, *parts;

    adapter = nc_kernel_injection_property(obj, "adapter", Nc_Sql_Adapter_ce);
    if (!adapter)
        return 0;

    if (Z_TYPE_P(value) != IS_STRING)
        convert_to_string(value);

    pz = zend_call_method_with_1_params(&adapter, Z_OBJCE_P(adapter), NULL, "quote", &pz, value);
    zval_ptr_dtor(&adapter);

    if (!pz || Z_TYPE_P(pz) != IS_STRING)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot call adapter method #quote()#.", 0 TSRMLS_CC);
        if (pz)
            zval_ptr_dtor(&pz);
        return 0;
    }

    partLen = fLen + opLen + Z_STRLEN_P(pz);
    part = emalloc(partLen + 1);

    strncpy(part, f, fLen);
    strncpy(part + fLen, op, opLen);
    strncpy(part + fLen + opLen, Z_STRVAL_P(pz), Z_STRLEN_P(pz));
    part[partLen] = '\0';

    zval_ptr_dtor(&pz);

    parts = nc_array_property(obj, "parts");
    if (!parts)
        return 0;

    add_next_index_stringl(parts, part, partLen, 0);

    return 1;
}

ZEND_METHOD(Nc_Sql_Predicate, raw)
{
    char *raw;
    uint rawLen;
    zval *parts;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &raw, &rawLen) == FAILURE)
        return;

    parts = nc_array_property(getThis(), "parts");
    if (!parts)
        return;

    add_next_index_stringl(parts, raw, rawLen, 1);

    RETURN_ZVAL(getThis(), 1, 0);
}

ZEND_METHOD(Nc_Sql_Predicate, eq)
{
    char *field;
    uint fieldLen;
    zval *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &field, &fieldLen, &value) == FAILURE)
        return;

    if (!fieldValueOp(getThis(), field, fieldLen, value, ZEND_STRL(" = ") TSRMLS_CC))
        return;

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, neq)
{
    char *field;
    uint fieldLen;
    zval *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &field, &fieldLen, &value) == FAILURE)
        return;

    if (!fieldValueOp(getThis(), field, fieldLen, value, ZEND_STRL(" <> ") TSRMLS_CC))
        return;

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, gt)
{
    char *field;
    uint fieldLen;
    zval *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &field, &fieldLen, &value) == FAILURE)
        return;

    if (!fieldValueOp(getThis(), field, fieldLen, value, ZEND_STRL(" > ") TSRMLS_CC))
        return;

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, gte)
{
    char *field;
    uint fieldLen;
    zval *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &field, &fieldLen, &value) == FAILURE)
        return;

    if (!fieldValueOp(getThis(), field, fieldLen, value, ZEND_STRL(" >= ") TSRMLS_CC))
        return;

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, lt)
{
    char *field;
    uint fieldLen;
    zval *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &field, &fieldLen, &value) == FAILURE)
        return;

    if (!fieldValueOp(getThis(), field, fieldLen, value, ZEND_STRL(" < ") TSRMLS_CC))
        return;

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, lte)
{
    char *field;
    uint fieldLen;
    zval *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &field, &fieldLen, &value) == FAILURE)
        return;

    if (!fieldValueOp(getThis(), field, fieldLen, value, ZEND_STRL(" <= ") TSRMLS_CC))
        return;

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, between)
{
    char *field;
    uint fieldLen;
    zval *v1, *v2;
    char *part;
    uint partLen;
    zval *adapter, *pz1, *pz2, *parts;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "szz", &field, &fieldLen, &v1, &v2) == FAILURE)
        return;

    parts = nc_array_property(getThis(), "parts");
    if (!parts)
        return;

    adapter = nc_kernel_injection_property(getThis(), "adapter", Nc_Sql_Adapter_ce);
    if (!adapter)
        return;

    if (Z_TYPE_P(v1) != IS_STRING)
        convert_to_string(v1);
    if (Z_TYPE_P(v2) != IS_STRING)
        convert_to_string(v2);

    pz1 = zend_call_method_with_1_params(&adapter, Z_OBJCE_P(adapter), NULL, "quote", &pz1, v1);
    pz2 = zend_call_method_with_1_params(&adapter, Z_OBJCE_P(adapter), NULL, "quote", &pz2, v2);

    zval_ptr_dtor(&adapter);

    if (!pz1 || !pz2 || Z_TYPE_P(pz1) != IS_STRING || Z_TYPE_P(pz2) != IS_STRING)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Cannot call adapter method #quote()#.", 0 TSRMLS_CC);
        if (pz1)
            zval_ptr_dtor(&pz1);
        if (pz2)
            zval_ptr_dtor(&pz2);
        return;
    }

    partLen = fieldLen + 9 + Z_STRLEN_P(pz1) + 5 + Z_STRLEN_P(pz2);
    part = emalloc(partLen + 1);

    strncpy(part, field, fieldLen);
    strncpy(part + fieldLen, " between ", 9);
    strncpy(part + fieldLen + 9, Z_STRVAL_P(pz1), Z_STRLEN_P(pz1));
    strncpy(part + fieldLen + 9 + Z_STRLEN_P(pz1), " and ", 5);
    strncpy(part + fieldLen + 9 + Z_STRLEN_P(pz1) + 5, Z_STRVAL_P(pz2), Z_STRLEN_P(pz2));
    part[partLen] = '\0';

    zval_ptr_dtor(&pz1);
    zval_ptr_dtor(&pz2);

    add_next_index_stringl(parts, part, partLen, 0);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, in)
{
    char *field;
    uint fieldLen;
    zval *adapter, *values, **ppz;
    zval delim, tmp;
    char *part;
    uint partLen;
    zval *parts;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &field, &fieldLen, &values) == FAILURE)
        return;

    parts = nc_array_property(getThis(), "parts");
    if (!parts)
        return;

    adapter = nc_kernel_injection_property(getThis(), "adapter", Nc_Sql_Adapter_ce);
    if (!adapter)
        return;

    for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(values));
        zend_hash_has_more_elements(Z_ARRVAL_P(values)) == SUCCESS;
        zend_hash_move_forward(Z_ARRVAL_P(values)))
    {
        zval *r;

        if (zend_hash_get_current_data(Z_ARRVAL_P(values), (void **) &ppz) == FAILURE)
        {
            zend_throw_exception(Nc_Sql_Exception_ce, "Cannot fetch array data.", 0 TSRMLS_CC);
            return;
        }

        r = zend_call_method_with_1_params(&adapter, Z_OBJCE_P(adapter), NULL, "quote", &r, *ppz);
        zval_ptr_dtor(ppz);
        *ppz = r;
    }

    zval_ptr_dtor(&adapter);

    ZVAL_STRING(&delim, ", ", 0);
    php_implode(&delim, values, &tmp TSRMLS_CC);

    partLen = fieldLen + 5 + Z_STRLEN(tmp) + 1;
    part = emalloc(partLen + 1);

    strncpy(part, field, fieldLen);
    strncpy(part + fieldLen, " in (", 5);
    strncpy(part + fieldLen + 5, Z_STRVAL(tmp), Z_STRLEN(tmp));
    part[partLen - 1] = ')';
    part[partLen] = '\0';

    efree(Z_STRVAL(tmp));

    add_next_index_stringl(parts, part, partLen, 0);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, inSelect)
{
    char *field;
    uint fieldLen;
    zval *select;
    zval *tmp;
    char *part;
    uint partLen;
    zval *parts;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "so", &field, &fieldLen, &select) == FAILURE)
        return;

    parts = nc_array_property(getThis(), "parts");
    if (!parts)
        return;

    tmp = zend_call_method_with_0_params(&select, Z_OBJCE_P(select), NULL, "__tostring", &tmp);
    if (!tmp || Z_TYPE_P(tmp) != IS_STRING)
    {
        zend_throw_exception(Nc_Sql_Exception_ce, "Invalid stringified value of object select.", 0 TSRMLS_CC);
        if (tmp)
            zval_ptr_dtor(&tmp);
        return;
    }

    partLen = fieldLen + 5 + Z_STRLEN_P(tmp) + 1;
    part = emalloc(partLen + 1);

    strncpy(part, field, fieldLen);
    strncpy(part + fieldLen, " in (", 5);
    strncpy(part + fieldLen + 5, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp));
    part[partLen - 1] = ')';
    part[partLen] = '\0';

    zval_ptr_dtor(&tmp);

    add_next_index_stringl(parts, part, partLen, 0);

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, like)
{
    char *field, *value;
    uint fieldLen, valueLen;
    zend_bool headingWildcard = 1;
    char *part;
    uint partLen;
    zval v;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssb", &field, &fieldLen, &value, &valueLen, &headingWildcard) == FAILURE)
        return;

    if (headingWildcard)
    {
        partLen = valueLen + 2;
        part = emalloc(partLen + 1);
        part[0] = '%';
        strncpy(part + 1, value, valueLen);
        part[partLen - 1] = '%';
        part[partLen] = '\0';
    }
    else
    {
        partLen = valueLen + 1;
        part = emalloc(partLen + 1);
        strncpy(part, value, valueLen);
        part[partLen - 1] = '%';
        part[partLen] = '\0';
    }

    ZVAL_STRINGL(&v, part, partLen, 0);

    if (fieldValueOp(getThis(), field, fieldLen, &v, ZEND_STRL(" like ") TSRMLS_CC))
        RETVAL_ZVAL(getThis(), 1, 0);

    efree(part);
}

ZEND_METHOD(Nc_Sql_Predicate, setAnd)
{
    zval *parts;
    parts = zend_read_property(Nc_Sql_Predicate_ce, getThis(), ZEND_STRL("parts"), 0 TSRMLS_CC);

    if (Z_TYPE_P(parts) == IS_ARRAY)
    {
        zval delim, newPart, *newParts;

        ZVAL_STRING(&delim, " and ", 0);
        php_implode(&delim, parts, &newPart TSRMLS_CC);

        ALLOC_INIT_ZVAL(newParts);
        array_init(newParts);
        add_next_index_stringl(newParts, Z_STRVAL(newPart), Z_STRLEN(newPart), 0);

        zend_update_property(Nc_Sql_Predicate_ce, getThis(), ZEND_STRL("parts"), newParts TSRMLS_CC);
        zval_ptr_dtor(&newParts);
    }

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, setOr)
{
    zval *parts;
    parts = zend_read_property(Nc_Sql_Predicate_ce, getThis(), ZEND_STRL("parts"), 0 TSRMLS_CC);

    if (Z_TYPE_P(parts) == IS_ARRAY)
    {
        zval delim, newPart, *newParts;

        ZVAL_STRING(&delim, " or ", 0);
        php_implode(&delim, parts, &newPart TSRMLS_CC);

        ALLOC_INIT_ZVAL(newParts);
        array_init(newParts);
        add_next_index_stringl(newParts, Z_STRVAL(newPart), Z_STRLEN(newPart), 0);

        zend_update_property(Nc_Sql_Predicate_ce, getThis(), ZEND_STRL("parts"), newParts TSRMLS_CC);
        zval_ptr_dtor(&newParts);
    }

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Sql_Predicate, __toString)
{
    zval *parts;
    parts = zend_read_property(Nc_Sql_Predicate_ce, getThis(), ZEND_STRL("parts"), 0 TSRMLS_CC);

    if (Z_TYPE_P(parts) == IS_ARRAY)
    {
        zval delim, newPart;

        ZVAL_STRING(&delim, " and ", 0);
        php_implode(&delim, parts, &newPart TSRMLS_CC);

        RETURN_STRINGL(Z_STRVAL(newPart), Z_STRLEN(newPart), 0)
    }

    RETURN_EMPTY_STRING()
}

zend_function_entry Nc_Sql_Predicate_methods[] = {
    ZEND_ME(Nc_Sql_Predicate, raw, Nc_Sql_Predicate_raw_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, eq, Nc_Sql_Predicate_eq_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, neq, Nc_Sql_Predicate_neq_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, gt, Nc_Sql_Predicate_gt_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, gte, Nc_Sql_Predicate_gte_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, lt, Nc_Sql_Predicate_lt_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, lte, Nc_Sql_Predicate_lte_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, between, Nc_Sql_Predicate_between_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, in, Nc_Sql_Predicate_in_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, inSelect, Nc_Sql_Predicate_inSelect_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, like, Nc_Sql_Predicate_like_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, setAnd, Nc_Sql_Predicate_setAnd_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, setOr, Nc_Sql_Predicate_setOr_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Predicate, __toString, Nc_Sql_Predicate___toString_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
