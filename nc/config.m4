PHP_ARG_ENABLE(nc, whether to enable nc framework,
[  --enable-nc           enable nc framework])

if test "$PHP_NC" != "no"; then
  PHP_NEW_EXTENSION(nc, nc.c kernel.c loader.c sql.c, $ext_shared)
fi
