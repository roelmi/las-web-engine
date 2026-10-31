#ifndef LAS_TEGRA_GOB_H
#define LAS_TEGRA_GOB_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
/* Switch Tegra X1 2D block-linear GOB: width/height in elements, not pixels.
   bpe=1,2,4,8,16. GOB height=1,2,4,8,16,32. No mips, layers or 3D. */
int las_tegra_gob_size(unsigned w,unsigned h,unsigned bpe,unsigned gh,size_t *physical,size_t *linear);
int las_tegra_gob_offset(unsigned x,unsigned y,unsigned w,unsigned h,unsigned bpe,unsigned gh,size_t *offset);
int las_tegra_linear_to_gob(const uint8_t *src,size_t sl,uint8_t *dst,size_t dl,unsigned w,unsigned h,unsigned bpe,unsigned gh);
int las_tegra_gob_to_linear(const uint8_t *src,size_t sl,uint8_t *dst,size_t dl,unsigned w,unsigned h,unsigned bpe,unsigned gh);
#ifdef __cplusplus
}
#endif
#endif
