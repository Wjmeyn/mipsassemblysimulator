#include "catch.hpp"
#include <fstream>
#include <ostream>
#include <string>
#include "simulator.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
// put your simulator tests here

TEST_CASE("test data movement", "[simulator]") {
	//pass
	{
		std::map<std::string, std::vector<int>> labels;
		std::map<std::string, std::vector<unsigned char>> registers;
		std::vector<unsigned char> memory;
		std::vector<Instruction> instructions;
		std::vector<std::string> error;
		std::vector<std::string> args;
		std::vector<int> indices;
		std::vector<int> indices1;
		initialize(registers);
		indices1.push_back(-1);
		indices.push_back(memory.size());
		memory.push_back('h');
		indices.push_back(memory.size());
		memory.push_back('e');
		indices.push_back(memory.size());
		memory.push_back('l');
		indices.push_back(memory.size());
		memory.push_back('l');
		indices.push_back(memory.size());
		memory.push_back('o');
		labels.insert(std::pair<std::string, std::vector<int>>("str", indices));
		labels.insert(std::pair<std::string, std::vector<int>>("end", indices1));
		instructions.push_back(Instruction("nop", args)); //pc = 0
		args.push_back("$t8");
		args.push_back("str");
		instructions.push_back(Instruction("lb", args)); //pc = 0
		args.at(0) = "$t0";
		args.at(1) = "str";
		instructions.push_back(Instruction("la", args)); // pc = 1
		args.at(0) = "$t0";
		args.at(1) = "str";
		instructions.push_back(Instruction("la", args)); // pc = 2
		args.at(0) = "$t1";
		args.at(1) = "($t0)";
		instructions.push_back(Instruction("lb", args)); //pc = 3
		args.at(0) = "$t2";
		args.at(1) = "1($t0)";
		instructions.push_back(Instruction("lb", args)); //pc = 4
		args.at(0) = "$t3";
		args.at(1) = "2($t0)";
		instructions.push_back(Instruction("lb", args)); //pc = 5
		args.at(0) = "$t4";
		args.at(1) = "3($t0)";
		instructions.push_back(Instruction("lb", args)); //pc = 6
		args.at(0) = "$t5";
		args.at(1) = "4($t0)";
		instructions.push_back(Instruction("lb", args)); //pc = 7
		bool cont = true;

		while (cont) {
			bool b = simulator(memory, instructions, labels, registers, error);

			//Updates pc register

			if (b) {


				int index = byteToInt(registers.at("$pc")); //pc value
				std::vector<unsigned char> indices;
				std::vector<int> newIndex;
				index = index + 1;
				if (index < (int)instructions.size()) {


					newIndex.push_back(index);
					indices = intToBytes1(newIndex);
					registers.at("$pc") = indices;
			
				}
				else {
					cont = false;
				}
			}
			else if (error.size() > 0) {
				cont = false;
			}
		}
		//Begin program loop
		
		REQUIRE(memory.at(0) == 'h');
		REQUIRE(memory.at(1) == 'e');
		REQUIRE(memory.at(2) == 'l');
		REQUIRE(memory.at(3) == 'l');
		REQUIRE(memory.at(4) == 'o');
		REQUIRE(registers.at(getAlias("$t1")).at(0) == 'h');
		REQUIRE(registers.at(getAlias("$t2")).at(0) == 'e');
		REQUIRE(registers.at(getAlias("$t3")).at(0) == 'l');
		REQUIRE(registers.at(getAlias("$t4")).at(0) == 'l');
		REQUIRE(registers.at(getAlias("$t5")).at(0) == 'o');
	}

	//pass
	{
		std::map<std::string, std::vector<int>> labels;
		std::map<std::string, std::vector<unsigned char>> registers;
		std::vector<unsigned char> memory;
		std::vector<Instruction> instructions;
		std::vector<std::string> error;
		std::vector<std::string> args, args1;
		std::vector<int> indices;
		std::vector<int> indices1;
		initialize(registers);
		indices.push_back(memory.size());
		memory.push_back(0x1);
		indices.push_back(memory.size());
		memory.push_back(0x0);
		indices.push_back(memory.size());
		memory.push_back(0x0);
		indices.push_back(memory.size());
		memory.push_back(0x0);

		labels.insert(std::pair<std::string, std::vector<int>>("x", indices));
	
		args.push_back("$t0");
		args.push_back("1");
		instructions.push_back(Instruction("li", args)); //pc = 0
		args.at(0) = "$t1";
		args.at(1) = "x";
		instructions.push_back(Instruction("sw", args)); // pc = 1
		args.at(0) = "$t2";
		args.at(1) = "x";
		instructions.push_back(Instruction("sh", args)); // pc = 2
		args.at(0) = "$t3";
		args.at(1) = "x";
		instructions.push_back(Instruction("sb", args)); //pc = 3
		args.at(0) = "$t4";
		args.at(1) = "$t3";
		instructions.push_back(Instruction("move", args)); //pc = 4
		args.at(0) = "$t0";
		instructions.push_back(Instruction("mthi", args)); //pc = 5
		instructions.push_back(Instruction("mtlo", args)); //pc = 5
		instructions.push_back(Instruction("mfhi", args)); //pc = 5
		instructions.push_back(Instruction("mflo", args)); //pc = 5

		bool cont = true;

		while (cont) {
			bool b = simulator(memory, instructions, labels, registers, error);

			//Updates pc register

			if (b) {


				int index = byteToInt(registers.at("$pc")); //pc value
				std::vector<unsigned char> indices;
				std::vector<int> newIndex;
				index = index + 1;
				if (index < (int)instructions.size()) {


					newIndex.push_back(index);
					indices = intToBytes1(newIndex);
					registers.at("$pc") = indices;

				}
				else {
					cont = false;
				}
			}
			else if (error.size() > 0) {
				cont = false;
			}
		}
		//Begin program loop

		REQUIRE(memory.at(0) == 0x0);
		REQUIRE(memory.at(1) == 0x0);
		REQUIRE(memory.at(2) == 0x0);
		REQUIRE(memory.at(3) == 0x0);

		REQUIRE(registers.at(getAlias("$t1")).at(0) == 0x0);
		REQUIRE(registers.at(getAlias("$t2")).at(0) == 0x0);
		REQUIRE(registers.at(getAlias("$t3")).at(0) == 0x0);
		REQUIRE(registers.at(getAlias("$t4")).at(0) == 0x0);
		REQUIRE(registers.at(getAlias("$t0")).at(0) == 0x1);
		REQUIRE(registers.at(getAlias("$hi")).at(0) == 0x1);
		REQUIRE(registers.at(getAlias("$lo")).at(0) == 0x1);
	}
}

TEST_CASE("test arithmetic movement", "[simulator]") {
	//pass
	{
		std::map<std::string, std::vector<int>> labels;
		std::map<std::string, std::vector<unsigned char>> registers;
		std::vector<unsigned char> memory;
		std::vector<Instruction> instructions;
		std::vector<std::string> error;
		std::vector<std::string> args;
		std::vector<std::string> args1;
		std::vector<std::string> args3;
		std::vector<int> indices;
		std::vector<int> indices1;
		std::vector<int> indices2;
		initialize(registers);

		indices.push_back(memory.size());
		memory.push_back(0x1);
		indices.push_back(memory.size());
		memory.push_back(0x0);
		indices.push_back(memory.size());
		memory.push_back(0x0);
		indices.push_back(memory.size());
		memory.push_back(0x0);

		indices1.push_back(memory.size());
		memory.push_back(0x1);
		indices1.push_back(memory.size());
		memory.push_back(0x0);
		indices1.push_back(memory.size());
		memory.push_back(0x0);
		indices1.push_back(memory.size());
		memory.push_back(0x0);

		indices2.push_back(memory.size());
		memory.push_back(0x4);
		indices2.push_back(memory.size());
		memory.push_back(0x0);
		indices2.push_back(memory.size());
		memory.push_back(0x0);
		indices2.push_back(memory.size());
		memory.push_back(0x0);

		labels.insert(std::pair<std::string, std::vector<int>>("x", indices));
		labels.insert(std::pair<std::string, std::vector<int>>("y", indices1));
		labels.insert(std::pair<std::string, std::vector<int>>("z", indices2));
		args.push_back("$t0");
		args.push_back("x");
		instructions.push_back(Instruction("lw", args)); //pc = 0
		args.at(0) = "$t1";
		args.at(1) = "y";
		instructions.push_back(Instruction("lw", args)); // pc = 1
		args.at(0) = "$t2";
		args.at(1) = "$t0";
		args.push_back("$t1");
		instructions.push_back(Instruction("add", args)); // pc = 2
		args3.push_back("$t2");
		args3.push_back("$t0");
		args3.push_back("2");
		instructions.push_back(Instruction("add", args3)); // pc = 3
		instructions.push_back(Instruction("addu", args)); //pc = 4
		instructions.push_back(Instruction("addu", args3)); // pc = 5
		instructions.push_back(Instruction("sub", args)); //pc = 6
		instructions.push_back(Instruction("sub", args3)); // pc = 7
		instructions.push_back(Instruction("subu", args)); //pc = 8
		instructions.push_back(Instruction("subu", args3)); // pc = 9
		instructions.push_back(Instruction("mul", args)); //pc = 10
		instructions.push_back(Instruction("mul", args3)); // pc = 11
		instructions.push_back(Instruction("mulo", args)); //pc = 12
		instructions.push_back(Instruction("mulo", args3)); //pc = 13
		instructions.push_back(Instruction("mulou", args)); // pc = 14
		instructions.push_back(Instruction("mulou", args3)); // pc = 15
		args1.push_back("$t0");
		args1.push_back("$t1");
		instructions.push_back(Instruction("mult", args1)); //pc = 16
		instructions.push_back(Instruction("multu", args1)); //pc = 17

		instructions.push_back(Instruction("div", args)); //pc = 18
		instructions.push_back(Instruction("div", args3)); //pc = 19
		instructions.push_back(Instruction("divu", args)); //pc = 20
		instructions.push_back(Instruction("divu", args3)); //pc = 21
		instructions.push_back(Instruction("div", args1)); //pc = 22
		instructions.push_back(Instruction("divu", args1)); //pc = 23

		instructions.push_back(Instruction("rem", args)); //pc = 24
		instructions.push_back(Instruction("rem", args3)); //pc = 25
		instructions.push_back(Instruction("remu", args1)); //pc = 26
		instructions.push_back(Instruction("remu", args1)); //pc = 27

		instructions.push_back(Instruction("abs", args1)); //pc = 28
		instructions.push_back(Instruction("neg", args1)); //pc = 29
		instructions.push_back(Instruction("negu", args1)); //pc = 30

		bool cont = true;

		while (cont) {
			bool b = simulator(memory, instructions, labels, registers, error);

			//Updates pc register

			if (b) {


				int index = byteToInt(registers.at("$pc")); //pc value
				std::vector<unsigned char> indices;
				std::vector<int> newIndex;
				index = index + 1;
				if (index < (int)instructions.size()) {


					newIndex.push_back(index);
					indices = intToBytes1(newIndex);
					registers.at("$pc") = indices;
				
				}
				else {
					cont = false;
				}
			}
			else if (error.size() > 0) {
				cont = false;
			}
		}
		
		//Begin program loop

		REQUIRE(memory.at(0) == 0x1);
		REQUIRE(memory.at(4) == 0x1);

		REQUIRE(registers.at(getAlias("$t0")).at(0) == 0xff);
		REQUIRE(registers.at(getAlias("$t0")).at(1) == 0xff);
		REQUIRE(registers.at(getAlias("$t0")).at(2) == 0xff);
		REQUIRE(registers.at(getAlias("$t0")).at(3) == 0xff);

		REQUIRE(registers.at(getAlias("$t2")).at(0) == 0x1);
		REQUIRE(registers.at(getAlias("$t2")).at(1) == 0x0);
		REQUIRE(registers.at(getAlias("$t2")).at(2) == 0x0);
		REQUIRE(registers.at(getAlias("$t2")).at(3) == 0x0);
	}

}
	TEST_CASE("test logical ", "[simulator]") {
		//pass
		{
			std::map<std::string, std::vector<int>> labels;
			std::map<std::string, std::vector<unsigned char>> registers;
			std::vector<unsigned char> memory;
			std::vector<Instruction> instructions;
			std::vector<std::string> error;
			std::vector<std::string> args;
			std::vector<std::string> args1;
			std::vector<std::string> args3;
			std::vector<int> indices;
			std::vector<int> indices1;
			std::vector<int> indices2;
			initialize(registers);

			indices.push_back(memory.size());
			memory.push_back(0x1);
			indices.push_back(memory.size());
			memory.push_back(0x0);
			indices.push_back(memory.size());
			memory.push_back(0x0);
			indices.push_back(memory.size());
			memory.push_back(0x0);

			indices1.push_back(memory.size());
			memory.push_back(0x0);
			indices1.push_back(memory.size());
			memory.push_back(0x0);
			indices1.push_back(memory.size());
			memory.push_back(0x0);
			indices1.push_back(memory.size());
			memory.push_back(0x0);


			labels.insert(std::pair<std::string, std::vector<int>>("TRUE", indices));
			labels.insert(std::pair<std::string, std::vector<int>>("FALSE", indices1));
			labels.insert(std::pair<std::string, std::vector<int>>("test1", indices));
			labels.insert(std::pair<std::string, std::vector<int>>("test2", indices1));
			args.push_back("$t0");
			args.push_back("test1");
			args1.push_back("$t2");
			args1.push_back("$t0");
			instructions.push_back(Instruction("lw", args)); //pc = 0
			args.at(0) = "$t1";
			args.at(1) = "test2";
			instructions.push_back(Instruction("lw", args)); // pc = 1
			args.at(0) = "$t2";
			args.at(1) = "$t0";
			args.push_back("$t1");
			instructions.push_back(Instruction("and", args)); // pc = 2
			args3.push_back("$t2");
			args3.push_back("$t0");
			args3.push_back("TRUE");
			instructions.push_back(Instruction("and", args3)); // pc = 3
			instructions.push_back(Instruction("nor", args)); //pc = 4
			instructions.push_back(Instruction("nor", args3)); // pc = 5
			instructions.push_back(Instruction("not", args1)); //pc = 6
			instructions.push_back(Instruction("not", args1)); // pc = 7
			instructions.push_back(Instruction("or", args)); //pc = 8
			instructions.push_back(Instruction("or", args3)); // pc = 9
			instructions.push_back(Instruction("xor", args)); //pc = 10
			instructions.push_back(Instruction("xor", args3)); // pc = 11
			

			bool cont = true;

			while (cont) {
				bool b = simulator(memory, instructions, labels, registers, error);

				//Updates pc register

				if (b) {


					int index = byteToInt(registers.at("$pc")); //pc value
					std::vector<unsigned char> indices;
					std::vector<int> newIndex;
					index = index + 1;
					if (index < (int)instructions.size()) {


						newIndex.push_back(index);
						indices = intToBytes1(newIndex);
						registers.at("$pc") = indices;
						
					}
					else {
						cont = false;
					}
				}
				else if (error.size() > 0) {
					cont = false;
				}
			}

			//Begin program loop

			REQUIRE(memory.at(0) == 0x1);

			REQUIRE(registers.at(getAlias("$t2")).at(0) == 0x0);
			REQUIRE(registers.at(getAlias("$t2")).at(1) == 0x0);
			REQUIRE(registers.at(getAlias("$t2")).at(2) == 0x0);
			REQUIRE(registers.at(getAlias("$t2")).at(3) == 0x0);
		}
	}

	TEST_CASE("test control ", "[simulator]") {
		//pass
		{
			std::map<std::string, std::vector<int>> labels;
			std::map<std::string, std::vector<unsigned char>> registers;
			std::vector<unsigned char> memory;
			std::vector<Instruction> instructions;
			std::vector<std::string> error;
			std::vector<std::string> args;
			std::vector<std::string> args1;
			std::vector<std::string> args3;
			std::vector<int> indices;
			std::vector<int> indices1;
			std::vector<int> indices2;
			initialize(registers);

			indices.push_back(memory.size());
			memory.push_back(0x1);
			indices.push_back(memory.size());
			memory.push_back(0x0);
			indices.push_back(memory.size());
			memory.push_back(0x0);
			indices.push_back(memory.size());
			memory.push_back(0x0);

			indices1.push_back(memory.size());
			memory.push_back(0x2);
			indices1.push_back(memory.size());
			memory.push_back(0x0);
			indices1.push_back(memory.size());
			memory.push_back(0x0);
			indices1.push_back(memory.size());
			memory.push_back(0x0);

			indices2.push_back(0);
			labels.insert(std::pair<std::string, std::vector<int>>("main", indices2));
			indices2.at(0) = 3;
			labels.insert(std::pair<std::string, std::vector<int>>("next1", indices2));
			indices2.at(0) = 4;
			labels.insert(std::pair<std::string, std::vector<int>>("next2", indices2));
			indices2.at(0) = 5;
			labels.insert(std::pair<std::string, std::vector<int>>("next3", indices2));
			indices2.at(0) = 6;
			labels.insert(std::pair<std::string, std::vector<int>>("next4", indices2));
			indices2.at(0) = 7;
			labels.insert(std::pair<std::string, std::vector<int>>("next5", indices2));
			indices2.at(0) = 8;
			labels.insert(std::pair<std::string, std::vector<int>>("next6", indices2));
			indices2.at(0) = 8;
			labels.insert(std::pair<std::string, std::vector<int>>("end", indices2));


			labels.insert(std::pair<std::string, std::vector<int>>("x", indices));
			labels.insert(std::pair<std::string, std::vector<int>>("y", indices1));

			args.push_back("$t0");
			args.push_back("x");
			instructions.push_back(Instruction("lw", args)); //pc = 0
			args.at(0) = "$t1";
			args.at(1) = "y";
			instructions.push_back(Instruction("lw", args)); //pc = 1
			args1.push_back("$t0");
			args1.push_back("$t0");
			args1.push_back("next1");
			instructions.push_back(Instruction("beq", args1)); // pc = 2
			args1.at(1) = "$t1";
			args1.at(2) = "next2";
			instructions.push_back(Instruction("bne", args1)); // pc = 3

			args1.at(1) = "$t1";
			args1.at(2) = "next3";
			instructions.push_back(Instruction("blt", args1)); // pc = 4
			args1.at(1) = "$t0";
			args1.at(1) = "$t0";
			args1.at(2) = "next4";
			instructions.push_back(Instruction("ble", args1)); // pc = 5
			args1.at(0) = "$t1";
			args1.at(2) = "next5";
			instructions.push_back(Instruction("bgt", args1)); // pc = 6
			args1.at(0) = "$t0";
			args1.at(2) = "next6";
			instructions.push_back(Instruction("bge", args1)); // pc = 7
			args3.push_back("end");
			instructions.push_back(Instruction("j", args1)); // pc = 3
			//Begin program loop


			bool cont = true;

			while (cont) {
				bool b = simulator(memory, instructions, labels, registers, error);

				//Updates pc register

				if (b) {


					int index = byteToInt(registers.at("$pc")); //pc value
					std::vector<unsigned char> indices;
					std::vector<int> newIndex;
					index = index + 1;
					if (index < (int)instructions.size()) {

						newIndex.push_back(index);
						indices = intToBytes1(newIndex);
						registers.at("$pc") = indices;
						
					}
					else {
						cont = false;
					}
				}
				else if (error.size() > 0) {
					cont = false;
				}
			}


			REQUIRE(registers.at(getAlias("$pc")).at(0) == 0x8);
			
		
		}
	}

	
