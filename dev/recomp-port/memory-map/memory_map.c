#include <stdint.h>
#include <stddef.h>
#include <string.h>
#define BASE_A UINT64_C(0x7100000000)
#define BASE_B UINT64_C(0x7200000000)
#define REGION_SIZE 4096u
static uint8_t region_a[REGION_SIZE];
static uint8_t region_b[REGION_SIZE];
static uint8_t permission_a = 3; /* read/write */
static uint8_t permission_b = 1; /* read only */
static unsigned faults;
static uint8_t* translate(uint64_t addr, uint32_t count, unsigned perm){
    uint64_t offset;
    if (addr>=BASE_A && (offset=addr-BASE_A)<=REGION_SIZE && count<=REGION_SIZE-offset) {
        if ((permission_a & perm)==perm) return region_a+(uint32_t)offset;
    }
    if (addr>=BASE_B && (offset=addr-BASE_B)<=REGION_SIZE && count<=REGION_SIZE-offset) {
        if ((permission_b & perm)==perm) return region_b+(uint32_t)offset;
    }
    faults++;
    return NULL;
}
__attribute__((export_name("reset_map"))) void reset_map(void){memset(region_a,0,sizeof region_a);memset(region_b,0,sizeof region_b);permission_a=3;permission_b=1;faults=0;}
__attribute__((export_name("store32"))) int store32(uint64_t addr,uint32_t v){uint8_t*p=translate(addr,4,2);if(!p)return 0;memcpy(p,&v,4);return 1;}
__attribute__((export_name("load32"))) uint32_t load32(uint64_t addr){uint8_t*p=translate(addr,4,1);uint32_t v=0;if(p)memcpy(&v,p,4);return v;}
__attribute__((export_name("valid"))) int valid(uint64_t addr,uint32_t n,uint32_t perm){uint32_t before=faults;int ok=translate(addr,n,perm)!=NULL;faults=before;return ok;}
__attribute__((export_name("fault_count"))) unsigned fault_count(void){return faults;}
__attribute__((export_name("set_permission_a"))) void set_permission_a(uint32_t p){permission_a=(uint8_t)(p&3);}
