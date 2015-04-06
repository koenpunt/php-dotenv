#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_streams.h"

#if HAVE_DOTENV

#include <stdbool.h>
#include "TSRM.h"
#include "php_dotenv.h"

static zend_function_entry dotenv_functions[] = {
    PHP_FE(dotenv_load, NULL)
    {NULL, NULL, 0, 0, 0}
};

zend_module_entry dotenv_module_entry = {
  STANDARD_MODULE_HEADER,
  PHP_DOTENV_EXTNAME,
  dotenv_functions,
  PHP_MINIT(dotenv),
  PHP_MSHUTDOWN(dotenv),
  NULL, // PHP_RINIT(dotenv)
  NULL, // PHP_RSHUTDOWN(dotenv)
  NULL, // PHP_MINFO(dotenv)
  PHP_DOTENV_VERSION,
  STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DOTENV
ZEND_GET_MODULE(dotenv)
#endif

ZEND_DECLARE_MODULE_GLOBALS(dotenv);

PHP_FUNCTION(dotenv_load)
{
  // Arguments
  char *filename = ".env";
  int filename_len;
  zend_bool replace = 0;
  char resolved_path[MAXPATHLEN];
  HashTable env_files;
  HashTable *vars;

  // recovers a filepath as string (p) and an boolean (b) from arguments
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|pb", &filename, &filename_len, &replace) == FAILURE) {
    RETURN_NULL();
  }

  // expand filepath
  if(VCWD_REALPATH(filename, resolved_path)){
    if(php_check_open_basedir(resolved_path TSRMLS_CC)){ // is this necessary?
      RETURN_FALSE;
    }
  }

  env_files = DOTENV_G(env_files);

  // Allocate hash table for current variables
  ALLOC_HASHTABLE(vars);
  zend_hash_init(vars, 8, NULL, NULL, 0); // 0 should be 1 to live on after request

  if(zend_hash_find(&env_files, resolved_path, sizeof(resolved_path), (void **) &vars) == FAILURE){

    // .env file is not found in hashtable so we parse the file
    php_stream *stream;

    stream = php_stream_open_wrapper(resolved_path, "rb", ENFORCE_SAFE_MODE | REPORT_ERRORS, NULL);
    if(!stream) {
      RETURN_FALSE;
    }

    dotenv_parse_stream(stream, vars);
    zend_hash_update(&env_files, resolved_path, sizeof(resolved_path), vars, sizeof(vars), NULL);
  }

  dotenv_inject_vars(vars, replace);

  zend_hash_destroy(vars);
  FREE_HASHTABLE(vars);

  RETURN_TRUE;
}

// parse php_stream into HashTable
static void dotenv_parse_stream(php_stream *stream, HashTable *vars)
{
  char name[256];
  char value[256];

  while(!php_stream_eof(stream TSRMLS_CC)) {
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
}

static void dotenv_inject_vars(HashTable *vars, bool replace)
{
  char *data;
  char *key;
  uint keylen;

  ulong keyindex;

  HashPosition position;

  // Iterate through hash table
  for(zend_hash_internal_pointer_reset_ex(vars, &position);
       zend_hash_get_current_data_ex(vars, (void **) &data, &position) == SUCCESS;
       zend_hash_move_forward_ex(vars, &position)
  ){
    zend_hash_get_current_key_ex(vars, &key, &keylen, &keyindex, 0, &position);
      // set environment variable
      setenv(key, data, replace);
  }
}


PHP_MINIT_FUNCTION(dotenv)
{
  // Initialize persistent hash table
  zend_hash_init(&DOTENV_G(env_files), 8, NULL, NULL, 1);

  return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(dotenv)
{
#ifdef ZTS
  ts_free_id(dotenv_globals_id);
#else

#endif
  return SUCCESS;
}

#endif
