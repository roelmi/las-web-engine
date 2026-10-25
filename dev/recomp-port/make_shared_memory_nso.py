#!/usr/bin/env python3
"""Copyright-free ARM64 two-function shared-memory fixture."""
from pathlib import Path
import struct
image=bytearray(0x1000)
def write32(pos,x): struct.pack_into("<I",image,pos,x)
def write64(pos,x): struct.pack_into("<Q",image,pos,x)
write32(4,0x300)
instructions=[0x52800501,0xb9000001,0x94000002,0xd65f03c0,0xb9400002,0x11000842,0xb9000002,0xd65f03c0]
for i,ins in enumerate(instructions): write32(0x100+i*4,ins)
image[0x300:0x304]=b"MOD0"
for i,target in enumerate([0x340,0x600,0x600,0,0,0]):
    struct.pack_into("<i",image,0x304+i*4,target-0x300)
for i,(tag,val) in enumerate([(5,0x440),(6,0x480),(4,0x4c0),(0,0)]):
    write64(0x340+i*16,tag)
    write64(0x348+i*16,val)
image[0x440:0x449]=b"\x00fixture\x00"
struct.pack_into("<IBBHQQ",image,0x480+24,1,2,0,1,0x100,16)
write32(0x4c0,1)
write32(0x4c4,2)
out=bytearray(0x1100)
out[:4]=b"NSO0"
def header(pos,x):struct.pack_into("<I",out,pos,x)
for off,val in [(0x10,0x100),(0x14,0),(0x18,0x120),(0x20,0x220),(0x24,0x120),(0x28,0xEE0),(0x30,0x1100),(0x34,0x1000),(0x38,0),(0x60,0x120),(0x64,0xEE0)]:header(off,val)
out[0x40:0x60]=bytes.fromhex("aabbccddeeff00112233445566778899aabbccddeeff00112233445566778899")
out[0x100:]=image
Path("synthetic.nso").write_bytes(out)
print("Generated test NSO with eight instructions.")
