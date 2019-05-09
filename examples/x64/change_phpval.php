<?php
require __DIR__."/../../src/autoload.php";
$code = <<<CODE
	mov eax, 'rld!'
	mov rdi, [rdi]
	mov [rdi+8], eax
	ret
CODE;

$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();

// Fix me to "Hello World!"
$a = "Hello Woooo?";

$exe = new PhpNasm\Executor($code);
$exe->execute($a);

var_dump($a); // Must be "Hello World!"
