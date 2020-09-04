
#include <cstdio>
#include <cstdbool>
#include <sys/mman.h>

#include "helpers.hpp"


/* /home/ammarfaizi2/project/now/fresh-tea-asm/src.frag/classes/FreshTeaASM/JIT/x86_64/Executor.php.cpp */

#include <php.h>

#ifdef __cplusplus
extern "C" {
#endif

zend_class_entry *ce_ltp_FreshTeaASM_JIT_x86_64_Executor;

/**
 * Constructor.
 *
 * @param string $code
 */
static PHP_METHOD(ltp_FreshTeaASM_JIT_x86_64_Executor, __construct) {
  char *code;
  void *jited;
  size_t code_len;
  zval *_this;

  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STRING(code, code_len)
  ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

  jited = mmap(NULL, code_len, PROT_READ|PROT_EXEC|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  memcpy(jited, code, code_len);

  _this = getThis();

  zend_update_property_stringl(
    ce_ltp_FreshTeaASM_JIT_x86_64_Executor, _this, ZEND_STRL("code"), code, code_len TSRMLS_CC);
  zend_update_property_stringl(
    ce_ltp_FreshTeaASM_JIT_x86_64_Executor, _this, ZEND_STRL("jited"), (char *)&jited, sizeof(void *) TSRMLS_CC);
}

/**
 * Constructor.
 *
 * @param string $code
 */
static PHP_METHOD(ltp_FreshTeaASM_JIT_x86_64_Executor, execute) {

  int argc;
  zval *jited_zv, *_this, *args, rv;

  _this = getThis();
  jited_zv = zend_read_property(ce_ltp_FreshTeaASM_JIT_x86_64_Executor, _this, ZEND_STRL("jited"), 1, &rv TSRMLS_CC);

  ZEND_PARSE_PARAMETERS_START(0, -1)
    Z_PARAM_VARIADIC('*', args, argc)
  ZEND_PARSE_PARAMETERS_END();

  for (int i = 0; i < argc; i++) {
    register void *arg_val;

    switch (Z_TYPE_P(args+i)) {
      case IS_LONG:
        arg_val = (void *)Z_LVAL_P((args+i));
        break;

      case IS_STRING:
        arg_val = (void *)&((args+i)->value.str);
        break;
    }

    __asm__ volatile ("mov %0, %%r9; push %%r9" :: "r"(arg_val));
  }

  {
    register void *rdi = NULL;
    register void (*callback)(void *rdi);
    callback = (void (*)(void *rdi))(*((void **)Z_STRVAL_P(jited_zv)));

    __asm__ volatile ("lea (%rsp), %rdi");
    callback(rdi);

    for (int i = 0; i < argc; ++i) {
      __asm__ volatile ("pop %r9");
    }
  }
}

zend_function_entry methods_ltp_FreshTeaASM_JIT_x86_64_Executor[] = {
  PHP_ME(ltp_FreshTeaASM_JIT_x86_64_Executor, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
  PHP_ME(ltp_FreshTeaASM_JIT_x86_64_Executor, execute, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
  PHP_FE_END
};

#ifdef __cplusplus
} /* extern "C" */
#endif

/* End of file */
