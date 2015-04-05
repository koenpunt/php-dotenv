--TEST--
dotenv_load() function - basic test for dotenv_load()
--FILE--
<?php

$env_file = __DIR__ . '/.env';

copy(__DIR__ . '/fixtures/example.env', $env_file);

var_dump(getenv('DOTENV_LOAD_VAR'));

dotenv_load(__DIR__ . '/fixtures/example.env');

var_dump(getenv('DOTENV_LOAD_VAR'));

unlink($env_file);

?>
--EXPECT--
bool(false)
string(18) "example.env loaded"
