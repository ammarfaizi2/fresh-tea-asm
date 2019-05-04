/* vgd.c */

#include "php_vgd.h"
#include "zend_exceptions.h"

ZEND_DECLARE_MODULE_GLOBALS(vgd);

static PHP_METHOD(Vgd, __construct) {

}


static PHP_METHOD(Vgd, rstr) {

    char *charset;
    char *user_charset = NULL;

    int charset_size;
    int size = 32;

    ZEND_PARSE_PARAMETERS_START(0, 2)

        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(size)

        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(user_charset, charset_size)

    ZEND_PARSE_PARAMETERS_END();

    if (user_charset == NULL) {
        charset = "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM___";
        charset_size = (sizeof("1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM___") - 1);
    } else {
        charset = user_charset;
        charset_size--;
    }

    char str[size];
    for (size_t n = 0; n < size; n++) {
        uint8_t key = rand() % charset_size;
        str[n] = charset[key];
    }

    RETURN_STRINGL(str, size)
}

static zend_function_entry vgd_methods[] = {
    // Prohibit construct.
    PHP_ME(Vgd, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PRIVATE)

    // Generate random string.
    PHP_ME(Vgd, rstr, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

static PHP_MINIT_FUNCTION(vgd) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Vgd\\Vgd", vgd_methods);
    zend_register_internal_class(&ce);
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION(vgd) {
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

static PHP_GINIT_FUNCTION(vgd) {
#if defined(COMPILE_DL_ASTKIT) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
}

zend_module_entry vgd_module_entry = {
    STANDARD_MODULE_HEADER,
    "vgd",
    NULL, /* functions */
    PHP_MINIT(vgd),
    PHP_MSHUTDOWN(vgd),
    NULL, /* RINIT */
    NULL, /* RSHUTDOWN */
    NULL, /* MINFO */
    "0.1",
    PHP_MODULE_GLOBALS(vgd),
    PHP_GINIT(vgd),
    NULL, /* GSHUTDOWN */
    NULL, /* RPOSTSHUTDOWN */
    STANDARD_MODULE_PROPERTIES_EX
};
