zend_class_entry *Nc_Sql_Orm_Model_ce;

NC_INIT_CLASS_FUNCTION(Nc_Sql_Orm_Model)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Sql\\Orm\\Model", Nc_Sql_Orm_Model_methods);
    Nc_Sql_Orm_Model_ce = zend_register_internal_class_ex(&ce, Nc_Kernel_ce, NULL TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Orm_Model_getRelation_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Orm_Model_getEntity_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Orm_Model_findEntity_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, condition)
    ZEND_ARG_INFO(0, field)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Orm_Model_find_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, condition)
    ZEND_ARG_INFO(0, field)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Orm_Model_drop_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, condition)
    ZEND_ARG_INFO(0, field)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Sql_Orm_Model_save_arginfo, 0, 0, 1)
    ZEND_ARG_ARRAY_INFO(0, data, 0)
    ZEND_ARG_INFO(0, condition)
    ZEND_ARG_INFO(0, field)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Sql_Orm_Model, getRelation)
{
    char *target;
    uint targetLen;
    zval *relations, *relation, *relationProperties;
    zval *modelFactory, *adapter;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &target, &targetLen) != SUCCESS)
        return;

    relations = nc_kernel_injection_property(getThis(), "relations", Nc_Sql_Orm_RelationContainer_ce);
    if (!relations)
        return;

    relation = nc_kernel_injection_property(relations, target, Nc_Sql_Orm_Relation_ce);
    if (!relation)
        return;

    relationProperties = nc_array_property(relation, "properties");
    if (!relationProperties)
        return;

    modelFactory = nc_kernel_injection_property(getThis(), "modelFactory", Nc_Sql_Orm_ModelFactory_ce);
    if (!modelFactory)
        return;

    adapter = nc_kernel_injection_property(getThis(), "adapter", Nc_Sql_Adapter_ce);
    if (!adapter)
        return;

    Z_ADDREF_P(getThis());
    add_assoc_zval(relationProperties, "model", getThis());

    Z_ADDREF_P(modelFactory);
    add_assoc_zval(relationProperties, "modelFactory", modelFactory);

    Z_ADDREF_P(adapter);
    add_assoc_zval(relationProperties, "adapter", adapter);

    RETURN_ZVAL(relation, 0, 0);
}

ZEND_METHOD(Nc_Sql_Orm_Model, getEntity)
{
    zval *entity, *entityProperties;
    zval *injections;
    zval *modelFactory, *adapter, *id;

    injections = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("injections"), 0 TSRMLS_CC);

    if (Z_TYPE_P(injections) == IS_ARRAY && zend_hash_exists(Z_ARRVAL_P(injections), ZEND_STRS("entity")))
    {
        entity = nc_kernel_injection_property(getThis(), "entity", Nc_Sql_Orm_Entity_ce);
        if (!entity)
            return;
    }
    else
    {
        ALLOC_INIT_ZVAL(entity);
        object_init_ex(entity, Nc_Sql_Orm_Entity_ce);
    }

    entityProperties = nc_array_property(entity, "properties");
    if (!entityProperties)
        return;

    modelFactory = nc_kernel_injection_property(getThis(), "modelFactory", Nc_Sql_Orm_ModelFactory_ce);
    if (!modelFactory)
        return;

    adapter = nc_kernel_injection_property(getThis(), "adapter", Nc_Sql_Adapter_ce);
    if (!adapter)
        return;

    id = nc_kernel_injection_property(getThis(), "id", NULL);
    if (!id)
        return;

    Z_ADDREF_P(getThis());
    add_assoc_zval(entityProperties, "model", getThis());

    Z_ADDREF_P(modelFactory);
    add_assoc_zval(entityProperties, "modelFactory", modelFactory);

    Z_ADDREF_P(adapter);
    add_assoc_zval(entityProperties, "adapter", adapter);

    Z_ADDREF_P(id);
    add_assoc_zval(entityProperties, "id", id);

    RETURN_ZVAL(entity, 0, 0);
}

ZEND_METHOD(Nc_Sql_Orm_Model, findEntity)
{
    zval *condition, *field = NULL;
    zval *entity;
    zval *r;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &condition, &field) != SUCCESS)
        return;

    entity = zend_call_method_with_0_params(&getThis(), Z_OBJCE_P(getThis()), NULL, "getentity", &entity);
    if (!entity)
    {
        zend_throw_exception(Nc_Sql_Orm_Exception_ce, "Cannot invoke getEntity method.", 0 TSRMLS_CC);
        return;
    }

    r = zend_call_method_with_2_params(&entity, Z_OBJCE_P(entity), NULL, "load", &r, condition, field);
    if (!r)
    {
        zval_ptr_dtor(&entity);
        zend_throw_exception(Nc_Sql_Orm_Exception_ce, "Cannot invoke load method of entity.", 0 TSRMLS_CC);
        return;
    }

    zval_ptr_dtor(&r);

    RETURN_ZVAL(entity, 0, 0);
}

ZEND_METHOD(Nc_Sql_Orm_Model, find)
{
    zval *condition;
    char *fieldStr = NULL;
    uint fieldStrLen = 0;
    zval *adapter;
    zval *select, *table, *name;
    zval *r;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &condition, &fieldStr, &fieldStrLen) != SUCCESS)
        return;

    adapter = nc_kernel_injection_property(getThis(), "adapter", Nc_Sql_Orm_RelationContainer_ce);
    if (!adapter)
        return;

    if (Z_TYPE_P(condition) == IS_ARRAY)
    {
        zval *field, *predicate;

        if (zend_hash_num_elements(Z_ARRVAL_P(condition)) < 1)
        {
            zval *arr;
            ALLOC_INIT_ZVAL(arr);
            array_init(arr);
            RETURN_ZVAL(arr, 0, 0);
            return;
        }

        if (fieldStr && fieldStrLen > 0)
        {
            ALLOC_INIT_ZVAL(field);
            ZVAL_STRINGL(field, fieldStr, fieldStrLen, 1);
        }
        else
        {
            field = nc_kernel_injection_property(getThis(), "id", NULL);
        }

        predicate = nc_kernel_injection_property(adapter, "predicate", Nc_Sql_Predicate_ce);
        if (!predicate)
            return;

        r = zend_call_method_with_2_params(&predicate, Z_OBJCE_P(predicate), NULL, "in", &r, field, condition);
        if (!r)
        {
            zval_ptr_dtor(&field);
            zval_ptr_dtor(&predicate);

            zend_throw_exception(Nc_Sql_Orm_Exception_ce, "Cannot invoke load method of entity.", 0 TSRMLS_CC);
            return;
        }

        condition = predicate;

        zval_ptr_dtor(&field);
        zval_ptr_dtor(&r);
    }

    select = nc_kernel_injection_property(adapter, "select", Nc_Sql_Select_ce);
    if (select)
        return;

    table = nc_kernel_injection_property(adapter, "table", NULL);
    if (table)
        return;

    name = nc_kernel_injection_property(adapter, "name", NULL);
    if (name)
        return;

    r = zend_call_method_with_2_params(&select, Z_OBJCE_P(select), NULL, "table", &r, table, name);
    if (!r)
    {
        // gc & exceptions
        return;
    }
    // gc

    r = zend_call_method_with_1_params(&select, Z_OBJCE_P(select), NULL, "where", &r, condition);
    if (!r)
    {
        // gc & exceptions
        return;
    }
    // gc

    r = zend_call_method_with_0_params(&select, Z_OBJCE_P(select), NULL, "__invoke", &r);
    if (!r)
    {
        // gc & exceptions
        return;
    }
    // gc

    RETURN_ZVAL(r, 0, 0);
}

ZEND_METHOD(Nc_Sql_Orm_Model, drop)
{
}

ZEND_METHOD(Nc_Sql_Orm_Model, save)
{
}

zend_function_entry Nc_Sql_Orm_Model_methods[] = {
    ZEND_ME(Nc_Sql_Orm_Model, getRelation, Nc_Sql_Orm_Model_getRelation_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Orm_Model, getEntity, Nc_Sql_Orm_Model_getEntity_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Orm_Model, findEntity, Nc_Sql_Orm_Model_findEntity_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Orm_Model, find, Nc_Sql_Orm_Model_find_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Orm_Model, drop, Nc_Sql_Orm_Model_drop_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Sql_Orm_Model, save, Nc_Sql_Orm_Model_save_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
