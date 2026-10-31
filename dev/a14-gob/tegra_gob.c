#include "tegra_gob.h"
#include <stdint.h>
#include <string.h>
static int calc(unsigned w,unsigned h,unsigned bpe,unsigned gh,size_t *physical,size_t *linear){
 if(!physical||!linear||!w||!h||w>16384||h>16384||
   (bpe!=1&&bpe!=2&&bpe!=4&&bpe!=8&&bpe!=16)||!gh||gh>32||(gh&(gh-1)))return -1;
 if((size_t)w>SIZE_MAX/bpe)return -1;
 size_t row=(size_t)w*bpe;if((size_t)h>SIZE_MAX/row)return -1;
 size_t gx=(row+63u)/64u, rh=8u*(size_t)gh, my=((size_t)h+rh-1)/rh;
 if(gx>SIZE_MAX/512u/(size_t)gh)return -1;
 size_t macro=gx*512u*gh;if(my>SIZE_MAX/macro)return -1;
 *physical=macro*my;*linear=row*h;return 0;
}
int las_tegra_gob_size(unsigned w,unsigned h,unsigned bpe,unsigned gh,size_t *p,size_t *l){return calc(w,h,bpe,gh,p,l);}
static size_t pos(unsigned x,unsigned y,unsigned w,unsigned bpe,unsigned gh){
 size_t xb=(size_t)x*bpe, gx=((size_t)w*bpe+63u)/64u;
 size_t base=(((size_t)y/(8u*gh))*gx+xb/64u)*512u*gh+
             (((size_t)y%(8u*gh))/8u)*512u;
 return base+((xb%64u)/32u)*256u+((y%8u)/2u)*64u+
             ((xb%32u)/16u)*32u+(y%2u)*16u+xb%16u;
}
int las_tegra_gob_offset(unsigned x,unsigned y,unsigned w,unsigned h,unsigned bpe,unsigned gh,size_t *o){
 size_t p,l;if(!o||x>=w||y>=h||calc(w,h,bpe,gh,&p,&l))return -1;
 size_t offset=pos(x,y,w,bpe,gh);if(offset>p-bpe)return -1;*o=offset;return 0;
}
static int move(const uint8_t *src,size_t sl,uint8_t *dst,size_t dl,unsigned w,unsigned h,unsigned bpe,unsigned gh,int decode){
 size_t p,l;if(!src||!dst||calc(w,h,bpe,gh,&p,&l))return -1;
 if(sl<(decode?p:l)||dl<(decode?l:p))return -1;
 if(!decode)memset(dst,0,p);
 size_t row=(size_t)w*bpe;
 for(unsigned y=0;y<h;y++)for(unsigned x=0;x<w;x++){
   size_t a=pos(x,y,w,bpe,gh), b=(size_t)y*row+(size_t)x*bpe;
   if(decode)memcpy(dst+b,src+a,bpe);else memcpy(dst+a,src+b,bpe);
 }
 return 0;
}
int las_tegra_linear_to_gob(const uint8_t *s,size_t sl,uint8_t *d,size_t dl,unsigned w,unsigned h,unsigned b,unsigned gh){return move(s,sl,d,dl,w,h,b,gh,0);}
int las_tegra_gob_to_linear(const uint8_t *s,size_t sl,uint8_t *d,size_t dl,unsigned w,unsigned h,unsigned b,unsigned gh){return move(s,sl,d,dl,w,h,b,gh,1);}
