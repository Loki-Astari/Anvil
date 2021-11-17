#ifndef THORSANVIL_ANVIL_ICE_COMPILER_H
#define THORSANVIL_ANVIL_ICE_COMPILER_H

#include "Action.h"
#include "Lexer.h"
#include "Parser.h"

#include <fstream>
#include <map>
#include <variant>

namespace ThorsAnvil::Anvil::Ice
{

// Console::print("Hello World");

class Void
{
};

class Namespace
{
};

class TypeObject
{
};

class TypeArray
{
};

class TypeMap
{
};

class TypeFunc
{
};

class Object
{
};

class Statement
{
};

using DeclValue = std::variant<Void, Namespace, TypeObject, TypeArray, TypeMap, TypeFunc, Statement>;
enum class DeclType {Void, Namespace, TypeObject, TypeArray, TypeMap, TypeFunc, Object, Statement};

class Decl
{
    DeclType    type;
    DeclValue   value;
    public:
        Decl()
            : type(DeclType::Void)
            , value(Void{})
        {}
        Decl(Namespace&& nameSpace)
            : type(DeclType::Namespace)
            , value(std::move(nameSpace))
        {}
};

class Scope
{
    std::map<std::string, Decl>     names;
    public:
        void add(std::string const& name, Namespace&& nameSpace)
        {
            names[name] = std::move(nameSpace);
        }
};

class Compiler: public Action
{
    std::ifstream   file;
    Parser          parser;
    public:
        Compiler(std::string const& fileName);
        virtual ~Compiler() override;

        void go();
};

}

#endif
