<?php

require __DIR__."/light-tea/framework/autoload.php";

LightTeaPHP::setExtName("fresh_tea_asm");
LightTeaPHP::setExtDescription("Fresh Tea ASM");
LightTeaPHP::setBuildDir(__DIR__."/src");
LightTeaPHP::setSourceDir(__DIR__."/src.frag");

LightTeaPHP::setPrePattern("/^(.*)\.php\.(s|c|h|cc|cpp|hpp|asm)$/S");
LightTeaPHP::setCopyPattern("/^.*\.(s|c|h|cc|cpp|hpp|asm)$/S");

LightTeaPHP::addIncludePath("include");

LightTeaPHP::init();
LightTeaPHP::build();
