<?php

namespace PhpNasm;

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
	 * @return void
	 */
	public function compile(): void
	{
		
	}

	/**
	 * @param string $opcode
	 * @return string
	 */
	private function buildMnemonic(string $opcode): string
	{
		$opcode = strtolower($opcode);
		switch ($opcode) {
			case "rax":
				
				break;
			
			default:
				# code...
				break;
		}
	}
}
