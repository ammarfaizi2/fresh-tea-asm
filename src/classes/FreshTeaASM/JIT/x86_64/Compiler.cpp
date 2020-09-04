
#include <cstdio>


/* /home/ammarfaizi2/project/now/fresh-tea-asm/src.frag/classes/FreshTeaASM/JIT/x86_64/Compiler.php.cpp */

#include <php.h>

#ifdef __cplusplus
extern "C" {

#endif

zend_class_entry *ce_ltp_FreshTeaASM_JIT_x86_64;

/**
 * Constructor.
 *
 * @param string $code
 */
static PHP_METHOD(ltp_FreshTeaASM_JIT_x86_64, __construct) {
  char *code;
  size_t code_len;

  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STRING(code, code_len)
  ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

  zend_update_property_string(
    ce_ltp_FreshTeaASM_JIT_x86_64, _this, ZEND_STRL("code"), code, code_len TSRMLS_CC);
}

/**
 * @return bool
 */
static PHP_METHOD(ltp_FreshTeaASM_JIT_x86_64, compile) {
  FILE *handle;
  zval *code_zv, rv;
  char filename[512];

  code_zv = zend_read_property(ce_ltp_FreshTeaASM_JIT_x86_64, _this, ZEND_STRL("code"), 1, &rv TSRMLS_CC);

  Z_STRVAL_P(code_zv);

  handle = fopen("/tmp/")
}

zend_function_entry methods_ltp_FreshTeaASM_JIT_x86_64[] = {
  PHP_ME(ltp_FreshTeaASM_JIT_x86_64, __construct, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(ltp_FreshTeaASM_JIT_x86_64, compile, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

#ifdef __cplusplus
} /* extern "C" */

#endif

/* End of file */
