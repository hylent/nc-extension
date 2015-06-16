zend_class_entry *Nc_Loader_ClassPathLoader_ce;

NC_INIT_CLASS_FUNCTION(Nc_Loader_ClassPathLoader)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "Nc\\Loader\\ClassPathLoader", Nc_Loader_ClassPathLoader_methods);
    Nc_Loader_ClassPathLoader_ce = zend_register_internal_class_ex(&ce, Nc_Loader_AbstractLoader_ce, NULL TSRMLS_CC);

    zend_declare_property_null(Nc_Loader_ClassPathLoader_ce, ZEND_STRL("classPaths"), ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Loader_ClassPathLoader___invoke_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Loader_ClassPathLoader_add_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, className)
    ZEND_ARG_INFO(0, classPath)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Loader_ClassPathLoader, __invoke)
{
    char *name;
    uint nameLen;

    zval *paths;
    zval **path;
    zval chk;

    zend_file_handle zfh;
    zval *ret;

    paths = zend_read_property(Nc_Loader_ClassPathLoader_ce, getThis(), ZEND_STRL("classPaths"), 0 TSRMLS_CC);
    if (Z_TYPE_P(paths) != IS_ARRAY)
        return;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &nameLen) == FAILURE)
        return;

    if (zend_hash_find(Z_ARRVAL_P(paths), name, nameLen + 1, (void **) &path) == FAILURE)
        return;

    php_stat(Z_STRVAL_PP(path), Z_STRLEN_PP(path), FS_IS_FILE, &chk TSRMLS_CC);
    if (Z_TYPE(chk) != IS_BOOL || !Z_BVAL(chk))
    {
        zend_throw_exception_ex(Nc_Loader_Exception_ce, 0 TSRMLS_CC, "Cannot find class in #%s#.", Z_STRVAL_PP(path));
        return;
    }

    zfh.type = ZEND_HANDLE_FILENAME;
    zfh.filename = Z_STRVAL_PP(path);
    zfh.free_filename = 0;
    zfh.opened_path = NULL;

    if (zend_execute_scripts(ZEND_REQUIRE TSRMLS_CC, &ret, 1, &zfh) == FAILURE)
    {
        zend_throw_exception(Nc_Loader_Exception_ce, "Cannot require class path.", 0 TSRMLS_CC);
        return;
    }

    RETURN_ZVAL(ret, 0, 1);
}

ZEND_METHOD(Nc_Loader_ClassPathLoader, add)
{
    char *name, *path;
    uint nameLen, pathLen;

    zval *zpath;
    zval *paths;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &name, &nameLen, &path, &pathLen) == FAILURE)
    {
        zend_throw_exception(Nc_Loader_Exception_ce, "Invalid arguments.", 0 TSRMLS_CC);
        return;
    }

    MAKE_STD_ZVAL(zpath);
    ZVAL_STRINGL(zpath, path, pathLen, 1);

    paths = nc_array_property(getThis(), "classPaths");

    if (!path || zend_hash_update(Z_ARRVAL_P(paths), name, nameLen + 1, &zpath, sizeof(zval *), NULL) == FAILURE)
    {
        zval_ptr_dtor(&zpath);
        zend_throw_exception(Nc_Loader_Exception_ce, "Cannot add class path.", 0 TSRMLS_CC);
        return;
    }

    RETURN_ZVAL(getThis(), 1, 0);
}

zend_function_entry Nc_Loader_ClassPathLoader_methods[] = {
    ZEND_ME(Nc_Loader_ClassPathLoader, __invoke, Nc_Loader_ClassPathLoader___invoke_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Loader_ClassPathLoader, add, Nc_Loader_ClassPathLoader_add_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
