--TEST--
dotenv_load() function - basic test for dotenv_load()
--FILE--
<?php

$env_file = __DIR__ . '/.env';

chdir(__DIR__);

copy(__DIR__ . '/fixtures/example.env', $env_file);

var_dump(getenv('DOTENV_LOAD_VAR'));

dotenv_load();

var_dump(getenv('DOTENV_LOAD_VAR'));

unlink($env_file);

?>
--EXPECT--
bool(false)
string(18) "example.env loaded"
