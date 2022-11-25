#ifndef DATAPARSE_HPP
#define DATAPARSE_HPP

#include <cstddef>
#include <list>
#include <ostream>
#include <string>
#include <iterator>
#include <cassert>
#include <variant>
#include <vector>
#include "token.hpp"
#include "instruction.hpp"
#include "lexer.hpp"

// define the parser module here

enum InputValue {
	DATA, //Data
	WORD,
	HALF,
	BYTE,
	SPACE,
	ASCII,
	ASCIIZ,
	COMMENT,
	EOT,
	LABEL,
	CONSTANT,
	TEXT, //Text
	CASE1, //Data Movemet Instructions: lw, lh, lb, la, sw, sh, sb (DM1)
	CASE2, //Data Movemet Instructions: li (DM2)
	CASE3, //Data Movemet Instructions: move | Integer Arithmetic Instructions: mult, multu, div, divu, abs, neg, negu (DM3)
	CASE4, //Data Movemet Instructions: mfhi, mflo, mthi, mtlo (CASE4)
	CASE5, //Integer Arithmetic Instructions: add, addu, sub , subu, mul, mulo, mulou, rem, remu | Logical Instructions: and, nor, or, xor (CASE5)
	NOT,
	DIV,
	DIVU,
	J, //Control
	CASE6, //Control Instructions: beq, bne, blt, ble, bgt, bge (CASE6)
	NOP,
	ERROR2
};

enum Statetype {
	S0,
	S1,
	S2
};


//Parser Method
bool parse(const TokenList& tokens, Statetype state, InstructionList& instructions, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers);

//Used to classify tokens
InputValue classify(TokenList::const_iterator it, TokenList::const_iterator end);
InputValue classify(TokenList::const_iterator it, TokenList::const_iterator end, Statetype state, std::map<std::string, std::vector<int>>& labels);



//Check to see if string is 32-bit
bool is32Int(std::string str);

//Check to see if string is 16-bit
bool is16Int(std::string str);

//Check to see if string is 8-bit
bool isByte(std::string str);

//Check to see if token is a constant
bool isConstant(TokenList::const_iterator it);

//Check to see if token is an immediate value
bool isImmediate(TokenList::const_iterator it);

//Checks to see if sequence of tokens form a memory reference 
std::string isMemref(TokenList::const_iterator it, const TokenList& tokens, std::map<std::string, std::vector<int>>& labels);

//Checks to see if sequence of tokens form a register
bool isRegister(TokenList::const_iterator it);

//Initializes registers map
void initialize(std::map<std::string, std::vector<unsigned char>>& registers);

//Update methods check syntax of token sequences
bool update0(TokenList::const_iterator it, const TokenList& tokens);
bool update1(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& registers, std::string name);
bool update2(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& registers, std::string name);
bool update3(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& registers, std::string name);
bool update4(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& registers, std::string name);
bool update5(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& registers, std::string name);
bool update6(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& registers, std::string name);
bool update7(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type, std::map<std::string, std::vector<int>>& labels);
bool update8(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type);
bool update9(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type);
bool update10(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type);
bool update11(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type);
bool update12(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type);
bool update13(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type);
bool update14(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type);
bool update15(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type);
bool update16(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::string name);




#endif
