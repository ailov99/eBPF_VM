#include "Registers.h"

Register::Register() 
: data(0)
{};

Register::Register(uint64_t initValue) 
: data(initValue)
{};

uint64_t Register::Read64() const
{
  return data;
}

uint32_t Register::Read32() const
{
  uint32_t ls = data & (0x0f << 28);
  
  return ls;
}

uint32_t Register::ReadMS32() const
{
  uint32_t ms = (data & (0xf << 28)) >> 32;
  
  return ms;
}

void Register::Write64(uint64_t newVal)
{
  data = newVal;
}

void Register::Write32(uint32_t newVal)
{
  data = newVal;
}

void Register::WriteMS32(uint32_t newVal)
{
  data = newVal;
  data <<= 32;
}