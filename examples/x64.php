<?php

require __DIR__."/../src/autoload.php";

$code = <<<CODE


	mov rax, "Hello Wo"
	push rax
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 8
	syscall
	pop rax

	mov rax, "rld!"
	push rax
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 4
	syscall
	pop rax

	mov rax, 10
	push rax
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 1
	syscall
	pop rax

	mov rax, 60
	xor rdi, rdi
	syscall


CODE;

$code = (new PhpNasm\Arch\x64($code))->compile();
$exe = new PhpNasm\Executor($code);
$exe->execute();