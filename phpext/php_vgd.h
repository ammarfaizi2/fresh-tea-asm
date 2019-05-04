
#ifndef PHP_VGD_H
#define PHP_VGD_H

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include "php.h"

extern zend_module_entry vgd_module_entry;

static PHP_METHOD(Vgd, __construct);
static PHP_METHOD(Vgd, rstr);

PHP_INI_BEGIN()
PHP_INI_END()


ZEND_BEGIN_MODULE_GLOBALS(vgd)
ZEND_END_MODULE_GLOBALS(vgd)
ZEND_EXTERN_MODULE_GLOBALS(vgd)
#define VGDG(v) ZEND_MODULE_GLOBALS_ACCESSOR(vgd, v)

#ifdef COMPILE_DL_VGD
ZEND_GET_MODULE(vgd)
#endif

#define phpext_vgd_ptr &vgd_module_entry

#endif
