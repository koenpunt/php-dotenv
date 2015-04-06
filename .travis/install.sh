phpize
./configure
make all install
EXTENSIONDIR=`php -r 'echo ini_get("extension_dir");'`
echo "extension=${EXTENSIONDIR}/dotenv.so" >> ~/.phpenv/versions/$(phpenv version-name)/etc/php.ini
echo "extension=${EXTENSIONDIR}/dotenv.so" > /tmp/temp-php-config.ini
cat /tmp/temp-php-config.ini