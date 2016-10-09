#pragma once

#include <cstdint>

class Register
{
private:
  uint64_t data;
  
public:
  Register();
  Register(uint64_t val);
  
  uint64_t Read64() const;
  uint32_t Read32() const;
  uint32_t ReadMS32() const;
  
  void Write64(uint64_t);
  void Write32(uint32_t);
  void WriteMS32(uint32_t);
};


