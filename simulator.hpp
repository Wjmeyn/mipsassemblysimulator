#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <istream>
#include <map>
#include <string>
#include <variant>

#include "token.hpp"
#include "parser.hpp"

/*
Simulates mips program.
 */



//Simulates instructuctions
bool simulator(std::vector<unsigned char>& memory, InstructionList& instructions, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers, std::vector<std::string>& error);

//Converts vector of bytes to int
int byteToInt(std::vector<unsigned char> indices);

//Converts vector of ints to vector of bytes
std::vector<unsigned char> intToBytes1(std::vector<int> indices);

//Gets alias of registers.
std::string getAlias(std::string reg);




/*
Handle methods handle simulation of instructions.
*/

//handles load_word, load_half, load_byte
bool handle0(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers, std::vector<std::string>& error);

//handles load_imm
bool handle1(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers);

//handles store_word, store_half, store_byte
bool handle2(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers, std::vector<std::string>& error);

//move
bool handle3(Instruction& instruction, std::map<std::string, std::vector<unsigned char>>& registers);

//handles move_from_hi, move_from_lo
bool handle4(Instruction& instruction, std::map<std::string, std::vector<unsigned char>>& registers);

//handles add_s, sub_s, mult_s, div2_s, div3_s, mul_los, mul, 
bool handle5(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers);

//handles Logic Instructions
bool handle6(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers);

//handles Control Instructions
bool handle7(Instruction& instruction, InstructionList& instructions, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers, std::vector<std::string>& error);

//handles move_to_hi, move_to_lo
bool handle8(Instruction& instruction, std::map<std::string, std::vector<unsigned char>>& registers);

//handles add_u, sub_u, mult_u, div2_u, div3_u, mul_lou, 
bool handle9(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers);



#endif