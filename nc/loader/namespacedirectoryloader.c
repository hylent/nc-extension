zend_class_entry *Nc_Loader_NamespaceDirectoryLoader_ce;

NC_INIT_CLASS_FUNCTION(Nc_Loader_NamespaceDirectoryLoader)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "Nc\\Loader\\NamespaceDirectoryLoader", Nc_Loader_NamespaceDirectoryLoader_methods);
    Nc_Loader_NamespaceDirectoryLoader_ce = zend_register_internal_class_ex(&ce, Nc_Loader_AbstractLoader_ce, NULL TSRMLS_CC);

    zend_declare_property_null(Nc_Loader_NamespaceDirectoryLoader_ce, ZEND_STRL("nsDirs"), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(Nc_Loader_NamespaceDirectoryLoader_ce, ZEND_STRL("nsDirLcs"), ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}

ZEND_BEGIN_ARG_INFO_EX(Nc_Loader_NamespaceDirectoryLoader___invoke_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Nc_Loader_NamespaceDirectoryLoader_add_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, ns)
    ZEND_ARG_INFO(0, dir)
    ZEND_ARG_INFO(0, lcase)
ZEND_END_ARG_INFO()

ZEND_METHOD(Nc_Loader_NamespaceDirectoryLoader, __invoke)
{

    zval *nsDirs;
    zval *nsDirLcs;
    char *name;
    uint nameLen;
    uint nsLen;
    char *pos;
    char *ns;
    zval **zdir;
    zend_bool lc = 1;
    zval **zlc;
    uint pathLen;
    char *path;
    char *p;
    uint i;
    zval chk;
    zend_file_handle zfh;
    zval *ret;

    nsDirs = zend_read_property(Nc_Loader_NamespaceDirectoryLoader_ce, getThis(), ZEND_STRL("nsDirs"), 0 TSRMLS_CC);
    nsDirLcs = zend_read_property(Nc_Loader_NamespaceDirectoryLoader_ce, getThis(), ZEND_STRL("nsDirLcs"), 0 TSRMLS_CC);

    if (Z_TYPE_P(nsDirs) != IS_ARRAY || Z_TYPE_P(nsDirLcs) != IS_ARRAY)
        return;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &nameLen) == FAILURE)
    {
        return;
    }

    nsLen = nameLen;
    pos = strchr(name, '\\');
    if (pos)
        nsLen = pos - name;

    ns = emalloc(nsLen + 1);
    strncpy(ns, name, nsLen);
    ns[nsLen] = '\0';

    if (zend_hash_find(Z_ARRVAL_P(nsDirs), ns, nsLen + 1, (void **) &zdir) == FAILURE)
    {
        efree(ns);
        return;
    }

    if (zend_hash_find(Z_ARRVAL_P(nsDirLcs), ns, nsLen + 1, (void **) &zlc) == SUCCESS
        && Z_TYPE_PP(zlc) == IS_BOOL && !Z_BVAL_PP(zlc))
    {
        lc = 0;
    }

    efree(ns);

    pathLen = Z_STRLEN_PP(zdir) + 1 + nameLen + 4;
    path = emalloc(pathLen + 1);
    p = path;

    strncpy(p, Z_STRVAL_PP(zdir), Z_STRLEN_PP(zdir));
    p += Z_STRLEN_PP(zdir);

    *p = DEFAULT_SLASH;
    p++;

    i = 0;
    while (i < nameLen)
    {
        char c = name[i];

        if (c == '\\')
            *p = DEFAULT_SLASH;
        else
            *p = lc ? tolower(c) : c;

        i++;
        p++;
    }

    strncpy(p, ".php", 4);
    p += 4;
    *p = '\0';

    php_stat(path, pathLen, FS_IS_FILE, &chk TSRMLS_CC);
    if (Z_TYPE(chk) != IS_BOOL || !Z_BVAL(chk))
    {
        zend_throw_exception_ex(Nc_Loader_Exception_ce, 0 TSRMLS_CC, "Cannot find class in #%s#.", path);
        efree(path);
        return;
    }

    zfh.type = ZEND_HANDLE_FILENAME;
    zfh.filename = path;
    zfh.free_filename = 0;
    zfh.opened_path = NULL;

    if (zend_execute_scripts(ZEND_REQUIRE TSRMLS_CC, &ret, 1, &zfh) == FAILURE)
    {
        zend_throw_exception(Nc_Loader_Exception_ce, "Cannot require class path.", 0 TSRMLS_CC);
        efree(path);
        return;
    }

    efree(path);

    RETURN_ZVAL(ret, 0, 1);
}

ZEND_METHOD(Nc_Loader_NamespaceDirectoryLoader, add)
{
    char *ns, *dir;
    uint nsLen, dirLen;
    zend_bool lc = 1;

    zval *nsDirs;
    zval *zdir;

    zval *nsDirLcs;
    zval *zlc;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|b", &ns, &nsLen, &dir, &dirLen, &lc) == FAILURE)
    {
        zend_throw_exception(Nc_Loader_Exception_ce, "Invalid arguments.", 0 TSRMLS_CC);
        return;
    }

    nsDirs = nc_array_property(getThis(), "nsDirs");
    nsDirLcs = nc_array_property(getThis(), "nsDirLcs");
    if (!nsDirs || !nsDirLcs)
    {
        zend_throw_exception(Nc_Loader_Exception_ce, "Cannot fetch properties.", 0 TSRMLS_CC);
        return;
    }

    MAKE_STD_ZVAL(zdir);
    ZVAL_STRINGL(zdir, dir, dirLen, 1);

    if (zend_hash_update(Z_ARRVAL_P(nsDirs), ns, nsLen + 1, &zdir, sizeof(zval *), NULL) == FAILURE)
    {
        zval_ptr_dtor(&zdir);
        zend_throw_exception(Nc_Loader_Exception_ce, "Cannot add namespace directory.", 0 TSRMLS_CC);
    }

    MAKE_STD_ZVAL(zlc);
    ZVAL_BOOL(zlc, lc);

    if (zend_hash_update(Z_ARRVAL_P(nsDirLcs), ns, nsLen + 1, &zlc, sizeof(zval *), NULL) == FAILURE)
    {
        zval_ptr_dtor(&zlc);
        zend_throw_exception(Nc_Loader_Exception_ce, "Cannot add namespace directory lcase.", 0 TSRMLS_CC);
    }

    RETURN_ZVAL(getThis(), 1, 0);
}

zend_function_entry Nc_Loader_NamespaceDirectoryLoader_methods[] = {
    ZEND_ME(Nc_Loader_NamespaceDirectoryLoader, __invoke, Nc_Loader_NamespaceDirectoryLoader___invoke_arginfo, ZEND_ACC_PUBLIC)
    ZEND_ME(Nc_Loader_NamespaceDirectoryLoader, add, Nc_Loader_NamespaceDirectoryLoader_add_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
