<?php
require __DIR__."/../../src/autoload.php";

$code = <<<CODE
	mov rax, [rdi]
	mov rax, [rax]
	add rax, 16
	mov rsi, 2
	sub [rax], rsi
	add rax, 8+6
	lea rdx, [rax + 2]
	xor rdi, rdi
.A1:
	mov rsi, [rdx + (rdi * 8)]
	mov [rax + (rdi * 8)], rsi
	inc rdi
	cmp rdi, 5
	jl .A1
	ret
CODE;

$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();
$a = "You doesn't have any objection to it, right?";
$exe = new PhpNasm\Executor($code);
unset($code);
$exe->execute($a);
var_dump($a); // You don't have any objection to it, right?
