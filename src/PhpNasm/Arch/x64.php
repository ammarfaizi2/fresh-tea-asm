<?php

namespace PhpNasm\Arch;

use PhpNasm\Exceptions\PhpNasmException;

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
	 * @var string
	 */
	private $tmpDir = "/tmp";

	/**
	 * @var int
	 */
	private $optimization = 0;

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
	 * @param string $tmpDir
	 * @return void
	 */
	public function setTmpDir(string $tmpDir): void
	{
		if (is_dir($tmpDir)) {
			throw PhpNasmException("tmp_dir is not a directory: {$tmpDir}");
		}

		if (!is_writable($tmpDir)) {
			throw PhpNasmException("tmp_dir is not writeable: {$tmpDir}");
		}

		$this->tmpDir = $tmpDir;
	}

	/**
	 * @throws \PhpNasm\PhpNasmException
	 * @param int $o
	 * @return void
	 */
	public function setOptimization(int $o = 0): void
	{
		$this->optimization = $o;
	}

	/**
	 * @throws \PhpNasm\PhpNasmException
	 * @return string
	 */
	public function compile(): string
	{
		$successFlag = time().rand().rand();
		$tmpFile = $this->tmpDir."/phpnasm_".time()."_".rand().".asm";

		$nasm = escapeshellarg(trim(shell_exec("which nasm")));
		$objcopy = escapeshellarg(trim(shell_exec("which objcopy")));

		if (!$nasm) {
			throw new PhpNasmException("nasm binary not found");
		}

		if (!$objcopy) {
			throw new PhpNasmException("objcopy binary not found");
		}

		file_put_contents($tmpFile, "section .text\n\n_start:\n{$this->text}");

		$nasmCompile = shell_exec($nasm." -f elf64 -O{$this->optimization} ".escapeshellarg($tmpFile)." -o ".escapeshellarg($tmpFile.".o")." && echo ".$successFlag);
		if (strpos($nasmCompile, $successFlag) === false) {
			throw new PhpNasmException("Compile error");
		}

		$copier = shell_exec($objcopy." -O binary -j .text ".escapeshellarg($tmpFile.".o")." ".escapeshellarg($tmpFile.".bin")." && echo ".$successFlag);
		if (strpos($copier, $successFlag) === false) {
			throw new PhpNasmException("Link error");
		}

		unset($nasmCompile, $copier, $successFlag, $nasm, $objcopy);

		$compiled = file_get_contents($tmpFile.".bin");
		unlink($tmpFile);
		unlink($tmpFile.".o");
		unlink($tmpFile.".bin");
		return $compiled;
	}
}
