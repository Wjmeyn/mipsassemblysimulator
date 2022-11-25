#include <QTest>
#include <QPlainTextEdit>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>

#include "virtual_machine_gui.hpp"

#include "test_config.hpp"

class VirtualMachineGUITest : public QObject {
	Q_OBJECT

private slots:

	void initTestCase();

	// TODO: implement additional tests here
	void testStep();
	void testRegisters();
	void testMemory();
	void testMissingFile();
	void testRun();

private:

	VirtualMachineGUI widget;
};

// this section just verifies the object names and API
void VirtualMachineGUITest::initTestCase() {

	widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test00.asm"));

	auto textWidget = widget.findChild<QPlainTextEdit*>("text");
	QVERIFY2(textWidget, "Could not find QPLainText widget");

	auto registerViewWidget = widget.findChild<QTableView*>("registers");
	QVERIFY2(registerViewWidget, "Could not find QTableView widget for registers");

	auto memoryViewWidget = widget.findChild<QTableView*>("memory");
	QVERIFY2(memoryViewWidget, "Could not find QTableView widget for memory");

	auto statusViewWidget = widget.findChild<QLineEdit*>("status");
	QVERIFY2(statusViewWidget, "Could not find QLineEdit widget for status");

	auto stepButtonWidget = widget.findChild<QPushButton*>("step");
	QVERIFY2(stepButtonWidget, "Could not find QTableView widget for step");

	auto runButtonWidget = widget.findChild<QPushButton*>("run");
	QVERIFY2(runButtonWidget, "Could not find QTableView widget for run");

	auto breakButtonWidget = widget.findChild<QPushButton*>("break");
	QVERIFY2(breakButtonWidget, "Could not find QTableView widget for break");

	
}

// tests step()
void VirtualMachineGUITest::testStep() {

	widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test01.asm"));

	widget.step();
	QString pc = widget.getRegTable()->getModel()[0][2];
	bool test(pc == "0x00000001");
	QVERIFY2(test, "Program does not step correctly");

	widget.step();
	pc = widget.getRegTable()->getModel()[0][2];
	test = pc == "0x00000002";
	QVERIFY2(test, "Program does not step correctly");

	widget.step();
	widget.step();
	widget.step();
	widget.step();
	widget.step();
	widget.step();
	pc = widget.getRegTable()->getModel()[0][2];
	test = pc == "0x00000006";
	QVERIFY2(test, "Program does not step correctly");


}

// tests registers()
void VirtualMachineGUITest::testRegisters() {

	widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test03.asm"));

	//$t0
	widget.step();
	QString reg = widget.getRegTable()->getModel()[11][2];
	bool test(reg == "0x00000064");
	QVERIFY2(test, "Incorrect register value");

	//$t1
	widget.step();
	reg = widget.getRegTable()->getModel()[12][2];
	test = reg == "0x00000000";
	QVERIFY2(test, "Incorrect register value");

	//$t2
	widget.step();
	reg = widget.getRegTable()->getModel()[13][2];
	test = reg == "0x00000001";
	QVERIFY2(test, "Incorrect register value");

	//$t3
	widget.step();
	reg = widget.getRegTable()->getModel()[14][2];
	test = reg == "0x00000002";
	QVERIFY2(test, "Incorrect register value");

	//$t4
	widget.step();
	reg = widget.getRegTable()->getModel()[15][2];
	test = reg == "0x00000004";
	QVERIFY2(test, "Incorrect register value");




}

// tests registers()
void VirtualMachineGUITest::testMemory() {

	widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test09.asm"));

	//&0x00000000
	widget.step();
	QString reg = widget.getMemTable()->getModel()[0][1];
	bool test(reg == "0x04");
	QVERIFY2(test, "Incorrect register value");

	//&0x00000001
	widget.step();
	reg = widget.getMemTable()->getModel()[1][1];
	test = reg == "0x00";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000002
	widget.step();
	reg = widget.getMemTable()->getModel()[2][1];
	test = reg == "0x00";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000003
	widget.step();
	reg = widget.getMemTable()->getModel()[3][1];
	test = reg == "0x00";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000004
	widget.step();
	reg = widget.getMemTable()->getModel()[4][1];
	test = reg == "0xfc";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000005
	widget.step();
	reg = widget.getMemTable()->getModel()[5][1];
	test = reg == "0xff";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000006
	widget.step();
	reg = widget.getMemTable()->getModel()[6][1];
	test = reg == "0xff";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000007
	widget.step();
	reg = widget.getMemTable()->getModel()[7][1];
	test = reg == "0xff";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000008
	widget.step();
	reg = widget.getMemTable()->getModel()[8][1];
	test = reg == "0x02";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000009
	widget.step();
	reg = widget.getMemTable()->getModel()[9][1];
	test = reg == "0x00";
	QVERIFY2(test, "Incorrect register value");

	//&0x0000000a
	widget.step();
	reg = widget.getMemTable()->getModel()[10][1];
	test = reg == "0x00";
	QVERIFY2(test, "Incorrect register value");

	//&0x0000000b
	widget.step();
	reg = widget.getMemTable()->getModel()[11][1];
	test = reg == "0x00";
	QVERIFY2(test, "Incorrect register value");

	//&0x0000000c
	widget.step();
	reg = widget.getMemTable()->getModel()[12][1];
	test = reg == "0xfe";
	QVERIFY2(test, "Incorrect register value");

	//&0x0000000d
	widget.step();
	reg = widget.getMemTable()->getModel()[13][1];
	test = reg == "0xff";
	QVERIFY2(test, "Incorrect register value");

	//&0x0000000e
	widget.step();
	reg = widget.getMemTable()->getModel()[14][1];
	test = reg == "0xff";
	QVERIFY2(test, "Incorrect register value");

	//&0x0000000f
	widget.step();
	reg = widget.getMemTable()->getModel()[15][1];
	test = reg == "0xff";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000010
	widget.step();
	reg = widget.getMemTable()->getModel()[16][1];
	test = reg == "0x00";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000011
	widget.step();
	reg = widget.getMemTable()->getModel()[17][1];
	test = reg == "0x00";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000012
	widget.step();
	reg = widget.getMemTable()->getModel()[18][1];
	test = reg == "0x00";
	QVERIFY2(test, "Incorrect register value");

	//&0x00000013
	widget.step();
	reg = widget.getMemTable()->getModel()[19][1];
	test = reg == "0x40";
	QVERIFY2(test, "Incorrect register value");

}

// tests registers()
void VirtualMachineGUITest::testMissingFile() {
	widget.load(QString::fromStdString(TEST_FILE_DIR + "missingfile"));

	auto statusViewWidget = widget.findChild<QLineEdit*>("status");
	bool test(statusViewWidget->text().contains("Error"));
	QVERIFY2(test, "Could not find QLineEdit widget for status");

}

// tests run()
void VirtualMachineGUITest::testRun() {

	{
		VirtualMachineGUI widget1;

		widget1.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test01.asm"));

		widget1.run();
		widget1.breakRun();
		widget1.run();

		widget1.stepProgram();

		QString pc = widget1.getRegTable()->getModel()[0][2];
		bool test(pc == "0x00000006");
		QVERIFY2(test, "Program does not step correctly");
	}

}

QTEST_MAIN(VirtualMachineGUITest)
#include "virtual_machine_gui_test.moc"

