<?php
require __DIR__."/../../src/autoload.php";
$code = <<<CODE

	; Write your code here...

	ret
CODE;

$x64 = new PhpNasm\Arch\x64($code);
$code = $x64->compile();

$a = "....................";
$b = "Hello World!";
$c = 8;

$exe = new PhpNasm\Executor($code);
$exe->execute($a, $b, $c);

var_dump($a, $b);
printf("c = %d\n", $c);
