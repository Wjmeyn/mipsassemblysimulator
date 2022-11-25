#include "catch.hpp"

#include "parser.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
// put your parser tests here 

TEST_CASE("test data", "[parser]") {

    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //incoorect
    {
       
        std::string input = ".data 5";
        std::istringstream iss(input);

        TokenList tl = tokenize(iss);
        
       
        
        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S0, instructions, memory, labels, registers) == false);
    }

    //correct
    {
        std::string input = ".data";
        std::istringstream iss(input);
        TokenList t2 = tokenize(iss);
        REQUIRE(t2.size() == 2);
        REQUIRE(parse(t2, S0, instructions, memory, labels, registers) == true);
    }
    
}

TEST_CASE("test Word", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //fail
    {
        
        std::string input = ".word 5, 777773737373737";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

    //pass
    {
     
        std::string input = ".word 20, -5";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss); 

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }

    //pass
    {
        std::string input = "var1:  .word 1024             # int var1 = 1024";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);
        REQUIRE(tl.size() == 9);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }

    //fail
    {
        std::string input = "var1:  .word 6, 102400000000000000000";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);
        REQUIRE(tl.size() == 6);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }


    //fail
    {
        std::string input = "var1:  .word 6, ";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);
        REQUIRE(tl.size() == 4);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

}

TEST_CASE("test Half", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //incoorect
    {

        std::string input = ".half 5, 70000";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

    //correct
    {

        std::string input = ".half -5, 777";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }


    //fail
    {
        std::string input = ".half 6, 102400000000000000000";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);
        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }


    //fail
    {
        std::string input = ".half  .word 6, ";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);
        REQUIRE(tl.size() == 4);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

}

TEST_CASE("test Byte", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //incoorect
    {

        std::string input = ".byte 5, 300";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

    //fail
    {
        std::string input = ".byte -500, 20";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);
        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

    //pass
    {

        std::string input = ".byte 5, 130";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }


    //fail
    {
        std::string input = ".byte 6, 102400000000000000000";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);
        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }


    //fail
    {
        std::string input = ".byte 6, ";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);
        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

}

TEST_CASE("test Space", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //fail
    {

        std::string input = ".space 5999999999999999999999";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

    //pass
    {

        std::string input = ".space 3";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }

    //pass
    {

        std::string input = ".space ARR";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }

}

TEST_CASE("test Ascii", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //fail
    {

        std::string input = ".ascii\n";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 2);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

    //pass
    {

        std::string input = ".ascii hello";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }

    //fail
    {

        std::string input = ".ascii \"hello";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

    //pass, test with string delimiters
    {

        std::string input = ".ascii \"hello\"";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 4);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }

}


TEST_CASE("test Asciiz", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //fail
    {

        std::string input = ".asciiz\n";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 2);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

    //pass
    {

        std::string input = ".asciiz hello";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }

    //fail
    {

        std::string input = ".asciiz \"hello";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == false);
    }

    //pass, test with string delimiters
    {

        std::string input = ".asciiz \"hello\"";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 4);
        REQUIRE(parse(tl, S1, instructions, memory, labels, registers) == true);
    }

}

TEST_CASE("test Data Movement", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //correct
    {

        std::string input = "lw $30, arr";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }

    //correct
    {

        std::string input = "lw $30, 1($t0)";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 7);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }
    //correct
    {

        std::string input = "lw $30, 1(arr)";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 7);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }

    {

        std::string input = "nop";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 2);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }

    //incorrect
    {

        std::string input = "lh $30, (8288)";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 6);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == false);
    }

    //correct
    {
        std::string input = "li $30, 11";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }
    //incorrect
    {

        std::string input = "li $30, (8288)";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 6);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == false);
    }

    //correct
    {
        std::string input = "move $30, $b0";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }
    //incorrect
    {

        std::string input = "move $30";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == false);
    }

    //correct
    {
        std::string input = "mfhi $30";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }
    //incorrect
    {

        std::string input = "mflo ,";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 2);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == false);
    }

}


TEST_CASE("test Arithmetic", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //correct
    {

        std::string input = "add $15, $30, 100";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 7);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }

    //incorrect
    {

        std::string input = "addu hello";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == false);
    }

    //correct
    {

        std::string input = "mult $15, $30";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }

    //pass
    {

        std::string input = "div $15, $t0 ";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }

    //incorrect
    {

        std::string input = "multu hello";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == false);
    }

}


TEST_CASE("test Logical", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //correct
    {

        std::string input = "not $05, 20";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 5);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }

    //incorrect
    {

        std::string input = "not hello";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == false);
    }

}

TEST_CASE("test Control", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //correct
    {

        std::string input = "j end";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 3);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }

    //incorrect
    {

        std::string input = "j";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 2);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == false);
    }

    //correct
    {

        std::string input = "beq $12, 100, end:";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 7);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }

    //incorrect
    {

        std::string input = "ble $20, tucan,,,";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 7);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == false);
    }

}

TEST_CASE("test Constant", "[parser]") {

    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //correct
    {

        std::string input = "TRUE = 1";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 4);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }



}

TEST_CASE("test Label", "[parser]") {
    std::map<std::string, std::vector<int>> labels;
    std::map<std::string, std::vector<unsigned char>> registers;
    std::vector<unsigned char> memory;
    std::vector<Instruction> instructions;
    //pass
    {
        std::string input = "label:";
        std::istringstream iss(input);
        TokenList tl = tokenize(iss);

        REQUIRE(tl.size() == 2);
        REQUIRE(parse(tl, S2, instructions, memory, labels, registers) == true);
    }
}


