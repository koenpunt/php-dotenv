--TEST--
dotenv_load() function - basic test for dotenv_load()
--FILE--
<?php

$env_file = __DIR__ . '/.env';

copy(__DIR__ . '/fixtures/example.env', $env_file);

var_dump(getenv('DOTENV_LOAD_VAR'));

dotenv_load($env_file);

var_dump(getenv('DOTENV_LOAD_VAR'));

file_put_contents($env_file, 'DOTENV_LOAD_VAR=somenewvar');

dotenv_load($env_file);

var_dump(getenv('DOTENV_LOAD_VAR'));

unlink($env_file);

?>
--EXPECT--
bool(false)
string(18) "example.env loaded"
string(18) "example.env loaded"
