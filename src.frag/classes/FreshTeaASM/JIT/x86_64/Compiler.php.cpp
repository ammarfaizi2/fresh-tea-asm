
#include <cstdio>
#include <cstdbool>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "helpers.hpp"

<?php
LightTeaPHP::beginFile(__FILE__);
$fta = new PHPClass("FreshTeaASM\\JIT\\x86_64\\Compiler", __FILE__);
$fta->start();
?>

inline static bool shell_exec(const char *cmd, char **target, size_t *len)
{
  bool ret = true;
  zval *retval;
  php_cf *func;
  zval params[1];

  ZVAL_STRING(&params[0], cmd);

  func = php_cf_ctor((char *)"shell_exec", 1);
  func->params = params;
  retval = php_call_func(func);

  if (retval) {
    if (Z_TYPE_P(retval) == IS_STRING) {

      *len = Z_STRLEN_P(retval);

      *target = (char *)emalloc((*len) + 1);
      memcpy(*target, Z_STRVAL_P(retval), (*len));
      (*target)[(*len)] = '\0';

    } else {
      ret = false;
    }

    zval_dtor(retval);

  } else {
    zend_error(E_WARNING, "Cannot execute shell_exec");
    ret = false;
  }

  php_cf_dtor(func);
  zval_dtor(&params[0]);

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
  bool
    ret = true,
    o_file_created = false,
    bin_file_created = false,
    asm_file_created = false;
  zval *code_zv = NULL, *_this, rv;
  zend_string *jited_code = NULL;
  char
    hash[41] /* sha1 */,
    optimize_lvl = 0,
    o_filename[sizeof("/tmp/") + 40 + sizeof(".o")],
    bin_filename[sizeof("/tmp/") + 40 + sizeof(".bin")],
    asm_filename[sizeof("/tmp/") + 40 + sizeof(".asm")];


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
    zval *retval;
    php_cf *func;
    func = php_cf_ctor((char *)"sha1", 1);
    func->params = code_zv;
    retval = php_call_func(func);

    if (!hash) {
      zend_error(E_WARNING, "Cannot call sha1 hash");
      ret = false;
      goto hash_done;
    }

    if (Z_TYPE_P(retval) == IS_NULL) {
      zend_error(E_WARNING, "sha1 returned null");
      ret = false;
      goto hash_done;
    }

    memcpy(hash, Z_STRVAL_P(retval), 40);
    hash[40] = '\0';

    hash_done:
    zval_dtor(retval);
    php_cf_dtor(func);
    if (!ret) goto ret;
  }



  /* Write the file. */
  {
    sprintf(asm_filename, "/tmp/%s.asm", hash);
    FILE *handle = fopen(asm_filename, "wb");
    if (!handle) {
      zend_error(E_WARNING, "Cannot write to %s", asm_filename);
      ret = false;
      goto ret;
    }

    asm_file_created = true;

    #define _START_FILE \
      "section .text\n\n_start:\n"

    fwrite(_START_FILE, sizeof(char), sizeof(_START_FILE) - 1, handle);
    fwrite(Z_STRVAL_P(code_zv), sizeof(char), Z_STRLEN_P(code_zv), handle);
    fclose(handle);
  }



  /* Compile the file. */
  {
    FILE *handle;
    char *nasm_bin = NULL, *compile_ret = NULL;
    size_t nasm_binl, compile_retl;

    if (!shell_exec("which nasm", &nasm_bin, &nasm_binl)) {
      zend_error(E_WARNING, "Cannot find nasm executable binary");
      ret = false;
      goto compile_ret;
    }

    if (nasm_bin[nasm_binl - 1] == '\n') {
      nasm_bin[nasm_binl - 1] = '\0';
    }

    {
      char cmd[nasm_binl + (sizeof(asm_filename) * 2) + 64];

      sprintf(o_filename, "/tmp/%s.o", hash);
      sprintf(cmd, "%s -felf64 -O%d %s -o %s 2>&1 && echo ok",
        nasm_bin, optimize_lvl, asm_filename, o_filename);

      if (!shell_exec(cmd, &compile_ret, &compile_retl)) {
        zend_error(E_WARNING, "Cannot execute: %s", cmd);
        ret = false;
        goto compile_ret;
      }
    }

    handle = fopen(o_filename, "rb");
    if (!handle) {
      zend_error(E_WARNING, "Compilation failed!");
      zend_error(E_WARNING, "NASM Output: %s", compile_ret);
      ret = false;
      goto compile_ret;
    }
    o_file_created = true;
    fclose(handle);

    compile_ret:
    if (nasm_bin) efree(nasm_bin);
    if (compile_ret) efree(compile_ret);
    if (!ret) goto ret;
  }



  /* Copy the machine code. */
  {
    FILE *handle;
    char *objcp_bin = NULL, *cmd = NULL, *copy_ret = NULL;
    size_t objcp_binl, compile_retl, code_size;

    if (!shell_exec("which objcopy", &objcp_bin, &objcp_binl)) {
      zend_error(E_WARNING, "Cannot find objcopy executable binary");
      ret = false;
      goto copy_ret;
    }

    if (objcp_bin[objcp_binl - 1] == '\n') {
      objcp_bin[objcp_binl - 1] = '\0';
    }

    {
      char cmd[objcp_binl + (sizeof(asm_filename) * 2) + 64];
      sprintf(bin_filename, "/tmp/%s.bin", hash);
      sprintf(cmd, "%s -O binary -j .text %s %s 2>&1 && echo ok",
        objcp_bin, o_filename, bin_filename);

      if (!shell_exec(cmd, &copy_ret, &compile_retl)) {
        zend_error(E_WARNING, "Cannot execute: %s", cmd);
        ret = false;
        goto copy_ret;
      }
    }

    handle = fopen(bin_filename, "rb");
    if (!handle) {
      zend_error(E_WARNING, "Copy failed!");
      zend_error(E_WARNING, "objcopy output: %s", copy_ret);
      ret = false;
      goto copy_ret;
    }
    bin_file_created = true;
    fseek(handle, 0L, SEEK_END);
    code_size = ftell(handle);
    rewind(handle);

    jited_code = zend_string_init("\0", 1, 0);
    jited_code = zend_string_extend(jited_code, code_size, 0);

    {
      char *jited_code_str = ZSTR_VAL(jited_code);
      while (!feof(handle)) {
        jited_code_str += fread(jited_code_str, sizeof(char), 1, handle);
      }
      *jited_code_str = '\0';
    }

    fclose(handle);

    copy_ret:
    if (cmd) efree(cmd);
    if (objcp_bin) efree(objcp_bin);
    if (copy_ret) efree(copy_ret);
    if (!ret) goto ret;
  }


ret:
  if (bin_file_created) {
    remove(bin_filename);
  }
  if (asm_file_created) {
    remove(asm_filename);
  }
  if (o_file_created) {
    remove(o_filename);
  }
  if (!ret) {
    RETURN_FALSE;
  }

  ZVAL_STR(return_value, jited_code);
}

<?php
$fta->end();
LightTeaPHP::addClass($fta);
LightTeaPHP::endFile(__FILE__);
