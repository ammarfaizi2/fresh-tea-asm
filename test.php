<?php

$code = <<<CODE
  xor rax, rax
  mov rax, "Hello"a
  ret
CODE;


$compiler = new \FreshTeaASM\JIT\x86_64\Compiler($code);
$code = $compiler->compile();
var_dump($code);
