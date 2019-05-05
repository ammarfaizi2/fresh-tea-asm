<?php

require __DIR__."/../../src/autoload.php";

$code = <<<CODE
	mov r10,0
me:
	mov rax, "Hello Wo"
	push rax
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 8
	syscall
	pop rax

	mov rax, "rld"
	or rax, 0x0a000000
	push rax
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 5
	syscall
	pop rax

	inc r10
	cmp r10, 10
	jl me
	ret
CODE;

$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();

$exe = new PhpNasm\Executor($code);
$exe->execute();
