#include "Action.h"
#include "Lexer.h"

#include <iostream>
#include <sstream>
#include <string_view>

using namespace ThorsAnvil::Anvil::Ice;

Action::Action(std::ostream& output)
    : output(output)
    , lineNo(0)
    , offset(0)
{}

Action::~Action()
{}

void Action::log(char const* msg)
{
    output << msg << "\n";
}

void Action::token(Lexer& lexer)
{
#pragma vera-pushoff
    using namespace std::string_literals;
#pragma vera-pop
    log("Token: "s .append(lexer.lexem()).c_str());
    addToLine(lexer);
}

void Action::error(Lexer& lexer, std::string const& msg)
{
    std::stringstream extended;
    extended << "Error: " << msg << " -> Last Token: >" << lexer.lexem() << "<\n"
             << "Line:  " << lineNo << " : " << currentLine.size() << "     FileOffset: " << offset << "\n"
             << currentLine << "\n"
             << "\n";

    throw std::runtime_error(extended.str());
}

void Action::addToLine(Lexer& lexer)
{
    std::string_view    token = lexer.lexem();

    currentLine += token;
    offset += std::size(token);
}

void Action::resetLine(Lexer& /*lexer*/)
{
    currentLine.clear();
    ++lineNo;
    ++offset;
}
