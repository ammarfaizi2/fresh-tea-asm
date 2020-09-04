
/**
 * @author Ammar Faizi
 * @license MIT
 * @version 0.3
 */
#include "phpnasm.h"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

ZEND_DECLARE_MODULE_GLOBALS(phpnasm);

zend_class_entry *phpnasm_ce;

/**
 * @param string $code
 *
 * Constructor.
 */
PHP_METHOD(PhpNasm__x86_64__ASM_JIT, __construct)
{
  void *map;
  char *code;
  size_t siz;
  zval *obj;

  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STRING(code, siz)
  ZEND_PARSE_PARAMETERS_END();

  map = mmap(NULL, siz,
    PROT_READ|PROT_EXEC|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  memcpy(map, code, siz);

  obj = getThis();
  zend_update_property_stringl(phpnasm_ce, obj,
    ZEND_STRL("map_addr"), (char *)&map, sizeof(map) TSRMLS_DC);
  zend_update_property_long(phpnasm_ce, obj, ZEND_STRL("len"), siz TSRMLS_DC);
}

/**
 * Destructor.
 */
PHP_METHOD(PhpNasm__x86_64__ASM_JIT, __destruct)
{
  zval rv, *args, *map_addr, *obj, *len;
  obj = getThis();
  len = zend_read_property(phpnasm_ce, obj, ZEND_STRL("len"), 0, &rv);
  map_addr = zend_read_property(phpnasm_ce, obj, ZEND_STRL("map_addr"), 0, &rv);

  munmap(*((void **)Z_STRVAL_P(map_addr)), Z_LVAL_P(len));
}


/**
 * @param mixed ...$args
 * @return mixed
 */
PHP_METHOD(PhpNasm__x86_64__ASM_JIT, execute)
{
  int argc = 0;
  void *ret = NULL;
  register void *arg_val;
  size_t jit_stack_size;
  zval rv, *args, *map_addr, *obj;

  ZEND_PARSE_PARAMETERS_START(0, -1)
    Z_PARAM_VARIADIC('*', args, argc)
  ZEND_PARSE_PARAMETERS_END();

  obj = getThis();
  map_addr = zend_read_property(phpnasm_ce, obj, ZEND_STRL("map_addr"), 0, &rv);

  // Get all args
  for (int i = 0; i < argc; i++) {
    switch (Z_TYPE_P(args+i)) {
      case IS_DOUBLE:
        arg_val = (void *)(&((args+i)->value.dval));
        break;

      case IS_TRUE:
        arg_val = (void *)1;
        break;

      case IS_FALSE:
        arg_val = (void *)0;
        break;

      case IS_LONG:
        arg_val = (void *)((args+i)->value.lval);
        break;

      case IS_STRING:
        arg_val = (void *)((args+i)->value.str);
        break;

      default:
        arg_val = (void *)(args+i);
        break;
    }
    __asm__ volatile ("push %0;" :: "r"(arg_val));
  }


  {
    // Call JIT-ed code.
    __asm__ volatile (
      "lea (%%rsp), %%rdi;"
      "callq *%1;"
      "mov %%rax, %0;"
      : "=m" (ret)
      : "r"  ( *((void **)Z_STRVAL_P(map_addr)) ) : "cc", "rax", "rdi"
    );

    jit_stack_size = argc * sizeof(void *);

    // Clean up JIT stack.
    __asm__ volatile ("add %0, %%rsp;" :: "m"(jit_stack_size));
  }
}


zend_function_entry phpnasm_methods[] = {
  PHP_ME(PhpNasm__x86_64__ASM_JIT, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
  PHP_ME(PhpNasm__x86_64__ASM_JIT, __destruct, NULL, ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
  PHP_ME(PhpNasm__x86_64__ASM_JIT, execute, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};


PHP_MINIT_FUNCTION(phpnasm)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "PHPNasm\\x86_64\\ASM_JIT", phpnasm_methods);
  phpnasm_ce = zend_register_internal_class(&ce TSRMLS_CC);

  zend_declare_property_string(
    phpnasm_ce, ZEND_STRL("map_addr"), "", ZEND_ACC_PRIVATE TSRMLS_CC);

  zend_declare_property_long(
    phpnasm_ce, ZEND_STRL("len"), 0, ZEND_ACC_PRIVATE TSRMLS_CC);

  REGISTER_INI_ENTRIES();
  return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(phpnasm)
{
  UNREGISTER_INI_ENTRIES();
  return SUCCESS;
}


PHP_GINIT_FUNCTION(phpnasm)
{
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
  "0.3",
  PHP_MODULE_GLOBALS(phpnasm),
  PHP_GINIT(phpnasm),
  NULL, /* GSHUTDOWN */
  NULL, /* RPOSTSHUTDOWN */
  STANDARD_MODULE_PROPERTIES_EX
};
