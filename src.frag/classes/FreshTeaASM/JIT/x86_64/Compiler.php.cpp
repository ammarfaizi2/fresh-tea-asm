
#include <cstdio>

<?php
LightTeaPHP::beginFile(__FILE__);
$fta = new PHPClass("FreshTeaASM\\JIT_x86_64", __FILE__);
$fta->start();
?>

/**
 * Constructor.
 *
 * @param string $code
 */
static <?= $fta->method("__construct"); ?> {
  char *code;
  size_t code_len;

  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STRING(code, code_len)
  ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

  zend_update_property_string(
    <?= $fta->ce; ?>, _this, ZEND_STRL("code"), code, code_len TSRMLS_CC);
}

/**
 * @return bool
 */
static <?= $fta->method("compile"); ?> {
  FILE *handle;
  zval *code_zv, rv;
  char filename[512];

  code_zv = zend_read_property(<?= $fta->ce; ?>, _this, ZEND_STRL("code"), 1, &rv TSRMLS_CC);

  Z_STRVAL_P(code_zv);

  handle = fopen("/tmp/")
}

<?php
$fta->end();
LightTeaPHP::addClass($fta);
LightTeaPHP::endFile(__FILE__);
