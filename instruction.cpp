#include "instruction.hpp"

Instruction::Instruction(std::string type, std::vector<std::string> args) : m_type(type), m_args(args) {}

std::string Instruction::type() const { return m_type; }

std::vector<std::string> Instruction::args() const { return m_args; }


