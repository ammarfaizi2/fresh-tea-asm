dnl config.m4
PHP_ARG_ENABLE(phpnasm, for phpnasm support,
[  --enable-phpnasm            Include phpnasm support])

if test "$PHP_PHPNASM" != "no"; then
  PHP_NEW_EXTENSION(phpnasm, php_phpnasm.c, $ext_shared)
  PHP_SUBST(phpnasm_SHARED_LIBADD)
fi
