#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <istream>
#include <map>
#include <string>
#include <vector>

/*
An instruction has a type and vector of values
*/

class Instruction {
public:
	// construct a instruction type with type and arguments
	Instruction(std::string type, std::vector<std::string> args);


	// return the instruction type
	std::string type() const;

	// return the instruction's arguments
	std::vector<std::string> args() const;


private:
	std::string m_type;
	std::vector<std::string> m_args;

};

// The instruction sequence is defined as a vector of instructions.
typedef std::vector<Instruction> InstructionList;

#endif