export CFLAGS="-Wall -Wextra -Wdeclaration-after-statement -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -ggdb3"
phpize
./configure --enable-dotenv
make all install
echo "extension=dotenv.so" >> ~/.phpenv/versions/$(phpenv version-name)/etc/php.ini
echo "extension=dotenv.so" > /tmp/temp-php-config.ini
cat /tmp/temp-php-config.ini
