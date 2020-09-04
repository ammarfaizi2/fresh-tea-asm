<?php

$code = <<<CODE
  mov rax, "Hello"
  push rax
  mov rax, 1
  mov rdi, 1
  lea rsi, [rsp]
  syscall
  ret
CODE;


$compiler = new \FreshTeaASM\JIT\x86_64\Compiler($code);
$compiler->setOptimization(3);
$code = $compiler->compile();
var_dump($code);
