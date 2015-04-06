PHP_ARG_ENABLE(dotenv, whether to .env support,
[  --enable-dotenv         Enable .env support])
if test "$PHP_DOTENV" = "yes"; then
  AC_DEFINE(HAVE_DOTENV, 1, [Whether you have .env])
  PHP_NEW_EXTENSION(dotenv, dotenv.c, $ext_shared)
  PHP_ADD_MAKEFILE_FRAGMENT
fi
