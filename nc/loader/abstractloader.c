zend_class_entry *Nc_Loader_AbstractLoader_ce;

NC_INIT_CLASS_FUNCTION(Nc_Loader_AbstractLoader)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "Nc\\Loader\\AbstractLoader", Nc_Loader_AbstractLoader_methods);
    Nc_Loader_AbstractLoader_ce = zend_register_internal_class_ex(&ce, Nc_Kernel_ce, NULL TSRMLS_CC);
    Nc_Loader_AbstractLoader_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Loader_AbstractLoader___construct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Loader_AbstractLoader___invoke_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Loader_AbstractLoader, __construct)
{
    zval f;
    zval *r = NULL;
    zval **argv[1];

    ZVAL_STRING(&f, "spl_autoload_register", 0);
    argv[0] = &getThis();

    if (call_user_function_ex(CG(function_table), NULL, &f, &r, 1, argv, 1, NULL TSRMLS_CC) == FAILURE || !r || Z_TYPE_P(r) != IS_BOOL || !Z_BVAL_P(r))
        zend_throw_exception(Nc_Loader_Exception_ce, "Cannot register autoload.", 0 TSRMLS_CC);

    if (r)
        zval_ptr_dtor(&r);
}

zend_function_entry Nc_Loader_AbstractLoader_methods[] = {
    ZEND_ME(Nc_Loader_AbstractLoader, __construct, Nc_Loader_AbstractLoader___construct_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ABSTRACT_ME(Nc_Loader_AbstractLoader, __invoke, Nc_Loader_AbstractLoader___invoke_arginfo)
    ZEND_FE_END
};
