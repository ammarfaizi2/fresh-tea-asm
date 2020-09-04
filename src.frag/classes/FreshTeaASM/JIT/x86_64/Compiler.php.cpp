
#include <cstdio>
#include <cstdbool>

#include "helpers.hpp"

<?php
LightTeaPHP::beginFile(__FILE__);
$fta = new PHPClass("FreshTeaASM\\JIT\\x86_64\\Compiler", __FILE__);
$fta->start();
?>

inline static zval *shell_exec(const char *cmd)
{
  zval *ret;
  php_cf *func;
  zval params[1];

  ZVAL_STRING(&params[0], cmd);

  func = php_cf_ctor((char *)"shell_exec", 1);
  func->params = params;
  ret = php_call_func(func);
  php_cf_dtor(func);
  zval_dtor(&params[0]);

  if (!ret) {
    zend_error(E_WARNING, "Cannot execute shell_exec");
    return NULL;
  }

  return ret;
}

/**
 * Constructor.
 *
 * @param string $code
 */
static <?= $fta->method("__construct"); ?> {
  char *code;
  size_t code_len;
  zval *_this;

  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STRING(code, code_len)
  ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

  _this = getThis();
  zend_update_property_stringl(
    <?= $fta->ce; ?>, _this, ZEND_STRL("code"), code, code_len TSRMLS_CC);
}

/**
 * @return bool
 */
static <?= $fta->method("compile"); ?> {
  FILE *handle;
  bool ret = true;
  zval *code_zv = NULL, *hash = NULL, *_this, rv, *exefile = NULL;
  char filename[sizeof("/tmp/") + 41 + sizeof(".asm")];

  _this = getThis();
  code_zv = zend_read_property(<?= $fta->ce; ?>, _this, ZEND_STRL("code"), 1, &rv TSRMLS_CC);

  if (!code_zv) {
    zend_error(E_WARNING, "Cannot read code_zv");
    ret = false;
    goto ret;
  }

  if (Z_TYPE_P(code_zv) != IS_STRING) {
    zend_error(E_WARNING, "code_zv is not a string");
    ret = false;
    goto ret; 
  }

  /* Hash the content. */
  {
    php_cf *func;
    func = php_cf_ctor((char *)"sha1", 1);
    func->params = code_zv;
    hash = php_call_func(func);

    if (!hash) {
      zend_error(E_WARNING, "Cannot call sha1 hash");
      ret = false;
      goto hash_done;
    }

    if (Z_TYPE_P(hash) == IS_NULL) {
      zend_error(E_WARNING, "sha1 returned null");
      ret = false;
      goto hash_done;
    }

    hash_done:
    if (func) php_cf_dtor(func);
    if (!ret) goto ret;
  }

  /* Write to file. */
  {
    sprintf(filename, "/tmp/%s.asm", Z_STRVAL_P(hash));

    FILE *handle = fopen(filename, "wb");
    if (!handle) {
      zend_error(E_WARNING, "Cannot open file: %s", filename);
      ret = false;
      goto ret;
    }
    #define _START_FILE \
      "section .text\n\n_start:\n"

    fwrite(_START_FILE, sizeof(char), sizeof(_START_FILE) - 1, handle);
    fwrite(Z_STRVAL_P(code_zv), sizeof(char), Z_STRLEN_P(code_zv), handle);
    fclose(handle);
  }

  /* Compile the file. */
  {
    size_t exefile_len;
    exefile = shell_exec("which nasm");

    if ((!exefile) || (Z_TYPE_P(exefile) == IS_NULL)) {
      zend_error(E_WARNING, "Cannot find nasm binary");
      ret = false;
      goto ret;
    }

    char nasm_cmd[Z_STRLEN_P(exefile) + (sizeof(filename) * 2) + 128];

    printf("exefile = %s\n", Z_STRVAL_P(exefile));

    sprintf(nasm_cmd, "%s -felf64 %s -o /tmp/%s.o",
      Z_STRVAL_P(exefile), filename, Z_STRVAL_P(hash));

    php_printf("%s\n", nasm_cmd);
    zval_dtor(exefile);
    exefile = NULL;
  }

  /* Take .text section. */
  {

  }

  /* Clean up. */
  {
    remove(filename);
  }


ret:
  if (hash) zval_dtor(hash);
  if (exefile) zval_dtor(exefile);
  if (!ret) {
    RETURN_FALSE;
  }
}

<?php
$fta->end();
LightTeaPHP::addClass($fta);
LightTeaPHP::endFile(__FILE__);
