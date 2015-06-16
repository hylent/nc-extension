zend_class_entry *Nc_Loader_Exception_ce;

NC_INIT_CLASS_FUNCTION(Nc_Loader_Exception)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "Nc\\Loader\\Exception", Nc_Loader_Exception_methods);
    Nc_Loader_Exception_ce = zend_register_internal_class_ex(&ce, Nc_Exception_ce, NULL TSRMLS_CC);

    return SUCCESS;
}

zend_function_entry Nc_Loader_Exception_methods[] = {
    ZEND_FE_END
};
