#include "cpu.h"
#include <stdint.h>
#include <string.h>
#define CHILD_ADDR UINT64_C(0x710000010c)
extern void f_7100000100(Cpu *);
extern void f_710000010c(Cpu *);
volatile int g_fnprof=0;
static int dispatches=0, faults=0;
void fnprof_hit(uint64_t a,uint64_t b,uint64_t d){(void)a;(void)b;(void)d;}
void rt_dispatch(Cpu *c) {
  if(c->pc==CHILD_ADDR){dispatches++;f_710000010c(c);}
  else {faults++; }
}
int rt_hook(Cpu*c,uint64_t a){(void)c;(void)a;return 0;}
void rt_svc(Cpu*c,uint32_t a,uint64_t b){(void)c;(void)a;(void)b;}
__attribute__((noreturn)) void rt_udf(Cpu*c,uint64_t a){(void)c;(void)a;__builtin_trap();}
__attribute__((noreturn)) void rt_brk(Cpu*c,uint32_t a,uint64_t b){(void)c;(void)a;(void)b;__builtin_trap();}
uint64_t rt_ticks(void){return 0;}
__attribute__((export_name("run_indirect"))) int run_indirect(void) {
  static Cpu c;memset(&c,0,sizeof(c));dispatches=0;faults=0;
  c.x[3]=CHILD_ADDR;
  f_7100000100(&c);
  return (int)(uint32_t)c.x[0];
}
__attribute__((export_name("get_dispatches"))) int get_dispatches(void){return dispatches;}
__attribute__((export_name("get_faults"))) int get_faults(void){return faults;}
