<?php

if (!defined("__PHPNASM_INIT")):
	define("__PHPNASM_INIT", true);

	/**
	 * @param string $class
	 * @return void
	 */
	function phpNasmInternalAutoloader(string $class): void
	{
		if (file_exists($file = __DIR__."/".str_replace("\\", "/", $class).".php")) {
			require $file;
		}
	}

	spl_autoload_register("phpNasmInternalAutoloader");

endif;
