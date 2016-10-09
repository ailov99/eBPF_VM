#include <cstdio>
#include "VM.h"


VM::VM()
: pc(0), running(false)
{
 
}

VM::Registers::Registers()
: R0(0),R1(0),R2(0),R3(0),R4(0),R5(0),R6(0),
        R7(0),R8(0),R9(0),R10(0)
{
  
}

void VM::Decode(const uint16_t instr)
{
  R0().Write64((instr & 0xf000) >> 12);
  R1().Write64((instr & 0xf00) >> 8);
  R2().Write64((instr & 0xf0) >> 4);
  R3().Write64((instr & 0xf));
  R4().Write64((instr & 0xff));
}

void VM::Eval()
{
  switch(R0().Read64())
  {
    case 0x0:
      printf("HALT\n");;
      running = false;
      break;
    case 0x1:
      printf("ONE\n");
      break;
    default:
      break;
  }
}

Register& VM::GetRegister(const int num)
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

void VM::Run(const std::vector<uint16_t>& program)
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