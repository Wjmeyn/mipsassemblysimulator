#include "lexer.hpp"
#include <ostream>
#include <iostream>
// implement the tokenize function here
// currently it just always returns an empty TokenList
TokenList tokenize(std::istream& ins) {

	TokenList list;
	TokenType type = EOL;
	char c; 
	std::string word = "";
	int line = 1; // Keeps track of line number
	int count = 0; // Used for strings
	int error1 = 0; //Error tracking for open parenthesis
    int error2 = 0; //Error tracking for closed parenthesis
	int error3 = 0; //Error tracking for string delimiter
	bool beg = true; //Used for string tracking
	while (ins.get(c)) { //Reads each char in the line
		

			
			if (c == '\n' && beg != true) { // EOL
				if (count > 0)
				{
					type = STRING;
					list.push_back(Token(type, line, word)); // Adds token to list
					word = "";
					count = 0;
				}
				type = EOL;
				list.push_back(Token(type, line));
				line++;
			}
			else if (c == '\n' && beg == true) { // Checks for new line
				line++;
			}
			else if (c == ' ' || c == '	') {
				if (count > 0) {
					list.push_back(Token(type, line, word));// Adds token to list
					word = "";
					count = 0;
				}
			}
			else if (c == 0x2c) { // SEP
				beg = false;
				if (count > 0)
				{
					type = STRING;
					list.push_back(Token(type, line, word));// Adds token to list
					word = "";
					count = 0;
				}
				type = SEP;
				list.push_back(Token(type, line));
			}
			else if (c == 0x28) { // OPEN_PAREN
				beg = false;
				if (count > 0)
				{
					type = STRING;
					list.push_back(Token(type, line, word));// Adds token to list
					word = "";
					count = 0;
				}
				type = OPEN_PAREN;
				list.push_back(Token(type, line));// Adds token to list
				error1++;
			}
			else if (c == 0x29) { // CLOSE_PAREN
				beg = false;
				if (count > 0)
				{
					type = STRING;
					list.push_back(Token(type, line, word)); // Adds token to list
					word = "";
					count = 0;
				}
				type = CLOSE_PAREN;
				list.push_back(Token(type, line));
				error2++;
			}
			else if (c == 0x22) { // STRING_DELIM
				beg = false;
				error3++;
				if (count > 0)
				{
					type = STRING;
					list.push_back(Token(type, line, word)); // Adds token to list
					word = "";
					count = 0;
				}
				type = STRING_DELIM;
				list.push_back(Token(type, line));
				while (ins.get(c)) {
					if (c != 0x22) {
						word.push_back(c);
						count++;
					}				
					else {
						if (count >= 0) {
						type = STRING;
						list.push_back(Token(type, line, word)); // Adds token to list
						word = "";
						count = 0;
					}
						error3++;
						type = STRING_DELIM;
						list.push_back(Token(type, line)); // Adds token to list
						break;
					}
				}
				
				
			}
			else if (c == 0x3d) { // EQUAL
				beg = false;
				if (count > 0)
				{
					list.push_back(Token(type, line, word)); // Adds token to list
					word = "";
					count = 0;
				}
				type = EQUAL;
				list.push_back(Token(type, line)); // Adds token to list
			}
			else if (!(((c == 0x20 || c == 0x0d) || (c == 0x09 || c == 0x0b)) || (c == 0x0c || c == ' '))) { //Build the string while avoiding certain white space chars
				if (c != '\n') {
					type = STRING;
					word.push_back(c);
					count++;
					beg = false;
				}
				
					
			}
			


	}
	if (type == STRING) {
		if (count > 0) {
			list.push_back(Token(type, line, word)); // Adds token to list
			word = "";
			count = 0;
		}
		type = EOL;
		list.push_back(Token(type, line)); // Adds token to list
		line++;
	}
	if (error1 != error2) { // Checks if there are any unmatched parenthesis
		type = ERRORS;
		std::string warning;
		if (error1 > error2) {
			warning = "Error: Unmatched open parenthesis";
		}
		else {
			warning = "Error: Unmatched closed parenthesis";
		}
		list.push_back(Token(type, line, warning));
	}
	if (error3 % 2 != 0) { // Checks for unmatched string delimeters
		type = ERRORS;
		std::string warning = "Error: Unmatched string delemiter";
		list.push_back(Token(type, line, warning)); //"Error: unmatched string delim on line "
	}
	return list; 
}
