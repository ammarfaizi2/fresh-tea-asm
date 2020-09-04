
#ifndef HELPERS__PHP_H
#define HELPERS__PHP_H

#include <php.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  zval *params;
  zval retval;
  zval callable;
  uint32_t param_count;
} php_cf;

/* Call PHP function. */
zval *php_call_func(php_cf *in);
php_cf *php_cf_ctor(char *fname, uint32_t param_count);
void php_cf_dtor(php_cf *st);

/* Get PHP global variable. */
zval *get_global_var(char *name);
zval *get_global_var_safe(char *name);

/* $_SERVER, $_POST, $_GET, $_FILES */
zval *get_server_var(char *key);
zval *get_post_var(char *key);
zval *get_get_var(char *key);
zval *get_files_var(char *key);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
