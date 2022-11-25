// implement the parser here
#include "parser.hpp"
#include <ostream>
#include <iterator>
#include <iostream>
#include <vector>
#include "token.hpp"
#include "lexer.hpp"
#include "instruction.hpp"


//Parse a sequence of tokens and allocates memory
bool parse(const TokenList& tokens, Statetype state, InstructionList& instructions, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::map<std::string, std::vector<unsigned char>>& registers) {

    auto it = tokens.begin();
    InputValue input = classify(it, tokens.end(), state, labels);
    std::string name = it->value();
    std::vector<std::string> args;
    if (input == ERROR2) {
        return false;
    }
    do {
        name = it->value();
        switch (input) {
        case DATA:
            ++it;
            state = S1;
            return (update0(it, tokens) && (state != S2));
            break;
        case TEXT:
            ++it;
            state = S2;
            return update0(it, tokens);
            break;
        case WORD:
            ++it;
            return (update1(it, tokens, memory, labels, name) && state == S1);
            break;
        case HALF:
            ++it;
            return (update2(it, tokens, memory, labels, name) && state == S1);
            break;
        case BYTE:
            ++it;
            return (update3(it, tokens, memory, labels, name) && state == S1);
            break;
        case SPACE:
            ++it;
            return (update4(it, tokens, memory, labels, name) && state == S1);
            break;
        case ASCII:
            ++it;
            return (update5(it, tokens, memory, labels, name) && state == S1);
            break;
        case ASCIIZ:
            ++it;
            return (update6(it, tokens, memory, labels, name) && state == S1);
            break;
        case COMMENT:
            return true;
            break;
        case LABEL:

            ++it;
            input = classify(it, tokens.end());
            if (state == S2) {
                std::vector<int> indices;

                if (name == "main:") {

                    initialize(registers);
                    indices.push_back(instructions.size());
                    labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
                }
                else { //labels for control
                    std::vector<int> indices;


                    if (it->type() == STRING && it->value() == "nop") {
                        indices.push_back(-1);
                        labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
                        it++;
                        input = classify(it, tokens.end());
                    }
                    else {

                        indices.push_back(instructions.size());
                        labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
                    }
                }
            }
            else if (state == S1) {
                std::vector<int> indices;
                indices.push_back(memory.size());
                labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
            }

            break;
        case CONSTANT:
            ++it;
            return update16(it, tokens, memory, labels, name);
            break;
        case CASE1: //Data Movemet Instructions: lw, lh, lb, la, sw, sh, sb (CASE1)
            ++it;
            return (update7(it, tokens, instructions, name, labels) && state == S2);
            break;
        case CASE2: //Data Movemet Instructions: li (CASE2)
            ++it;
            return (update8(it, tokens, instructions, name) && state == S2);
            break;
            //Data Movemet Instructions: move | Integer Arithmetic Instructions: mult, multu, div, divu, abs, neg, negu (CASE3)
        case CASE3:
            ++it;
            return (update9(it, tokens, instructions, name) && state == S2);
            break;
            //Data Movemet Instructions: mfhi, mflo, mthi, mtlo (CASE4)
        case CASE4:
            ++it;
            return (update10(it, tokens, instructions, name) && state == S2);
            break;
            //Integer Arithmetic Instructions: add, addu, sub , subu, mul, mulo, mulou, rem, remu
            //Logical Instructions: and, nor, or, xor (CASE5)
        case CASE5:
            ++it;
            return (update11(it, tokens, instructions, name) && state == S2);
            break;
            //Logical Instructions: not
        case NOT:
            ++it;
            return (update12(it, tokens, instructions, name) && state == S2);
            break;
            //Control Instructions: j
        case J:
            ++it;
            return (update13(it, tokens, instructions, name) && state == S2);
            break;
            //Control Instructions: beq, bne, blt, ble, bgt, bge (CASE6)
        case CASE6:
            ++it;
            return (update14(it, tokens, instructions, name) && state == S2);
            break;
            //Integer Arithmetic Instructions: div, divu
        case DIV: case DIVU:
            ++it;
            return (update15(it, tokens, instructions, name) && state == S2);
            break;
            //Integer Arithmetic Instructions: div, divu
        case NOP:
            instructions.push_back(Instruction("nop", args));
            return true;
            break;
        case EOT:
            return true;
            break;
        default:
            return false;
        };
    } while (input != ERROR2);

    return false;

};

InputValue classify(TokenList::const_iterator it, TokenList::const_iterator end, Statetype state, std::map<std::string, std::vector<int>>& labels) {
    InputValue input = EOT;
    if (it == end) {
        input = EOT;
    }
    else if (it->type() == STRING) {

        if (it->value() == ".data") { // Data
            input = DATA;
        }
        else if (it->value() == ".word") {
            input = WORD;
        }
        else if (it->value() == ".half") {
            input = HALF;
        }
        else if (it->value() == ".byte") {
            input = BYTE;
        }
        else if (it->value() == ".space") {
            input = SPACE;
        }
        else if (it->value() == ".ascii") {
            input = ASCII;
        }
        else if (it->value() == ".asciiz") {
            input = ASCIIZ;
        }
        else if (it->value() == ".text") {
            input = TEXT;
        }
        else if (it->value() == "lw") { // Data Movements
            input = CASE1;
        }
        else if (it->value() == "lh") {
            input = CASE1;
        }
        else if (it->value() == "lb") {
            input = CASE1;
        }
        else if (it->value() == "li") {
            input = CASE2;
        }
        else if (it->value() == "la") {
            input = CASE1;
        }
        else if (it->value() == "sw") {
            input = CASE1;
        }
        else if (it->value() == "sh") {
            input = CASE1;
        }
        else if (it->value() == "lb") {
            input = CASE1;
        }
        else if (it->value() == "move") {
            input = CASE3;
        }
        else if (it->value() == "mfhi") {
            input = CASE4;
        }
        else if (it->value() == "mflo") {
            input = CASE4;
        }
        else if (it->value() == "mthi") {
            input = CASE4;
        }
        else if (it->value() == "mtlo") {
            input = CASE4;
        }
        else if (it->value() == "add") { // Integer Arithmetic
            input = CASE5;
        }
        else if (it->value() == "addu") {
            input = CASE5;
        }
        else if (it->value() == "sub") {
            input = CASE5;
        }
        else if (it->value() == "subu") {
            input = CASE5;
        }
        else if (it->value() == "mul") {
            input = CASE5;
        }
        else if (it->value() == "mulo") {
            input = CASE5;
        }
        else if (it->value() == "mulou") {
            input = CASE5;
        }
        else if (it->value() == "mult") {
            input = CASE3;
        }
        else if (it->value() == "multu") {
            input = CASE3;
        }
        else if (it->value() == "div") {
            input = CASE3;
        }
        else if (it->value() == "divu") {
            input = CASE3;
        }
        else if (it->value() == "rem") {
            input = CASE5;
        }
        else if (it->value() == "remu") {
            input = CASE5;
        }
        else if (it->value() == "abs") {
            input = CASE3;
        }
        else if (it->value() == "neg") {
            input = CASE3;
        }
        else if (it->value() == "negu") {
            input = CASE3;
        }
        else if (it->value() == "and") { // Logical
            input = CASE5;
        }
        else if (it->value() == "nor") {
            input = CASE5;
        }
        else if (it->value() == "not") {
            input = NOT;
        }
        else if (it->value() == "or") {
            input = CASE5;
        }
        else if (it->value() == "xor") {
            input = CASE5;
        }
        else if (it->value() == "j") { // Control
            input = J;
        }
        else if (it->value() == "beq") {
            input = CASE6;
        }
        else if (it->value() == "bne") {
            input = CASE6;
        }
        else if (it->value() == "blt") {
            input = CASE6;
        }
        else if (it->value() == "ble") {
            input = CASE6;
        }
        else if (it->value() == "bgt") {
            input = CASE6;
        }
        else if (it->value() == "bge") {
            input = CASE6;
        }
        else if (it->value() == "#" || it->value().find("#") != std::string::npos) {
            input = COMMENT;
        }
        else if (it->value() == "nop") {
            input = NOP;
        }
        else if ((it->value()[it->value().size() - 1] == ':') && isalpha((it->value()[0]))) {
            input = LABEL;

        }
        else if (isConstant(it)) {
            input = CONSTANT;
        }
        else {
            if (state == S1) {
                input = LABEL;
            }
            else {
                if (labels.find(it->value()) == labels.end()) {
                    input = ERROR2;
                }
                else {
                    input = LABEL;
                }
            }
        }


    }
    else if (it->type() == EOL) {
        input = EOT;
    }

    else if (it->type() == ERRORS) {
        input = ERROR2;
    }

    return input;
}

InputValue classify(TokenList::const_iterator it, TokenList::const_iterator end) {
    InputValue input = EOT;
    if (it == end) {
        input = EOT;
    }
    else if (it->type() == STRING) {

        if (it->value() == ".data") { // Data
            input = DATA;
        }
        else if (it->value() == ".word") {
            input = WORD;
        }
        else if (it->value() == ".half") {
            input = HALF;
        }
        else if (it->value() == ".byte") {
            input = BYTE;
        }
        else if (it->value() == ".space") {
            input = SPACE;
        }
        else if (it->value() == ".ascii") {
            input = ASCII;
        }
        else if (it->value() == ".asciiz") {
            input = ASCIIZ;
        }
        else if (it->value() == ".text") {
            input = TEXT;
        }
        else if (it->value() == "lw") { // Data Movements
            input = CASE1;
        }
        else if (it->value() == "lh") {
            input = CASE1;
        }
        else if (it->value() == "lb") {
            input = CASE1;
        }
        else if (it->value() == "li") {
            input = CASE2;
        }
        else if (it->value() == "la") {
            input = CASE1;
        }
        else if (it->value() == "sw") {
            input = CASE1;
        }
        else if (it->value() == "sh") {
            input = CASE1;
        }
        else if (it->value() == "lb") {
            input = CASE1;
        }
        else if (it->value() == "move") {
            input = CASE3;
        }
        else if (it->value() == "mfhi") {
            input = CASE4;
        }
        else if (it->value() == "mflo") {
            input = CASE4;
        }
        else if (it->value() == "mthi") {
            input = CASE4;
        }
        else if (it->value() == "mtlo") {
            input = CASE4;
        }
        else if (it->value() == "add") { // Integer Arithmetic
            input = CASE5;
        }
        else if (it->value() == "addu") {
            input = CASE5;
        }
        else if (it->value() == "sub") {
            input = CASE5;
        }
        else if (it->value() == "subu") {
            input = CASE5;
        }
        else if (it->value() == "mul") {
            input = CASE5;
        }
        else if (it->value() == "mulo") {
            input = CASE5;
        }
        else if (it->value() == "mulou") {
            input = CASE5;
        }
        else if (it->value() == "mult") {
            input = CASE3;
        }
        else if (it->value() == "multu") {
            input = CASE3;
        }
        else if (it->value() == "div") {
            input = CASE3;
        }
        else if (it->value() == "divu") {
            input = CASE3;
        }
        else if (it->value() == "rem") {
            input = CASE5;
        }
        else if (it->value() == "remu") {
            input = CASE5;
        }
        else if (it->value() == "abs") {
            input = CASE3;
        }
        else if (it->value() == "neg") {
            input = CASE3;
        }
        else if (it->value() == "negu") {
            input = CASE3;
        }
        else if (it->value() == "and") { // Logical
            input = CASE5;
        }
        else if (it->value() == "nor") {
            input = CASE5;
        }
        else if (it->value() == "not") {
            input = NOT;
        }
        else if (it->value() == "or") {
            input = CASE5;
        }
        else if (it->value() == "xor") {
            input = CASE5;
        }
        else if (it->value() == "j") { // Control
            input = J;
        }
        else if (it->value() == "beq") {
            input = CASE6;
        }
        else if (it->value() == "bne") {
            input = CASE6;
        }
        else if (it->value() == "blt") {
            input = CASE6;
        }
        else if (it->value() == "ble") {
            input = CASE6;
        }
        else if (it->value() == "bgt") {
            input = CASE6;
        }
        else if (it->value() == "bge") {
            input = CASE6;
        }
        else if (it->value() == "#" || it->value().find("#") != std::string::npos) {
            input = COMMENT;
        }
        else if (it->value() == "nop") {
            input = NOP;
        }
        else if ((it->value()[it->value().size() - 1] == ':') && isalpha((it->value()[0]))) {
            input = LABEL;

        }
        else if (isConstant(it)) {
            input = CONSTANT;
        }
        else {
            input = LABEL;
        }


    }
    else if (it->type() == EOL) {
        input = EOT;
    }

    else if (it->type() == ERRORS) {
        input = ERROR2;
    }

    return input;
}

//Checks to see if stryng is 32-bit Int
bool is32Int(std::string str) {
    if (str.size() == 0) {
        return false;
    }
    int i = 0;
    long long upperbound = 0;
    long lowerbound = 0;
    if (str.substr(0, 1) == "-" || str.substr(0, 1) == "+") {
        upperbound = 2147483647;
        lowerbound = -2147483648;
    }
    else {
        upperbound = 4294967295;
        lowerbound = 0;
    }
    try {
        i = stoi(str);

    }
    catch (...)
    {
        return false;
    }
    if (i <= upperbound && i >= lowerbound) {
        return true;
    }
    return false;
}
//Checks to see if stryng is 16-bit Int
bool is16Int(std::string str) {
    if (str.size() == 0) {
        return false;
    }
    int i = 0;
    long upperbound = 0;
    long lowerbound = 0;
    if (str.substr(0, 1) == "-" || str.substr(0, 1) == "+") {
        upperbound = 32767;
        lowerbound = -32768;
    }
    else {
        upperbound = 65535;
        lowerbound = 0;
    }
    try {
        i = stoi(str);

    }
    catch (...)
    {
        return false;
    }
    if (i <= upperbound && i >= lowerbound) {
        return true;
    }
    return false;
}

//Checks to see if string is byte
bool isByte(std::string str) {
    if (str.size() == 0) {
        return false;
    }
    int i = -1000;
    long upperbound = 0;
    long lowerbound = 0;
    if (str.substr(0, 1) == "-" || str.substr(0, 1) == "+") {
        upperbound = 127;
        lowerbound = -128;
    }
    else {
        upperbound = 255;
        lowerbound = 0;
    }
    try {
        i = stoi(str);

    }
    catch (...)
    {
        return false;
    }
    if (i <= upperbound && i >= lowerbound) {
        return true;
    }
    return false;
}

//Checks to see if string is constant
bool isConstant(TokenList::const_iterator it) {
    bool isConstant = true;
    int size = it->value().size();
    if (size <= 0) {
        return false;
    }
    for (int i = 0; i < size; i++) {
        if (i == 0 && it->value()[i] >= 0x30 && it->value()[i] <= 0x39) {
            return false;
        }
        else if (it->value()[i] >= 0x30 && it->value()[i] <= 0x39) {
            isConstant = true;
        }
        else if (it->value()[i] >= 0x41 && it->value()[i] <= 0x5a) {
            isConstant = true;
        }
        else {
            return false;
        }


    }
    return isConstant;
}


//Checks to see if string is memref
std::string isMemref(TokenList::const_iterator it, const TokenList& tokens, std::map<std::string, std::vector<int>>& labels) {
    std::string str = "";
    if (it->type() == STRING && isRegister(it)) {
        if ((classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT)) {
            return it->value();
        }
    }
    else if (it->type() == STRING && isNumber(it->value())) {
        str = str + it->value();
        it++;
        if (it->type() == OPEN_PAREN) {
            str = str + "(";
            it++;
            if (it->type() == STRING && isRegister(it)) {
                str = str + it->value();
                it++;
                if (it->type() == CLOSE_PAREN) {
                    str = str + ")";
                    it++;
                    if ((classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT)) {
                        return str;
                    }
                }
            }
            else if (it->type() == STRING && labels.find(it->value()) != labels.end()) {
                str = str + it->value();
                it++;
                if (it->type() == CLOSE_PAREN) {
                    str = str + ")";
                    it++;
                    if ((classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT)) {
                        return str;
                    }
                }
            }
        }
    }
    else if (it->type() == OPEN_PAREN) {
        str = str + "(";
        it++;
        if (it->type() == STRING && (isRegister(it))) {
            str = str + it->value();
            it++;
            if (it->type() == CLOSE_PAREN) {
                str = str + ")";
                it++;
                if ((classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT)) {
                    return str;
                }
            }
        }
    }
    else if (it->type() == STRING) {
        it++;
        if ((classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT)) {
            return it->value();
        }

    }
    return "";
}


//Checks to see if string is register
bool isRegister(TokenList::const_iterator it) {
    int size = -1;
    if (it->type() == STRING && it->value().substr(0, 1) == "$") {
        size = it->value().size();
        for (int i = 1; i < size; i++) {
            if (!isalnum(it->value()[i])) {
                return false;
            }

        }
        int i = 33;
        if (isNumber(it->value().substr(1, 3))) {
            try {
                i = stoi(it->value().substr(1, 3));

            }
            catch (...)
            {
                return false;
            }
            if (i > 31) {
                return false;
            }
        }



        return true;
    }


    return false;
}

//Checks to see if string is immediate
bool isImmediate(TokenList::const_iterator it) {
    if (it->type() == STRING && is32Int(it->value())) {
        return true;
    }
    else if (it->type() == STRING) {
        std::string str = it->value();
        if (isalpha(str[0])) {
            int size = str.size();
            for (int i = 1; i < size; i++) {
                if (!isalnum(str[i])) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

//Checks to see if string is source
bool isSource(TokenList::const_iterator it) {
    if (it->type() == STRING && isRegister(it)) {
        return true;

    }
    if (isImmediate(it)) {
        return true;
    }
    return false;
}

//Convert ints to vector of bytes
std::vector<unsigned char> intToBytes(std::vector<int> paramInt)
{
    std::vector<unsigned char> arrayOfByte(4 * paramInt.size());
    int size = paramInt.size();
    for (int j = 0; j < size; j++) {
        for (int i = 0; i < 4 * size; i++) {
            arrayOfByte[(4 * size) - 1 - i] = (paramInt.at(j) >> (i * 8));
        }
    }
    return arrayOfByte;
}

/*

Update methods parse token sequences to check for correct syntax.
Return false if incorrect sequence of tokens for the designated instruction;

*/

//Handles .data
bool update0(TokenList::const_iterator it, const TokenList& tokens) {

    if (it->type() == EOL || classify(it, tokens.end()) == COMMENT) {
        return true;
    }
    return false;
}

//Handles .word
bool update1(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::string name) {
    bool error = true;
    std::vector<int> numbers;
    int count = 0;
    while (is32Int(it->value())) {
        numbers.push_back(stoi(it->value()));
        count++;
        it++;
        if (it->type() == SEP) {
            it++;
            if ((it == tokens.end() || it->type() != STRING) || !is32Int(it->value())) {
                error = true;
                break;
            }
            else {
                numbers.push_back(stoi(it->value()));
                count++;
                error = false;
            }
        }
        else if ((classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) && count > 0) {
            error = false;
            break;
        }
        else {
            error = true;
            break;
        }

    }
    std::vector<int> indices;
    if ((it != tokens.end() && it->type() == STRING) && (isConstant(it) && labels.find(it->value()) != labels.end())) {
        error = false;
        labels.at(name.substr(0, name.size() - 1)) = labels.at(it->value());
    }
    else {
        std::vector<int> indices;
        if (error == false) {
            std::vector<unsigned char> mem = intToBytes(numbers);
            for (int i = 0; i < (int)mem.size(); i++) {
                indices.push_back(memory.size()); // saves index in memory
                memory.push_back(mem.at((mem.size() - 1) - i)); // adds bytes to memory
            }
            labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
        }
    }
    return !error;
}

//Handles .half
bool update2(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::string name) {
    bool error = true;
    std::vector<int> numbers;
    while (is16Int(it->value())) {
        numbers.push_back(stoi(it->value()));
        it++;
        if (it->type() == SEP) {
            it++;
            if ((it == tokens.end() || it->type() != STRING) || !is16Int(it->value())) {
                error = true;
                break;
            }
            else {
                numbers.push_back(stoi(it->value()));
                error = false;
            }
        }
        else if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
            error = false;
            break;
        }
        else {
            error = true;
            break;
        }

    }
    std::vector<int> indices;
    if (error == false) {
        std::vector<unsigned char> mem = intToBytes(numbers);
        for (int i = 0; i < (int)mem.size(); i++) {
            indices.push_back(memory.size()); // saves index in memory
            memory.push_back(mem.at((mem.size() - 1) - i)); // adds bytes to memory
        }
        labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
    }
    return !error;
}

//Handles .byte
bool update3(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::string name) {
    bool error = true;
    std::vector<int> numbers;
    while (isByte(it->value())) {
        numbers.push_back(stoi(it->value()));
        it++;
        if (it->type() == SEP) {
            it++;
            if ((it == tokens.end() || it->type() != STRING) || !isByte(it->value())) {
                error = true;
                break;
            }
            else {
                numbers.push_back(stoi(it->value()));
                error = false;
            }
        }
        else if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
            error = false;
            break;
        }
        else {
            error = true;
            break;
        }

    }
    std::vector<int> indices;
    if (error == false) {
        for (int i = 0; i < (int)numbers.size(); i++) {
            indices.push_back(memory.size());
            memory.push_back(numbers.at(i)); // adds bytes to memory      
        }
        labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
    }

    return !error;
}


//Handles .space
bool update4(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::string name) {
    int number = 0;
    if (is32Int(it->value())) {
        number = stoi(it->value());
        ++it;
        if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
            std::vector<int> indices;
            for (int i = 0; i < number; i++) {
                indices.push_back(memory.size()); // saves index in memory
                memory.push_back(0); // adds bytes to memory
            }
            labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
            return true;
        }
    }
    else if (isConstant(it)) { //TODO
        ++it;
        if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
            return true;
        }
    }


    return false;
}

//Handles .ascii
bool update5(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::string name) {
    std::string str = "";
    if (it->type() == STRING) {
        str = it->value();
        it++;
        if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
            std::vector<int> indices;
            for (int i = 0; i < (int)str.size(); i++) {
                indices.push_back(memory.size()); // saves index in memory
                memory.push_back(str[i]); // adds bytes to memory
            }
            labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));

            return true;
        }
    }
    else if (it->type() == STRING_DELIM) {
        it++;
        if (it->type() == STRING) {
            str = it->value();
            it++;
            if (it->type() == STRING_DELIM) {
                it++;
                if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                    std::vector<int> indices;
                    for (int i = 0; i < (int)str.size(); i++) {
                        indices.push_back(memory.size()); // saves index in memory
                        memory.push_back(str[i]); // adds bytes to memory
                    }
                    labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));

                    return true;
                }

            }
        }
    }
    return false;
}

//Handles .asciiz
bool update6(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::string name) {
    std::string str = "";
    if (it->type() == STRING) {
        str = it->value();
        it++;
        if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
            std::vector<int> indices;
            for (int i = 0; i < (int)str.size(); i++) {
                indices.push_back(memory.size()); // saves index in memory
                memory.push_back(str[i]); // adds bytes to memory
            }
            indices.push_back(memory.size()); // saves index in memory
            memory.push_back(0x0); // adds bytes to memory
            labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
            return true;
        }
    }
    else if (it->type() == STRING_DELIM) {
        it++;
        if (it->type() == STRING) {
            str = it->value();
            it++;
            if (it->type() == STRING_DELIM) {
                it++;
                if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                    std::vector<int> indices;
                    for (int i = 0; i < (int)str.size(); i++) {
                        indices.push_back(memory.size()); // saves index in memory
                        memory.push_back(str[i]); // adds bytes to memory
                    }
                    indices.push_back(memory.size()); // saves index in memory
                    memory.push_back(0x0); // adds bytes to memory
                    labels.insert(std::pair<std::string, std::vector<int>>(name.substr(0, name.size() - 1), indices));
                    return true;
                }

            }
        }
    }
    return false;
}

//Handles Data Movemet Instructions: lw, lf, lb, la, sw, sh, sb
bool update7(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type, std::map<std::string, std::vector<int>>& labels) {
    std::vector<std::string> args;
    std::string val = "";
    if (isRegister(it)) {
        args.push_back(it->value());
        ++it;
        if (it->type() == SEP) {
            ++it;
            val = isMemref(it, tokens, labels);
            if (val != "") {
                args.push_back(val);
                instructions.push_back(Instruction(type, args));
                return true;
            }
            else if (it->type() == STRING) {
                args.push_back(it->value());
                instructions.push_back(Instruction(type, args));
                it++;
                if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                    return true;
                }
            }
        }
    }
    return false;
}

//Handles Data Movemet Instructions: li
bool update8(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type) {
    std::vector<std::string> args;
    if (isRegister(it)) {
        args.push_back(it->value());
        ++it;
        if (it->type() == SEP) {
            ++it;
            if (isImmediate(it)) {
                args.push_back(it->value());
                instructions.push_back(Instruction(type, args));
                ++it;
                if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                    return true;
                }
            }
        }
    }
    return false;
}

//Handles Data Movemet Instructions: move | Integer Arithmetic Instructions: mult, multu, div, divu, abd, neg, negu
bool update9(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type) {
    std::vector<std::string> args;
    if (isRegister(it)) {
        args.push_back(it->value());
        ++it;
        if (it->type() == SEP) {
            ++it;
            if (isRegister(it)) {
                args.push_back(it->value());
                instructions.push_back(Instruction(type, args));
                ++it;
                if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                    return true;
                }
            }
        }
    }
    return false;
}

//Handles Data Movemet Instructions: mfhi, mflo, mthi, mtlo
bool update10(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type) {
    std::vector<std::string> args;
    if (isRegister(it)) {
        args.push_back(it->value());
        instructions.push_back(Instruction(type, args));
        ++it;
        if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
            return true;
        }

    }
    return false;
}

//Handles Integer Arithmetic Instructions: add, addu, sub , subu, mul, mulo, mulou, rem, remu
//Handles Logical Instructions: and, nor, or, xor
bool update11(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type) {
    std::vector<std::string> args;
    if (isRegister(it)) {
        args.push_back(it->value());
        ++it;
        if (it->type() == SEP) {
            ++it;
            if (isRegister(it)) {
                args.push_back(it->value());
                ++it;
                if (it->type() == SEP) {
                    ++it;
                    if (isSource(it)) {
                        args.push_back(it->value());
                        instructions.push_back(Instruction(type, args));
                        ++it;
                        if ((classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) && it->type() != SEP) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

//Handles Logical Instructions: not
bool update12(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type) {
    std::vector<std::string> args;
    if (isRegister(it)) {
        args.push_back(it->value());
        ++it;
        if (it->type() == SEP) {
            ++it;
            if (isSource(it)) {
                args.push_back(it->value());
                instructions.push_back(Instruction(type, args));
                ++it;
                if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                    return true;
                }
            }
        }
    }
    return false;
}

//Handles Control Instructions: j
bool update13(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type) {
    std::vector<std::string> args;
    if (classify(it, tokens.end()) == LABEL) {
        args.push_back(it->value());
        instructions.push_back(Instruction(type, args));
        ++it;

        if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {

            return true;

        }
    }
    return false;
}

//Handles Control Instructions: beq, bne, blt, ble, bgt, bge
bool update14(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type) {
    std::vector<std::string> args;
    if (isRegister(it)) {
        args.push_back(it->value());
        ++it;
        if (it->type() == SEP) {
            ++it;
            if (isSource(it)) {
                args.push_back(it->value());
                ++it;
                if (it->type() == SEP) {
                    ++it;
                    if (classify(it, tokens.end()) == LABEL) {
                        args.push_back(it->value());
                        instructions.push_back(Instruction(type, args));
                        ++it;
                        if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}


//Handles Integer Arithmetic Instructions: div, divu
bool update15(TokenList::const_iterator it, const TokenList& tokens, InstructionList& instructions, std::string type) {
    std::vector<std::string> args;
    if (isRegister(it)) {
        args.push_back(it->value());
        ++it;
        if (it->type() == SEP) {
            ++it;
            if (isRegister(it)) {
                args.push_back(it->value());
                ++it;
                if (it->type() == SEP) {
                    ++it;
                    if (isSource(it)) {
                        args.push_back(it->value());
                        instructions.push_back(Instruction(type, args));
                        ++it;
                        if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                            return true;
                        }
                    }
                }
                else if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                    instructions.push_back(Instruction(type, args));
                    --it;
                    return true;
                }
            }
        }
    }
    return false;
}

//Handles Constant
bool update16(TokenList::const_iterator it, const TokenList& tokens, std::vector<unsigned char>& memory, std::map<std::string, std::vector<int>>& labels, std::string name) {
    std::vector<std::string> args;

    if (it->type() == EQUAL) {
        it++;
        if (it->type() == STRING && is32Int(it->value())) {

            std::vector<int> indices;
            std::vector<int> number;
            number.push_back(stoi(it->value()));
            std::vector<unsigned char> mem = intToBytes(number);
            for (int i = 0; i < (int)mem.size(); i++) {
                indices.push_back(memory.size()); // saves index in memory
                memory.push_back(mem.at(3 - i)); // adds bytes to memory
            }
            labels.insert(std::pair<std::string, std::vector<int>>(name, indices));

            it++;
            if (classify(it, tokens.end()) == EOT || classify(it, tokens.end()) == COMMENT) {
                return true;
            }
        }

    }
    return false;
}

//Initializes registers map
void initialize(std::map<std::string, std::vector<unsigned char>>& registers) {
    std::vector<unsigned char> indices5;

    registers.insert(std::pair<std::string, std::vector<unsigned char>>("$pc", indices5)); //Allocates memory for pc

    std::vector<unsigned char> indices6;

    registers.insert(std::pair<std::string, std::vector<unsigned char>>("$hi", indices6)); //Allocates memory for hi
    std::vector<unsigned char> indices7;

    registers.insert(std::pair<std::string, std::vector<unsigned char>>("$lo", indices7)); //Allocates memory for lo

    std::vector<unsigned char> indices1;

    registers.insert(std::pair<std::string, std::vector<unsigned char>>("$gp", indices1)); //Allocates memory for gp

    std::vector<unsigned char> indices2;

    registers.insert(std::pair<std::string, std::vector<unsigned char>>("$sp", indices2)); //Allocates memory for sp
    std::vector<unsigned char> indices3;

    registers.insert(std::pair<std::string, std::vector<unsigned char>>("$fp", indices3)); //Allocates memory for fp

    std::vector<unsigned char> indices4;

    registers.insert(std::pair<std::string, std::vector<unsigned char>>("$ra", indices4)); //Allocates memory for ra

    for (int i = 0; i < 4; i++) {
        registers.at("$gp").push_back(0x0);
        registers.at("$sp").push_back(0x0);
        registers.at("$fp").push_back(0x0);
        registers.at("$ra").push_back(0x0);
    }

    for (int i = 0; i < 4; i++) {
        registers.at("$pc").push_back(0x0);
        registers.at("$hi").push_back(0x0);
        registers.at("$lo").push_back(0x0);
    }
    std::vector<unsigned char> reg;
    for (int i = 0; i < 32; i++) {
        registers.insert(std::pair<std::string, std::vector<unsigned char>>("$" + std::to_string(i), reg)); //Initializes registers
        for (int j = 0; j < 4; j++) {
            registers.at("$" + std::to_string(i)).push_back(0x0);
        }
    }

}
