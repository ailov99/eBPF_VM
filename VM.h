#pragma once

#include <cstdint>
#include <vector>
#include "Registers.h"

#define NUM_REGS 10
#define NUM_MEMSLOTS 16

class VM
{
private:
  /* --------------- State -----------------*/
  uint64_t pc;  // program counter
  bool running; // running/halt flag
  
  uint8_t _opcode;  // instruction opcode
  uint8_t _dst;     // destination
  uint8_t _src;     // source
  uint16_t _offset; // offset
  uint32_t _imm;    // immediate
  
  uint64_t _mem[NUM_MEMSLOTS];   // scratch memory
  
  struct Registers
  {
    Registers();
    
    // return value from in-kernel function, and 
    // exit value for eBPF program
    Register R0;
    
    // arguments from eBPF program to in-kernel function
    Register R1;
    Register R2;
    Register R3;
    Register R4;
    Register R5;
    
    // callee saved registers that in-kernel function 
    // will preserve
    Register R6;
    Register R7;
    Register R8;
    Register R9;
    
    // read-only frame pointer to access stack
    Register R10;
  } Regs;
  
  void Decode(const uint64_t);
  uint64_t Eval();
  
public:
  VM();
  uint64_t Run(const std::vector<uint64_t>&);
  bool IsRunning() const;
  void DisplayRegs() const;
  void DisplayState() const;
  void DisplayAll() const;
  
  uint64_t GetPc() const {return pc;};
  Register& GetReg(const unsigned);
  Register& R0() {return Regs.R0;};
  Register& R1() {return Regs.R1;};
  Register& R2() {return Regs.R2;};
  Register& R3() {return Regs.R3;};
  Register& R4() {return Regs.R4;};
  Register& R5() {return Regs.R5;};
  Register& R6() {return Regs.R6;};
  Register& R7() {return Regs.R7;};
  Register& R8() {return Regs.R8;};
  Register& R9() {return Regs.R9;};
  Register& R10(){return Regs.R10;};
};




