#pragma once
#include "upstream/runtime/cpu.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#define LAB_A UINT64_C(0x7100000000)
#define LAB_B UINT64_C(0x7200000000)
#define LAB_SIZE 4096u
extern uint8_t lab_a[LAB_SIZE],lab_b[LAB_SIZE];
extern uint32_t lab_faults;
extern uint32_t lab_a_perm, lab_b_perm;
static inline uint8_t *lab_translate(uint64_t a,uint32_t n,uint32_t perm){
 uint64_t delta;
 if(a>=LAB_A && (delta=a-LAB_A)<=LAB_SIZE && n<=LAB_SIZE-delta && (lab_a_perm&perm)==perm)return lab_a+(uint32_t)delta;
 if(a>=LAB_B && (delta=a-LAB_B)<=LAB_SIZE && n<=LAB_SIZE-delta && (lab_b_perm&perm)==perm)return lab_b+(uint32_t)delta;
 lab_faults++; __builtin_trap();
}
static inline uint32_t lab_read32(uint64_t a){uint32_t v;memcpy(&v,lab_translate(a,4,1),4);return v;}
static inline void lab_write32(uint64_t a,uint32_t v){memcpy(lab_translate(a,4,2),&v,4);}
#undef LD32
#undef ST32
#define LD32(a) lab_read32((uint64_t)(a))
#define ST32(a,v) lab_write32((uint64_t)(a),(uint32_t)(v))
