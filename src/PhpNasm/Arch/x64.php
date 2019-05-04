<?php

namespace PhpNasm\Arch;

use PhpNasm\PhpNasmException;

/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com>
 * @license MIT
 * @version 0.0.1
 * @package \PhpNasm
 */
final class x64
{
	/**
	 * @var string
	 */
	private $text;

	/**
	 * @param string $text
	 *
	 * Constructor.
	 */
	public function __construct(string $text)
	{
		$this->text = $text;
	}

	/**
	 * @throws \PhpNasm\PhpNasmException
	 * @return string
	 */
	public function compile(): string
	{
		$tmpFile = "/tmp/phpnasm_".time()."_".rand().".asm";
		$nasm = escapeshellarg(trim(shell_exec("which nasm")));
		$objcopy = escapeshellarg(trim(shell_exec("which objcopy")));
		if (!$nasm) {
			throw new PhpNasmException("nasm binary not found");
		}
		file_put_contents($tmpFile, "section .text\n\n_start:\n{$this->text}");
		shell_exec($nasm." -f elf64 -O0 ".escapeshellarg($tmpFile)." -o ".escapeshellarg($tmpFile.".o"));
		shell_exec($objcopy." -O binary -j .text ".escapeshellarg($tmpFile.".o")." ".escapeshellarg($tmpFile.".bin"));
		$compiled = file_get_contents($tmpFile.".bin");
		unlink($tmpFile);
		unlink($tmpFile.".o");
		unlink($tmpFile.".bin");
		return $compiled;
	}
}
