#ifndef PHP_DOTENV_H
#define PHP_DOTENV_H 1
#define PHP_DOTENV_VERSION "0.1"
#define PHP_DOTENV_EXTNAME "dotenv"

#include "Zend/zend_hash.h"

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(dotenv);
PHP_MSHUTDOWN_FUNCTION(dotenv);

PHP_FUNCTION(dotenv_load);

ZEND_BEGIN_MODULE_GLOBALS(dotenv);
    HashTable        env_files;
ZEND_END_MODULE_GLOBALS(dotenv);

static void dotenv_inject_vars(HashTable *vars, bool replace);
static void dotenv_parse_stream(php_stream *stream, HashTable *vars);

extern zend_module_entry dotenv_module_entry;
#define phpext_dotenv_ptr &dotenv_module_entry

#ifdef ZTS
#define DOTENV_G(v) TSRMG(dotenv_globals_id, zend_dotenv_globals *, v)
#else
#define DOTENV_G(v) (dotenv_globals.v)
#endif

#endif
