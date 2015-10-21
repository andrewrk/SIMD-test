# SIMD Test

Results: both gcc and clang can perform simple SIMD optimization when -O3 is
enabled. More difficult SIMD optimization problems were not explored.

```
test-gcc-debug
sum: 99897982687.000000  time: 0.446810
test-gcc-o3
sum: 99897982687.000000  time: 0.157358
test-gcc-simd
sum: 99897982687.000000  time: 0.157701
test-gcc-debug-simd
sum: 99897982687.000000  time: 0.213039
test-clang-debug
sum: 99897982687.000000  time: 0.410983
test-clang-o3
sum: 99897982687.000000  time: 0.159946
test-clang-simd
sum: 99897982687.000000  time: 0.160412
test-clang-debug-simd
sum: 99897982687.000000  time: 0.204966
```

## test-gcc-debug

```asm
  41:test.c        ****     for (int i = 0; i < size; i += 1) {
 165              		.loc 1 41 0
 166 0123 C745F800 		movl	$0, -8(%rbp)	#, i
 166      000000
 167 012a EB2A     		jmp	.L8	#
 168              	.L9:
  42:test.c        ****         c[i] = a[i] + b[i];
 169              		.loc 1 42 0 discriminator 3
 170 012c 8B45F8   		movl	-8(%rbp), %eax	# i, tmp120
 171 012f 4898     		cltq
 172 0131 8B148500 		movl	a(,%rax,4), %edx	# a, D.3584
 172      000000
 173 0138 8B45F8   		movl	-8(%rbp), %eax	# i, tmp122
 174 013b 4898     		cltq
 175 013d 8B048500 		movl	b(,%rax,4), %eax	# b, D.3584
 175      000000
 176 0144 01C2     		addl	%eax, %edx	# D.3584, D.3584
 177 0146 8B45F8   		movl	-8(%rbp), %eax	# i, tmp124
 178 0149 4898     		cltq
 179 014b 89148500 		movl	%edx, c(,%rax,4)	# D.3584, c
 179      000000
  41:test.c        ****     for (int i = 0; i < size; i += 1) {
 180              		.loc 1 41 0 discriminator 3
 181 0152 8345F801 		addl	$1, -8(%rbp)	#, i
 182              	.L8:
  41:test.c        ****     for (int i = 0; i < size; i += 1) {
 183              		.loc 1 41 0 is_stmt 0 discriminator 1
 184 0156 817DF8FF 		cmpl	$99999999, -8(%rbp)	#, i
 184      E0F505
 185 015d 7ECD     		jle	.L9	#,
 186              	.LBE3:
  43:test.c        ****     }
```

## test-gcc-o3

```asm
 163              	.L4:
  44:test.c        ****     for (int i = 0; i < size; i += 1) {
  45:test.c        ****         c[i] = a[i] + b[i];
 167              		.loc 1 45 0 discriminator 3
 168 00c0 660F6F80 		movdqa	a(%rax), %xmm0	# MEM[symbol: a, index: ivtmp.29_77, offset: 0B], MEM[symbol: a, index: ivtm
 168      00000000 
 169 00c8 4883C010 		addq	$16, %rax	#, ivtmp.29
 170 00cc 660FFE80 		paddd	b-16(%rax), %xmm0	# MEM[symbol: b, index: ivtmp.29_77, offset: 0B], vect__33.14
 170      00000000 
 171 00d4 0F298000 		movaps	%xmm0, c-16(%rax)	# vect__33.14, MEM[symbol: c, index: ivtmp.29_77, offset: 0B]
 171      000000
 172 00db 483D0084 		cmpq	$400000000, %rax	#, ivtmp.29
 172      D717
 173 00e1 75DD     		jne	.L4	#,
```

## test-gcc-simd

```asm
 163              	.L4:
  34:test.c        **** 
  35:test.c        **** #ifdef USE_SIMD
  36:test.c        ****     for (int i = 0; i < size; i += 4) {
  37:test.c        ****         v4si *a_ptr = (v4si *) &a[i];
  38:test.c        ****         v4si *b_ptr = (v4si *) &b[i];
  39:test.c        ****         v4si *c_ptr = (v4si *) &c[i];
  40:test.c        ****         *c_ptr = *a_ptr + *b_ptr;
 168              		.loc 1 40 0 discriminator 3
 169 00c0 660F6F80 		movdqa	a(%rax), %xmm0	# MEM[symbol: a, index: ivtmp.25_86, offset: 0B], MEM[symbol: a, index: ivtm
 169      00000000 
 170 00c8 4883C010 		addq	$16, %rax	#, ivtmp.25
 171              	.LVL10:
 172 00cc 660FFE80 		paddd	b-16(%rax), %xmm0	# MEM[symbol: b, index: ivtmp.25_86, offset: 0B], D.2962
 172      00000000 
 173 00d4 0F298000 		movaps	%xmm0, c-16(%rax)	# D.2962, MEM[symbol: c, index: ivtmp.25_86, offset: 0B]
 173      000000
 174              	.LBE26:
  36:test.c        ****     for (int i = 0; i < size; i += 4) {
 175              		.loc 1 36 0 discriminator 3
 176 00db 483D0084 		cmpq	$400000000, %rax	#, ivtmp.25
 176      D717
 177 00e1 75DD     		jne	.L4	#,
```

## test-gcc-debug-simd

```asm
 168              	.L9:
 169              	.LBB4:
  37:test.c        ****         v4si *a_ptr = (v4si *) &a[i];
 170              		.loc 1 37 0 discriminator 3
 171 012c 8B45F8   		movl	-8(%rbp), %eax	# i, tmp120
 172 012f 4898     		cltq
 173 0131 48C1E002 		salq	$2, %rax	#, tmp121
 174 0135 48050000 		addq	$a, %rax	#, tmp122
 174      0000
 175 013b 488945D8 		movq	%rax, -40(%rbp)	# tmp122, a_ptr
  38:test.c        ****         v4si *b_ptr = (v4si *) &b[i];
 176              		.loc 1 38 0 discriminator 3
 177 013f 8B45F8   		movl	-8(%rbp), %eax	# i, tmp124
 178 0142 4898     		cltq
 179 0144 48C1E002 		salq	$2, %rax	#, tmp125
 180 0148 48050000 		addq	$b, %rax	#, tmp126
 180      0000
 181 014e 488945D0 		movq	%rax, -48(%rbp)	# tmp126, b_ptr
  39:test.c        ****         v4si *c_ptr = (v4si *) &c[i];
 182              		.loc 1 39 0 discriminator 3
 183 0152 8B45F8   		movl	-8(%rbp), %eax	# i, tmp128
 184 0155 4898     		cltq
 185 0157 48C1E002 		salq	$2, %rax	#, tmp129
 186 015b 48050000 		addq	$c, %rax	#, tmp130
 186      0000
 187 0161 488945C8 		movq	%rax, -56(%rbp)	# tmp130, c_ptr
  40:test.c        ****         *c_ptr = *a_ptr + *b_ptr;
 188              		.loc 1 40 0 discriminator 3
 189 0165 488B45D8 		movq	-40(%rbp), %rax	# a_ptr, tmp131
 190 0169 660F6F08 		movdqa	(%rax), %xmm1	# *a_ptr_38, D.2868
 191 016d 488B45D0 		movq	-48(%rbp), %rax	# b_ptr, tmp132
 192 0171 660F6F00 		movdqa	(%rax), %xmm0	# *b_ptr_39, D.2868
 193 0175 660FFEC1 		paddd	%xmm1, %xmm0	# D.2868, D.2868
 194 0179 488B45C8 		movq	-56(%rbp), %rax	# c_ptr, tmp133
 195 017d 0F2900   		movaps	%xmm0, (%rax)	# D.2868, *c_ptr_40
 196              	.LBE4:
  36:test.c        ****     for (int i = 0; i < size; i += 4) {
 197              		.loc 1 36 0 discriminator 3
 198 0180 8345F804 		addl	$4, -8(%rbp)	#, i
 199              	.L8:
  36:test.c        ****     for (int i = 0; i < size; i += 4) {
 200              		.loc 1 36 0 is_stmt 0 discriminator 1
 201 0184 817DF8FF 		cmpl	$99999999, -8(%rbp)	#, i
 201      E0F505
 202 018b 7E9F     		jle	.L9	#,
```

## How to get that nice assembly output

```
gcc -std=c11 -S -D_POSIX_C_SOURCE=199309L -fverbose-asm -g -D USE_SIMD test.c -o test.s
as -alhnd test.s
```

from http://stackoverflow.com/a/137479/432
