#include "upstream/runtime/cpu.h"
#include <stdint.h>
#include <string.h>
#define LAB_GUEST_BASE UINT64_C(0x7100000000)
#define LAB_GUEST_SIZE 4096u
uint8_t lab_guest_ram[LAB_GUEST_SIZE];
extern void f_7100000100(Cpu *);
volatile int g_fnprof=0;
void fnprof_hit(uint64_t a,uint64_t b,uint64_t c){(void)a;(void)b;(void)c;}
void rt_dispatch(Cpu*c){(void)c;}
int rt_hook(Cpu*c,uint64_t a){(void)c;(void)a;return 0;}
void rt_svc(Cpu*c,uint32_t a,uint64_t b){(void)c;(void)a;(void)b;}
__attribute__((noreturn)) void rt_udf(Cpu*c,uint64_t a){(void)c;(void)a;__builtin_trap();}
__attribute__((noreturn)) void rt_brk(Cpu*c,uint32_t a,uint64_t b){(void)c;(void)a;(void)b;__builtin_trap();}
uint64_t rt_ticks(void){return 0;}
__attribute__((export_name("valid_guest"))) int valid_guest(uint64_t a,uint32_t n){
return a>=LAB_GUEST_BASE && a-LAB_GUEST_BASE<=LAB_GUEST_SIZE && n<=LAB_GUEST_SIZE-(a-LAB_GUEST_BASE);
}
__attribute__((export_name("run_guest"))) uint32_t run_guest(void){
static Cpu cpu;memset(&cpu,0,sizeof(cpu));memset(lab_guest_ram,0,sizeof(lab_guest_ram));
cpu.x[0]=LAB_GUEST_BASE+16;
f_7100000100(&cpu);
uint32_t result;memcpy(&result,lab_guest_ram+16,4);return result;
}
