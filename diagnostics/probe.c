#include <stdint.h>
__attribute__((export_name("mix"))) uint32_t mix(uint32_t n){uint32_t x=0x12345678u;for(uint32_t i=0;i<n;i++)x=(x^i)*1664525u+1013904223u;return x;}
