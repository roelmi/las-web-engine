#include <stdint.h>
#define BASE 0x7100000000ULL
#define CAP (1024U*1024U)
static uint8_t ram[CAP];
static uint32_t translate(uint64_t addr, uint32_t count) {
  if (addr < BASE) return UINT32_MAX;
  uint64_t off=addr-BASE;
  if (off > CAP || count > CAP-off) return UINT32_MAX;
  return (uint32_t)off;
}
__attribute__((export_name("store32"))) int store32(uint64_t addr,uint32_t val){uint32_t o=translate(addr,4);if(o==UINT32_MAX)return 0;for(int i=0;i<4;i++)ram[o+i]=(uint8_t)(val>>(8*i));return 1;}
__attribute__((export_name("load32"))) uint32_t load32(uint64_t addr){uint32_t o=translate(addr,4);if(o==UINT32_MAX)return 0;uint32_t v=0;for(int i=0;i<4;i++)v|=(uint32_t)ram[o+i]<<(8*i);return v;}
__attribute__((export_name("valid"))) int valid(uint64_t addr,uint32_t n){return translate(addr,n)!=UINT32_MAX;}
__attribute__((export_name("byte"))) int byte(uint64_t addr){uint32_t o=translate(addr,1);return o==UINT32_MAX?-1:ram[o];}
__attribute__((export_name("capacity"))) int capacity(void){return CAP;}
