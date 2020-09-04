
#include <cstdio>
#include <cstdbool>
#include <sys/mman.h>

#include "helpers.hpp"

<?php
LightTeaPHP::beginFile(__FILE__);
$exe = new PHPClass("FreshTeaASM\\JIT\\x86_64\\Executor", __FILE__);
$exe->start();
?>

/**
 * Constructor.
 *
 * @param string $code
 */
static <?= $exe->method("__construct", [ZEND_ACC_CTOR, ZEND_ACC_PUBLIC]); ?> {
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
    <?= $exe->ce; ?>, _this, ZEND_STRL("code"), code, code_len TSRMLS_CC);
  zend_update_property_stringl(
    <?= $exe->ce; ?>, _this, ZEND_STRL("jited"), (char *)&jited, sizeof(void *) TSRMLS_CC);
}

/**
 * Constructor.
 *
 * @param string $code
 */
static <?= $exe->method("execute", [ZEND_ACC_CTOR, ZEND_ACC_PUBLIC]); ?> {

  int argc;
  zval *jited_zv, *_this, *args, rv;

  _this = getThis();
  jited_zv = zend_read_property(<?= $exe->ce; ?>, _this, ZEND_STRL("jited"), 1, &rv TSRMLS_CC);

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

<?php
$exe->end();
LightTeaPHP::addClass($exe);
LightTeaPHP::endFile(__FILE__);
