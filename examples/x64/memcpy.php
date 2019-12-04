<?php
require __DIR__."/../../src/autoload.php";
$code = <<<CODE
    mov r9, [rdi]
    mov r9, [r9]
    mov rax, [rdi + 8]
    mov rax, [rax]
    mov rdi, [rdi + 16]
    mov rdi, [rdi]
.L1:
    dec r9
    mov sil, byte [rax + r9 + 24]
    mov byte [rdi + r9 + 24], sil
    cmp r9, 0
    jge .L1
    ret
CODE;

$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();

$a = "....................";
$b = "Hello World!";
$c = 12;

$exe = new PhpNasm\Executor($code);
$exe->execute($a, $b, $c);

var_dump($a, $b);
printf("c = %d\n", $c);
