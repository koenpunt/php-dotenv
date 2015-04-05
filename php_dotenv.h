#ifndef PHP_DOTENV_H
#define PHP_DOTENV_H 1
#define PHP_DOTENV_VERSION "0.1"
#define PHP_DOTENV_EXTNAME "dotenv"

PHP_FUNCTION(dotenv_load);

void dotenv_inject_vars(HashTable *vars, bool replace);
HashTable* dotenv_parse_stream(php_stream *stream);

extern zend_module_entry dotenv_module_entry;
#define phpext_dotenv_ptr &dotenv_module_entry

#endif
