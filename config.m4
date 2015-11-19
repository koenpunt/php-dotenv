PHP_ARG_ENABLE(dotenv, whether to enable dotenv support,
[  --enable-dotenv           Enable dotenv support])
if test "$PHP_DOTENV" != "no"; then
  AC_DEFINE(HAVE_DOTENV, 1, [Whether you have dotenv])

  PHP_SUBST(DOTENV_SHARED_LIBADD)
  PHP_NEW_EXTENSION(dotenv, dotenv.c, $ext_shared)
  PHP_ADD_MAKEFILE_FRAGMENT
fi
