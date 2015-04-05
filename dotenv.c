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

  // recovers a string (s) and an boolean (b) from arguments
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|pb", &filename, &filename_len, &replace) == FAILURE) {
    RETURN_NULL();
  }

  HashTable *vars;

  char resolved_path[MAXPATHLEN];

  if(VCWD_REALPATH(filename, resolved_path)){
    if(php_check_open_basedir(resolved_path)){ // is this necessary?
      RETURN_FALSE;
    }
  }

  // TODO: Test if there's configuration in memory for filename

  // Stream
  php_stream *stream;

  stream = php_stream_open_wrapper(filename, "rb", ENFORCE_SAFE_MODE | REPORT_ERRORS, NULL);
  if(!stream) {
    RETURN_FALSE;
  }

  // Allocate new hash table
  ALLOC_HASHTABLE(vars);
  zend_hash_init(vars, 8, NULL, NULL, 0); // 0 should be 1 to live on after request

  char name[256];
  char value[256];

  while(!php_stream_eof(stream)) {
    char buf[512];

    // Parse stream line by line
    if(php_stream_gets(stream, buf, sizeof(buf))) {
      // phpdotenv does not judge, environment variables can contain anything
      // as long as it's not a =
      if(sscanf(buf, "%[^=]=%[^\n]", name, value)){
        // Store variable in hash table
        zend_hash_update(vars, name, sizeof(name), value, sizeof(value), NULL);
      }
    }
  }
  php_stream_close(stream);

  dotenv_inject_vars(vars, replace);

  RETURN_TRUE;
}

void dotenv_inject_vars(HashTable *vars, bool replace)
{
  zval **data;
  char *key, *value;
  uint keylen, valuelen;

  ulong keyindex;

  HashPosition position;

  // Loop over hash table
  for(zend_hash_internal_pointer_reset_ex(vars, &position);
       zend_hash_get_current_data_ex(vars, (void **) &data, &position) == SUCCESS;
       zend_hash_move_forward_ex(vars, &position)
  ){
    if(zend_hash_get_current_key_ex(vars, &key, &keylen,
      &keyindex, 0, &position) == HASH_KEY_IS_STRING){

      // valuelen = Z_STRLEN_PP(data);
      // value = estrndup(Z_STRVAL_PP(data), valuelen);

      // set environment variables
      // value = Z_STRVAL_PP(data);
      setenv(key, data, replace);
    };
  }
}