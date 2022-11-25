// implement entry point for simmips here
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <vector>
#include <sstream>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <map>
#include "parser.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include "instruction.hpp"
#include "simulator.hpp"
#include "virtual_machine_gui.hpp"
#include <QApplication>

std::map<std::string, std::vector<int>> labels;
std::map<std::string, std::vector<unsigned char>> registers;
std::vector<unsigned char> memory;
std::vector<Instruction> instructions;
std::vector<std::string> error;
bool breakProgram = true;
bool finished = false;


//thread function for TUI, steps program counter and simulates instruction
void stepProgram() {
	bool b = false;
	
	while (!finished) {
		if (!breakProgram) {
			int index = byteToInt(registers.at("$pc")); //pc value
			if (index < (int)instructions.size()) {
				b = simulator(memory, instructions, labels, registers, error);
			}
			else {
				breakProgram = true;
				
			}

			//Updates pc register

			if (b) {

				std::string output, s;
				index = byteToInt(registers.at("$pc")); //pc value
				std::vector<unsigned char> indices;
				std::vector<int> newIndex;
				index = index + 1;
				if (index <= (int)instructions.size()) {

					newIndex.push_back(index);
					indices = intToBytes1(newIndex);
					registers.at("$pc") = indices;


				}


			}
			else if (error.size() > 0) {
				breakProgram = true;
			}

			//std::this_thread::sleep_for(std::chrono::microseconds(500000));
		}
	}
}

//thread function for Text Interface
void runGUI(VirtualMachineGUI& GUI) {
	while (GUI.getRunStatus() != 3) {
		if (GUI.getRunStatus() == 1) {
			GUI.stepProgram();
			
		}
	}
}


int main(int argc, char* argv[])
{
	//reads textfile
	bool gui = false;
	int line = 0;
	std::thread t1;

	if (argc != 2 && argc != 3) {
		std::cerr << "Error:" << line << ": invalid arguments" << std::endl;
		//return EXIT_FAILURE;
	}


	//testing

	
	
	QApplication app(argc, argv);
	VirtualMachineGUI GUI;
	//std::string file = argv[2];
	std::string file1 = "test20.asm";
	QString string = QString::fromUtf8(file1.c_str());
	GUI.load(string);
	GUI.show();

	//VirtualMachineGUI* pointer = &GUI;
	std::thread t2(runGUI, std::ref(GUI));

	int ret;
	ret = app.exec();
	t2.join();
	return ret;
	
	
	

	/*
	* Handles opening file
	*/
	
	//GUI
	
	
	
	if (argc == 3) {
		std::string arg1 = argv[1];
		std::string arg2 = argv[2];
		if (arg1.find("--gui") != std::string::npos) {
			//Create GUI
			
			QApplication app(argc, argv);
			VirtualMachineGUI GUI;
			std::string file = argv[2];
			QString string = QString::fromUtf8(file.c_str());
			GUI.load(string);
			GUI.show();
			std::thread t2(runGUI, std::ref(GUI));

			int ret;
			ret = app.exec();
			GUI.setRunStatus(3);
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
			t2.join();
			return ret;
		}
		else if (arg2.find("--gui") != std::string::npos) {
			//Create GUI
			
			QApplication app(argc, argv);
			VirtualMachineGUI GUI;
			std::string file = argv[1];
			QString string = QString::fromUtf8(file.c_str());
			GUI.load(string);
			GUI.show();
			std::thread t2(runGUI, std::ref(GUI));

			int ret;
			ret = app.exec();
			GUI.setRunStatus(3);
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
			t2.join();
			return ret;
		}
		else {
			std::cerr << "Error:" << line << ": invalid arguments" << std::endl;
			return EXIT_FAILURE;
		}
		gui = true;
	}
	

	//std::string filename = "test18.asm";
	std::string filename = argv[1];
	std::ifstream in;
	in.open(filename);
	std::string str;

	if (!in.is_open()) {
		std::cerr << "Error:" << line << ": file failed to open" << std::endl;
		return EXIT_FAILURE;
	}



	line++;
	std::ifstream file(filename);
	Statetype state = S0;
	std::string inputString = "";

	//Text Interface
	while (std::getline(in, str)) {
		if (str.size() > 0) {
			std::istringstream iss(str);
			TokenList tl = tokenize(iss);
			TokenList::iterator it = tl.begin();
			if (classify(it, tl.end()) == DATA) {
				state = S1;
			}
			else if (classify(it, tl.end()) == TEXT) {
				state = S2;
			}
			if (tl.size() > 0 && parse(tl, state, instructions, memory, labels, registers) == false) {
				std::cerr << "Error:" << line << ": parse failed" << std::endl;
				return EXIT_FAILURE;
			}
			else if (tl.size() == 0) {
				continue;
			}
			inputString = inputString + str;
		}

		inputString = inputString + "\n";
		line++;
	}

	

	bool cont = true;
	bool end = false;
	bool b = false;
	bool isRunning = false;
	t1 = std::thread(stepProgram);

	//Begin program loop
	while (cont && !gui) {
		std::string line = "";
		std::string arg;
		std::cout << "simmips> ";
		std::getline(std::cin >> std::ws, line);
		std::string cmd = line;
		
		if ((int)line.find(" ") >= (int)0)
		{
			cmd = line.substr(0, line.find(" "));
			line.erase(0, line.find(" ") + 1);
			arg = line;
		}

		if (breakProgram) {
			isRunning = false;
		}

		if (cmd == "print" && !isRunning) {

			if (registers.find(getAlias(arg)) != registers.end()) {
				arg = getAlias(arg);
				std::stringstream stream;
				std::string output;
				std::string result(stream.str());
				output = output + "0x";
				
				char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a',   'b','c','d','e','f' };
				for (int i = registers.at(arg).size() - 1; i >= 0; i--) {
					if (registers.at(arg).at(i) != 0x0)
					{
						const char ch = registers.at(arg).at(i);
						output.append(&hex[(ch & 0xF0) >> 4], 1);
						output.append(&hex[ch & 0xF], 1);

					}
					else {
						output = output + "00";
					}


				}
				std::cout << output << std::endl;

			}
			else if (arg.substr(0, 1) == "$") {
				std::cerr << "Error: unknown register." << std::endl;

			}
			else if (arg.substr(0, 3) == "&0x" && stoi(arg.substr(3, arg.size())) < (int)memory.size()) {
				std::string str = arg.substr(3, arg.size());
				std::string output;
				std::vector<unsigned char> bytes, num;
				for (unsigned int i = 0; i < str.length(); i += 2) {
					std::string byteString = str.substr(i, 2);
					char byte = (char)strtol(byteString.c_str(), NULL, 16);
					bytes.push_back(byte);
				}
				for (int i = 3; i >= 0; i--) {
					num.push_back(bytes.at(i));
				}
				int index = byteToInt(num);
				output = output + "0x";

				char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a',   'b','c','d','e','f' };
				if (memory.at(index) != 0x00)
				{
					const char ch = memory.at(index);
					output.append(&hex[(ch & 0xF0) >> 4], 1);
					output.append(&hex[ch & 0xF], 1);
				}
				else {
					output = output + "00";
				
				}
				std::cout << output << std::endl;
			
			}
			else {
				std::cerr << "Error: unknown command." << std::endl;
			}

		}
		else if (cmd == "step" && !isRunning) {

			int index = byteToInt(registers.at("$pc")); //pc value
			std::string output, s;
			b = false;
			if (index == (int)instructions.size()) {
				end = true;
				char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'c', 'b','c','d','e','f' };
				output = output + "0x";
				for (int i = registers.at("$pc").size() - 1; i >= 0; i--) {
					if (registers.at("$pc").at(i) != 0x0)
					{


						const char ch = registers.at("$pc").at(i);
						output.append(&hex[(ch & 0xF0) >> 4], 1);
						output.append(&hex[ch & 0xF], 1);
					}
					else {

						output = output + "00";

					}

				}
				std::cout << output << std::endl;
				output = "";
			}
			else if (!end) {
				b = simulator(memory, instructions, labels, registers, error);
			}
			index = byteToInt(registers.at("$pc")); //pc value
			//Updates pc register

			if (b) {

				
				std::vector<unsigned char> indices;
				std::vector<int> newIndex;
				index = index + 1;
				if (index <= (int)instructions.size()) {


					newIndex.push_back(index);
					indices = intToBytes1(newIndex);
					registers.at("$pc") = indices;
					output = output + "0x";
					//std::flush(std::cout);
					char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b','c','d','e','f' };

					for (int i = registers.at("$pc").size() - 1; i >= 0; i--) {
						if (registers.at("$pc").at(i) != 0x0)
						{
						
							const char ch = registers.at("$pc").at(i);
							output.append(&hex[(ch & 0xF0) >> 4], 1);
							output.append(&hex[ch & 0xF], 1);

						}
						else {
				
							output = output + "00";
							
						}

					}
					std::cout << output << std::endl;
					if (index == (int)instructions.size()) {
						end = true;
					}
				}
				else {
					char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'c', 'b','c','d','e','f' };

					for (int i = registers.at("$pc").size() - 1; i >= 0; i--) {
						if (registers.at("$pc").at(i) != 0x0)
						{
	

							const char ch = registers.at("$pc").at(i);
							output.append(&hex[(ch & 0xF0) >> 4], 1);
							output.append(&hex[ch & 0xF], 1);
						}
						else {

							output = output + "00";

						}

					}
					std::cout << output << std::endl;
				}
			}
			else if (error.size() > 0) {
				std::cerr << error.at(error.size() - 1) << std::endl;

			}


		}
		else if (cmd == "status") {
			if ((int)error.size() > 0) {
				for (int i = 0; i < (int)error.size(); i++) {
					std::cerr << error.at(i) << std::endl;
				
				}
			}
			else {
				std::cout << "" << std::endl;
			
			}
		}
		else if (cmd == "quit") {
			cont = false;
			finished = true;
			breakProgram = true;
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
			t1.join();
			return EXIT_SUCCESS;
		}
		else if (cmd == "run" && (!isRunning  && !end)) {
		breakProgram = false;
		isRunning = true;
		//t1 = std::thread(stepProgram);

		}
		else if (cmd == "break") {
			breakProgram = true;
			isRunning = false;
		}
		else if (cmd == "step") {
		std::cerr << "Error: simulation running. Type break to halt." << std::endl;
		}
		else if (cmd == "print") {

		std::cerr << "Error: simulation running. Type break to halt." << std::endl;
		}
		else if (cmd == "run") {
		continue;
		}
		else {
			std::cerr << "Error: unknown command." << std::endl;
			
		}
	}
	finished = true;
	breakProgram = true;
	std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
	t1.join();
	return EXIT_SUCCESS;


}




