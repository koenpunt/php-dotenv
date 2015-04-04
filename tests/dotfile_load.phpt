--TEST--
dotenv_load() function - basic test for dotenv_load()
--FILE--
<?php

var_dump(getenv('DOTENV_LOAD_VAR'));

dotenv_load(__DIR__ . '/fixtures/example.env');

var_dump(getenv('DOTENV_LOAD_VAR'));

?>
--EXPECT--
bool(false)
string(18) "example.env loaded"
