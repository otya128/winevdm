#pragma once
#include <stdio.h>
#include <windows.h>
#include <winioctl.h>
// Signed Types
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed long long    int64;

// Unsigned Types
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned int        uint;
typedef unsigned long long  uint64;
typedef unsigned long       ulong;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;
typedef unsigned long       ulong_t;

/* Common typedef for all platform */
typedef uint64 hax_pa_t;
typedef uint64 hax_pfn_t;
typedef uint64 paddr_t;
typedef uint64 vaddr_t;

#include <pshpack1.h>
struct hax_module_version {
    uint32_t compat_version;
    uint32_t cur_version;
};
struct vmx_msr {
    uint64_t entry;
    uint64_t value;
};
#define HAX_MAX_MSR_ARRAY 0x20
struct hax_msr_data {
    uint16_t nr_msr;
    uint16_t done;
    uint16_t pad[2];
    struct vmx_msr entries[HAX_MAX_MSR_ARRAY];
};
struct hax_set_ram_info {
    uint64_t pa_start;
    uint32_t size;
    uint8_t flags;
    uint8_t pad[3];
    uint64_t va;
};

struct hax_alloc_ram_info {
    uint32_t size;
    uint32_t pad;
    uint64_t va;
};
union interruptibility_state_t {
    uint32 raw;
    struct {
        uint32 sti_blocking : 1;
        uint32 movss_blocking : 1;
        uint32 smi_blocking : 1;
        uint32 nmi_blocking : 1;
        uint32 reserved : 28;
    };
    uint64_t pad;
};

typedef union interruptibility_state_t interruptibility_state_t;

struct segment_desc_t {
    uint16 selector;
    uint16 _dummy;
    uint32 limit;
    uint64 base;
    union {
        struct {
            uint32 type : 4;
            uint32 desc : 1;
            uint32 dpl : 2;
            uint32 present : 1;
            uint32 : 4;
                     uint32 available : 1;
                     uint32 long_mode : 1;
                     uint32 operand_size : 1;
                     uint32 granularity : 1;
                     uint32 null : 1;
                     uint32 : 15;
        };
        uint32 ar;
    };
    uint32 ipad;
};

typedef struct segment_desc_t segment_desc_t;

struct vcpu_state_t {
    union {
        uint64 _regs[16];
        struct {
            union {
                struct {
                    uint8 _al,
                        _ah;
                };
                uint16    _ax;
                uint32    _eax;
                uint64    _rax;
            };
            union {
                struct {
                    uint8 _cl,
                        _ch;
                };
                uint16    _cx;
                uint32    _ecx;
                uint64    _rcx;
            };
            union {
                struct {
                    uint8 _dl,
                        _dh;
                };
                uint16    _dx;
                uint32    _edx;
                uint64    _rdx;
            };
            union {
                struct {
                    uint8 _bl,
                        _bh;
                };
                uint16    _bx;
                uint32    _ebx;
                uint64    _rbx;
            };
            union {
                uint16    _sp;
                uint32    _esp;
                uint64    _rsp;
            };
            union {
                uint16    _bp;
                uint32    _ebp;
                uint64    _rbp;
            };
            union {
                uint16    _si;
                uint32    _esi;
                uint64    _rsi;
            };
            union {
                uint16    _di;
                uint32    _edi;
                uint64    _rdi;
            };

            uint64 _r8;
            uint64 _r9;
            uint64 _r10;
            uint64 _r11;
            uint64 _r12;
            uint64 _r13;
            uint64 _r14;
            uint64 _r15;
        };
    };

    union {
        uint32 _eip;
        uint64 _rip;
    };

    union {
        uint32 _eflags;
        uint64 _rflags;
    };

    segment_desc_t _cs;
    segment_desc_t _ss;
    segment_desc_t _ds;
    segment_desc_t _es;
    segment_desc_t _fs;
    segment_desc_t _gs;
    segment_desc_t _ldt;
    segment_desc_t _tr;

    segment_desc_t _gdt;
    segment_desc_t _idt;

    uint64 _cr0;
    uint64 _cr2;
    uint64 _cr3;
    uint64 _cr4;

    uint64 _dr0;
    uint64 _dr1;
    uint64 _dr2;
    uint64 _dr3;
    uint64 _dr6;
    uint64 _dr7;
    uint64 _pde;

    uint32 _efer;

    uint32 _sysenter_cs;
    uint64 _sysenter_eip;
    uint64 _sysenter_esp;

    uint32 _activity_state;
    uint32 pad;
    interruptibility_state_t _interruptibility_state;
};

/* This interface is support only after API version 2 */
struct hax_qemu_version {
    /* Current API version in QEMU*/
    uint32_t cur_version;
    /* The least API version supported by QEMU */
    uint32_t least_version;
};
#define HAX_IO_OUT 0
#define HAX_IO_IN  1

struct hax_tunnel_info {
    uint64_t va;
    uint64_t io_va;
    uint16_t size;
    uint16_t pad[3];
};
typedef enum exit_status {
    HAX_EXIT_IO = 1,
    HAX_EXIT_MMIO,
    HAX_EXIT_REALMODE,
    HAX_EXIT_INTERRUPT,
    HAX_EXIT_UNKNOWN,
    HAX_EXIT_HLT,
    HAX_EXIT_STATECHANGE,
    HAX_EXIT_PAUSED,
    HAX_EXIT_FAST_MMIO,
    HAX_EXIT_PAGEFAULT
} exit_status;
typedef enum exit_reason {
    EXIT_INTERRUPT_WIN = 0x07,
    EXIT_HLT = 0x0c,
    EXIT_INTERRUPT = 0x30
} exit_reason;
struct hax_tunnel {
    exit_reason _exit_reason;
    uint32_t pad0;
    exit_status _exit_status;/* exit_status */
    uint32_t user_event_pending;
    int ready_for_interrupt_injection;
    int request_interrupt_window;

    union {
        struct {
            uint8_t _direction;
            uint8_t _df;
            uint16_t _size;
            uint16_t _port;
            uint16_t _count;
            /* Followed owned by HAXM, QEMU should not touch them */
            /* bit 1 is 1 means string io */
            uint8_t _flags;
            uint8_t _pad0;
            uint16_t _pad1;
            uint32_t _pad2;
            vaddr_t _vaddr;
        } io;
        struct {
            paddr_t gla;
        } mmio;
        struct {
            paddr_t gpa;
#define HAX_PAGEFAULT_ACC_R  (1 << 0)
#define HAX_PAGEFAULT_ACC_W  (1 << 1)
#define HAX_PAGEFAULT_ACC_X  (1 << 2)
#define HAX_PAGEFAULT_PERM_R (1 << 4)
#define HAX_PAGEFAULT_PERM_W (1 << 5)
#define HAX_PAGEFAULT_PERM_X (1 << 6)
            uint32_t flags;
            uint32_t reserved1;
            uint64_t reserved2;
        } pagefault;
        struct {
            paddr_t dummy;
        } state;
    };
    uint64_t apic_base;
};
enum run_flag {
    HAX_EXIT = 0,
    HAX_RESUME = 1
};

#define HAX_RAM_INFO_ROM (1 << 0)
#define HAX_RAM_INFO_STANDALONE (1 << 6)
#define HAX_RAM_INFO_INVALID (1 << 7)
#include <poppack.h>
struct hax_vcpu_mem {
    uint32_t size;
    uint64_t uva;
    void *kva;
    void *hinfo;
};

#include <pshpack8.h>
struct fx_layout {
    uint16  fcw;
    uint16  fsw;
    uint8   ftw;
    uint8   res1;
    uint16  fop;
    union {
        struct {
            uint32  fip;
            uint16  fcs;
            uint16  res2;
        };
        uint64  fpu_ip;
    };
    union {
        struct {
            uint32  fdp;
            uint16  fds;
            uint16  res3;
        };
        uint64  fpu_dp;
    };
    uint32  mxcsr;
    uint32  mxcsr_mask;
    uint8   st_mm[8][16];
    uint8   mmx_1[8][16];
    uint8   mmx_2[8][16];
    uint8   pad[96];
};
#include <poppack.h>


#define HAX_CAP_STATUS_WORKING     (1 << 0)
#define HAX_CAP_MEMQUOTA           (1 << 1)
#define HAX_CAP_WORKSTATUS_MASK    0x01

#define HAX_CAP_FAILREASON_VT      (1 << 0)
#define HAX_CAP_FAILREASON_NX      (1 << 1)

#define HAX_CAP_EPT                (1 << 0)
#define HAX_CAP_FASTMMIO           (1 << 1)
#define HAX_CAP_UG                 (1 << 2)
#define HAX_CAP_64BIT_RAMBLOCK     (1 << 3)
#define HAX_CAP_64BIT_SETRAM       (1 << 4)
#define HAX_CAP_TUNNEL_PAGE        (1 << 5)
#define HAX_CAP_RAM_PROTECTION     (1 << 6)
#define HAX_CAP_DEBUG              (1 << 7)
#define HAX_CAP_IMPLICIT_RAMBLOCK  (1 << 8)
#include <pshpack1.h>
struct hax_capabilityinfo {
    /*
     * bit 0: 1 - working, 0 - not working, possibly because NT/NX disabled
     * bit 1: 1 - memory limitation working, 0 - no memory limitation
     */
    uint16_t wstatus;
    /*
     * valid when not working
     * bit0: VT not enabeld
     * bit1: NX not enabled
     */
     /*
      * valid when working
      * bit0: EPT enabled
      * bit1: fastMMIO
      */
    uint16_t winfo;
    uint32_t win_refcount;
    uint64_t mem_quota;
};
#include <poppack.h>

#define HAX_DEVICE_TYPE 0x4000

#define HAX_IOCTL_VERSION \
        CTL_CODE(HAX_DEVICE_TYPE, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_IOCTL_CREATE_VM \
        CTL_CODE(HAX_DEVICE_TYPE, 0x901, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_IOCTL_CAPABILITY \
        CTL_CODE(HAX_DEVICE_TYPE, 0x910, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_IOCTL_SET_MEMLIMIT \
        CTL_CODE(HAX_DEVICE_TYPE, 0x911, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define HAX_VM_IOCTL_VCPU_CREATE \
        CTL_CODE(HAX_DEVICE_TYPE, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VM_IOCTL_ALLOC_RAM \
        CTL_CODE(HAX_DEVICE_TYPE, 0x903, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VM_IOCTL_SET_RAM \
        CTL_CODE(HAX_DEVICE_TYPE, 0x904, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VM_IOCTL_VCPU_DESTROY \
        CTL_CODE(HAX_DEVICE_TYPE, 0x905, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VM_IOCTL_ADD_RAMBLOCK \
        CTL_CODE(HAX_DEVICE_TYPE, 0x913, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VM_IOCTL_SET_RAM2 \
        CTL_CODE(HAX_DEVICE_TYPE, 0x914, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VM_IOCTL_PROTECT_RAM \
        CTL_CODE(HAX_DEVICE_TYPE, 0x915, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define HAX_VCPU_IOCTL_RUN \
        CTL_CODE(HAX_DEVICE_TYPE, 0x906, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VCPU_IOCTL_SET_MSRS \
        CTL_CODE(HAX_DEVICE_TYPE, 0x907, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VCPU_IOCTL_GET_MSRS \
        CTL_CODE(HAX_DEVICE_TYPE, 0x908, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define HAX_VCPU_IOCTL_SET_FPU \
        CTL_CODE(HAX_DEVICE_TYPE, 0x909, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VCPU_IOCTL_GET_FPU \
        CTL_CODE(HAX_DEVICE_TYPE, 0x90a, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define HAX_VCPU_IOCTL_SETUP_TUNNEL \
        CTL_CODE(HAX_DEVICE_TYPE, 0x90b, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VCPU_IOCTL_INTERRUPT \
        CTL_CODE(HAX_DEVICE_TYPE, 0x90c, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VCPU_SET_REGS \
        CTL_CODE(HAX_DEVICE_TYPE, 0x90d, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VCPU_GET_REGS \
        CTL_CODE(HAX_DEVICE_TYPE, 0x90e, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define HAX_VCPU_IOCTL_KICKOFF \
        CTL_CODE(HAX_DEVICE_TYPE, 0x90f, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* API version 2.0 */
#define HAX_VM_IOCTL_NOTIFY_QEMU_VERSION \
        CTL_CODE(HAX_DEVICE_TYPE, 0x910, METHOD_BUFFERED, FILE_ANY_ACCESS)


