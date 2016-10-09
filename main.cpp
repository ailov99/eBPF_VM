#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <sstream>

#include <cstdio>
#include <cstdlib>

#include "VM.h"
#include "Opcodes.h"

// Shift values
#define SHL_DST 8
#define SHL_SRC 12
#define SHL_OFF 16
#define SHL_IMM 32

uint64_t parseALU(std::string& op, std::string& op1, std::string& op2)
{
  uint64_t instr;
  
  // SRC
  if (op2[0] == 'r')
  {
    instr &= 
            (op == "add")   ? BPF_ADD_SRC
            : (op == "sub") ? BPF_SUB_SRC
            : (op == "mul") ? BPF_MUL_SRC
            : (op == "div") ? BPF_DIV_SRC
            : (op == "or")  ? BPF_OR_SRC
            : (op == "and") ? BPF_AND_SRC
            : (op == "lsh") ? BPF_LSH_SRC
            : (op == "rsh") ? BPF_RSH_SRC
            : (op == "mod") ? BPF_MOD_SRC
            : (op == "xor") ? BPF_XOR_SRC
            : (op == "mov") ? BPF_MOV_SRC
            : (op == "arsh")? BPF_ARSH_SRC
            : (op == "add32") ? BPF_ADD32_SRC
            : (op == "sub32") ? BPF_SUB32_SRC
            : (op == "mul32") ? BPF_MUL32_SRC
            : (op == "div32") ? BPF_DIV32_SRC
            : (op == "or32")  ? BPF_OR32_SRC
            : (op == "and32") ? BPF_AND32_SRC
            : (op == "lsh32") ? BPF_LSH32_SRC
            : (op == "rsh32") ? BPF_RSH32_SRC
            : (op == "mod32") ? BPF_MOD32_SRC
            : (op == "xor32") ? BPF_XOR32_SRC
            : (op == "mov32") ? BPF_MOV32_SRC
            : BPF_ARSH32_SRC;
            
    instr &= ((op1[1] - '0') << SHL_DST);  // reg
    instr &= ((op2[1] - '0') << SHL_SRC);  // reg
  }
  // IMM
  else
  {
    instr &= 
            (op == "add")   ? BPF_ADD_IMM
            : (op == "sub") ? BPF_SUB_IMM
            : (op == "mul") ? BPF_MUL_IMM
            : (op == "div") ? BPF_DIV_IMM
            : (op == "or")  ? BPF_OR_IMM
            : (op == "and") ? BPF_AND_IMM
            : (op == "lsh") ? BPF_LSH_IMM
            : (op == "rsh") ? BPF_RSH_IMM
            : (op == "mod") ? BPF_MOD_IMM
            : (op == "xor") ? BPF_XOR_IMM
            : (op == "mov") ? BPF_MOV_IMM
            : (op == "arsh")? BPF_ARSH_IMM
            : (op == "add32") ? BPF_ADD32_IMM
            : (op == "sub32") ? BPF_SUB32_IMM
            : (op == "mul32") ? BPF_MUL32_IMM
            : (op == "div32") ? BPF_DIV32_IMM
            : (op == "or32")  ? BPF_OR32_IMM
            : (op == "and32") ? BPF_AND32_IMM
            : (op == "lsh32") ? BPF_LSH32_IMM
            : (op == "rsh32") ? BPF_RSH32_IMM
            : (op == "mod32") ? BPF_MOD32_IMM
            : (op == "xor32") ? BPF_XOR32_IMM
            : (op == "mov32") ? BPF_MOV32_IMM
            : BPF_ARSH32_IMM;
    
    instr &= ((op1[1] - '0') << SHL_DST);  // reg
    
    unsigned x = strtoul(op2.substr(1).c_str(), NULL, 16);
    instr &= (x << SHL_IMM);  // imm
  }
  
  return instr;
}

std::vector<uint64_t> assemble(std::string& filename)
{
  std::vector<uint64_t> prog;
  
  std::ifstream instream(filename);
  if (instream == nullptr)
  {
    std::cout << "Could not find BPF source file." << std::endl;
  }
  
  while (!instream.eof())
  {
    uint64_t instr = 0x0;
    std::string op;
    std::string op1;
    std::string op2;
    instream >> op;
    
    // Switch on instruction
    if (op == "add" || op == "sub" || op == "mul" || op == "div" 
     || op == "or" || op == "and" || op == "lsh" || op == "rsh"
     || op == "mod" || op == "xor" || op == "mov" || op == "arsh")
    {
      instream >> op1;
      instream >> op2;
      instr = parseALU(op, op1, op2);
    }
    else if (op == "neg")
    {
      instream >> op1;
      instr &= BPF_NEG;
      instr &= ((op1[1] - '0') << SHL_DST);
    }
    else if (op == "add32" || op == "sub32" || op == "mul32" || op == "div32"
            || op == "or32" || op == "and32" || op == "lsh32" || op == "rsh32"
            || op == "mod32" || op == "xor32" || op == "mov32" || op == "arsh32")
    {
      instream >> op1;
      instream >> op2;
      instr = parseALU(op, op1, op2);
    }
    else if (op == "neg32")
    {
      instream >> op1;
      instr &= BPF_NEG32;
      instr &= ((op1[1] - '0') << SHL_DST);
    }
    
    prog.push_back(instr);
  }
  
  return prog;
}


int main(int argc, char** argv) 
{
  //std::unique_ptr<VM> vm = std::make_unique<VM>();
  
  std::string filename("bpf_source.bpf");
  std::vector<uint64_t> prog = assemble(filename);
  
  // feed assembled bytecode to the VM
  //std::vector<uint16_t> program;
  //program.push_back(0x1064);
  //program.push_back(0x11c8);
  //program.push_back(0x2201);
  //program.push_back(0x0000);
  //vm->Run(program);
  //run();
}
