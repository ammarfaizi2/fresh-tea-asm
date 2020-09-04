<?php

require __DIR__."/light-tea/framework/autoload.php";

LightTeaPHP::setExtName("helloworld");
LightTeaPHP::setExtDescription("Hello World Program");
LightTeaPHP::setBuildDir(__DIR__."/build");
LightTeaPHP::setSourceDir(__DIR__."/src");

LightTeaPHP::setPrePattern("/^(.*)\.php\.(s|c|h|cc|cpp|hpp|asm)$/S");
LightTeaPHP::setCopyPattern("/^.*\.(s|c|h|cc|cpp|hpp|asm)$/S");

LightTeaPHP::addIncludePath("include");

LightTeaPHP::init();
LightTeaPHP::build();
