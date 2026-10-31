#include "tegra_gob.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void){
 size_t o,p,l;unsigned n=0;
 assert(las_tegra_gob_offset(1,0,5,9,16,1,&o)==0&&o==32);
 assert(las_tegra_gob_offset(2,0,5,9,16,1,&o)==0&&o==256);
 assert(las_tegra_gob_offset(0,8,5,9,16,1,&o)==0&&o==1024);
 assert(las_tegra_gob_offset(4,0,5,9,16,1,&o)==0&&o==512);
 for(unsigned w=1;w<=19;w+=3)for(unsigned h=1;h<=23;h+=4)
 for(unsigned b=1;b<=16;b*=2)for(unsigned gh=1;gh<=8;gh*=2){
  assert(las_tegra_gob_size(w,h,b,gh,&p,&l)==0);
  uint8_t *a=malloc(l),*t=malloc(p),*c=malloc(l);assert(a&&t&&c);
  for(size_t i=0;i<l;i++)a[i]=(uint8_t)(i*31u+w+h);
  assert(las_tegra_linear_to_gob(a,l,t,p,w,h,b,gh)==0);
  assert(las_tegra_gob_to_linear(t,p,c,l,w,h,b,gh)==0);
  assert(memcmp(a,c,l)==0);
  free(a);free(t);free(c);n++;
 }
 assert(las_tegra_gob_size(0,1,8,1,&p,&l)==-1);
 assert(las_tegra_gob_size(2,1,3,1,&p,&l)==-1);
 assert(las_tegra_gob_size(2,1,8,3,&p,&l)==-1);
 printf("PASS GOB offsets, invalid arguments and %u roundtrips\n",n);
 return 0;
}
