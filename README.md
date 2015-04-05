# PHP Dotenv

C Extension for PHP to load .env files into the environment.

## Introduction

Traditionally you had to load and parse a `.env` file on every request.
This extension removes the overhead of accessing the filesystem over and over again by caching it in memory.

## Installation

No dependencies are needed, other than the php-dev packages.

Default extension building process applies:

```sh
git clone https://github.com/koenpunt/php-dotenv-extension.git
cd php-dotenv-extension
phpize
./configure --enable-dotenv
make
make install
```

## Usage

The extension currently provides one method, `dotenv_load` which has 2 optional arguments.
First the file to load. This defaults to `.env` in the current directory.
The second is whether or not to replace existing environment variables.

## Known issues

- Currently values enclosed in quotes will include the literal quotes in the value.  
  So `MY_VAR="Some quoted value"` will result in `getenv('MY_VAR') == '"Some quoted value"'` 

## Contributing

1. [Fork it](https://github.com/koenpunt/php-dotenv-extension/fork)
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
