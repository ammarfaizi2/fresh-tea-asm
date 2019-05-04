<?php
require __DIR__."/../../src/autoload.php";
$code = <<<CODE
	mov r9, rdi
	mov rax, 1
	mov rdi, 1
	mov rsi, r9
	mov rdx, 13
	syscall
	ret
CODE;
$code = (new PhpNasm\Arch\x64($code))->compile();
$hello = "Hello World!\n";
$exe = new PhpNasm\Executor($code);
$exe->execute($hello);
