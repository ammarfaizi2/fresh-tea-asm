
#include <php.h>
#include "entry_point.h"

ZEND_DECLARE_MODULE_GLOBALS(fresh_tea_asm);

ZEND_GET_MODULE(fresh_tea_asm);

#include "class_ce_extern.frag.h"

static PHP_MINIT_FUNCTION(fresh_tea_asm)
{
  {
    zend_class_entry ce;
    #include "class.frag.cpp"
  }

  REGISTER_INI_ENTRIES();
  return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION(fresh_tea_asm)
{
  UNREGISTER_INI_ENTRIES();
  return SUCCESS;
}

static PHP_GINIT_FUNCTION(fresh_tea_asm)
{
#if defined(COMPILE_DL_ASTKIT) && defined(ZTS)
  ZEND_TSRMLS_CACHE_UPDATE();
#endif
}

zend_module_entry fresh_tea_asm_module_entry = {
  STANDARD_MODULE_HEADER,
  "fresh_tea_asm",
  NULL, /* functions */
  PHP_MINIT(fresh_tea_asm),
  PHP_MSHUTDOWN(fresh_tea_asm),
  NULL, /* RINIT */
  NULL, /* RSHUTDOWN */
  NULL, /* MINFO */
  "0.0",
  PHP_MODULE_GLOBALS(fresh_tea_asm),
  PHP_GINIT(fresh_tea_asm),
  NULL, /* GSHUTDOWN */
  NULL, /* RPOSTSHUTDOWN */
  STANDARD_MODULE_PROPERTIES_EX
};
