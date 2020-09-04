<?php
require __DIR__."/../../src/autoload.php";
$code = <<<CODE
	mov rax, [rdi]
  	mov rax, [rax]
  	add rax, 16
  	sub qword [rax], 7
  	ret
CODE;
$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();
$a = "PHP Indonesia for Student abcdef";
$exe = new PhpNasm\Executor($code);
unset($code);
$exe->execute($a);
var_dump($a); // PHP Indonesia for Student
