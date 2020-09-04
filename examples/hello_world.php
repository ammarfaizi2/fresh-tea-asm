<?php
$f = fn($str) => "0x".bin2hex(strrev($str));
$h = [$f("Hello Wo"), $f("rld!\n")];
$code = <<<ASM
  push rbp
  mov rbp, rsp
  sub rsp, 24
  mov rax, {$h[0]}
  mov qword [rbp - 16], rax
  mov rax, {$h[1]}
  mov qword [rbp - 8], rax
  mov eax, 1
  mov edi, 1
  lea rsi, [rbp - 16]
  mov edx, 13
  syscall
  mov rsp, rbp
  pop rbp
  ret
ASM;
$compiler = new \FreshTeaASM\JIT\x86_64\Compiler($code);
$compiler->setOptimization(3);
if ($code = $compiler->compile()) {
  $exe = new \FreshTeaASM\JIT\x86_64\Executor($code);
  $exe->execute();
} else {
  echo "Compilation failed!\n";
}
