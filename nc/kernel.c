#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_nc.h"

#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"
#include "standard/php_array.h"

zend_class_entry *Nc_Exception_ce;

NC_INIT_CLASS_FUNCTION(Nc_Exception)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Exception", Nc_Exception_methods);
    Nc_Exception_ce = zend_register_internal_class_ex(&ce, NULL, "exception" TSRMLS_CC);

    return SUCCESS;
}

zend_function_entry Nc_Exception_methods[] = {
    ZEND_FE_END
};

NC_API zval *_nc_array_property(zval *obj, const char *name TSRMLS_DC)
{
    zval *prop, *propNew;
    uint nameLen;

    if (!obj || Z_TYPE_P(obj) != IS_OBJECT)
        return NULL;

    nameLen = strlen(name);

    prop = zend_read_property(Z_OBJCE_P(obj), obj, name, nameLen, 0 TSRMLS_CC);
    if (Z_TYPE_P(prop) == IS_ARRAY)
        return prop;

    ALLOC_INIT_ZVAL(propNew);
    array_init(propNew);
    Z_DELREF_P(propNew);
    zend_update_property(Z_OBJCE_P(obj), obj, name, nameLen, propNew TSRMLS_CC);

    return propNew;
}

NC_API zval *_nc_kernel_injection_property(zval *obj, const char *name, zend_class_entry *valid_ce TSRMLS_DC)
{
    zval *param, *retval;

    ALLOC_INIT_ZVAL(param);
    ZVAL_STRING(param, name, 1);
    retval = zend_call_method_with_1_params(&obj, Z_OBJCE_P(obj), NULL, "offsetget", &retval, param);
    zval_ptr_dtor(&param);

    if (!retval || (valid_ce && (Z_TYPE_P(retval) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(retval), valid_ce TSRMLS_CC))))
    {
        zend_throw_exception_ex(Nc_Sql_Exception_ce, 0 TSRMLS_CC, "Invalid injection property '%s'.", name);
        if (retval)
            zval_ptr_dtor(&retval);
        return NULL;
    }

    return retval;
}

zend_class_entry *Nc_Kernel_ce;

NC_INIT_CLASS_FUNCTION(Nc_Kernel)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\Kernel", Nc_Kernel_methods);
    Nc_Kernel_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);
    Nc_Kernel_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    zend_class_implements(Nc_Kernel_ce TSRMLS_CC, 1, zend_ce_arrayaccess);

    zend_declare_property_null(Nc_Kernel_ce, ZEND_STRL("properties"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Kernel_ce, ZEND_STRL("injections"), ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Kernel_newInstance_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Kernel_offsetGet_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Kernel_offsetSet_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Kernel_offsetExists_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Kernel_offsetUnset_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Kernel_inject_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, target)
    ZEND_ARG_INFO(0, namedAs)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Kernel_setInjection_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, injection)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Kernel, newInstance)
{
    zval *options;
    char *className = NULL;
    uint classNameLen = 0;
    zval **ppz;
    zend_class_entry *ce;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &options) == FAILURE)
        return;

    do
    {
        if (Z_TYPE_P(options) == IS_STRING)
        {
            className = Z_STRVAL_P(options);
            classNameLen = Z_STRLEN_P(options);
            break;
        }

        if (Z_TYPE_P(options) != IS_ARRAY)
        {
            zend_throw_exception(Nc_Exception_ce, "Invalid options type.", 0 TSRMLS_CC);
            return;
        }

        if (zend_hash_find(Z_ARRVAL_P(options), ZEND_STRS("class"), (void **) &ppz) == SUCCESS)
        {
            if (Z_TYPE_PP(ppz) != IS_STRING)
            {
                zend_throw_exception(Nc_Exception_ce, "Invalid class option type.", 0 TSRMLS_CC);
                return;
            }

            className = Z_STRVAL_PP(ppz);
            classNameLen = Z_STRLEN_PP(ppz);
        }
    }
    while (0);

    if (classNameLen < 1)
    {
        zend_throw_exception(Nc_Exception_ce, "Empty class argument.", 0 TSRMLS_CC);
        return;
    }

    ce = zend_fetch_class(className, classNameLen, ZEND_FETCH_CLASS_DEFAULT TSRMLS_CC);

    if (!ce)
    {
        zend_throw_exception_ex(Nc_Exception_ce, 0 TSRMLS_CC, "Cannot load class #%s#.", className);
        return;
    }

    object_init_ex(return_value, ce);

    if (options && Z_TYPE_P(options) == IS_ARRAY && zend_hash_find(Z_ARRVAL_P(options), ZEND_STRS("properties"), (void **) &ppz) == SUCCESS)
    {
        if (Z_TYPE_PP(ppz) != IS_ARRAY)
        {
            zend_throw_exception(Nc_Exception_ce, "Invalid properties argument type.", 0 TSRMLS_CC);
            return;
        }

        zend_update_property(ce, return_value, ZEND_STRL("properties"), *ppz TSRMLS_CC);
    }

    if (options && Z_TYPE_P(options) == IS_ARRAY && zend_hash_find(Z_ARRVAL_P(options), ZEND_STRS("injections"), (void **) &ppz) == SUCCESS)
    {
        if (Z_TYPE_PP(ppz) != IS_ARRAY)
        {
            zend_throw_exception(Nc_Exception_ce, "Invalid injections argument type.", 0 TSRMLS_CC);
            return;
        }

        zend_update_property(ce, return_value, ZEND_STRL("injections"), *ppz TSRMLS_CC);
    }

    if (ce->constructor)
    {
        zval m;
        zval *r = NULL;
        uint argc = 0;
        zval ***argv = NULL;
        int argp = 0;

        ZVAL_STRING(&m, "__construct", 0);

        if (options
            && Z_TYPE_P(options) == IS_ARRAY
            && zend_hash_find(Z_ARRVAL_P(options), ZEND_STRS("arguments"), (void **) &ppz) == SUCCESS
            && Z_TYPE_PP(ppz) == IS_ARRAY)
        {
            argc = zend_hash_num_elements(Z_ARRVAL_PP(ppz));

            if (argc > 0)
            {
                argv = emalloc(argc * sizeof(zval **));

                for (zend_hash_internal_pointer_reset(Z_ARRVAL_PP(ppz));
                    zend_hash_has_more_elements(Z_ARRVAL_PP(ppz)) == SUCCESS;
                    zend_hash_move_forward(Z_ARRVAL_PP(ppz)))
                {
                    zend_hash_get_current_data(Z_ARRVAL_PP(ppz), (void **) &(argv[argp++]));
                }
            }
        }

        if (call_user_function_ex(NULL, &return_value, &m, &r, argc, argv, 1, NULL TSRMLS_CC) == FAILURE || !r)
            zend_throw_exception_ex(Nc_Exception_ce, 0 TSRMLS_CC, "Invocation of #%s# constructor failed.", ce->name);

        if (argv)
            efree(argv);

        if (r)
            zval_ptr_dtor(&r);
    }
}

ZEND_METHOD(Nc_Kernel, offsetGet)
{
    char *name;
    uint nameLen;
    zval *properties;
    zval *injections;
    zval **ppz;
    zval **alwaysNew;
    zval **namedAs;
    zval *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &nameLen) == FAILURE)
        return;

    properties = zend_read_property(Nc_Kernel_ce, getThis(), ZEND_STRL("properties"), 0 TSRMLS_CC);

    if (Z_TYPE_P(properties) == IS_ARRAY)
    {
        if (zend_hash_find(Z_ARRVAL_P(properties), name, nameLen + 1, (void **) &ppz) == SUCCESS)
            RETURN_ZVAL(*ppz, 1, 0)
    }
    else
    {
        zval *newProperties;
        ALLOC_INIT_ZVAL(newProperties);
        array_init(newProperties);
        Z_DELREF_P(newProperties);
        zend_update_property(Nc_Kernel_ce, getThis(), ZEND_STRL("properties"), newProperties TSRMLS_CC);
        properties = newProperties;
    }

    injections = zend_read_property(Nc_Kernel_ce, getThis(), ZEND_STRL("injections"), 0 TSRMLS_CC);
    if (Z_TYPE_P(injections) != IS_ARRAY || zend_hash_find(Z_ARRVAL_P(injections), name, nameLen + 1, (void **) &ppz) == FAILURE)
        return;

    retval = zend_call_method_with_1_params(NULL, Nc_Kernel_ce, NULL, "newinstance", &retval, *ppz);

    if (!retval)
    {
        zend_throw_exception(Nc_Exception_ce, "Cannot fetch new instance.", 0 TSRMLS_CC);
        return;
    }

    if (Z_TYPE_PP(ppz) != IS_ARRAY
        || zend_hash_find(Z_ARRVAL_PP(ppz), ZEND_STRS("alwaysNew"), (void **) &alwaysNew) != SUCCESS
        || Z_TYPE_PP(alwaysNew) != IS_BOOL
        || !Z_BVAL_PP(alwaysNew))
    {
        Z_ADDREF_P(retval);

        if (zend_hash_update(Z_ARRVAL_P(properties), name, nameLen + 1, &retval, sizeof(zval *), NULL) == FAILURE)
        {
            Z_DELREF_P(retval);
            zval_ptr_dtor(&retval);
            return;
        }
    }

    if (Z_TYPE_PP(ppz) == IS_ARRAY
        && zend_hash_find(Z_ARRVAL_PP(ppz), ZEND_STRS("namedAs"), (void **) &namedAs) == SUCCESS
        && Z_TYPE_PP(namedAs) == IS_STRING
        && Z_TYPE_P(retval) == IS_OBJECT
        && instanceof_function(zend_get_class_entry(retval TSRMLS_CC), Nc_Kernel_ce TSRMLS_CC))
    {
        zval *targetProperties;
        targetProperties = nc_array_property(retval, "properties");

        if (!targetProperties)
        {
            zend_throw_exception(Nc_Exception_ce, "Cannot fetch target oject properties.", 0 TSRMLS_CC);
            return;
        }

        Z_ADDREF_P(getThis());

        if (zend_hash_update(Z_ARRVAL_P(targetProperties), Z_STRVAL_PP(namedAs), Z_STRLEN_PP(namedAs) + 1, &getThis(), sizeof(zval *), NULL) == FAILURE)
        {
            Z_DELREF_P(getThis());
            return;
        }
    }

    RETURN_ZVAL(retval, 1, 1)
}

ZEND_METHOD(Nc_Kernel, offsetSet)
{
    char *name = NULL;
    int nameLen = 0;
    zval *target;
    zval *properties;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &name, &nameLen, &target) == FAILURE)
        return;

    properties = nc_array_property(getThis(), "properties");
    if (!properties)
        return;

    Z_ADDREF_P(target);

    if (zend_hash_update(Z_ARRVAL_P(properties), name, nameLen + 1, &target, sizeof(zval *), NULL) == FAILURE)
    {
        Z_DELREF_P(target);
        return;
    }
}

ZEND_METHOD(Nc_Kernel, offsetExists)
{
    char *name;
    int nameLen;
    zval *properties;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &nameLen) == FAILURE)
        return;

    properties = zend_read_property(Nc_Kernel_ce, getThis(), ZEND_STRL("properties"), 0 TSRMLS_CC);
    if (Z_TYPE_P(properties) != IS_ARRAY)
        RETURN_FALSE

    if (zend_hash_exists(Z_ARRVAL_P(properties), name, nameLen + 1))
        RETURN_TRUE

    RETURN_FALSE
}

ZEND_METHOD(Nc_Kernel, offsetUnset)
{
    char *name;
    int nameLen;
    zval *properties;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &nameLen) == FAILURE)
        return;

    properties = zend_read_property(Nc_Kernel_ce, getThis(), ZEND_STRL("properties"), 0 TSRMLS_CC);
    if (Z_TYPE_P(properties) != IS_ARRAY)
        return;

    if (zend_hash_del(Z_ARRVAL_P(properties), name, nameLen + 1) == SUCCESS)
        return;

    zend_throw_exception_ex(Nc_Exception_ce, 0 TSRMLS_CC, "Cannot unset data #%s#.", name);
}

ZEND_METHOD(Nc_Kernel, inject)
{
    zval *target;
    char *name = NULL, *namedAs = NULL;
    uint nameLen = 0, namedAsLen = 0;
    zval *properties;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz|s", &name, &nameLen, &target, &namedAs, &namedAsLen) == FAILURE)
        return;

    properties = nc_array_property(getThis(), "properties");
    if (!properties)
        return;

    Z_ADDREF_P(target);

    if (zend_hash_update(Z_ARRVAL_P(properties), name, nameLen + 1, &target, sizeof(zval *), NULL) == FAILURE)
    {
        Z_DELREF_P(target);
        return;
    }

    if (namedAsLen > 0 && Z_TYPE_P(target) == IS_OBJECT && instanceof_function(zend_get_class_entry(target TSRMLS_CC), Nc_Kernel_ce TSRMLS_CC))
    {
        zval *targetProperties;
        targetProperties = nc_array_property(target, "properties");

        if (!targetProperties)
        {
            zend_throw_exception(Nc_Exception_ce, "Cannot fetch target oject properties.", 0 TSRMLS_CC);
            return;
        }

        Z_ADDREF_P(getThis());

        if (zend_hash_update(Z_ARRVAL_P(targetProperties), namedAs, namedAsLen + 1, &getThis(), sizeof(zval *), NULL) == FAILURE)
        {
            Z_DELREF_P(getThis());
            return;
        }
    }

    RETURN_ZVAL(getThis(), 1, 0)
}

ZEND_METHOD(Nc_Kernel, setInjection)
{
    char *name = NULL;
    int nameLen = 0;
    zval *injection;
    zval *injections;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &name, &nameLen, &injection) == FAILURE)
        return;

    injections = nc_array_property(getThis(), "injections");
    if (!injections)
        return;


    Z_ADDREF_P(injection);

    if (zend_hash_update(Z_ARRVAL_P(injections), name, nameLen + 1, &injection, sizeof(zval *), NULL) == FAILURE)
    {
        Z_DELREF_P(injection);
        return;
    }

    RETURN_ZVAL(getThis(), 1, 0)
}

zend_function_entry Nc_Kernel_methods[] = {
    ZEND_ME(Nc_Kernel, newInstance, Nc_Kernel_newInstance_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    ZEND_ME(Nc_Kernel, offsetGet, Nc_Kernel_offsetGet_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Kernel, offsetSet, Nc_Kernel_offsetSet_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Kernel, offsetExists, Nc_Kernel_offsetExists_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Kernel, offsetUnset, Nc_Kernel_offsetUnset_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Kernel, inject, Nc_Kernel_inject_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Kernel, setInjection, Nc_Kernel_setInjection_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

zend_class_entry *Nc_PropertyAccess_ce;

NC_INIT_CLASS_FUNCTION(Nc_PropertyAccess)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\PropertyAccess", Nc_PropertyAccess_methods);
    Nc_PropertyAccess_ce = zend_register_internal_interface(&ce TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_PropertyAccess___get_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_PropertyAccess___isset_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

zend_function_entry Nc_PropertyAccess_methods[] = {
    ZEND_ABSTRACT_ME(Nc_PropertyAccess, __get, Nc_PropertyAccess___get_arginfo)
    ZEND_ABSTRACT_ME(Nc_PropertyAccess, __isset, Nc_PropertyAccess___isset_arginfo)
    ZEND_FE_END
};

zend_class_entry *Nc_InstantStorage_ce;

NC_INIT_CLASS_FUNCTION(Nc_InstantStorage)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Nc\\InstantStorage", Nc_InstantStorage_methods);
    Nc_InstantStorage_ce = zend_register_internal_interface(&ce TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_InstantStorage_set_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, id)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_InstantStorage_setMany_arginfo, 0, 0, 1)
    ZEND_ARG_ARRAY_INFO(0, idValues, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_InstantStorage_get_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_InstantStorage_getMany_arginfo, 0, 0, 1)
    ZEND_ARG_ARRAY_INFO(0, ids, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_InstantStorage_remove_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_InstantStorage_removeMany_arginfo, 0, 0, 1)
    ZEND_ARG_ARRAY_INFO(0, ids, 0)
ZEND_END_ARG_INFO()

zend_function_entry Nc_InstantStorage_methods[] = {
    ZEND_ABSTRACT_ME(Nc_InstantStorage, get, Nc_InstantStorage_get_arginfo)
    ZEND_ABSTRACT_ME(Nc_InstantStorage, getMany, Nc_InstantStorage_getMany_arginfo)
    ZEND_ABSTRACT_ME(Nc_InstantStorage, set, Nc_InstantStorage_set_arginfo)
    ZEND_ABSTRACT_ME(Nc_InstantStorage, setMany, Nc_InstantStorage_setMany_arginfo)
    ZEND_ABSTRACT_ME(Nc_InstantStorage, remove, Nc_InstantStorage_remove_arginfo)
    ZEND_ABSTRACT_ME(Nc_InstantStorage, removeMany, Nc_InstantStorage_removeMany_arginfo)
    ZEND_FE_END
};
