// Copyright-free test harness for C emitted by LASRecomp tools/gen.py.
// The fixture is mov w0,#42 ; ret. No Nintendo code or assets.
#include "cpu.h"
#include <stdint.h>
#include <string.h>
extern void f_7100000100(Cpu *);
volatile int g_fnprof = 0;
void fnprof_hit(uint64_t a, uint64_t b, uint64_t d) {(void)a;(void)b;(void)d;}
static int dispatch_seen = 0;
void rt_dispatch(Cpu *c) {(void)c; dispatch_seen++;}
int rt_hook(Cpu *c, uint64_t site) {(void)c;(void)site;return 0;}
void rt_svc(Cpu *c, uint32_t imm, uint64_t pc){(void)c;(void)imm;(void)pc;}
__attribute__((noreturn)) void rt_udf(Cpu *c,uint64_t pc){(void)c;(void)pc;__builtin_trap();}
__attribute__((noreturn)) void rt_brk(Cpu *c,uint32_t imm,uint64_t pc){(void)c;(void)imm;(void)pc;__builtin_trap();}
uint64_t rt_ticks(void){return 0;}
__attribute__((export_name("run_fixture"))) uint32_t run_fixture(void) {
  static Cpu state;
  memset(&state,0,sizeof(state));
  dispatch_seen=0;
  f_7100000100(&state);
  return (uint32_t)state.x[0];
}
__attribute__((export_name("dispatch_count"))) int dispatch_count(void){return dispatch_seen;}
