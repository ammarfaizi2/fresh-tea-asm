<?php

/**
 * Write file code.
 */
require __DIR__."/../../src/autoload.php";

$flag = 64 | 0x0002; // O_CREAT | O_RDWR

$code = <<<CODE
    mov r9, rdi
    mov rdi, [r9 + 8]
    mov rdi, [rdi]
    add rdi, 24
    mov rax, 2
    mov rsi, {$flag}
    mov rdx, 511 ; 0777
    syscall

    mov rdi, rax
    mov rax, 1
    mov rsi, [r9]
    mov rsi, [rsi]
    add rsi, 24
    mov rdx, 12
    syscall
    ret
CODE;
$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();
$exe = new PhpNasm\Executor($code);
unset($code);

$file = "test.txt.tmp";
$content = "Hello World!";
$exe->execute($file, $content);


$content = str_repeat("\0", strlen($content));
var_dump($content);


$flag = 0x0002; // O_RDWR
/**
 * Read file code.
 */
$code = <<<CODE
    mov r9, rdi
    mov rdi, [r9 + 8]
    mov rdi, [rdi]
    add rdi, 24
    mov rax, 2
    mov rsi, {$flag}
    syscall

    mov rdi, rax
    mov rax, 0
    mov rsi, [r9]
    mov rsi, [rsi]
    add rsi, 24
    mov rdx, 12
    syscall
    ret
CODE;
$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();
$exe = new PhpNasm\Executor($code);
unset($code);

$exe->execute($file, $content);
var_dump($content);
