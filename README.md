
# Fresh Tea ASM
Machine code executor and JIT compiler for PHP.

# Installation
```sh
sudo apt-get install nasm php7.4-dev -y;
git clone https://github.com/ammarfaizi2/fresh-tea-asm;
cd fresh-tea-asm/src;
phpize;
./configure;
make;
cd ..;
php -d extension=$(pwd)/src/modules/fresh_tea_asm.so examples/hello_world.php;
```

# License
This software is licensed under the MIT license.
