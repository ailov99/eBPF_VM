# pragma once

#include <string>
#include <vector>


uint64_t parsePktAccess(std::string&, std::string&, std::string&, std::string&);
uint64_t parseLdx(std::string&, std::string&, std::string&, std::string&);
uint64_t parseStSrc(std::string, std::string, std::string, std::string);
uint64_t parseStImm(std::string, std::string, std::string, std::string);
uint64_t parseBranch(std::string&, std::string&, std::string&, std::string&);
uint16_t seekLabel(std::string&);
uint64_t parseALU(std::string&, std::string&, std::string&);
std::vector<uint64_t> assemble();
