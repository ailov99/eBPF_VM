#include "VM.h"
#include "Opcodes.h"
#include <cstdio>


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
void VM::Eval()
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
    default:
    {
      
    }
  }
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

// "main" run routine for the VM
// Kicks off the Fetch->Decode->Eval
void VM::Run(const std::vector<uint64_t>& program)
{
  running = true;
  
  while (IsRunning())
  {
    // display register contents
    DisplayRegs();
    
    // fetch next instruction
    uint16_t instr = program[pc++];
    
    // decode fetched instruction
    Decode(instr);
    
    // evaluate
    Eval();
  }
}