dnl config.m4
PHP_ARG_ENABLE(vgd, for vgd support,
[  --enable-vgd            Include vgd support])

if test "$PHP_VGD" != "no"; then
  PHP_NEW_EXTENSION(vgd, php_vgd.c, $ext_shared)
  PHP_SUBST(VGD_SHARED_LIBADD)
fi
