#include "upstream/runtime/cpu.h"
#include <stdint.h>
#include <string.h>
#define LAB_A UINT64_C(0x7100000000)
#define LAB_B UINT64_C(0x7200000000)
#define LAB_SIZE 4096u
uint8_t lab_a[LAB_SIZE],lab_b[LAB_SIZE];
uint32_t lab_faults=0,lab_a_perm=3,lab_b_perm=1;
extern void f_7100000100(Cpu *);
volatile int g_fnprof=0;
void fnprof_hit(uint64_t a,uint64_t b,uint64_t c){(void)a;(void)b;(void)c;}
void rt_dispatch(Cpu*c){(void)c;}
int rt_hook(Cpu*c,uint64_t p){(void)c;(void)p;return 0;}
void rt_svc(Cpu*c,uint32_t n,uint64_t p){(void)c;(void)n;(void)p;}
__attribute__((noreturn)) void rt_udf(Cpu*c,uint64_t p){(void)c;(void)p;__builtin_trap();}
__attribute__((noreturn)) void rt_brk(Cpu*c,uint32_t n,uint64_t p){(void)c;(void)n;(void)p;__builtin_trap();}
uint64_t rt_ticks(void){return 0;}
__attribute__((export_name("run_region_a"))) uint32_t run_region_a(void){
 static Cpu cpu;memset(&cpu,0,sizeof(cpu));memset(lab_a,0,sizeof(lab_a));
 lab_a_perm=3;lab_b_perm=1;lab_faults=0;
 cpu.x[0]=LAB_A+16;
 f_7100000100(&cpu);
 uint32_t value;memcpy(&value,lab_a+16,4);return value;
}
__attribute__((export_name("read_region_a"))) uint32_t read_region_a(void){uint32_t v;memcpy(&v,lab_a+16,4);return v;}
__attribute__((export_name("valid_region"))) int valid_region(uint64_t addr,uint32_t n,uint32_t perm){
 uint64_t d;
 if(addr>=LAB_A&&(d=addr-LAB_A)<=LAB_SIZE&&n<=LAB_SIZE-d&&(lab_a_perm&perm)==perm)return 1;
 if(addr>=LAB_B&&(d=addr-LAB_B)<=LAB_SIZE&&n<=LAB_SIZE-d&&(lab_b_perm&perm)==perm)return 1;
 return 0;
}
