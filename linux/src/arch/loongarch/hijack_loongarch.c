#include "hijack_loongarch.h"
#include <asm/cacheflush.h>
#include <asm/inst.h>

/*
	addi.d     $sp, $sp, -16
	st.d       $t1, $sp, 8
	pcaddu12i  $t1, 0
	ld.d       $t1, $t1, 12
	jr         $t1
	.addr(low)
	.addr(high)
	ld.d       $t1, $sp, 8
	addi.d     $sp, $sp, 16
*/
const char long_jmp_code[36]="\x63\xc0\xff\x02\x6d\x20\xc0\x29\x0d\x00\x00\x1c\xad\x31\xc0\x28\xa0\x01\x00\x4c\x00\x00\x00\x00\x00\x00\x00\x00\x6d\x20\xc0\x28\x63\x40\xc0\x02";

inline void fill_long_jmp(void *fill_dest, void *hijack_to_func)
{
	memcpy(fill_dest, long_jmp_code, sizeof(long_jmp_code));
	memcpy(fill_dest + 5 * INSTRUCTION_SIZE, &hijack_to_func, sizeof(void *));
}

bool check_target_can_hijack(void *target)
{
	int i;

	for (i = 0; i < HIJACK_SIZE; i = i + INSTRUCTION_SIZE) {
		if (is_pc_ins((union loongarch_instruction *)(target + i)))
			return false;
	}

	return true;
}

int (*larch_insn_write_ptr)(void *addr, u32) = NULL;
void *find_func(const char *name);

__nocfi int hook_write_range(void *target, void *source, int size)
{
	int ret = 0, i;

	for (i = 0; i < size; i = i + INSTRUCTION_SIZE) {
		ret = larch_insn_write_ptr(target + i, *(u32 *)(source + i));
		if (ret) {
			goto out;
		}
	}
	flush_icache_range((unsigned long)target, (unsigned long)target + size);
out:
	return ret;
}

int init_arch(void)
{
	larch_insn_write_ptr = (void *)find_func("larch_insn_write");
	return !(larch_insn_write_ptr);
}