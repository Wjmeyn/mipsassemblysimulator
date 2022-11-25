#include "virtual_machine_gui.hpp"
#include <QPlainTextEdit>
#include <QTableView>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTextBlock>
#include <iostream>
#include <istream>
#include <fstream>
#include <ostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <thread>
#include <mutex>


// TODO implement the GUI class

VirtualMachineGUI::VirtualMachineGUI(QWidget* parent) : QWidget(parent) {

	

	//Setup "text"
	A = new QPlainTextEdit(parent);
	A->setReadOnly(true);
	A->setObjectName("text");
	mainPointer = 0;
	currentLine = 0;
	fileError = false;
	runProgram = 0; // 0 - Not run, 1 - currently running, 2 - partial run, 3 - completed run

	//Setup "registers"
	B = new QTableView;
	B->setObjectName("registers");

	//Create registers table
	regTable = new MyModel(1, 3, this);


	//Setup "memory"
	C = new QTableView;
	C->setObjectName("memory");
	//Create registers table
	memTable = new MyModel(1, 2, this);
	
	//Adds models to tableview
	B->setModel(regTable);
	C->setModel(memTable);

	//Setup "status"
	D = new QLineEdit;
	D->setPlaceholderText("Ok");
	D->setReadOnly(true);
	D->setFocus();
	D->setText("Ok");
	D->setObjectName("status");

	// status label
	QLabel *statusLabel = new QLabel("Status: ");


	//Setup "step"
	E = new QPushButton("Step");
	E->setObjectName("step");
	QWidget::connect(E, SIGNAL(clicked()), this, SLOT(step()));

	//Setup "run"
	F = new QPushButton("Run");
	F->setObjectName("run");
	QWidget::connect(F, SIGNAL(clicked()), this, SLOT(run()));

	//Setup "break"
	G = new QPushButton("Break");
	G->setObjectName("break");
	QWidget::connect(G, SIGNAL(clicked()), this, SLOT(breakRun()));
	G->setEnabled(false);

	//Set Layout
	auto layout = new QVBoxLayout;
	auto tables = new QHBoxLayout;
	auto stat = new QGridLayout;
	auto buttons = new QHBoxLayout;

	//add widgets to QHBoxLayout
	tables->addWidget(A);
	tables->addWidget(B);
	tables->addWidget(C);

	//adds wigets to main layout
	layout->addLayout(tables);
	stat->addWidget(statusLabel, 0, 0);
	stat->addWidget(D, 0, 1);
	layout->addLayout(stat);
	buttons->addWidget(E);
	buttons->addWidget(F);
	buttons->addWidget(G);
	layout->addLayout(buttons);

	setLayout(layout);


}

//Loads file into QPlainTextEdit
void VirtualMachineGUI::load(QString string) {

	std::map<std::string, std::vector<int>> lab;
	std::map<std::string, std::vector<unsigned char>> reg;
	std::vector<unsigned char> mem;
	std::vector<Instruction> ins;
	std::vector<std::string> err;

	//Parse file
	int line = 0;
	line++;
	std::string filename = string.toUtf8().constData();
	std::ifstream file(filename);
	Statetype state = S0;
	std::string inputString = "";
	std::ifstream in;
	in.open(filename);

	std::string str;
	if (!in.is_open()) {
		err.push_back("Error: file not found");
		QString string = QString::fromUtf8(err.back().c_str());
		D->setText(string);
		fileError = true;
	}

	while (std::getline(in, str) && fileError == false) {
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
			if (tl.size() > 0 && parse(tl, state, ins, mem, lab, reg) == false) {
				err.push_back("Error: parse error");
				QString string = QString::fromUtf8(err.back().c_str());
				D->setText(string);
				fileError = true;
				inputString = "";
				str = "";
			}
			else if (tl.size() == 0) {
				continue;
			}
			inputString = inputString + str;
		}

		inputString = inputString + "\n";
		line++;
	}

	//Sets variables
	memory = mem;
	instructions = ins;
	labels = lab;
	registers = reg;
	error = err;
	mainPointer = 0;

	if (fileError == false) {
		//Create registers table
		regTable = new MyModel((int)registers.size() - 4, 3, this);

		//Create registers table
		memTable = new MyModel((int)memory.size(), 2, this);
	}
	else {
		//Create registers table
		regTable = new MyModel(0, 3, this);

		//Create registers table
		memTable = new MyModel(0, 2, this);
	}

	//Adds models to tableview
	B->setModel(regTable);
	C->setModel(memTable);

	QString input = QString::fromUtf8(inputString.c_str());

	A->setPlainText(input);

	//initialize tables
	initializeTables();

	if (!fileError) {

		QTextDocument* doc = A->document();

		for (int i = 0; i < doc->lineCount(); i++) {
			QTextBlock tb = doc->findBlockByLineNumber(i);
			if (tb.text().contains("main:")) {
				if (tb.text().indexOf(" ") > 0) {
					mainPointer = i;
					break;
				}
				mainPointer = i + 1;
				break;
			}
		}
		int ind = 0;
		//aligns instructions with QPlainTextEdit
		for (int i = mainPointer; i < A->document()->blockCount(); i++)
		{
			QTextBlock block = A->document()->findBlockByNumber(i);

			if (block.isValid())
			{
				if (block.text().contains("main:")) {
					instructionIndex.insert(std::pair<int, int>(ind + mainPointer, i));
					ind++;
				}
				else if (block.text() == "" || block.text().contains(":")) {
					std::string utf8_text = block.text().toUtf8().constData();
					if (utf8_text.erase(0, utf8_text.find_first_of(":") + 1).size() <= 0) {
						continue;
					}
					else {
						instructionIndex.insert(std::pair<int, int>(ind + mainPointer, i));
						ind++;
					}
					
				}
				else {
					instructionIndex.insert(std::pair<int, int>(ind + mainPointer, i));
					ind++;
				}
			}

		}
		currentLine = mainPointer;
		updateLine();
		A->viewport()->update();
	}
	else {
		E->setEnabled(false);
		F->setEnabled(false);

	}
}


//steps program counter and simulates instruction
void VirtualMachineGUI::step() {
	bool b = false;
	if (fileError == false) {
		int index = byteToInt(registers.at("$pc")); //pc value
		if (index != (int)instructions.size()) {
			b = simulator(memory, instructions, labels, registers, error);
		}
		else {
			runProgram = 3;
			E->setEnabled(false);
			F->setEnabled(false);
			G->setEnabled(false);
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
				currentLine = mainPointer + index;
				

			}
			else {
				runProgram = 3;
				E->setEnabled(false);
				F->setEnabled(false);
			}

			updateRegisters();
			updateMemory();
			if (runProgram != 1) {
				updateLine();
			}
			D->setText("Ok");
			D->update();
			A->viewport()->update();
			
		}
		else if (error.size() > 0) {
			updateStatus();
		}
		if(index > (int)instructions.size()){
			runProgram = 3;
			E->setEnabled(false);
			F->setEnabled(false);
		}

	}
}
void VirtualMachineGUI::stepProgram() {
	while (runProgram == 1 && !fileError) {
		step();
		//std::this_thread::sleep_for(std::chrono::microseconds(5000));
	}
}

//changes run status to currently run
void VirtualMachineGUI::run() {
	runProgram = 1;
	G->setEnabled(true);
	E->setEnabled(false);
	F->setEnabled(false);
}

//changes run status based on current state
void VirtualMachineGUI::breakRun() {
	if (runProgram == 1) {
		runProgram = 2;
	}
	G->setEnabled(false);
	F->setEnabled(true);
	E->setEnabled(true);
	
	updateLine();
	A->viewport()->update();
	
	
}

//gets run status
int VirtualMachineGUI::getRunStatus() {
	return runProgram;
}

//sets run status
void VirtualMachineGUI::setRunStatus(int status) {
	runProgram = status;
}

//initializes tables in GUI
void VirtualMachineGUI::initializeTables() {
	if (fileError == false)
	{
		regTable->setData(0, 1, "$pc");
		regTable->setData(1, 1, "$hi");
		regTable->setData(2, 1, "$lo");
		regTable->setData(3, 1, "$zero");
		regTable->setData(4, 1, "$at");
		regTable->setData(5, 1, "$v0");
		regTable->setData(6, 1, "$v1");
		regTable->setData(7, 1, "$a0");
		regTable->setData(8, 1, "$a1");
		regTable->setData(9, 1, "$a2");
		regTable->setData(10, 1, "$a3");
		regTable->setData(11, 1, "$t0");
		regTable->setData(12, 1, "$t1");
		regTable->setData(13, 1, "$t2");
		regTable->setData(14, 1, "$t3");
		regTable->setData(15, 1, "$t4");
		regTable->setData(16, 1, "$t5");
		regTable->setData(17, 1, "$t6");
		regTable->setData(18, 1, "$t7");
		regTable->setData(19, 1, "$s0");
		regTable->setData(20, 1, "$s1");
		regTable->setData(21, 1, "$s2");
		regTable->setData(22, 1, "$s3");
		regTable->setData(23, 1, "$s4");
		regTable->setData(24, 1, "$s5");
		regTable->setData(25, 1, "$s6");
		regTable->setData(26, 1, "$s7");
		regTable->setData(27, 1, "$t8");
		regTable->setData(28, 1, "$t9");
		regTable->setData(29, 1, "$k0");
		regTable->setData(30, 1, "$k1");
		regTable->setData(31, 1, "$gp");
		regTable->setData(32, 1, "$sp");
		regTable->setData(33, 1, "$fp");
		regTable->setData(34, 1, "$ra");

		for (int i = 0; i < 32; i++) {
			std::string str = "$" + std::to_string(i);
			QString string = QString::fromUtf8(str.c_str());
			regTable->setData(3 + i, 0, string);
		}
		updateRegisters();
		updateMemory();
	}
	
}

void VirtualMachineGUI::updateRegisters() {
	std::string output;
	for (int j = 0; j < 35; j++) {
		output = output + "0x";

		char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b','c','d','e','f' };

		std::string str = regTable->getData(j, 0);
		if (j == 0)
			str = "$pc";
		else if (j == 1)
			str = "$hi";
		else if (j == 2)
			str = "$lo";

		for (int i = registers.at(str).size() - 1; i >= 0; i--) {
			if (registers.at(str).at(i) != 0x0)
			{

				const char ch = registers.at(str).at(i);
				output.append(&hex[(ch & 0xF0) >> 4], 1);
				output.append(&hex[ch & 0xF], 1);

			}
			else {
				output = output + "00";

			}

		}
		QString string = QString::fromUtf8(output.c_str());
		regTable->setData(j, 2, string);
		
		output = "";
	}
	B->viewport()->update();
}

void VirtualMachineGUI::updateMemory() {
	
	for (int j = 0; j < (int)memory.size(); j++) {
		std::vector<int> ind;
		std::vector<unsigned char> index;
		ind.push_back(j);
		index = intToBytes1(ind);
		std::string output, location;
		output = output + "0x";
		location = location + "0x";
		char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a',   'b','c','d','e','f' };

		for (int i = index.size() - 1; i >= 0; i--) {
			if (index.at(i) != 0x0)
			{
				const char ch = index.at(i);
				location.append(&hex[(ch & 0xF0) >> 4], 1);
				location.append(&hex[ch & 0xF], 1);
			}
			else {
				location = location + "00";
			}
		}
		if (memory.at(j) != 0x00)
		{
			const char ch = memory.at(j);
			output.append(&hex[(ch & 0xF0) >> 4], 1);
			output.append(&hex[ch & 0xF], 1);
		}
		else {
			output = output + "00";
		}

		QString string1 = QString::fromUtf8(output.c_str());
		memTable->setData(j, 1, string1);
		QString string = QString::fromUtf8(location.c_str());
		memTable->setData(j, 0, string);
		output = "";
		location = "";
	}
	C->viewport()->update();
}

void VirtualMachineGUI::updateStatus() {
	if (error.size() > 0) {
	QString string = QString::fromUtf8(error.at(error.size() - 1).c_str());
	D->setText(string);
}
	D->update();
}


void VirtualMachineGUI::updateLine() {
	QList<QTextEdit::ExtraSelection> extraSelections;

	QBrush backBrush(Qt::yellow);
	QBrush textBrush(Qt::black);
	//QPen outlinePen(Qt::gray, 1);
	QTextBlock tb;
	int j = 0;
	if (instructionIndex.find(currentLine) != instructionIndex.end()) {
		tb = A->document()->findBlockByLineNumber(instructionIndex.at(currentLine));
		j = instructionIndex.at(currentLine);
	}
	else {
		
		tb = A->document()->findBlockByLineNumber(instructionIndex.at(currentLine - 1));
		backBrush = (Qt::white);
		j = instructionIndex.at(currentLine - 1);
	
	}
		QString highlighText = tb.text();
		highlighText = highlighText.trimmed();
		std::string utf8_text = highlighText.toUtf8().constData();

		for (int i = j; i < A->document()->blockCount(); i++)
		{
			QTextBlock block = A->document()->findBlockByNumber(i);
			if (block.isValid())
			{
				QString text = block.text();
				int p;

				if ((p = text.indexOf(highlighText)) != -1)
				{
					int pos = block.position() + p;

					QTextEdit::ExtraSelection selection;
					selection.cursor = QTextCursor(A->document());
					selection.cursor.setPosition(pos);
					selection.cursor.setPosition(pos + highlighText.length(), QTextCursor::KeepAnchor);
					selection.format.setBackground(backBrush);
					selection.format.setForeground(textBrush);
					//selection.format.setProperty(QTextFormat::OutlinePen, outlinePen);

					extraSelections.append(selection);
					break;
				}
			}

		}
	
	

	A->setExtraSelections(extraSelections);
	A->update();

}


MyModel* VirtualMachineGUI::getRegTable() {
	return regTable;
}

MyModel* VirtualMachineGUI::getMemTable() {
	return memTable;
}