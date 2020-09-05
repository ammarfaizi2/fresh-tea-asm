<?php
$code = <<<ASM
  mov r9, rdi                 ; Copy parameters address to r9 register.
  xor r8, r8                  ; Zero r8 register.
.loop_1:
  mov r10, [r9 + r8 * 8]      ; Take the n parameter.
  lea rsi, [r10 + 24]         ; Take the string address.
  mov rdx, [r10 + 16]         ; Take the string length.
  mov byte [rsi + rdx], 0xa   ; Put linefeed to the end of string.
  inc rdx                     ; Increment the rdx register.
  mov rax, 1                  ; Syscall write.
  mov rdi, 1                  ; Write to stdout.
  syscall                     ; Do the syscall.
  mov byte [rsi + rdx], 0x0   ; Plug back the nullchar.
  inc r8                      ; Increment r8 register.
  cmp r8, 3                   ; Compare r8 register.
  jl .loop_1                  ; Jump to .loop_1 if r8 register is less than 3.
  ret                         ; Return.
ASM;
$compiler = new \FreshTeaASM\JIT\x86_64\Compiler($code);
$compiler->setOptimization(3);
if ($code = $compiler->compile()) {
  $exe = new \FreshTeaASM\JIT\x86_64\Executor($code);
  $param1 = "Parameter 1";
  $param2 = "Parameter 2";
  $param3 = "Parameter 3";
  $exe->execute($param1, $param2, $param3);
} else {
  echo "Compilation failed!\n";
}
