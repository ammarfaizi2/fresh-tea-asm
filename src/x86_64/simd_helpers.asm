
section .text

global __php_fast_memcmp

;
; @param zend_string *str1
; @param zend_string *str2
; @return bool
;
__php_fast_memcmp:
  
  mov rax, [rdi + 16]   ; Take the length of str1.
  mov rcx, [rsi + 16]   ; Take the length of str2.
  cmp rax, rcx
  jne .ret_0
  add rdi, 24
  add rsi, 24

.chk_act:
  test rcx, rcx
  jz .ret_1
  cmp rcx, 32
  jge .cmp_256b
  cmp rcx, 16
  jge .cmp_128b

.cmp_256b:
  vmovdqu ymm0, [rdi]
  vmovdqu ymm1, [rsi]
  vxorpd ymm0, ymm0, ymm1
  vptest ymm0, ymm0
  jnz .ret_0
  sub rcx, 32
  add rdi, 32
  add rsi, 32
  jmp .chk_act

.cmp_128b:
  vmovdqu xmm0, [rsi]
  vmovdqu xmm1, [rdx]
  pxor xmm0, xmm1
  ptest xmm0, xmm0
  jnz .ret_0
  sub rcx, 16
  add rsi, 16
  add rdx, 16
  jmp .chk_act

.ret_1:
  mov al, 1
  jmp .ret_x
.ret_0:
  xor al, al
.ret_x:
  ret
