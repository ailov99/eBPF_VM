#include "VM.h"
#include "Opcodes.h"
#include <cstdio>
#include <arpa/inet.h>


// Default constructor
VM::VM()
: pc(0), running(false), _opcode(0), _dst(0),
        _src(0), _offset(0), _imm(0)
{
 
}

// Registers struct default constructor
VM::Registers::Registers()
: R0(0),R1(0),R2(0),R3(0),R4(0),R5(0),R6(0),
        R7(0),R8(0),R9(0),R10(0)
{
  
}

// Decode an instruction
// sets up the internal VM state with data
// to be used during the following Eval stage
void VM::Decode(const uint64_t instr)
{
  _opcode = (instr & OP_MASK);
  _dst    = (instr & DST_MASK) >> SHL_DST;
  _src    = (instr & SRC_MASK) >> SHL_SRC;
  _offset = (instr & OFF_MASK) >> SHL_OFF;
  _imm    = (instr & IMM_MASK) >> SHL_IMM;
}

// Evaluate instruction using the set state
// A return value of 0 signifies no 
// program return -> carry on with next instruction
// Otherwise, we assume caller has loaded a return value in R0
uint64_t VM::Eval()
{
  switch(_opcode)
  {
    case BPF_ADD_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() + _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_ADD_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() + GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_SUB_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() - _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_SUB_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() - GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_MUL_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() * _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_MUL_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() * GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_DIV_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() / _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_DIV_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() / GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_OR_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() | _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_OR_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() | GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_AND_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() & _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_AND_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() & GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_LSH_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() << _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_LSH_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() << GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_RSH_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() >> _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_RSH_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() >> GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_NEG:
    {
     uint64_t res = ~(GetReg(_dst).Read64());
     GetReg(_dst).Write64(res);
     break;
    }
    case BPF_MOD_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() % _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_MOD_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() % GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_XOR_IMM:
    {
      uint64_t res = GetReg(_dst).Read64() ^ _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_XOR_SRC:
    {
      uint64_t res = GetReg(_dst).Read64() ^ GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_MOV_IMM:
    {
      GetReg(_dst).Write64(_imm);
      break;
    }
    case BPF_MOV_SRC:
    {
      GetReg(_dst).Write64(GetReg(_src).Read64());
      break;
    }
    case BPF_ARSH_IMM:
    {
      uint64_t res = ((int64_t)GetReg(_dst).Read64()) >> _imm;
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_ARSH_SRC:
    {
      uint64_t res = ((int64_t)GetReg(_dst).Read64()) >> GetReg(_src).Read64();
      GetReg(_dst).Write64(res);
      break;
    }
    case BPF_ADD32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() + _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_ADD32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() + GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_SUB32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() - _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_SUB32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() - GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_MUL32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() * _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_MUL32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() * GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_DIV32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() / _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_DIV32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() / GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_OR32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() | _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_OR32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() | GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_AND32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() & _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_AND32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() & GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_LSH32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() << _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_LSH32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() << GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_RSH32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() >> _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_RSH32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() >> GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_NEG32:
    {
      uint32_t res = ~(GetReg(_dst).Read32());
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_MOD32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() % _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_MOD32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() % GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_XOR32_IMM:
    {
      uint32_t res = GetReg(_dst).Read32() ^ _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_XOR32_SRC:
    {
      uint32_t res = GetReg(_dst).Read32() ^ GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_MOV32_IMM:
    {
      GetReg(_dst).Write32(_imm);
      break;
    }
    case BPF_MOV32_SRC:
    {
      GetReg(_dst).Write32(GetReg(_src).Read32());
      break;
    }
    case BPF_ARSH32_IMM:
    {
      uint32_t res = ((int32_t)GetReg(_dst).Read32()) >> _imm;
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_ARSH32_SRC:
    {
      uint32_t res = ((int32_t)GetReg(_dst).Read32()) >> GetReg(_src).Read32();
      GetReg(_dst).Write32(res);
      break;
    }
    case BPF_LE:
    {
      switch (_imm)
      {
        case 16:
        {
          uint16_t lsHw = (GetReg(_dst).Read64() & 0xffff);
          lsHw = ntohs(lsHw);
          uint32_t res = lsHw;
          GetReg(_dst).Write32(res);
          break;
        }
        case 32:
        {
          uint32_t res = GetReg(_dst).Read32();
          res = ntohl(res);
          GetReg(_dst).Write32(res);
          break;
        }
        case 64:
        default:
        {
          uint64_t res = ntohl(GetReg(_dst).ReadMS32());
          res = (res << 32) | ntohl(GetReg(_dst).Read32());
          GetReg(_dst).Write64(res);
          break;
        }
      }
      break;
    }
    case BPF_BE:
    {
        switch (_imm)
      {
        case 16:
        {
          uint16_t lsHw = (GetReg(_dst).Read64() & 0xffff);
          lsHw = htons(lsHw);
          uint32_t res = lsHw;
          GetReg(_dst).Write32(res);
          break;
        }
        case 32:
        {
          uint32_t res = GetReg(_dst).Read32();
          res = htonl(res);
          GetReg(_dst).Write32(res);
          break;
        }
        case 64:
        default:
        {
          uint64_t res = htonl(GetReg(_dst).ReadMS32());
          res = (res << 32) | htonl(GetReg(_dst).Read32());
          GetReg(_dst).Write64(res);
          break;
        }
      }
      break;
    }
    case BPF_JA:
    {
      pc += _offset;
      break;
    }
    case BPF_JEQ_IMM:
    {
      if (GetReg(_dst).Read64() == _imm)
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JEQ_SRC:
    {
      if (GetReg(_dst).Read64() == GetReg(_src).Read64())
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JGT_IMM:
    {
      if (GetReg(_dst).Read64() > _imm)
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JGT_SRC:
    {
      if (GetReg(_dst).Read64() > GetReg(_src).Read64())
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JGE_IMM:
    {
      if (GetReg(_dst).Read64() >= _imm)
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JGE_SRC:
    {
      if (GetReg(_dst).Read64() >= GetReg(_src).Read64())
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JSET_IMM:
    {
      if (GetReg(_dst).Read64() & _imm)
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JSET_SRC:
    {
      if (GetReg(_dst).Read64() & GetReg(_src).Read64())
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JNE_IMM:
    {
      if (GetReg(_dst).Read64() != _imm)
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JNE_SRC:
    {
      if (GetReg(_dst).Read64() != GetReg(_src).Read64())
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JSGT_IMM:
    {
      if ((int64_t)GetReg(_dst).Read64() > _imm)
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JSGT_SRC:
    {
      if ((int64_t)GetReg(_dst).Read64() > (int64_t)GetReg(_src).Read64())
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JSGE_IMM:
    {
      if ((int64_t)GetReg(_dst).Read64() >= _imm)
      {
        pc += _offset;
      }
      break;
    }
    case BPF_JSGE_SRC:
    {
      if ((int64_t)GetReg(_dst).Read64() >= (int64_t)GetReg(_src).Read64())
      {
        pc += _offset;
      }
      break;
    }
    case BPF_CALL_IMM:
    {
      pc = _imm;
      break;
    }
    case BPF_EXIT:
    {
      // caller loaded R0
      return R0().Read64();
      break;
    }
    case BPF_LDDW:
    {
      GetReg(_dst).Write64(_imm);
      break;
    }
    case BPF_LDXW:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _src + _offset;
      uint32_t offWord = *((uint32_t*)pByte);
      GetReg(_dst).Write32(offWord);
      break;
    }
    case BPF_LDXH:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _src + _offset;
      uint32_t offWord = *((uint16_t*)pByte);
      GetReg(_dst).Write32(offWord);
      break;
    }
    case BPF_LDXB:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _src + _offset;
      uint32_t offWord = *((uint8_t*)pByte);
      GetReg(_dst).Write32(offWord);
      break;
    }
    case BPF_LDXDW:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _src + _offset;
      uint64_t offWord = *((uint64_t*)pByte);
      GetReg(_dst).Write64(offWord);
      break;
    }
    case BPF_STW:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _dst + _offset;
      uint32_t stWord = _imm & 0xffffffff;
      *pByte = stWord;
      break;
    }
    case BPF_STH:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _dst + _offset;
      uint16_t stHword = _imm & 0xffff;
      *pByte = stHword;
      break;
    }
    case BPF_STB:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _dst + _offset;
      uint8_t stByte = _imm & 0xff;
      *pByte = stByte;
      break;
    }
    case BPF_STDW:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _dst + _offset;
      uint64_t stDword = _imm;
      *pByte = stDword;
      break;
    }
    case BPF_STXW:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _dst + _offset;
      uint32_t stWord = GetReg(_src).Read32();
      *pByte = stWord;
      break;
    }
    case BPF_STXH:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _dst + _offset;
      uint16_t stHword = GetReg(_src).Read32() & 0xffff;
      *pByte = stHword;
      break;
    }
    case BPF_STXB:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _dst + _offset;
      uint8_t stByte = GetReg(_src).Read32() & 0xff;
      *pByte = stByte;
      break;
    }
    case BPF_STXDW:
    {
      unsigned char *pByte = reinterpret_cast<unsigned char*>(_mem);
      pByte += _dst + _offset;
      uint64_t stDword = GetReg(_src).Read64();
      *pByte = stDword;
      break;
    }
    case BPF_LDABSW:
    case BPF_LDABSH:
    case BPF_LDABSB:
    case BPF_LDABSDW:
    case BPF_LDINDW:
    case BPF_LDINDH:
    case BPF_LDINDB:
    case BPF_LDINDDW:
    {
      printf("Unsupported instruction: LDIND | LDABS\n");
      break;
    }
    default:
    {
      printf("Could not evaluate instruction: %016X\n", _opcode);
      return 0;
      break;
    }
  }
  
  return 0;
}

// Register getter using an index argument.
// This is useful since we rely on the register
// numbering set into _dst and _src
Register& VM::GetReg(const unsigned num)
{
  switch(num)
  {
    case 0: return Regs.R0; break;
    case 1: return Regs.R1; break;
    case 2: return Regs.R2; break;
    case 3: return Regs.R3; break;
    case 4: return Regs.R4; break;
    case 5: return Regs.R5; break;
    case 6: return Regs.R6; break;
    case 7: return Regs.R7; break;
    case 8: return Regs.R8; break;
    case 9: return Regs.R9; break;
    case 10: return Regs.R10; break;
    default: return Regs.R0; break;
  }
}

bool VM::IsRunning() const
{
  return running;
}

// Display all register state
void VM::DisplayRegs() const
{
  printf("R0: %016X\n", Regs.R0.Read64());
  printf("R1: %016X\n", Regs.R1.Read64());
  printf("R2: %016X\n", Regs.R2.Read64());
  printf("R3: %016X\n", Regs.R3.Read64());
  printf("R4: %016X\n", Regs.R4.Read64());
  printf("R5: %016X\n", Regs.R5.Read64());
  printf("R6: %016X\n", Regs.R6.Read64());
  printf("R7: %016X\n", Regs.R7.Read64());
  printf("R8: %016X\n", Regs.R8.Read64());
  printf("R9: %016X\n", Regs.R9.Read64());
  printf("R10: %016X\n", Regs.R10.Read64());
}

// Display VM state variables
void VM::DisplayState() const
{
  printf("pc : %016X\n", pc);
  printf("run: %c\n", running ? 'T' : 'F');
  printf("op : %016X\n", _opcode);
  printf("dst: %016X\n", _dst);
  printf("src: %016X\n", _src);
  printf("off: %016X\n", _offset);
  printf("imm: %016X\n", _imm);
}

// Display all info (VM state + all regs)
void VM::DisplayAll() const
{
  DisplayState();
  DisplayRegs();
}

// "main" run routine for the VM
// Kicks off the Fetch->Decode->Eval
uint64_t VM::Run(const std::vector<uint64_t>& program)
{
  running = true;
  
  while (IsRunning())
  {
    // display register contents
    DisplayRegs();
    
    // fetch next instruction
    uint64_t instr = program[pc++];
    
    // decode fetched instruction
    Decode(instr);
    
    // evaluate
    if (Eval() != 0)
    {
      // program returned -> ret value is in R0
      running = false;
    }
  }
  
  // pass ret value
  return R0().Read64();
}