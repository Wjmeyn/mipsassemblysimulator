// implement the simulator here
#include "simulator.hpp"
#include <ostream>
#include <iterator>
#include <iostream>
#include <string>
#include <map>
#include "token.hpp"
#include "lexer.hpp"
#include "parser.hpp"

//Simulates instructions in instructionlist
bool simulator(std::vector<unsigned char>& memory, InstructionList& instructions, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers, std::vector<std::string>& error) {
	
	int index = byteToInt(registers.at("$pc")); //pc value
	std::string s = instructions.at(index).type();

		//lw, lh, lb, la
		if ((instructions.at(index).type() == "lw" || instructions.at(index).type() == "lh") || (instructions.at(index).type() == "lb" || instructions.at(index).type() == "la")) { //<load_word>
			return handle0(memory, instructions.at(index), labels, registers, error);
		}
		//li
		else if (instructions.at(index).type() == "li") {
			return handle1(memory, instructions.at(index), labels, registers);
		}
		//sw, sh, sb
		else if ((instructions.at(index).type() == "sw" || instructions.at(index).type() == "sh") || instructions.at(index).type() == "sb") {
			if (!handle2(memory, instructions.at(index), labels, registers, error)) {
				return false;
			}
		}
		//move
		else if (instructions.at(index).type() == "move") {
			if (!handle3(instructions.at(index), registers)) {
				return false;
			}
		}
		//mfhi, mflo
		else if (instructions.at(index).type() == "mfhi" || instructions.at(index).type() == "mflo") {
			if (!handle4(instructions.at(index), registers)) {
				return false;
			}
		}
		//mthi, mtlo
		else if (instructions.at(index).type() == "mthi" || instructions.at(index).type() == "mtlo") {
			if (!handle8(instructions.at(index), registers)) {
				return false;
			}
		}
		// add_s, sub_s, mult_s, div2_s, div3_s, mul_los, 
		else if ((instructions.at(index).type() == "add" || instructions.at(index).type() == "sub") || ((instructions.at(index).type() == "mult" || instructions.at(index).type() == "div") || (instructions.at(index).type() == "mulo" || instructions.at(index).type() == "mul"))) {
			if (!handle5(memory, instructions.at(index), labels, registers)) {
				return false;
			}
		}
		// add_u, sub_u, mult_u, div2_u, div3_u, mul_lou, 
		else if ((instructions.at(index).type() == "addu" || instructions.at(index).type() == "subu") || ((instructions.at(index).type() == "multu" || instructions.at(index).type() == "divu") || instructions.at(index).type() == "mulou")) {
			if (!handle9(memory, instructions.at(index), labels, registers)) {
				return false;
			}
		}
		//rem, neg, abs
		else if ((instructions.at(index).type() == "rem" || instructions.at(index).type() == "neg") || instructions.at(index).type() == "abs") {
			if (!handle5(memory, instructions.at(index), labels, registers)) {
				return false;
			}
		}
		//remu, negu
		else if (instructions.at(index).type() == "remu" || instructions.at(index).type() == "negu") {
			if (!handle9(memory, instructions.at(index), labels, registers)) {
				return false;
			}
		}
		// Logical
		else if ((instructions.at(index).type() == "and" || instructions.at(index).type() == "nor") || ((instructions.at(index).type() == "not" || instructions.at(index).type() == "xor") || instructions.at(index).type() == "or")) {
			if (!handle6(memory, instructions.at(index), labels, registers)) {
				return false;
			}
		}
		// Control 
		else if ((instructions.at(index).type() == "j" || instructions.at(index).type() == "beq") || ((instructions.at(index).type() == "bne" || instructions.at(index).type() == "blt") || (instructions.at(index).type() == "ble" || instructions.at(index).type() == "bgt"))) {
			if (!handle7(instructions.at(index), instructions, labels, registers, error)) {
				return false;
			}
		}
		// Control 
		else if (instructions.at(index).type() == "bge") {
			if (!handle7(instructions.at(index), instructions, labels, registers, error)) {
				return false;
			}
		}
		// nop 
		else if (instructions.at(index).type() == "nop") {
			return true;
		}
		
		return true;
		
		
	}


//Converts vector of bytes to int
int byteToInt(std::vector<unsigned char> indices) {
	int a = int((unsigned char)(indices[3]) << 24 |
		(unsigned char)(indices[2]) << 16 |
		(unsigned char)(indices[1]) << 8 |
		(unsigned char)(indices[0]));

	return a;
}


//Converts vector of ints to vector of bytes
std::vector<unsigned char> intToBytes1(std::vector<int> paramInt)
{
	std::vector<unsigned char> arrayOfByte(4 * paramInt.size());
	int size = paramInt.size();
	for (int j = 0; j < size; j++) {
		for (int i = 4 * size - 1; i >= 0; i--) {
			arrayOfByte[i] = (paramInt.at(j) >> (i * 8));
		}
	}
	return arrayOfByte;
}

//Gets alias of registers.
std::string getAlias(std::string reg) {
	if (reg == "$zero") {
		return "$0";
	}
	else if (reg == "$at"){
		return "$1";
	}
	else if (reg == "$v0") {
		return "$2";
	}
	else if (reg == "$v1") {
		return "$3";
	}
	else if (reg == "$a0") {
		return "$4";
	}
	else if (reg == "$a1") {
		return "$5";
	}
	else if (reg == "$a2") {
		return "$6";
	}
	else if (reg == "$a3") {
		return "$7";
	}
	else if (reg == "$gp") {
		return "$28";
	}
	else if (reg == "$sp") {
		return "$29";
	}
	else if (reg == "$fp") {
		return "$30";
	}
	else if (reg == "$ra") {
		return "$31";
	}
	else if ((reg[0] == '$' && reg[1] == 't') && (stoi(reg.substr(2, 3)) >= 0 && stoi(reg.substr(2, 3)) <= 7)) {
		return "$" + std::to_string(stoi(reg.substr(2, 3)) + 8);
	}
	else if ((reg[0] == '$' && reg[1] == 's') && (stoi(reg.substr(2, 3)) >= 0 && stoi(reg.substr(2, 3)) <= 7)) {
		return "$" + std::to_string(stoi(reg.substr(2, 3)) + 16);
	}
	else if (reg == "$t8") {
		return "$24";
	}
	else if (reg == "$t9") {
		return "$25";
	}
	else if (reg == "$k0") {
		return "$26";
	}
	else if (reg == "$k1") {
		return "$27";
	}
	
	else {
		return reg;
	}
	
}

/*
Handle methods handle simulation of instructions.
*/

//handles load_word, load_half, load_byte
bool handle0(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers, std::vector<std::string>& error) {

	int type = 0;
	if (instruction.type() == "lw") {
		type = 4;
	}
	else if (instruction.type() == "lh") {
		type = 2;
	}
	else if (instruction.type() == "lb" || instruction.type() == "la") {
		type = 1;
	}
	std::string reg = instruction.args().at(0);
	reg = getAlias(reg);
	std::string mem = instruction.args().at(1);
	int offset = 0;
	int str1 = mem.find("(");
	if (str1 > 0) {
		offset = stoi(mem.substr(0, mem.find("(")));
		mem = mem.substr(mem.find("(") + 1, (mem.size() - 1));
		mem = mem.substr(0, (mem.size() - 1));
	}
	else if (str1 == 0) {
		mem = mem.substr(mem.find("(") + 1, (mem.size() - 1));
		mem = mem.substr(0, (mem.size() - 1));
	}

	mem = getAlias(mem);
	int baseVal = -1;
	if (registers.find(mem) != registers.end()) { //checks for label and register


		baseVal = (byteToInt(registers.at(mem)));

	}
	else if (labels.find(mem) != labels.end()) { //label
		baseVal = labels.at(mem).at(0);
	}
	else if (stoi(mem) < (int)memory.size()) { // immediate
		baseVal = stoi(mem);
	}
	else {
		error.push_back("Error: memory location out of bounds");
		return false;
	}

			std::vector<unsigned char> number;
			for (int i = 0; i < 4; i++) {
				if (i < type) {
					if ((baseVal + offset + i) < (int)memory.size() && instruction.type() != "la") {
						number.push_back(memory.at(baseVal + offset + i));
					}
					else if (instruction.type() == "la") {
						std::vector<int> ind;
						ind.push_back(baseVal + offset);
						number = intToBytes1(ind);
						break;
					}
					else {
						error.push_back("Error: memory location out of bounds");
						return false;
					}
				}
				else {
					if (instruction.type() != "la") {
						number.push_back(0x0);
					}
				}

			}
			if (registers.find(reg) != registers.end()) { //checks for label and register			
				registers.at(reg) = number; //loads reg with word;
				return true;
			}
			
			
	
	
	return false;
}

//handles load_imm (TODO)
bool handle1(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers) {
	
	std::string reg = getAlias(instruction.args().at(0));
	std::string imm = instruction.args().at(1);
	if (labels.find(imm) != labels.end()) { //if immediate is a constant

		std::vector<unsigned char> number;
		std::vector<int> indices;
		for (int i = 0; i < 4; i++) {
			if (i < (int)labels.at(imm).size() && i < 4) {
				number.push_back(memory.at(labels.at(imm).at(i)));
			}
			else if (i < 4){
				number.push_back(0x0);
			}
		}
		if (registers.find(reg) != registers.end()) { //checks for label and register			
			registers.at(reg) = number; //loads reg with word;
			return true;
		}
		
		
	}
	else { // if immediate is int
		std::vector<int> num;
		num.push_back(stoi(imm));
		std::vector<unsigned char> number = intToBytes1(num);
		
		
		if (registers.find(reg) != registers.end()) { //checks for label and register			
			registers.at(reg) = number; //loads reg with word;
			return true;
		}
		

	}
	return false;
	
}

//handles store_word, store_half, store_byte
bool handle2(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers, std::vector<std::string>& error) {
	int type = 0;
	if (instruction.type() == "sw") {
		type = 4;
	}
	else if (instruction.type() == "sh") {
		type = 2;
	}
	else if (instruction.type() == "sb") {
		type = 1;
	}
	std::string reg = getAlias(instruction.args().at(0));
	std::string mem = instruction.args().at(1);
	int offset = 0;
	int str1 = mem.find("(");
	if (str1 > 0) {
		offset = stoi(mem.substr(0, mem.find("(")));
		mem = mem.substr(mem.find("(") + 1, (mem.size() - 1));
		mem = mem.substr(0, (mem.size() - 1));
	}
	else if (str1 == 0) {
		mem = mem.substr(mem.find("(") + 1, (mem.size() - 1));
		mem = mem.substr(0, (mem.size() - 1));
	}
	mem = getAlias(mem);
	int baseVal = -1;
	if (registers.find(mem) != registers.end()) { //checks for label and register


		baseVal = (byteToInt(registers.at(mem)));

	}
	else if (labels.find(mem) != labels.end()) {
		baseVal = labels.at(mem).at(0);
	}
	else if (stoi(mem) < (int)memory.size()) {
		baseVal = stoi(mem);
	}
	else {
		error.push_back("Error: memory location out of bounds");
		return false;
	}
	
		
		if (registers.find(reg) != registers.end()) {

			std::vector<int> indices;
			for (int i = 0; i < type; i++) {
				if ((baseVal + offset + i) < (int)memory.size()) {
					memory.at((baseVal + offset + i)) = registers.at(reg).at(i);

				}
				else {
					memory.push_back(registers.at(reg).at(i));
				}
			}
			return true;
		}			
		
	return false;
}

//move
bool handle3(Instruction& instruction, std::map<std::string, std::vector<unsigned char>>& registers) {
	
	std::string reg1 = getAlias(instruction.args().at(0));
	std::string reg2 = getAlias(instruction.args().at(1));
	
	if (registers.find(reg1) != registers.end()) { //checks for memref and register

		if (registers.find(reg2) != registers.end()) {
			for (int i = 0; i < 4; i++) {
				registers.at(reg1).at(i) = registers.at(reg2).at(i);
			}
			
			return true;
		}
	}
	return false;
}

// move_from_hi, move_from_lo
bool handle4(Instruction& instruction, std::map<std::string, std::vector<unsigned char>>& registers) {
	std::string reg;
	if (instruction.type() == "mfhi") {
		reg = "$hi";
	}
	else if (instruction.type() == "mflo") {
		reg = "$lo";
	}

	std::string reg1 = getAlias(instruction.args().at(0));
	

	if (registers.find(reg1) != registers.end()) { //checks for memref and register

		
			for (int i = 0; i < 4; i++) {
				registers.at(reg1).at(i) = registers.at(reg).at(i);
			}

			return true;
		
	}
	return false;
}
// move_to_hi, move_to_lo
bool handle8(Instruction& instruction, std::map<std::string, std::vector<unsigned char>>& registers) {
	std::string reg;
	if (instruction.type() == "mthi") {
		reg = "$hi";
	}
	else if (instruction.type() == "mtlo") {
		reg = "$lo";
	}

	std::string reg1 = getAlias(instruction.args().at(0));


	if (registers.find(reg1) != registers.end()) { //checks for memref and register


		for (int i = 0; i < 4; i++) {
			registers.at(reg).at(i) = registers.at(reg1).at(i);
		}

		return true;

	}
	return false;
}
// add_s, sub_s, mult_s, div2_s, div3_s, mul_los, mul, 
bool handle5(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers) {
	std::string reg1 = instruction.args().at(0);
	reg1 = getAlias(reg1);
	std::string reg2 = instruction.args().at(1);
	reg2 = getAlias(reg2);
	std::string reg3 = "";
	int num1 = 0;
	int num2 = 0;
	
	if (instruction.args().size() == 3) {
		if (registers.find(getAlias(instruction.args().at(2))) != registers.end()) {

			reg3 = getAlias(instruction.args().at(2));
			
			num1 = byteToInt(registers.at(reg2));
			num2 = byteToInt(registers.at(reg3));
		}
		else if (labels.find(instruction.args().at(2)) != labels.end()) {
			reg3 = getAlias(instruction.args().at(2));

			num1 = byteToInt(registers.at(reg2));
			std::vector<unsigned char> mem;
			for (int i = 0; i < (int)labels.at(reg3).size(); i++) {
				mem.push_back(memory.at(labels.at(reg3).at(i)));
			}
			num2 = byteToInt(mem);
		}
		else if(is32Int(instruction.args().at(2))) {
			reg3 = instruction.args().at(2);
			
			num1 = byteToInt(registers.at(reg2));
			num2 = stoi(reg3);
		}
		
	}
	else if (instruction.args().size() == 2) {
		std::vector<unsigned char> tnum1, tnum2;
		
		num1 = byteToInt(registers.at(reg1));
		num2 = byteToInt(registers.at(reg2));
	}

	long long int sum = 0;
if (instruction.type() == "add") { //handles add
	sum = (long long int)num1 + (long long int)num2;
	if (sum <= 2147483647) {
		if (sum >= -2147483647) {
			std::vector<int> s;
			s.push_back(num1 + num2);
			std::vector<unsigned char> number = intToBytes1(s);
			registers.at(reg1) = number;
			return true;
		}
	}
}
else if (instruction.type() == "sub") { //handles sub
	sum = num1 - num2;
	if (sum <= 2147483647) {
		if (sum >= -2147483647) {
			std::vector<int> s;
			s.push_back(num1 - num2);
			std::vector<unsigned char> number = intToBytes1(s);
			registers.at(reg1) = number;
			return true;
		}
	}
}
else if (instruction.type() == "div") { //handles div
	if (num2 != 0) {
		std::vector<int> q;
		q.push_back(num1 / num2);
		std::vector<int> r;
		r.push_back(num1 % num2);
		registers.at("$lo") = intToBytes1(q);
		registers.at("$hi") = intToBytes1(r);
		
		return true;
	}
}
else if (instruction.type() == "rem") { //handles rem
	if (num2 != 0) {
		std::vector<int> r;
		r.push_back(num1 % num2);
		registers.at(reg1) = intToBytes1(r);

		return true;
	}
}
else if (instruction.type() == "neg") { //handles neg
	if (num2 != 0) {
		std::vector<int> r;
		r.push_back(num2 * -1);
		registers.at(reg1) = intToBytes1(r);

		return true;
	}
}
else if (instruction.type() == "abs") { //handles abs
	if (num2 != 0) {
		std::vector<int> r;
		r.push_back(abs(num2));
		registers.at(reg1) = intToBytes1(r);

		return true;
	}
}
else if (instruction.type() == "mulo") { //handles mulo
	
	sum = num1 * num2;
	
	if (sum <= 2147483647 && sum >= -2147483647) {
		std::vector<int> s;
		s.push_back(num1 * num2);
		std::vector<unsigned char> number = intToBytes1(s);
		registers.at(reg1) = number;
		return true;
	}
}

else if (instruction.type() == "mul") { //handles mul

sum = num1 * num2;
	std::vector<int> s;
	s.push_back(num1 * num2);
	std::vector<unsigned char> number = intToBytes1(s);
	registers.at(reg1) = number;
	return true;

}
else if (instruction.type() == "mult") { //handles mult
	sum = num1 * num2;
	bool overflow = (num1 != 0 || num2 != 0) && sum == 0;
	if (!overflow) {
		sum = num1 * num2;
		int x = (int)(sum >> 32);
		int y = (int)sum;
		std::vector<int> q;
		q.push_back(x); //upper 32
		std::vector<int> r;
		r.push_back(y); // lower 32
		registers.at("$hi") = intToBytes1(q);
		registers.at("$lo") = intToBytes1(r);

		return true;
	}
	else  { //Handles overflow bits
		int x = 1;
		int y = 0;
		if (((num1 < 0 && num2 > 0) || (num2 < 0 && num1 > 0))) {
			x = -1;
			y = 0;
		}
		std::vector<int> q;
		q.push_back(x); //upper 32
		std::vector<int> r;
		r.push_back(y); // lower 32
		registers.at("$hi") = intToBytes1(q);
		registers.at("$lo") = intToBytes1(r);
		return true;
	}
	
}
return false;
}


// add_u, sub_u, mult_u, div2_u, div3_u, mul_lou, 
bool handle9(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers) {
	std::string reg1 = instruction.args().at(0);
	reg1 = getAlias(reg1);
	std::string reg2 = instruction.args().at(1);
	reg2 = getAlias(reg2);
	std::string reg3 = "";
	unsigned int num1 = 0;
	unsigned int num2 = 0;

	if (instruction.args().size() == 3) {
		if (registers.find(getAlias(instruction.args().at(2))) != registers.end()) {

			reg3 = getAlias(instruction.args().at(2));

			num1 = byteToInt(registers.at(reg2));
			num2 = byteToInt(registers.at(reg3));
		}
		else if (labels.find(instruction.args().at(2)) != labels.end()) {
			reg3 = getAlias(instruction.args().at(2));

			num1 = byteToInt(registers.at(reg2));
			std::vector<unsigned char> mem;
			for (int i = 0; i < (int)labels.at(reg3).size(); i++) {
				mem.push_back(memory.at(labels.at(reg3).at(i)));
			}
			num2 = byteToInt(mem);
		}
		else if (is32Int(instruction.args().at(2))) {
			reg3 = instruction.args().at(2);

			num1 = byteToInt(registers.at(reg2));
			num2 = stoi(reg3);
		}
	}
	else if (instruction.args().size() == 2) {
		std::vector<unsigned char> tnum1, tnum2;

		num1 = byteToInt(registers.at(reg1));
		num2 = byteToInt(registers.at(reg2));
	}

	long long int sum = 0;
	if (instruction.type() == "addu") { //handles add
		sum = num1 + num2;
			std::vector<int> s;
			s.push_back((int)(num1 + num2));
			std::vector<unsigned char> number = intToBytes1(s);
			registers.at(reg1) = number;
			return true;
		
	}
	else if (instruction.type() == "subu") { //handles subu
		sum = num1 - num2;

			std::vector<int> s;
			s.push_back(num1 - num2);
			std::vector<unsigned char> number = intToBytes1(s);
			registers.at(reg1) = number;
			return true;
		
	}
	else if (instruction.type() == "divu") { //handles divu
		if (num2 != 0) {
			std::vector<int> q;
			q.push_back(num1 / num2);
			std::vector<int> r;
			r.push_back(num1 % num2);
			registers.at("$lo") = intToBytes1(q);
			registers.at("$hi") = intToBytes1(r);

			return true;
		}
	}
	else if (instruction.type() == "remu") { //handles remu
		if (num2 != 0) {
			std::vector<int> r;
			r.push_back(num1 % num2);
			registers.at(reg1) = intToBytes1(r);

			return true;
		}
	}
	else if (instruction.type() == "negu") { //handles negu
		if (num2 != 0) {
			std::vector<int> r;
			r.push_back(num2 * -1);
			registers.at(reg1) = intToBytes1(r);

			return true;
		}
	}
	else if (instruction.type() == "mulou") { //handles mulou

		sum = num1 * num2;

			std::vector<int> s;
			s.push_back(num1 * num2);
			std::vector<unsigned char> number = intToBytes1(s);
			registers.at(reg1) = number;
			return true;
		
	}
	else if (instruction.type() == "multu") { //handles multu
		if (num2 != 0) {
			sum = num1 * num2;
			unsigned int x = (unsigned int)(sum >> 32); 
			unsigned int y = (unsigned int)sum;
			std::vector<int> q;
			q.push_back(x); //upper 32
			std::vector<int> r;
			r.push_back(y); // lower 32
			registers.at("$hi") = intToBytes1(q);
			registers.at("$lo") = intToBytes1(r);

			return true;
		}
	}
	return false;
}

//Logic Instructions
bool handle6(std::vector<unsigned char>& memory, Instruction& instruction, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers) {
	std::string reg1 = instruction.args().at(0);
	reg1 = getAlias(reg1);
	std::string reg2 = instruction.args().at(1);
	reg2 = getAlias(reg2);
	std::string reg3 = "";
	int num1;
	int num2;

	if (instruction.args().size() == 3) {
		reg3 = getAlias(instruction.args().at(2));

		num1 = byteToInt(registers.at(reg2));
		if (registers.find(reg3) != registers.end()) {
			num2 = byteToInt(registers.at(reg3));
		}
		else if (labels.find(reg3) != labels.end()) {
			std::vector<unsigned char> mem;
			for (int i = 0; i < (int)labels.at(reg3).size(); i++) {
				mem.push_back(memory.at(labels.at(reg3).at(i)));
			}
			num2 = byteToInt(mem);
		}
		else if (is32Int(reg3)) {
			num2 = stoi(reg3);
		}
		else {
			return false;
		}
		if (instruction.type() == "and") { //handles and
			std::vector<int> s;
			s.push_back(num1 & num2);
			registers.at(reg1) = intToBytes1(s);
			return true;
		}
		else if (instruction.type() == "nor") { //handles nor
			std::vector<int> s;
			s.push_back(~(num1 | num2));
			registers.at(reg1) = intToBytes1(s);
			return true;
		}
		else if (instruction.type() == "or") { //handles or
			std::vector<int> s;
			s.push_back(num1 | num2);
			registers.at(reg1) = intToBytes1(s);
			return true;
		}
		else if (instruction.type() == "xor") { //handles xor
			std::vector<int> s;
			s.push_back(num1 ^ num2);
			registers.at(reg1) = intToBytes1(s);
			return true;
		}
	}
	else if (instruction.args().size() == 2) {
		if (registers.find(reg2) != registers.end()) {
			num1 = byteToInt(registers.at(reg2));
		}
		else if (labels.find(reg2) != labels.end()) {
			std::vector<unsigned char> mem;
			for (int i = 0; i < (int)labels.at(reg2).size(); i++) {
				mem.push_back(memory.at(labels.at(reg2).at(i)));
			}
			num1 = byteToInt(mem);
		}
		else if (is32Int(reg2)) {
			num1 = stoi(reg2);
		}
		else {
			return false;
		}
		if (instruction.type() == "not") { //handles not
			std::vector<int> s;
			s.push_back(~num1);
			registers.at(reg1) = intToBytes1(s);
			return true;
		}
	}
	return false;
}

//Control Instructions
bool handle7(Instruction& instruction, InstructionList& instructions, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers, std::vector<std::string>& error) {
	std::string reg1 = instruction.args().at(0);
	reg1 = getAlias(reg1);
	std::string reg2;
	std::string reg3;
	if (instruction.args().size() == 3) 
	{
		reg2 = instruction.args().at(1);
		reg2 = getAlias(reg2);
		reg3 = instruction.args().at(2);
		reg3 = getAlias(reg3);
	}
	int num1;
	int num2;
	int baseVal = -1;
	bool branch = false;
	if (instruction.type() == "j") { //handles jump
			
		reg3 = reg1;
		branch = true;
		
			
	}
	else if (instruction.type() == "nop") { //handles nop
		
		//do nothing

	}
	else  { 
		if (registers.find(reg2) != registers.end()) {


			num2 = byteToInt(registers.at(reg2));
		}
		else {
			num2 = stoi(reg2);
		}
		num1 = byteToInt(registers.at(reg1));

		if (instruction.type() == "beq" && num1 == num2) {		//handles beq
			branch = true;			
		}
		else if (instruction.type() == "bne" && num1 != num2) { //handles add
			branch = true;			
		}
		else if (instruction.type() == "blt" && num1 < num2) { //handles blt
			branch = true;			
		}
		else if (instruction.type() == "ble" && num1 <= num2) { //handles ble
			branch = true;		
		}
		else if (instruction.type() == "bgt" && num1 > num2) { //handles bgt
			branch = true;		
		}
		else if (instruction.type() == "bge" && num1 >= num2) { //handles bge
			branch = true;		
		}
		
		
		
	}

	if (branch == true) {
		if (labels.find(reg3) != labels.end() && labels.at(reg3).at(0) != -1) {
			baseVal = labels.at(reg3).at(0);
		
			std::vector<int> index;
			index.push_back(baseVal - 1);
			if (index.at(0) < (int)instructions.size()) {
				std::vector<unsigned char> str = intToBytes1(index);
				registers.at("$pc") = intToBytes1(index); //loads reg with word;
				return true;
			}
			else {
				error.push_back("Error: pc out of bounds");
				return false;
			}

			
		}
			
		
	}
	

	return true;
	
	


	
}