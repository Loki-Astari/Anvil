#include "Action.h"
#include "Lexer.h"

#include <iostream>
#include <sstream>
#include <string_view>

using namespace ThorsAnvil::Anvil::Ice;

Action::Action(Lexer& lexer, std::ostream& output)
    : lexer(lexer)
    , output(output)
    , lineNo(0)
    , offset(0)
{}

Action::~Action()
{}

void Action::log(char const* msg) const
{
    output << msg << "\n";
}

void Action::token()
{
#pragma vera-pushoff
    using namespace std::string_literals;
#pragma vera-pop
    log("Token: "s .append(lexer.lexem()).c_str());
    addToLine();
}

void Action::error(std::string const& msg) const
{
    std::stringstream extended;
    extended << "Error: " << msg << " -> Last Token: >" << lexer.lexem() << "<\n"
             << "Line:  " << lineNo << " : " << currentLine.size() << "     FileOffset: " << offset << "\n"
             << currentLine << "\n"
             << "\n";

    throw std::runtime_error(extended.str());
}

void Action::addToLine()
{
    std::string_view    token = lexer.lexem();

    currentLine += token;
    offset += std::size(token);
}

void Action::resetLine()
{
    currentLine.clear();
    ++lineNo;
    ++offset;
}
