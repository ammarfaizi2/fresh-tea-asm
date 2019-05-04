
# PHPNasm
PHPNasm is an extension for PHP that makes the PHP able to execute NASM and raw machine code.

# Installation
```shell
sudo apt install nasm -y
git clone https://github.com/ammarfaizi2/PhpNasm
cd PhpNasm/ext
phpize
./configure
make
cd ..
php -d extension=$(pwd)/ext/modules/phpnasm.so examples/x64/hello_world.php
```

# License
This software is licensed under the MIT License.