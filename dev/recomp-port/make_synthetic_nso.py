#!/usr/bin/env python3
"""Minimal copyright-free NSO fixture with one AArch64 function.
Original ARM64: mov w0, #42; ret. This is NOT Nintendo content.
"""
from pathlib import Path
import struct
image=bytearray(0x1000)
def u32(pos,x): struct.pack_into("<I",image,pos,x)
def u64(pos,x): struct.pack_into("<Q",image,pos,x)
# NSO text contains headers plus instructions; remaining metadata is rodata, not executable.
u32(4,0x300)
u32(0x100,0x52800540) # mov w0,#42
u32(0x104,0xd65f03c0) # ret
image[0x300:0x304]=b"MOD0"
for n,target in enumerate([0x340,0x600,0x600,0,0,0]):
    struct.pack_into("<i",image,0x304+n*4,target-0x300)
# Dynamic table: DT_STRTAB, DT_SYMTAB, DT_HASH, DT_NULL
for i,(tag,val) in enumerate([(5,0x440),(6,0x480),(4,0x4c0),(0,0)]):
    u64(0x340+i*16,tag);u64(0x348+i*16,val)
image[0x440:0x449]=b"\x00fixture\x00"
# symbol table: one null symbol; one function at guest offset 0x100
struct.pack_into("<IBBHQQ",image,0x480+24,1,2,0,1,0x100,8)
u32(0x4c0,1);u32(0x4c4,2)
raw=bytearray(0x100+0x1000)
raw[:4]=b"NSO0"
# text segment is 0x1000 bytes and other two segments empty
u32raw=lambda o,n:struct.pack_into("<I",raw,o,n)
u32raw(0x10,0x100);u32raw(0x14,0);u32raw(0x18,0x108)
u32raw(0x20,0x208);u32raw(0x24,0x108);u32raw(0x28,0xEF8)
u32raw(0x30,0x1100);u32raw(0x34,0x1000);u32raw(0x38,0)
u32raw(0x60,0x108);u32raw(0x64,0xEF8)
raw[0x40:0x60]=bytes.fromhex("aabbccddeeff00112233445566778899aabbccddeeff00112233445566778899")
raw[0x100:]=image
out=Path("synthetic.nso")
out.write_bytes(raw)
print("Created",out,"bytes",len(raw))
