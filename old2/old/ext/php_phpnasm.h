
#ifndef PHP_PHPNASM_H
#define PHP_PHPNASM_H

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include "php.h"

extern zend_module_entry phpnasm_module_entry;

static PHP_METHOD(Vgd, __construct);
static PHP_METHOD(Vgd, rstr);

PHP_INI_BEGIN()
PHP_INI_END()


ZEND_BEGIN_MODULE_GLOBALS(phpnasm)
ZEND_END_MODULE_GLOBALS(phpnasm)
ZEND_EXTERN_MODULE_GLOBALS(phpnasm)
#define PHPNASMG(v) ZEND_MODULE_GLOBALS_ACCESSOR(phpnasm, v)

#ifdef COMPILE_DL_PHPNASM
ZEND_GET_MODULE(phpnasm)
#endif

#define phpext_phpnasm_ptr &phpnasm_module_entry

#endif
