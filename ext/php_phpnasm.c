/* phpnasm.c */

#include "php_phpnasm.h"
#include "zend_exceptions.h"
#include <string.h>
#include <sys/mman.h>

ZEND_DECLARE_MODULE_GLOBALS(phpnasm);

static zend_class_entry *phpnasm_ce;

static PHP_METHOD(PhpNasm, __construct) {
    char *code;
    size_t code_size;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(code, code_size)
    ZEND_PARSE_PARAMETERS_END();

    zend_update_property_stringl(
        phpnasm_ce,
        getThis(),
        ZEND_STRL("code"),
        code,
        code_size
    );
}

static PHP_METHOD(PhpNasm, execute) {
    zval rv, *_code, tmp;
    char *code;
    size_t code_size;

    _code = zend_read_property(phpnasm_ce, getThis(), ZEND_STRL("code"), 0, &rv);
    if (Z_TYPE_P(_code) != IS_STRING) {
        convert_to_string(_code);
    }
    code = Z_STRVAL_P(_code);
    code_size = Z_STRLEN_P(_code);

    void *map = mmap(NULL, code_size, PROT_READ | PROT_EXEC | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    memcpy(map, code, code_size);
    ((void * (*)())map )();
    munmap(map, code_size);
}

static zend_function_entry phpnasm_methods[] = {
    PHP_ME(PhpNasm, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(PhpNasm, execute, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static PHP_MINIT_FUNCTION(phpnasm) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "PhpNasm\\Executor", phpnasm_methods);
    zend_register_internal_class(&ce);
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION(phpnasm) {
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

static PHP_GINIT_FUNCTION(phpnasm) {
#if defined(COMPILE_DL_ASTKIT) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
}

zend_module_entry phpnasm_module_entry = {
    STANDARD_MODULE_HEADER,
    "phpnasm",
    NULL, /* functions */
    PHP_MINIT(phpnasm),
    PHP_MSHUTDOWN(phpnasm),
    NULL, /* RINIT */
    NULL, /* RSHUTDOWN */
    NULL, /* MINFO */
    "0.1",
    PHP_MODULE_GLOBALS(phpnasm),
    PHP_GINIT(phpnasm),
    NULL, /* GSHUTDOWN */
    NULL, /* RPOSTSHUTDOWN */
    STANDARD_MODULE_PROPERTIES_EX
};
