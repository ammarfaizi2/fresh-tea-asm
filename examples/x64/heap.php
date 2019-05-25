<?php
require __DIR__."/../../src/autoload.php";
$code = <<<CODE
	mov rax, 12
	xor rdi, rdi
	syscall
	mov rsi, rax
	mov rdi, rax
	add rdi, 5
	mov rax, 12
	syscall

	mov rax, "aaaaaaaa"
	mov [rsi], rax ; 0 - 7
	mov [rsi+8], rax ; 8 - 15
	mov [rsi+16], rax ; 16 - 23
	mov [rsi+24], eax ; 24 - 29
	mov al, 0
	mov [rsi+30], al

	mov rax, 1
	mov rdi, 1
	mov rdx, 30
	syscall

	ret
CODE;

$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();

$exe = new PhpNasm\Executor($code);
$exe->execute();
echo "\n";
