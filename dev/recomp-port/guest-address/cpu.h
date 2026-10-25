#pragma once
/* Experimental integer-only guest-memory adaptation for the synthetic LASRecomp
 * fixture. Does not implement the full Nintendo Switch virtual memory model. */
#include "upstream/runtime/cpu.h"
#include <stdint.h>
#define LAB_GUEST_BASE UINT64_C(0x7100000000)
#define LAB_GUEST_SIZE 4096u
extern uint8_t lab_guest_ram[LAB_GUEST_SIZE];
static inline int lab_guest_valid(uint64_t a, uint64_t n) {
    return a >= LAB_GUEST_BASE && (a - LAB_GUEST_BASE) <= LAB_GUEST_SIZE &&
           n <= (uint64_t)LAB_GUEST_SIZE - (a - LAB_GUEST_BASE);
}
static inline uint32_t *lab_guest_ptr32(uint64_t a) {
    if (!lab_guest_valid(a, 4) || ((a - LAB_GUEST_BASE) & 3)) __builtin_trap();
    return (uint32_t *)(void *)(lab_guest_ram + (uint32_t)(a - LAB_GUEST_BASE));
}
#undef LD32
#undef ST32
#define LD32(a) (*lab_guest_ptr32((uint64_t)(a)))
#define ST32(a,v) (*lab_guest_ptr32((uint64_t)(a)) = (uint32_t)(v))
