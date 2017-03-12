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

// Useful globals
std::string filename;
uint16_t curr_line;

// Use the supplied opcode and 2 operands to construct the bytecode
// for a whole ALU instruction.
// This works for both 32 and 64 bit instructions.
//
// op  - opcode
// op1 - dst (reg)
// op2 - src (reg OR imm)
uint64_t parseALU(std::string& op, std::string& op1, std::string& op2)
{
  uint64_t instr = 0x0;
  
  // SRC format
  if (op2[0] == 'r' || op2[0] == 'R')
  {
    instr |= 
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
            
    instr |= ((op1[1] - '0') << SHL_DST);  // dst reg
    instr |= ((op2[1] - '0') << SHL_SRC);  // src reg
  }
  // IMM format
  else
  {
    instr |= 
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
    
    instr |= ((op1[1] - '0') << SHL_DST);  // dst reg
    
    uint64_t imm_value = strtoul(op2.substr(1).c_str(), NULL, 16);
    instr |= (imm_value << SHL_IMM);  // imm
  }
  
  return instr;
}

// Parses entire source file looking for the line number of the
// supplied label.
// Returns the offset required for the PC to move to the label.
// This is used for branching instructions that use labels.
// TODO: consider a table to cache these for programs with lots of jumps
//
// label - label we are looking for
uint16_t seekLabel(std::string& label)
{
  std::ifstream file(filename);
  if (!file)
  {
    return 0;
  }
  
  std::string line;
  uint16_t line_num = 0;
  
  while (std::getline(file, line))
  {
    line_num++;
    if (line_num <= curr_line)
    {
      // Skip all lines prior to the one the call is coming from
      continue;
    }
    
    if (line == (label + ":"))
    {
      // label found
      return (line_num - curr_line);
    }
  }

  return 0;
}

// Parses a 3-operand branching instruction
//
// op  - opcode
// op1 - dst (reg)
// op2 - src (reg OR imm)
// op3 - offset
uint64_t parseBranch(std::string& op, std::string& op1, std::string& op2, std::string& op3)
{
  uint64_t instr = 0x0;
  
  // SRC format
  if (op2[0] == 'r' || op2[0] == 'R')
  {
    instr |=
            (op == "jeq")   ? BPF_JEQ_SRC
            : (op == "jgt") ? BPF_JGT_SRC
            : (op == "jge") ? BPF_JGE_SRC
            : (op == "jset")? BPF_JSET_SRC
            : (op == "jne") ? BPF_JNE_SRC
            : (op == "jsgt")? BPF_JSGT_SRC
            : BPF_JSGE_SRC;
    
    
    instr |= ((op2[1] - '0') << SHL_SRC);  // src reg
  }
  // IMM format
  else
  {
    instr |=
            (op == "jeq")   ? BPF_JEQ_IMM
            : (op == "jgt") ? BPF_JGT_IMM
            : (op == "jge") ? BPF_JGE_IMM
            : (op == "jset")? BPF_JSET_IMM
            : (op == "jne") ? BPF_JNE_IMM
            : (op == "jsgt")? BPF_JSGT_IMM
            : BPF_JSGE_IMM;
    
    uint64_t imm_value = strtoul(op2.substr(1).c_str(), NULL, 16);
    instr |= (imm_value << SHL_IMM);  // imm  
  }
  
  instr |= ((op1[1] - '0') << SHL_DST);  // dst reg
  instr |= (seekLabel(op3) << SHL_OFF); // offset   

  return instr;
}

// Parses a 3-operand packet access instruction.
// This is used for the BPF_IND and BPF_ABS families of instructions.
// Check kernel docs for more information.
//
// op  - opcode
// op1 - src (reg)
// op2 - dst (reg)
// op3 - imm
uint64_t parsePktAccess(std::string& op, std::string& op1, std::string& op2, std::string& op3)
{
  uint64_t instr = 0x0;
  
  instr |= 
          (op == "ldabsw") ? BPF_LDABSW
          : (op == "ldabsh") ? BPF_LDABSH
          : (op == "ldabsb") ? BPF_LDABSB
          : (op == "ldabsdw") ? BPF_LDABSDW
          : (op == "ldindw") ? BPF_LDINDW
          : (op == "ldindh") ? BPF_LDINDH
          : (op == "ldindb") ? BPF_LDINDB
          : BPF_LDINDDW;
  
  instr |= ((op1[1] - '0') << SHL_SRC);  // src reg
  instr |= ((op2[1] - '0') << SHL_DST);  // dst reg
  uint64_t imm_value = strtoul(op3.substr(1).c_str(), NULL, 16);
  instr |= (imm_value << SHL_IMM);  // imm  
  
  return instr;
}

// Parses a 3-operand LDX instruction
//
// op  - opcode
// op1 - dst (reg)
// op2 - src (reg)
// op3 - offset
uint64_t parseLdx(std::string& op, std::string& op1, std::string& op2, std::string& op3)
{
  uint64_t instr = 0x0;
  instr |=
          (op == "ldxw") ? BPF_LDXW
          : (op == "ldxh") ? BPF_LDXH
          : (op == "ldxb") ? BPF_LDXB
          : BPF_LDXDW;
  
  instr |= ((op2[2] - '0') << SHL_SRC);  // src reg
  instr |= ((op1[1] - '0') << SHL_DST);  // dst reg
  uint64_t offset_value = strtoul(op3.substr(1).c_str(), NULL, 16);
  instr |= (offset_value << SHL_OFF);  // offset 
  
  return instr;
}

// Parses 3-operand store immediate value
//
// op  - opcode
// op1 - dst (reg)
// op2 - offset
// op3 - imm
uint64_t parseStImm(std::string op, std::string op1, std::string op2, std::string op3)
{
  uint64_t instr = 0x0;
  instr |=
          (op == "stw") ? BPF_STW
          : (op == "sth") ? BPF_STH
          : (op == "stb") ? BPF_STB
          : BPF_STDW;
  
  instr |= ((op1[2] - '0') << SHL_DST);  // dst reg
  uint64_t offset_value = strtoul(op2.substr(1).c_str(), NULL, 16);
  instr |= (offset_value << SHL_OFF);  // offset 
  uint64_t imm_value = strtoul(op3.substr(1).c_str(), NULL, 16);
  instr |= (imm_value << SHL_IMM);  // imm  
  
  return instr;
}

// Parses a 3-operand store reg value
//
// op  - opcode
// op1 - dst (reg)
// op2 - offset
// op3 - src (reg)
uint64_t parseStSrc(std::string op, std::string op1, std::string op2, std::string op3)
{
  uint64_t instr = 0x0;
  instr |=
          (op == "stxw") ? BPF_STXW
          : (op == "stxdw") ? BPF_STXDW
          : (op == "stxb") ? BPF_STXB
          : BPF_STXH;
  
  instr |= ((op1[2] - '0') << SHL_DST);  // dst reg
  uint64_t offset_value = strtoul(op2.substr(1).c_str(), NULL, 16);
  instr |= (offset_value << SHL_OFF);  // offset 
  instr |= ((op3[1] - '0') << SHL_SRC);  // src reg
  
  return instr;
}

// Parses source file and assembles all instructions into 
// bytecode.
// Note: Leave spaces around punctuation: ',' '+'
// TODO: Make this robust enough to handle code not conforming with above
std::vector<uint64_t> assemble()
{
  std::vector<uint64_t> prog;
  
  std::ifstream instream(filename);
  if (!instream)
  {
    std::cout << "Could not find BPF source file." << std::endl;
  }
  
  // Pattern is :
  // 1. Fetch instruction mnemonic
  // 2. Fetch as many operands as needed by instruction
  // 3. Emit bytecode
  // 4. Repeat
  while (!instream.eof())
  {
    curr_line++; // keep track of current line via global
    
    uint64_t instr = 0x0;
    
    // Parse opcode (mnemonic)
    std::string op;
    instream >> op;
    
    // Operands
    std::string op1;
    std::string op2;
    
    // Parse different instruction formats
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
      instr |= BPF_NEG;
      instr |= ((op1[1] - '0') << SHL_DST);
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
      instr |= BPF_NEG32;
      instr |= ((op1[1] - '0') << SHL_DST);
    }
    else if (op == "le16")
    {
      instream >> op1;
      instr |= BPF_LE;
      instr |= ((op1[1] - '0') << SHL_DST);
      instr |= (0x10UL << SHL_IMM);  // imm 
    }
    else if (op == "le32")
    {
      instream >> op1;
      instr |= BPF_LE;
      instr |= ((op1[1] - '0') << SHL_DST);
      instr |= (0x20UL << SHL_IMM);  // imm 
    }
    else if (op == "le64")
    {
      instream >> op1;
      instr |= BPF_LE;
      instr |= ((op1[1] - '0') << SHL_DST);
      instr |= (0x40UL << SHL_IMM);  // imm 
    }
    else if (op == "be16")
    {
      instream >> op1;
      instr |= BPF_BE;
      instr |= ((op1[1] - '0') << SHL_DST);
      instr |= (0x10UL << SHL_IMM);  // imm 
    }
    else if (op == "be32")
    {
      instream >> op1;
      instr |= BPF_BE;
      instr |= ((op1[1] - '0') << SHL_DST);
      instr |= (0x20UL << SHL_IMM);  // imm 
    }
    else if(op == "be64")
    {
      instream >> op1;
      instr |= BPF_BE;
      instr |= ((op1[1] - '0') << SHL_DST);
      instr |= (0x40UL << SHL_IMM);  // imm 
    }
    else if (op == "ja")
    {
      instream >> op1;
      instr |= BPF_JA;
      uint64_t lbl_line = seekLabel(op1);
      instr |= (lbl_line << SHL_OFF);
    }
    else if (op == "jeq" || op == "jgt" || op == "jge" || op == "jset"
            || op == "jne" || op == "jsgt" || op == "jsge")
    {
      instream >> op1;
      instream >> op2;
      op2.pop_back(); // erase ','
      // we need an extra operand for the label
      std::string op3;
      instream >> op3;
      instr = parseBranch(op, op1, op2, op3);
    }
    else if (op == "call")
    {
      instream >> op1;
      instr |= BPF_CALL_IMM;
      uint64_t imm_value = strtoul(op1.substr(1).c_str(), NULL, 16);
      instr |= (imm_value << SHL_IMM);
    }
    else if (op == "exit")
    {
      instr |= BPF_EXIT;
    }
    // label case 
    else if (op.back() == ':')
    {
      // discard it
      // Note: labels are only relevant to branching
      continue;
    }
    else if (op == "lddw")
    {
      instr |= BPF_LDDW;
      instream >> op1;
      instr |= ((op1[1] - '0') << SHL_DST);
      instream >> op2;
      uint64_t imm_value = strtoul(op2.substr(1).c_str(), NULL, 16);
      instr |= (imm_value << SHL_IMM);
    }
    else if (op == "ldabsw" || op == "ldabsh" || op == "ldabsb" || op == "ldabsdw"
            || op == "ldindw" || op == "ldindh" || op == "ldindb" || op == "ldinddw")
    {
      instream >> op1;
      instream >> op2;
      std::string op3;
      instream >> op3;
      instr = parsePktAccess(op, op1, op2, op3);
    }
    else if (op == "ldxw" || op == "ldxh" || op == "ldxb" || op == "ldxdw")
    {
      instream >> op1;
      instream >> op2; // [RX
      std::string op3;
      instream >> op3; // +
      instream >> op3; // #offset]
      op3.pop_back(); // erase ']'
      instr = parseLdx(op, op1, op2, op3);
    }
    else if (op == "stw" || op == "sth" || op == "stb" || op == "stdw")
    {
      instream >> op1;
      instream >> op2; // +
      instream >> op2;
      op2.pop_back(); // erase ']'
      std::string op3;
      instream >> op3;
      instr = parseStImm(op, op1, op2, op3);
    }
    else if (op == "stxw" || op == "stxh" || op =="stxb" || op == "stxdw")
    {
      instream >> op1;
      instream >> op2; // +
      instream >> op2;
      op2.pop_back(); // erase ']'
      op2.pop_back(); // erase ','
      std::string op3;
      instream >> op3;
      instr = parseStSrc(op, op1, op2, op3);
    }
    else if (op == ";;")
    {
      // Skip comments
      std::string line;
      std::getline(instream, line);
      continue;
    }
    else
    {
      std::cout << "Bad instruction: " << op 
              << ". Exiting parse routine..." << std::endl;
      break;
    }
    
    // Add the generated instruction to the program
    prog.push_back(instr);
  }
  
  return prog;
}


int main(int argc, char** argv) 
{
  //std::unique_ptr<VM> vm = std::make_unique<VM>();
  VM vm = VM();
  filename = "bpf_source.bpf";
  std::vector<uint64_t> prog = assemble();
  
  vm.Run(prog);
  // feed assembled bytecode to the VM
  //std::vector<uint16_t> program;
  //program.push_back(0x1064);
  //program.push_back(0x11c8);
  //program.push_back(0x2201);
  //program.push_back(0x0000);
  //vm->Run(program);
  //run();
}
