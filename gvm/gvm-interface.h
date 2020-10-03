/*
 * QEMU GVM support
 *
 * Copyright IBM, Corp. 2008
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *
 * Copyright (c) 2017 Intel Corporation
 *  Written by:
 *  Haitao Shan <hshan@google.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */

#ifndef GVM_INTERFACE_H
#define GVM_INTERFACE_H

#ifdef _WIN32
#include <windows.h>
#include <winioctl.h>
#include <windef.h>
#endif
#include <memory.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#define __u8 uint8_t
#define __u16 uint16_t
#define __u32 uint32_t
#define __u64 uint64_t
#define __s8 int8_t
#define __s16 int16_t
#define __s32 int32_t
#define __s64 int64_t

/*
 * GVM x86 specific structures and definitions
 *
 */

#define DE_VECTOR 0
#define DB_VECTOR 1
#define BP_VECTOR 3
#define OF_VECTOR 4
#define BR_VECTOR 5
#define UD_VECTOR 6
#define NM_VECTOR 7
#define DF_VECTOR 8
#define TS_VECTOR 10
#define NP_VECTOR 11
#define SS_VECTOR 12
#define GP_VECTOR 13
#define PF_VECTOR 14
#define MF_VECTOR 16
#define AC_VECTOR 17
#define MC_VECTOR 18
#define XM_VECTOR 19
#define VE_VECTOR 20

/* Architectural interrupt line count. */
#define GVM_NR_INTERRUPTS 256

struct gvm_memory_alias {
	__u32 slot;  /* this has a different namespace than memory slots */
	__u32 flags;
	__u64 guest_phys_addr;
	__u64 memory_size;
	__u64 target_phys_addr;
};

/* for GVM_GET_IRQCHIP and GVM_SET_IRQCHIP */
struct gvm_pic_state {
	__u8 last_irr;	/* edge detection */
	__u8 irr;		/* interrupt request register */
	__u8 imr;		/* interrupt mask register */
	__u8 isr;		/* interrupt service register */
	__u8 priority_add;	/* highest irq priority */
	__u8 irq_base;
	__u8 read_reg_select;
	__u8 poll;
	__u8 special_mask;
	__u8 init_state;
	__u8 auto_eoi;
	__u8 rotate_on_auto_eoi;
	__u8 special_fully_nested_mode;
	__u8 init4;		/* true if 4 byte init */
	__u8 elcr;		/* PIIX edge/trigger selection */
	__u8 elcr_mask;
};

#define GVM_IOAPIC_NUM_PINS  24
struct gvm_ioapic_state {
	__u64 base_address;
	__u32 ioregsel;
	__u32 id;
	__u32 irr;
	__u32 pad;
	union {
		__u64 bits;
		struct {
			__u8 vector;
			__u8 delivery_mode:3;
			__u8 dest_mode:1;
			__u8 delivery_status:1;
			__u8 polarity:1;
			__u8 remote_irr:1;
			__u8 trig_mode:1;
			__u8 mask:1;
			__u8 reserve:7;
			__u8 reserved[4];
			__u8 dest_id;
		} fields;
	} redirtbl[GVM_IOAPIC_NUM_PINS];
};

#define GVM_IRQCHIP_PIC_MASTER   0
#define GVM_IRQCHIP_PIC_SLAVE    1
#define GVM_IRQCHIP_IOAPIC       2
#define GVM_NR_IRQCHIPS          3

#define GVM_RUN_X86_SMM		 (1 << 0)

/* for GVM_GET_REGS and GVM_SET_REGS */
struct gvm_regs {
	/* out (GVM_GET_REGS) / in (GVM_SET_REGS) */
	__u64 rax, rbx, rcx, rdx;
	__u64 rsi, rdi, rsp, rbp;
	__u64 r8,  r9,  r10, r11;
	__u64 r12, r13, r14, r15;
	__u64 rip, rflags;
};

/* for GVM_GET_LAPIC and GVM_SET_LAPIC */
#define GVM_APIC_REG_SIZE 0x400
struct gvm_lapic_state {
	char regs[GVM_APIC_REG_SIZE];
};

struct gvm_segment {
	__u64 base;
	__u32 limit;
	__u16 selector;
	__u8  type;
	__u8  present, dpl, db, s, l, g, avl;
	__u8  unusable;
	__u8  padding;
};

struct gvm_dtable {
	__u64 base;
	__u16 limit;
	__u16 padding[3];
};


/* for GVM_GET_SREGS and GVM_SET_SREGS */
struct gvm_sregs {
	/* out (GVM_GET_SREGS) / in (GVM_SET_SREGS) */
	struct gvm_segment cs, ds, es, fs, gs, ss;
	struct gvm_segment tr, ldt;
	struct gvm_dtable gdt, idt;
	__u64 cr0, cr2, cr3, cr4, cr8;
	__u64 efer;
	__u64 apic_base;
	__u64 interrupt_bitmap[(GVM_NR_INTERRUPTS + 63) / 64];
};

/* for GVM_GET_FPU and GVM_SET_FPU */
struct gvm_fpu {
	__u8  fpr[8][16];
	__u16 fcw;
	__u16 fsw;
	__u8  ftwx;  /* in fxsave format */
	__u8  pad1;
	__u16 last_opcode;
	__u64 last_ip;
	__u64 last_dp;
	__u8  xmm[16][16];
	__u32 mxcsr;
	__u32 pad2;
};

struct gvm_msr_entry {
	__u32 index;
	__u32 reserved;
	__u64 data;
};

/* for GVM_GET_MSRS and GVM_SET_MSRS */
struct gvm_msrs {
	__u32 nmsrs; /* number of msrs in entries */
	__u32 pad;

	struct gvm_msr_entry entries[0];
};

/* for GVM_GET_MSR_INDEX_LIST */
struct gvm_msr_list {
	__u32 nmsrs; /* number of msrs in entries */
	__u32 indices[0];
};

struct gvm_cpuid_entry {
	__u32 function;
	__u32 index;
	__u32 flags;
	__u32 eax;
	__u32 ebx;
	__u32 ecx;
	__u32 edx;
	__u32 padding[3];
};

#define GVM_CPUID_FLAG_SIGNIFCANT_INDEX		(1 << 0)
#define GVM_CPUID_FLAG_STATEFUL_FUNC		(1 << 1)
#define GVM_CPUID_FLAG_STATE_READ_NEXT		(1 << 2)

/* for GVM_SET_CPUID */
struct gvm_cpuid {
	__u32 nent;
	__u32 padding;
	struct gvm_cpuid_entry entries[0];
};

struct gvm_debug_exit_arch {
	__u32 exception;
	__u32 pad;
	__u64 pc;
	__u64 dr6;
	__u64 dr7;
};

#define GVM_GUESTDBG_USE_SW_BP		0x00010000
#define GVM_GUESTDBG_USE_HW_BP		0x00020000
#define GVM_GUESTDBG_INJECT_DB		0x00040000
#define GVM_GUESTDBG_INJECT_BP		0x00080000

/* for GVM_SET_GUEST_DEBUG */
struct gvm_guest_debug_arch {
	__u64 debugreg[8];
};

/* When set in flags, include corresponding fields on GVM_SET_VCPU_EVENTS */
#define GVM_VCPUEVENT_VALID_NMI_PENDING	0x00000001
#define GVM_VCPUEVENT_VALID_SIPI_VECTOR	0x00000002
#define GVM_VCPUEVENT_VALID_SHADOW	0x00000004
#define GVM_VCPUEVENT_VALID_SMM		0x00000008

/* Interrupt shadow states */
#define GVM_X86_SHADOW_INT_MOV_SS	0x01
#define GVM_X86_SHADOW_INT_STI		0x02

/* for GVM_GET/SET_VCPU_EVENTS */
struct gvm_vcpu_events {
	struct {
		__u8 injected;
		__u8 nr;
		__u8 has_error_code;
		__u8 pad;
		__u32 error_code;
	} exception;
	struct {
		__u8 injected;
		__u8 nr;
		__u8 soft;
		__u8 shadow;
	} interrupt;
	struct {
		__u8 injected;
		__u8 pending;
		__u8 masked;
		__u8 pad;
	} nmi;
	__u32 sipi_vector;
	__u32 flags;
	struct {
		__u8 smm;
		__u8 pending;
		__u8 smm_inside_nmi;
		__u8 latched_init;
	} smi;
	__u32 reserved[9];
};

/* for GVM_GET/SET_DEBUGREGS */
struct gvm_debugregs {
	__u64 db[4];
	__u64 dr6;
	__u64 dr7;
	__u64 flags;
	__u64 reserved[9];
};

/* for GVM_CAP_XSAVE */
struct gvm_xsave {
	__u32 region[1024];
};

#define GVM_MAX_XCRS	16

struct gvm_xcr {
	__u32 xcr;
	__u32 reserved;
	__u64 value;
};

struct gvm_xcrs {
	__u32 nr_xcrs;
	__u32 flags;
	struct gvm_xcr xcrs[GVM_MAX_XCRS];
	__u64 padding[16];
};

/* definition of registers in gvm_run */
struct gvm_sync_regs {
    __u32 dummy[0];
};

#define GVM_X86_QUIRK_LINT0_REENABLED	(1 << 0)
#define GVM_X86_QUIRK_CD_NW_CLEARED	(1 << 1)

#define FILE_DEVICE_GVM 0xE3E3

/* Macros to convert Linux style ioctl to Windows */
#define __IO(a,b) CTL_CODE(FILE_DEVICE_GVM, b, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define __IOR(a,b,c) __IO(a,b)
#define __IOW(a,b,c) __IO(a,b)
#define __IOWR(a,b,c) __IO(a,b)

#define GVM_API_VERSION 1

/* for GVM_CREATE_MEMORY_REGION */
struct gvm_memory_region {
	__u32 slot;
	__u32 flags;
	__u64 guest_phys_addr;
	__u64 memory_size; /* bytes */
};

/* for GVM_SET_USER_MEMORY_REGION */
struct gvm_userspace_memory_region {
	__u32 slot;
	__u32 flags;
	__u64 guest_phys_addr;
	__u64 memory_size; /* bytes */
	__u64 userspace_addr; /* start of the userspace allocated memory */
};

/*
 * The bit 0 ~ bit 15 of gvm_memory_region::flags are visible for userspace,
 * other bits are reserved for gvm internal use which are defined in
 * include/linux/gvm_host.h.
 */
#define GVM_MEM_LOG_DIRTY_PAGES	(1UL << 0)
#define GVM_MEM_READONLY	(1UL << 1)

/* for GVM_IRQ_LINE */
struct gvm_irq_level {
	/*
	 * ACPI gsi notion of irq.
	 * For IA-64 (APIC model) IOAPIC0: irq 0-23; IOAPIC1: irq 24-47..
	 * For X86 (standard AT mode) PIC0/1: irq 0-15. IOAPIC0: 0-23..
	 * For ARM: See Documentation/virtual/gvm/api.txt
	 */
	union {
		__u32 irq;
		__s32 status;
	};
	__u32 level;
};


struct gvm_irqchip {
	__u32 chip_id;
	__u32 pad;
        union {
		char dummy[512];  /* reserving space */
                struct gvm_pic_state pic;
		struct gvm_ioapic_state ioapic;
	} chip;
};

#define GVM_EXIT_UNKNOWN          0
#define GVM_EXIT_EXCEPTION        1
#define GVM_EXIT_IO               2
#define GVM_EXIT_HYPERCALL        3
#define GVM_EXIT_DEBUG            4
#define GVM_EXIT_HLT              5
#define GVM_EXIT_MMIO             6
#define GVM_EXIT_IRQ_WINDOW_OPEN  7
#define GVM_EXIT_SHUTDOWN         8
#define GVM_EXIT_FAIL_ENTRY       9
#define GVM_EXIT_INTR             10
#define GVM_EXIT_SET_TPR          11
#define GVM_EXIT_TPR_ACCESS       12
#define GVM_EXIT_NMI              16
#define GVM_EXIT_INTERNAL_ERROR   17
#define GVM_EXIT_OSI              18
#define GVM_EXIT_PAPR_HCALL	  19
#define GVM_EXIT_WATCHDOG         21
#define GVM_EXIT_EPR              23
#define GVM_EXIT_SYSTEM_EVENT     24
#define GVM_EXIT_IOAPIC_EOI       26

/* For GVM_EXIT_INTERNAL_ERROR */
/* Emulate instruction failed. */
#define GVM_INTERNAL_ERROR_EMULATION	1
/* Encounter unexpected simultaneous exceptions. */
#define GVM_INTERNAL_ERROR_SIMUL_EX	2
/* Encounter unexpected vm-exit due to delivery event. */
#define GVM_INTERNAL_ERROR_DELIVERY_EV	3

/* for GVM_RUN, returned by mmap(vcpu_fd, offset=0) */
struct gvm_run {
	/* in */
	__u8 request_interrupt_window;
	__u8 user_event_pending;
	__u8 padding1[6];

	/* out */
	__u32 exit_reason;
	__u8 ready_for_interrupt_injection;
	__u8 if_flag;
	__u16 flags;

	/* in (pre_gvm_run), out (post_gvm_run) */
	__u64 cr8;
	__u64 apic_base;

	union {
		/* GVM_EXIT_UNKNOWN */
		struct {
			__u64 hardware_exit_reason;
		} hw;
		/* GVM_EXIT_FAIL_ENTRY */
		struct {
			__u64 hardware_entry_failure_reason;
		} fail_entry;
		/* GVM_EXIT_EXCEPTION */
		struct {
			__u32 exception;
			__u32 error_code;
		} ex;
		/* GVM_EXIT_IO */
		struct {
#define GVM_EXIT_IO_IN  0
#define GVM_EXIT_IO_OUT 1
			__u8 direction;
			__u8 size; /* bytes */
			__u16 port;
			__u32 count;
			__u64 data_offset; /* relative to gvm_run start */
		} io;
		/* GVM_EXIT_DEBUG */
		struct {
			struct gvm_debug_exit_arch arch;
		} debug;
		/* GVM_EXIT_MMIO */
		struct {
			__u64 phys_addr;
			__u8  data[8];
			__u32 len;
			__u8  is_write;
		} mmio;
		/* GVM_EXIT_HYPERCALL */
		struct {
			__u64 nr;
			__u64 args[6];
			__u64 ret;
			__u32 longmode;
			__u32 pad;
		} hypercall;
		/* GVM_EXIT_TPR_ACCESS */
		struct {
			__u64 rip;
			__u32 is_write;
			__u32 pad;
		} tpr_access;
		/* GVM_EXIT_INTERNAL_ERROR */
		struct {
			__u32 suberror;
			/* Available with GVM_CAP_INTERNAL_ERROR_DATA: */
			__u32 ndata;
			__u64 data[16];
		} internal;
		/* GVM_EXIT_OSI */
		struct {
			__u64 gprs[32];
		} osi;
		/* GVM_EXIT_PAPR_HCALL */
		struct {
			__u64 nr;
			__u64 ret;
			__u64 args[9];
		} papr_hcall;
		/* GVM_EXIT_EPR */
		struct {
			__u32 epr;
		} epr;
		/* GVM_EXIT_SYSTEM_EVENT */
		struct {
#define GVM_SYSTEM_EVENT_SHUTDOWN       1
#define GVM_SYSTEM_EVENT_RESET          2
#define GVM_SYSTEM_EVENT_CRASH          3
			__u32 type;
			__u64 flags;
		} system_event;
		/* GVM_EXIT_IOAPIC_EOI */
		struct {
			__u8 vector;
		} eoi;
		/* Fix the size of the union. */
		char padding[256];
	};

	/*
	 * shared registers between gvm and userspace.
	 * gvm_valid_regs specifies the register classes set by the host
	 * gvm_dirty_regs specified the register classes dirtied by userspace
	 * struct gvm_sync_regs is architecture specific, as well as the
	 * bits for gvm_valid_regs and gvm_dirty_regs
	 */
	__u64 gvm_valid_regs;
	__u64 gvm_dirty_regs;
	union {
		struct gvm_sync_regs regs;
		char padding[2048];
	} s;
};

/* for GVM_TRANSLATE */
struct gvm_translation {
	/* in */
	__u64 linear_address;

	/* out */
	__u64 physical_address;
	__u8  valid;
	__u8  writeable;
	__u8  usermode;
	__u8  pad[5];
};

/* for GVM_INTERRUPT */
struct gvm_interrupt {
	/* in */
	__u32 irq;
};

/* for GVM_GET_DIRTY_LOG */
struct gvm_dirty_log {
	__u32 slot;
	__u32 padding1;
	union {
		void *dirty_bitmap; /* one bit per page */
		__u64 padding2;
	};
};

/* for GVM_TPR_ACCESS_REPORTING */
struct gvm_tpr_access_ctl {
	__u32 enabled;
	__u32 flags;
	__u32 reserved[8];
};

/* for GVM_SET_VAPIC_ADDR */
struct gvm_vapic_addr {
	__u64 vapic_addr;
};

/* for GVM_SET_MP_STATE */

/* not all states are valid on all architectures */
#define GVM_MP_STATE_RUNNABLE          0
#define GVM_MP_STATE_UNINITIALIZED     1
#define GVM_MP_STATE_INIT_RECEIVED     2
#define GVM_MP_STATE_HALTED            3
#define GVM_MP_STATE_SIPI_RECEIVED     4
#define GVM_MP_STATE_STOPPED           5
#define GVM_MP_STATE_CHECK_STOP        6
#define GVM_MP_STATE_OPERATING         7
#define GVM_MP_STATE_LOAD              8

struct gvm_mp_state {
	__u32 mp_state;
};

/* for GVM_SET_GUEST_DEBUG */

#define GVM_GUESTDBG_ENABLE		0x00000001
#define GVM_GUESTDBG_SINGLESTEP		0x00000002

struct gvm_guest_debug {
	__u32 control;
	__u32 pad;
	struct gvm_guest_debug_arch arch;
};

/* for GVM_ENABLE_CAP */
struct gvm_enable_cap {
	/* in */
	__u32 cap;
	__u32 flags;
	__u64 args[4];
	__u8  pad[64];
};

/*
 * ioctls for /dev/gvm fds:
 */
#define GVM_GET_API_VERSION       __IO(GVMIO,   0x00)
#define GVM_CREATE_VM             __IO(GVMIO,   0x01) /* returns a VM fd */
#define GVM_GET_MSR_INDEX_LIST    __IOWR(GVMIO, 0x02, struct gvm_msr_list)
/*
 * Check if a gvm extension is available.  Argument is extension number,
 * return is 1 (yes) or 0 (no, sorry).
 */
#define GVM_CHECK_EXTENSION       __IO(GVMIO,   0x03)
/*
 * Get size for mmap(vcpu_fd)
 */
#define GVM_GET_VCPU_MMAP_SIZE    __IO(GVMIO,   0x04) /* in bytes */
#define GVM_GET_SUPPORTED_CPUID   __IOWR(GVMIO, 0x05, struct gvm_cpuid)
#define GVM_GET_EMULATED_CPUID	  __IOWR(GVMIO, 0x09, struct gvm_cpuid)
/*
 * Extension capability list.
 */
#define GVM_CAP_IRQCHIP	  0
#define GVM_CAP_HLT	  1
#define GVM_CAP_MMU_SHADOW_CACHE_CONTROL 2
#define GVM_CAP_VAPIC 6
#define GVM_CAP_NR_VCPUS 9       /* returns recommended max vcpus per vm */
#define GVM_CAP_NR_MEMSLOTS 10   /* returns max memory slots per vm */
#define GVM_CAP_SYNC_MMU 16  /* Changes to host mmap are reflected in guest */
#define GVM_CAP_IOMMU 18
#define GVM_CAP_USER_NMI 22
#define GVM_CAP_IRQ_ROUTING 25
#define GVM_CAP_SET_BOOT_CPU_ID 34
#define GVM_CAP_SET_IDENTITY_MAP_ADDR 37
#define GVM_CAP_PCI_SEGMENT 47
#define GVM_CAP_INTR_SHADOW 49
#define GVM_CAP_ENABLE_CAP 54
#define GVM_CAP_XSAVE 55
#define GVM_CAP_XCRS 56
#define GVM_CAP_MAX_VCPUS 66       /* returns max vcpus per vm */
#define GVM_CAP_SW_TLB 69
#define GVM_CAP_SYNC_REGS 74
#define GVM_CAP_READONLY_MEM 81
#define GVM_CAP_EXT_EMUL_CPUID 95
#define GVM_CAP_IOAPIC_POLARITY_IGNORED 97
#define GVM_CAP_ENABLE_CAP_VM 98
#define GVM_CAP_VM_ATTRIBUTES 101
#define GVM_CAP_CHECK_EXTENSION_VM 105
#define GVM_CAP_DISABLE_QUIRKS 116
#define GVM_CAP_X86_SMM 117
#define GVM_CAP_MULTI_ADDRESS_SPACE 118
#define GVM_CAP_GUEST_DEBUG_HW_BPS 119
#define GVM_CAP_GUEST_DEBUG_HW_WPS 120
#define GVM_CAP_VCPU_ATTRIBUTES 127
#define GVM_CAP_MAX_VCPU_ID 128
#define GVM_CAP_X2APIC_API 129
#define GVM_CAP_MSI_DEVID 131

struct gvm_irq_routing_irqchip {
	__u32 irqchip;
	__u32 pin;
};

struct gvm_irq_routing_msi {
	__u32 address_lo;
	__u32 address_hi;
	__u32 data;
	union {
		__u32 pad;
		__u32 devid;
	};
};

struct gvm_irq_routing_hv_sint {
	__u32 vcpu;
	__u32 sint;
};

/* gsi routing entry types */
#define GVM_IRQ_ROUTING_IRQCHIP 1
#define GVM_IRQ_ROUTING_MSI 2
#define GVM_IRQ_ROUTING_HV_SINT 4

struct gvm_irq_routing_entry {
	__u32 gsi;
	__u32 type;
	__u32 flags;
	__u32 pad;
	union {
		struct gvm_irq_routing_irqchip irqchip;
		struct gvm_irq_routing_msi msi;
		struct gvm_irq_routing_hv_sint hv_sint;
		__u32 pad[8];
	} u;
};

struct gvm_irq_routing {
	__u32 nr;
	__u32 flags;
	struct gvm_irq_routing_entry entries[0];
};

/* For GVM_CAP_SW_TLB */

#define GVM_MMU_FSL_BOOKE_NOHV		0
#define GVM_MMU_FSL_BOOKE_HV		1

struct gvm_config_tlb {
	__u64 params;
	__u64 array;
	__u32 mmu_type;
	__u32 array_len;
};

struct gvm_dirty_tlb {
	__u64 bitmap;
	__u32 num_dirty;
};

/* Available with GVM_CAP_ONE_REG */

#define GVM_REG_ARCH_MASK	0xff00000000000000ULL
#define GVM_REG_GENERIC		0x0000000000000000ULL

/*
 * Architecture specific registers are to be defined in arch headers and
 * ORed with the arch identifier.
 */
#define GVM_REG_PPC		0x1000000000000000ULL
#define GVM_REG_X86		0x2000000000000000ULL
#define GVM_REG_IA64		0x3000000000000000ULL
#define GVM_REG_ARM		0x4000000000000000ULL
#define GVM_REG_S390		0x5000000000000000ULL
#define GVM_REG_ARM64		0x6000000000000000ULL
#define GVM_REG_MIPS		0x7000000000000000ULL

#define GVM_REG_SIZE_SHIFT	52
#define GVM_REG_SIZE_MASK	0x00f0000000000000ULL
#define GVM_REG_SIZE_U8		0x0000000000000000ULL
#define GVM_REG_SIZE_U16	0x0010000000000000ULL
#define GVM_REG_SIZE_U32	0x0020000000000000ULL
#define GVM_REG_SIZE_U64	0x0030000000000000ULL
#define GVM_REG_SIZE_U128	0x0040000000000000ULL
#define GVM_REG_SIZE_U256	0x0050000000000000ULL
#define GVM_REG_SIZE_U512	0x0060000000000000ULL
#define GVM_REG_SIZE_U1024	0x0070000000000000ULL

struct gvm_reg_list {
	__u64 n; /* number of regs */
	__u64 reg[0];
};

struct gvm_one_reg {
	__u64 id;
	__u64 addr;
};

#define GVM_MSI_VALID_DEVID	(1U << 0)
struct gvm_msi {
	__u32 address_lo;
	__u32 address_hi;
	__u32 data;
	__u32 flags;
	__u32 devid;
	__u8  pad[12];
};

/*
 * ioctls for VM fds
 */
#define GVM_SET_MEMORY_REGION     __IOW(GVMIO,  0x40, struct gvm_memory_region)
/*
 * GVM_CREATE_VCPU receives as a parameter the vcpu slot, and returns
 * a vcpu fd.
 */
#define GVM_CREATE_VCPU           __IO(GVMIO,   0x41)
#define GVM_GET_DIRTY_LOG         __IOW(GVMIO,  0x42, struct gvm_dirty_log)
/* GVM_SET_MEMORY_ALIAS is obsolete: */
#define GVM_SET_MEMORY_ALIAS      __IOW(GVMIO,  0x43, struct gvm_memory_alias)
#define GVM_SET_NR_MMU_PAGES      __IO(GVMIO,   0x44)
#define GVM_GET_NR_MMU_PAGES      __IO(GVMIO,   0x45)
#define GVM_SET_USER_MEMORY_REGION __IOW(GVMIO, 0x46, \
					struct gvm_userspace_memory_region)
#define GVM_SET_TSS_ADDR          __IO(GVMIO,   0x47)
#define GVM_SET_IDENTITY_MAP_ADDR __IOW(GVMIO,  0x48, __u64)
#define GVM_KICK_VCPU             __IO(GVMIO,   0x49)

/* Device model IOC */
#define GVM_CREATE_IRQCHIP        __IO(GVMIO,   0x60)
#define GVM_GET_IRQCHIP           __IOWR(GVMIO, 0x62, struct gvm_irqchip)
#define GVM_SET_IRQCHIP           __IOR(GVMIO,  0x63, struct gvm_irqchip)
#define GVM_IRQ_LINE_STATUS       __IOWR(GVMIO, 0x67, struct gvm_irq_level)
#define GVM_SET_GSI_ROUTING       __IOW(GVMIO,  0x6a, struct gvm_irq_routing)
/* deprecated, replaced by GVM_ASSIGN_DEV_IRQ */
#define GVM_ASSIGN_IRQ            __GVM_DEPRECATED_VM_R_0x70
#define GVM_ASSIGN_DEV_IRQ        __IOW(GVMIO,  0x70, struct gvm_assigned_irq)
#define GVM_REINJECT_CONTROL      __IO(GVMIO,   0x71)
#define GVM_SET_BOOT_CPU_ID       __IO(GVMIO,   0x78)

/*
 * ioctls for vcpu fds
 */
#define GVM_RUN                   __IO(GVMIO,   0x80)
#define GVM_VCPU_MMAP             __IO(GVMIO,   0x87)
#define GVM_GET_REGS              __IOR(GVMIO,  0x81, struct gvm_regs)
#define GVM_SET_REGS              __IOW(GVMIO,  0x82, struct gvm_regs)
#define GVM_GET_SREGS             __IOR(GVMIO,  0x83, struct gvm_sregs)
#define GVM_SET_SREGS             __IOW(GVMIO,  0x84, struct gvm_sregs)
#define GVM_TRANSLATE             __IOWR(GVMIO, 0x85, struct gvm_translation)
#define GVM_INTERRUPT             __IOW(GVMIO,  0x86, struct gvm_interrupt)
#define GVM_GET_MSRS              __IOWR(GVMIO, 0x88, struct gvm_msrs)
#define GVM_SET_MSRS              __IOW(GVMIO,  0x89, struct gvm_msrs)
#define GVM_GET_FPU               __IOR(GVMIO,  0x8c, struct gvm_fpu)
#define GVM_SET_FPU               __IOW(GVMIO,  0x8d, struct gvm_fpu)
#define GVM_GET_LAPIC             __IOR(GVMIO,  0x8e, struct gvm_lapic_state)
#define GVM_SET_LAPIC             __IOW(GVMIO,  0x8f, struct gvm_lapic_state)
#define GVM_SET_CPUID             __IOW(GVMIO,  0x90, struct gvm_cpuid)
#define GVM_GET_CPUID             __IOWR(GVMIO, 0x91, struct gvm_cpuid)
/* Available with GVM_CAP_VAPIC */
#define GVM_TPR_ACCESS_REPORTING  __IOWR(GVMIO, 0x92, struct gvm_tpr_access_ctl)
/* Available with GVM_CAP_VAPIC */
#define GVM_SET_VAPIC_ADDR        __IOW(GVMIO,  0x93, struct gvm_vapic_addr)
#define GVM_GET_MP_STATE          __IOR(GVMIO,  0x98, struct gvm_mp_state)
#define GVM_SET_MP_STATE          __IOW(GVMIO,  0x99, struct gvm_mp_state)
/* Available with GVM_CAP_USER_NMI */
#define GVM_NMI                   __IO(GVMIO,   0x9a)
/* Available with GVM_CAP_SET_GUEST_DEBUG */
#define GVM_SET_GUEST_DEBUG       __IOW(GVMIO,  0x9b, struct gvm_guest_debug)
/* Available with GVM_CAP_VCPU_EVENTS */
#define GVM_GET_VCPU_EVENTS       __IOR(GVMIO,  0x9f, struct gvm_vcpu_events)
#define GVM_SET_VCPU_EVENTS       __IOW(GVMIO,  0xa0, struct gvm_vcpu_events)
/* Available with GVM_CAP_DEBUGREGS */
#define GVM_GET_DEBUGREGS         __IOR(GVMIO,  0xa1, struct gvm_debugregs)
#define GVM_SET_DEBUGREGS         __IOW(GVMIO,  0xa2, struct gvm_debugregs)
/*
 * vcpu version available with GVM_ENABLE_CAP
 * vm version available with GVM_CAP_ENABLE_CAP_VM
 */
#define GVM_ENABLE_CAP            __IOW(GVMIO,  0xa3, struct gvm_enable_cap)
/* Available with GVM_CAP_XSAVE */
#define GVM_GET_XSAVE		  __IOR(GVMIO,  0xa4, struct gvm_xsave)
#define GVM_SET_XSAVE		  __IOW(GVMIO,  0xa5, struct gvm_xsave)
/* Available with GVM_CAP_XCRS */
#define GVM_GET_XCRS		  __IOR(GVMIO,  0xa6, struct gvm_xcrs)
#define GVM_SET_XCRS		  __IOW(GVMIO,  0xa7, struct gvm_xcrs)
/* Available with GVM_CAP_SW_TLB */
#define GVM_DIRTY_TLB		  __IOW(GVMIO,  0xaa, struct gvm_dirty_tlb)
/* Available with GVM_CAP_X86_SMM */
#define GVM_SMI                   __IO(GVMIO,   0xb7)

#define GVM_X2APIC_API_USE_32BIT_IDS            (1ULL << 0)
#define GVM_X2APIC_API_DISABLE_BROADCAST_QUIRK  (1ULL << 1)

#endif /*GVM_INTERFACE_H */
