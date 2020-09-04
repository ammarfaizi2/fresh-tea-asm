<?php

require __DIR__."/src/php/PhpNasm/x86_64/JITCompiler.php";

use PhpNasm\x86_64\ASM_JIT;
use PhpNasm\x86_64\JITCompiler;

$code = <<<CODE
  mov rsi, [rdi + 16]  ; Param 1
  mov rdx, [rdi + 8]   ; Param 2

  mov rax, [rsi + 16]
  mov rcx, [rdx + 16]
  cmp rax, rcx
  je _do_compare
  mov al, 1
  jmp _ret

_do_compare:
  add rsi, 24
  add rdx, 24

_start_loop:
  test rcx, rcx
  jz _ret
  cmp rcx, 32
  jge _jge_32
  cmp rcx, 16
  jge _jge_16
  cmp rcx, 8
  jge _jge_8
  cmp rcx, 4
  jge _jge_4
  cmp rcx, 2
  jge _jge_2
  jmp _otge

_jge_32:
  vmovdqu ymm0, [rsi]
  vmovdqu ymm1, [rdx]
  vxorpd ymm0, ymm0, ymm1
  vptest ymm0, ymm0
  jnz _ret
  sub rcx, 32
  add rsi, 32
  add rdx, 32
  jmp _start_loop

_jge_16:
  vmovdqu xmm0, [rsi]
  vmovdqu xmm1, [rdx]
  pxor xmm0, xmm1
  ptest xmm0, xmm0
  jnz _ret
  sub rcx, 16
  add rsi, 16
  add rdx, 16
  jmp _start_loop

_jge_8:
  mov rax, [rsi]
  mov r10, [rdx]
  cmp rax, r10
  jne _ret
  sub rcx, 8
  add rsi, 8
  add rdx, 8
  jmp _start_loop

_jge_4:
  mov eax, [rsi]
  cmp eax, dword [rdx]
  jne _ret
  sub rcx, 4
  add rsi, 4
  add rdx, 4
  jmp _start_loop

_jge_2:
  mov ax, [rsi]
  cmp ax, word [rdx]
  sub rcx, 2
  add rsi, 2
  add rdx, 2
  jmp _start_loop

_otge:
  mov al, [rsi]
  cmp al, byte [rdx]
  jne _ret
  dec rcx

_ret:
  mov al, 1
  test rcx, rcx
  jz _identic
  jmp _real_ret
_identic:
  xor al, al
_real_ret:
  mov rdi, [rdi]
  add rdi, 24
  mov byte [rdi], al
  ret
CODE;

$compiler = new JITCompiler($code);
$machineCode = $compiler->compile();
$jit = new ASM_JIT($machineCode);

$str1 = "a".str_repeat("a", 1024 * 1024);
$str2 = "b".str_repeat("a", 1024 * 1024);

$start = microtime(true);
$retVal = "x";
$jit->execute($str1, $str2, $retVal);
if (!ord($retVal)) {
  echo "OK!\n";
}
echo "JIT: ".(microtime(true) - $start)."\n";


$start = microtime(true);
if ($str1 === $str2) {
  echo "OK!\n";
}
echo "Non-JIT: ".(microtime(true) - $start)."\n";

