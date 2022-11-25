#ifndef VIRTUAL_MACHINE_GUI_HPP
#define VIRTUAL_MACHINE_GUI_HPP

#include <QString>
#include <QWidget>
#include <QPlainTextEdit>
#include <QTableView>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "instruction.hpp"
#include "simulator.hpp"
#include "mymodel.hpp"
#include "parser.hpp"
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <thread>
#include <mutex>

// TODO define the GUI class
class QPlainTextEdit;
class QTableView;
class QLineEdit;
class QPushButton;
class QTextBlock;

class VirtualMachineGUI: public QWidget{
	Q_OBJECT

public:
	VirtualMachineGUI(QWidget* parent = nullptr);
	void load(QString string);
	MyModel* getRegTable();
	MyModel* getMemTable();
	void stepProgram();
	int getRunStatus();
	void setRunStatus(int status);
	
public slots:
	void step();
	void breakRun();
	void run();

private:
	QPlainTextEdit* A;
	QTableView* B;
	QTableView* C;
	QLineEdit* D;
	QPushButton* E;
	QPushButton* F;
	QPushButton* G;
	MyModel* regTable;
	MyModel* memTable;
	int mainPointer;
	int currentLine;
	bool fileError;
	int runProgram;
	std::vector<unsigned char> memory;
	InstructionList instructions;
	std::map<std::string, std::vector<int>> labels;
	std::map<std::string, std::vector<unsigned char>> registers;
	std::map<int, int> instructionIndex;
	std::vector<std::string> error;
	void updateRegisters();
	void updateMemory();
	void initializeTables();
	void updateStatus();
	void updateLine();
};


#endif
