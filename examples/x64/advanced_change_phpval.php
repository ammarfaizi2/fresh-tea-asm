<?php
require __DIR__."/../../src/autoload.php";

$code = <<<CODE
	mov rax, [rdi]
  	mov rax, [rax]

  	;; Write your code here...

CODE;

$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();
$a = "You doesn't have any objection to it, right?";
$exe = new PhpNasm\Executor($code);
unset($code);
$exe->execute($a);
var_dump($a); // You don't have any objection to it, right?
