
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
  register char *code;
  register void *jited;
  register zval *_this;
  register size_t code_len;

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

  zval rv;
  register int argc;
  register zval *jited_zv, *_this, *args;

  _this = getThis();
  jited_zv = zend_read_property(ce_ltp_FreshTeaASM_JIT_x86_64_Executor, _this, ZEND_STRL("jited"), 1, &rv TSRMLS_CC);

  ZEND_PARSE_PARAMETERS_START(0, -1)
    Z_PARAM_VARIADIC('*', args, argc)
  ZEND_PARSE_PARAMETERS_END();

  for (register int i = argc - 1; i >= 0; i--) {
    __asm__ volatile (
      "mov %0, %%r9;"
      "push %%r9"
      :: "m"((args+i)->value)
      :  "%r9"
    );
  }

  {
    register void (*callback)();
    callback = (void (*)())(*((void **)Z_STRVAL_P(jited_zv)));
    __asm__ volatile ("lea (%%rsp), %%rdi;" ::: "%rdi");
    callback();
    register uint64_t argc_add = argc * sizeof(void *);
    __asm__ volatile ("add %0, %%rsp" :: "r"(argc_add));
  }
}

/**
 * Destructor.
 */
static PHP_METHOD(ltp_FreshTeaASM_JIT_x86_64_Executor, __destruct) {
  zval rv;
  register zval *jited_zv, *_this, *code;

  _this = getThis();
  code     = zend_read_property(ce_ltp_FreshTeaASM_JIT_x86_64_Executor, _this, ZEND_STRL("code"), 1, &rv TSRMLS_CC);
  jited_zv = zend_read_property(ce_ltp_FreshTeaASM_JIT_x86_64_Executor, _this, ZEND_STRL("jited"), 1, &rv TSRMLS_CC);

  munmap(*((void **)Z_STRVAL_P(jited_zv)), Z_STRLEN_P(code));
}

zend_function_entry methods_ltp_FreshTeaASM_JIT_x86_64_Executor[] = {
  PHP_ME(ltp_FreshTeaASM_JIT_x86_64_Executor, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
  PHP_ME(ltp_FreshTeaASM_JIT_x86_64_Executor, execute, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(ltp_FreshTeaASM_JIT_x86_64_Executor, __destruct, NULL, ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
  PHP_FE_END
};

#ifdef __cplusplus
} /* extern "C" */
#endif

/* End of file */
