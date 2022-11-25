#include "token.hpp"

Token::Token(TokenType type, std::size_t line) : m_type(type), m_line(line) {}

Token::Token(TokenType type, std::size_t line, const std::string &value)
    : m_type(type), m_line(line), m_value(value) {}

TokenType Token::type() const { return m_type; }

std::size_t Token::line() const { return m_line; }

std::string Token::value() const { return m_value; }

bool operator==(const Token &t1, const Token &t2) {
  return (t1.type() == t2.type()) && (t1.line() == t2.line()) &&
         (t1.value() == t2.value());
}

bool operator!=(const Token &t1, const Token &t2) { return !(t1 == t2); }

std::ostream &operator<<(std::ostream &oss, const Token &t) {
  oss << "Token: type(" << t.type() << ") value (" << t.value()
      << ") source line (" << t.line() << ")";

  return oss;
}


bool isOperator(const std::string& str) {
    if (str == "+") {
        return true;
    }
    else if (str == "-") {
        return true;
    }
    else if (str == "*") {
        return true;
    }
    if (str == "/") {
        return true;
    }
    return false;
}

bool isNumber(const std::string& str) {
    std::string::const_iterator it = str.begin();
    while (it != str.end() && std::isdigit(*it)) {
    ++it;
}
    return !str.empty() && it == str.end();
}
