#ifndef _HOOK_POWERPC_H_
#define _HOOK_POWERPC_H_

#define HOOK_FUNC_TEMPLATE(s)			\
extern void hook_##s##_template(void);		\
asm (						\
	".section \".data\"			\n\t" \
	#s"_r2:					\n\t" \
	".quad 0				\n\t" \
	#s"_lr:					\n\t" \
	".quad 0				\n\t" \
						\
	".section \".text\"			\n\t" \
	".quad .TOC.-hook_"#s"_template		\n\t" \
	".globl hook_"#s"_template		\n\t" \
	"hook_"#s"_template:			\n\t" \
	"mr	0, 2				\n\t" \
	"ld	2, -8(12)			\n\t" \
	"add	2, 2, 12			\n\t" \
						\
	"addis	12, 2, "#s"_r2@toc@ha		\n\t" \
	"addi	12, 12, "#s"_r2@toc@l		\n\t" \
	"std	0, 0(12)			\n\t" \
						\
	"mflr	0				\n\t" \
	"addis	12, 2, "#s"_lr@toc@ha		\n\t" \
	"addi	12, 12, "#s"_lr@toc@l		\n\t" \
	"std	0, 0(12)			\n\t" \
						\
	"addis	12, 2, hook_"#s"_template@toc@ha\n\t" \
	"addi	12, 12, hook_"#s"_template@toc@l\n\t" \
	"addi	12, 12, 72			\n\t" \
	"mtlr	12				\n\t" \
						\
	"addis	12, 2, hook_"#s"@toc@ha		\n\t" \
	"addi	12, 12, hook_"#s"@toc@l		\n\t" \
	"mtctr	12				\n\t" \
	"bctr					\n\t" \
						\
	"addis	12, 2, "#s"_r2@toc@ha		\n\t" \
	"addi	12, 12, "#s"_r2@toc@l		\n\t" \
	"ld	0, 0(12)			\n\t" \
	"addis	12, 2, "#s"_lr@toc@ha		\n\t" \
	"addi	12, 12, "#s"_lr@toc@l		\n\t" \
	"mr	2, 0				\n\t" \
	"ld	0, 0(12)			\n\t" \
	"mtlr	0				\n\t" \
	"blr					\n\t" \
						\
	".globl "#s"_code_space\n\t"		\
	#s"_code_space:\n\t"			\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long 	0\n\t"				\
	".long 	0\n\t"				\
	".long 	0\n\t"				\
	".long 	0\n\t"				\
	".long 	0\n\t"				\
	".long 	0\n\t"				\
	".long 	0\n\t"				\
	".long 	0\n\t"				\
	/********** */				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
	".long	0\n\t"				\
);

#define GET_TEMPLATE_ADDERSS(s)					\
({								\
	void *template;						\
	__asm__ volatile (					\
		"addis %0,2,hook_"#s"_template@toc@ha\n\t"	\
		"addi %0,%0,hook_"#s"_template@toc@l\n\t":	\
		"=r"(template)					\
	);							\
	template;						\
})

#define GET_CODESPACE_ADDERSS(s)				\
({								\
	void *codespace;					\
	__asm__ volatile (					\
		"addis %0,2,"#s"_code_space@toc@ha\n\t"		\
		"addi %0,%0,"#s"_code_space@toc@l\n\t":		\
		"=r"(codespace)					\
	);							\
	codespace;					 	\
})

#define GET_HOOK_FUNC_ADDRESS(s)				\
({								\
	void *hook_func;					\
	__asm__ volatile (					\
		"addis %0,2,hook_"#s"@toc@ha\n\t"		\
		"addi %0,%0,hook_"#s"@toc@l\n\t":		\
		"=r"(hook_func)					\
	);							\
	hook_func;						\
})

#define INSTRUCTION_SIZE 4
#define HIJACK_INST_NUM 11
#define HIJACK_SIZE (INSTRUCTION_SIZE * HIJACK_INST_NUM)
#define fill_nop_for_target(x, y) (0)
#define fill_nop_for_code_space(x, y) (0)
#define HOOK_TARGET_OFFSET (0)
#define CODE_SPACE_OFFSET (0)
int init_arch(void);
#endif