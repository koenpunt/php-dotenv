#ifndef PHP_DOTENV_H
#define PHP_DOTENV_H 1
#define PHP_DOTENV_VERSION "0.1"
#define PHP_DOTENV_EXTNAME "dotenv"

#include "php.h"

#ifdef ZTS
#include "TSRM.h"
#define DOTENV_G(v) TSRMG(dotenv_globals_id, zend_dotenv_globals *, v)
#else
#define DOTENV_G(v) (dotenv_globals.v)
#endif

ZEND_BEGIN_MODULE_GLOBALS(dotenv)
    HashTable        env_files;
ZEND_END_MODULE_GLOBALS(dotenv)

PHP_FUNCTION(dotenv_load);


static void dotenv_inject_vars(HashTable *vars, bool replace);
static void dotenv_parse_stream(php_stream *stream, HashTable *vars);

extern zend_module_entry dotenv_module_entry;
#define phpext_dotenv_ptr &dotenv_module_entry

#endif
