/* LAS Web Engine v0.5: isolated CPU helper adaptation based on MIT-licensed
 * LASRecomp runtime/cpu.h, https://github.com/Nycolazs/LASRecomp-Android-Plus
 * No game assets or code included. */
#include <stdint.h>
static uint64_t nzcv;
static uint64_t mk(uint64_t n,uint64_t z,uint64_t cy,uint64_t v){return(n<<31)|(z<<30)|(cy<<29)|(v<<28);}
__attribute__((export_name("reset"))) void reset(void){nzcv=0;}
__attribute__((export_name("flags"))) uint32_t flags(void){return(uint32_t)nzcv;}
__attribute__((export_name("adds64"))) uint64_t adds64(uint64_t a,uint64_t b){uint64_t r=a+b;nzcv=mk(r>>63,r==0,r<a,((a^r)&(b^r))>>63);return r;}
__attribute__((export_name("subs64"))) uint64_t subs64(uint64_t a,uint64_t b){uint64_t r=a-b;nzcv=mk(r>>63,r==0,a>=b,((a^b)&(a^r))>>63);return r;}
__attribute__((export_name("adds32"))) uint32_t adds32(uint32_t a,uint32_t b){uint32_t r=a+b;nzcv=mk(r>>31,r==0,r<a,((a^r)&(b^r))>>31);return r;}
__attribute__((export_name("subs32"))) uint32_t subs32(uint32_t a,uint32_t b){uint32_t r=a-b;nzcv=mk(r>>31,r==0,a>=b,((a^b)&(a^r))>>31);return r;}
