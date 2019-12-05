<?php

require __DIR__."/../../src/autoload.php";

$string = "Hello World!\n";
$len = strlen($string);

$code = <<<CODE
    push rbp
    mov rbp, rsp
    sub rsp, {$len}
    mov rax, "Hello Wo"
    mov [rbp - {$len}], rax
    mov rax, "rld!"
    mov [rbp - {$len} + 8], rax
    mov byte [rbp - {$len} + 8 + 4], 10
    xor r9, r9

print_hello_world:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rbp - {$len}]
    mov rdx, {$len}
    syscall
    inc r9
    cmp r9, 10
    jl print_hello_world

    mov rsp, rbp
    pop rbp
    ret
CODE;

$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();

$exe = new PhpNasm\Executor($code);
$exe->execute();
