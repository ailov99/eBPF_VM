#pragma once

// Shift values
#define SHL_DST 8UL
#define SHL_SRC 12UL
#define SHL_OFF 16UL
#define SHL_IMM 32UL

// Masks
#define OP_MASK  0xFF
#define DST_MASK 0xF00
#define SRC_MASK 0xF000
#define OFF_MASK 0xFFFF0000
#define IMM_MASK 0xFFFFFFFF00000000

/* --------------------- ALU 64-bit -------------------------- */
#define BPF_ADD_IMM  0x07
#define BPF_ADD_SRC  0x0f
#define BPF_SUB_IMM  0x17
#define BPF_SUB_SRC  0x1f
#define BPF_MUL_IMM  0x27
#define BPF_MUL_SRC  0x2f
#define BPF_DIV_IMM  0x37
#define BPF_DIV_SRC  0x3f
#define BPF_OR_IMM   0x47
#define BPF_OR_SRC   0x4f
#define BPF_AND_IMM  0x57
#define BPF_AND_SRC  0x5f
#define BPF_LSH_IMM  0x67
#define BPF_LSH_SRC  0x6f
#define BPF_RSH_IMM  0x77
#define BPF_RSH_SRC  0x7f
#define BPF_NEG      0x87
#define BPF_MOD_IMM  0x97
#define BPF_MOD_SRC  0x9f
#define BPF_XOR_IMM  0xa7
#define BPF_XOR_SRC  0xaf
#define BPF_MOV_IMM  0xb7
#define BPF_MOV_SRC  0xbf
#define BPF_ARSH_IMM 0xc7
#define BPF_ARSH_SRC 0xcf

/* --------------------- ALU 64-bit -------------------------- */
/* These use only the lower 32 bits of their operands and zero the 
   upper 32 bits of the destination register. */
#define BPF_ADD32_IMM  0x04
#define BPF_ADD32_SRC  0x0c
#define BPF_SUB32_IMM  0x14
#define BPF_SUB32_SRC  0x1c
#define BPF_MUL32_IMM  0x24
#define BPF_MUL32_SRC  0x2c
#define BPF_DIV32_IMM  0x34
#define BPF_DIV32_SRC  0x3c
#define BPF_OR32_IMM   0x44
#define BPF_OR32_SRC   0x4c
#define BPF_AND32_IMM  0x54
#define BPF_AND32_SRC  0x5c
#define BPF_LSH32_IMM  0x64
#define BPF_LSH32_SRC  0x6c
#define BPF_RSH32_IMM  0x74
#define BPF_RSH32_SRC  0x7c
#define BPF_NEG32      0x84
#define BPF_MOD32_IMM  0x94
#define BPF_MOD32_SRC  0x9c
#define BPF_XOR32_IMM  0xa4
#define BPF_XOR32_SRC  0xac
#define BPF_MOV32_IMM  0xb4
#define BPF_MOV32_SRC  0xbc
#define BPF_ARSH32_IMM 0xc4
#define BPF_ARSH32_SRC 0xcc

/* ------------------ Byteswap --------------------- */
// (imm == 16) -> le16 dst -> dst = htole16(dst)
// (imm == 32) -> le32 dst -> dst = htole32(dst)
// (imm == 64) -> le64 dst -> dst = htole64(dst)
#define BPF_LE 0xd4

// (imm == 16) -> be16 dst -> dst = htobe16(dst)
// (imm == 32) -> be32 dst -> dst = htobe32(dst)
// (imm == 64) -> be64 dst -> dst = htobe64(dst)
#define BPF_BE 0xdc

/* -------------------- Memory Instructions ------------------ */
#define BPF_LDDW    0x18 // dst = imm
#define BPF_LDABSW  0x20 // kernel
#define BPF_LDABSH  0x28 // kernel
#define BPF_LDABSB  0x30 // kernel
#define BPF_LDABSDW 0x38 // kernel
#define BPF_LDINDW  0x40 // kernel
#define BPF_LDINDH  0x48 // kernel
#define BPF_LDINDB  0x50 // kernel
#define BPF_LDINDDW 0x58 // kernel
#define BPF_LDXW    0x61 // dst = *(uint32_t *) (src + off)
#define BPF_LDXH    0x69 // dst = *(uint16_t *) (src + off)
#define BPF_LDXB    0x71 // dst = *(uint8_t *) (src + off)
#define BPF_LDXDW   0x79 // dst = *(uint64_t *) (src + off)
#define BPF_STW     0x62 // *(uint32_t *) (dst + off) = imm
#define BPF_STH     0x6a // *(uint16_t *) (dst + off) = imm
#define BPF_STB     0x72 // *(uint8_t *) (dst + off) = imm
#define BPF_STDW    0x7a // *(uint64_t *) (dst + off) = imm
#define BPF_STXW    0x63 // *(uint32_t *) (dst + off) = src
#define BPF_STXH    0x6b // *(uint16_t *) (dst + off) = src
#define BPF_STXB    0x73 // *(uint8_t *) (dst + off) = src
#define BPF_STXDW   0x7b // *(uint64_t *) (dst + off) = src

/* ------------------- Branch Instructions --------------- */
#define BPF_JA       0x05
#define BPF_JEQ_IMM  0x15
#define BPF_JEQ_SRC  0x1d
#define BPF_JGT_IMM  0x25
#define BPF_JGT_SRC  0x2d
#define BPF_JGE_IMM  0x35
#define BPF_JGE_SRC  0x3d
#define BPF_JSET_IMM 0x45
#define BPF_JSET_SRC 0x4d
#define BPF_JNE_IMM  0x55
#define BPF_JNE_SRC  0x5d
#define BPF_JSGT_IMM 0x65
#define BPF_JSGT_SRC 0x6d
#define BPF_JSGE_IMM 0x75
#define BPF_JSGE_SRC 0x7d
#define BPF_CALL_IMM 0x85
#define BPF_EXIT     0x95


