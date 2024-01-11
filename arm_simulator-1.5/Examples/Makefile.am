CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
# Our simulator :
# - is big-endian
# - does not initialize properly fp (so we omit it), see arm_core.c
# - has a non standard main (no OS so no return value), we disable the warning
AM_CFLAGS=-mbig-endian -fomit-frame-pointer -Wno-main
# Still big-endian for ARM assembly language sources
AM_CCASFLAGS=-mbig-endian
# We don't have any OS, so no stdlib as well
# The linker script maps loadable segments into memory, the overall size must match the simulator memory
# The entry point will always be main
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
# Our simulator is big-endian
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=example1 example2 example3 example4 insertion_sort example_add_entier example_add_registre example_MRS_CPSR example_MRS_SPSR example_sub_entier example_sub_registre example_vide example_and_0 example_and_1 example_and_table example_bic_0 example_bic_1 example_bic_table example_eor_0 example_eor_1 example_eor_table example_orr_0 example_orr_1 example_orr_table example_teq_0 example_teq_1 example_teq_table example_tst_0 example_tst_1 example_tst_table 

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
insertion_sort_SOURCES=insertion_sort.c my_syscalls.s my_syscalls.h
example_add_entier_SOURCES=example_add_entier.s
example_add_registre_SOURCES=example_add_registre.s
example_MRS_CPSR_SOURCES=example_MRS_CPSR.s
example_MRS_SPSR_SOURCES=example_MRS_SPSR.s
example_sub_entier_SOURCES=example_sub_entier.s
example_sub_registre_SOURCES=example_sub_registre.s
example_vide_SOURCES=example_vide.s
example_and_0_SOURCES=example_and_0.s
example_and_1_SOURCES=example_and_1.s
example_and_table_SOURCES=example_and_table.s
example_bic_0_SOURCES=example_bic_0.s
example_bic_1_SOURCES=example_bic_1.s
example_bic_table_SOURCES=example_bic_table.s
example_eor_0_SOURCES=example_eor_0.s
example_eor_1_SOURCES=example_eor_1.s
example_eor_table_SOURCES=example_eor_table.s
example_orr_0_SOURCES=example_orr_0.s
example_orr_1_SOURCES=example_orr_1.s
example_orr_table_SOURCES=example_orr_table.s
example_teq_0_SOURCES=example_teq_0.s
example_teq_1_SOURCES=example_teq_1.s
example_teq_table_SOURCES=example_teq_table.s
example_tst_0_SOURCES=example_tst_0.s
example_tst_1_SOURCES=example_tst_1.s
example_tst_table_SOURCES=example_tst_table.s

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
