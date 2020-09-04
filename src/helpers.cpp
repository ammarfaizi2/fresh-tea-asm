
#include <php.h>

#include "helpers.hpp"

#ifdef __cplusplus
extern "C" {
#endif

zval *php_call_func(php_cf *in)
{
  char *error = NULL;
  zend_fcall_info fci;
  zend_fcall_info_cache fci_cache;

  zend_fcall_info_init(&(in->callable), 0, &fci, &fci_cache, NULL, &error);

  if (error) {
    php_printf("Error: %s\n", error);
    efree(error);
    return NULL;
  }

  fci.params = in->params;
  fci.param_count = in->param_count;
  fci.retval = &(in->retval);

  if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(in->retval) != IS_UNDEF) {
    if (Z_ISREF(in->retval)) {
      zend_unwrap_reference(&(in->retval));
    }
  }
  return &(in->retval);
}

php_cf *php_cf_ctor(char *fname, uint32_t param_count)
{
  php_cf *st = (php_cf *)emalloc(sizeof(php_cf));
  ZVAL_STRING(&(st->callable), fname);
  st->param_count = param_count;
  return st;
}

void php_cf_dtor(php_cf *st)
{
  if (st == NULL) {
    return;
  }
  zval_dtor(&(st->callable));
  efree(st);
}

zval *get_server_var(char *key)
{
  return zend_hash_str_find(
    Z_ARRVAL(PG(http_globals)[TRACK_VARS_SERVER]),
    key, strlen(key));
}

zval *get_post_var(char *key)
{
  return zend_hash_str_find(
    Z_ARRVAL(PG(http_globals)[TRACK_VARS_POST]),
    key, strlen(key));
}

zval *get_get_var(char *key)
{
  return zend_hash_str_find(
    Z_ARRVAL(PG(http_globals)[TRACK_VARS_GET]),
    key, strlen(key));
}

zval *get_files_var(char *key)
{
  return zend_hash_str_find(
    Z_ARRVAL(PG(http_globals)[TRACK_VARS_FILES]),
    key, strlen(key));
}

zval *get_global_var(char *name)
{
  return zend_hash_str_find(&EG(symbol_table), name, strlen(name));
}

zval *get_global_var_safe(char *name)
{
  zval *r = get_global_var(name);
  if ((r != NULL) && (Z_TYPE_P(r) == IS_INDIRECT)) {
    return Z_INDIRECT_P(r);
  }
  return r;
}


#ifdef __cplusplus
} /* extern "C" */
#endif
