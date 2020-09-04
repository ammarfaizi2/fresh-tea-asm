
#ifndef __FRESH_TEA_ASM_H
#define __FRESH_TEA_ASM_H

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include <php.h>

extern zend_module_entry fresh_tea_asm_module_entry;

PHP_INI_BEGIN()
PHP_INI_END()

ZEND_BEGIN_MODULE_GLOBALS(fresh_tea_asm)
ZEND_END_MODULE_GLOBALS(fresh_tea_asm)

ZEND_EXTERN_MODULE_GLOBALS(fresh_tea_asm)

#define FRESH_TEA_ASMG(v) ZEND_MODULE_GLOBALS_ACCCESSOR(fresh_tea_asm, v)

#define phpext_fresh_tea_asm_ptr (&fresh_tea_asm_module_entry)

#endif
