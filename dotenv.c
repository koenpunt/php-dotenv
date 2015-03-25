#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_dotenv.h"

static zend_function_entry dotenv_functions[] = {
    PHP_FE(dotenv_load, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry dotenv_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_DOTENV_EXTNAME,
    dotenv_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_DOTENV_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DOTENV
ZEND_GET_MODULE(dotenv)
#endif

PHP_FUNCTION(dotenv_load)
{
  // Arguments
  char *filename = ".env";
  int filename_len;
  zend_bool replace = 0;

  // Stream
  php_stream *stream;

  // recovers a string (s) and an boolean (b) from arguments
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sb", &filename, &filename_len, &replace) == FAILURE) {
    RETURN_NULL();
  }

  stream = php_stream_open_wrapper(filename, "rb", ENFORCE_SAFE_MODE | REPORT_ERRORS, NULL);
  if (!stream) {
    RETURN_FALSE;
  }

  char name[256];
  char value[256];

  while(!php_stream_eof(stream)) {
    char buf[512];

    if (php_stream_gets(stream, buf, sizeof(buf))) {
      if(sscanf(buf, "%[^=]=%s", name, value)){
        setenv(name, value, replace);
      }
    }
  }
  php_stream_close(stream);

  RETURN_TRUE;
}
