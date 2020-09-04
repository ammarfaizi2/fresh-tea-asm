<?php

$code = <<<CODE
  xor rax, rax

  ret
CODE;


$compiler = new FreshTeaASM\JIT\x86_64\Compiler($code);
$compiler->compile();
