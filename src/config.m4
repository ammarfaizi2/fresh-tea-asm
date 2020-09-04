

dnl config.m4

PHP_ARG_ENABLE(fresh_tea_asm, for fresh_tea_asm support, [  --enable-fresh_tea_asm            Include fresh_tea_asm support])

PHP_REQUIRE_CXX()

PHP_NEW_EXTENSION(fresh_tea_asm, entry_point.cpp helpers.cpp classes/FreshTeaASM/JIT/x86_64/Compiler.cpp classes/FreshTeaASM/JIT/x86_64/Executor.cpp, $ext_shared)
PHP_ADD_INCLUDE(include)
PHP_SUBST(FRESH_TEA_ASM_SHARED_LIBADD)
