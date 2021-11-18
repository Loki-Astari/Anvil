#ifndef THORSANVIL_ANVIL_ICE_COMPILER_H
#define THORSANVIL_ANVIL_ICE_COMPILER_H

#include "Action.h"
#include "Lexer.h"
#include "Parser.h"

#include <fstream>
#include <map>
#include <variant>
#include <vector>

namespace ThorsAnvil::Anvil::Ice
{

// Console::print("Hello World");

class Decl;
class Container
{
    std::map<std::string, Decl>     names;
    public:
        template<typename T>
        void add(std::string const& name, T&& nameSpace);
};

class Void
{
};

class Namespace: public Container
{
};

class Type
{
    public:
        virtual ~Type() = 0;
};

class TypeObject: public Type, public Container
{
};

class TypeArray: public Type
{
};

class TypeMap: public Type
{
};

class TypeFunc: public Type
{
};

class Object
{
    Type const&   type;
    public:
        Object(Type const& type)
            : type(type)
        {}
};

class Statement
{
};
using UPVoid        = std::unique_ptr<Void>;
using UPNamespace   = std::unique_ptr<Namespace>;
using UPTypeObject  = std::unique_ptr<TypeObject>;
using UPTypeArray   = std::unique_ptr<TypeArray>;
using UPTypeMap     = std::unique_ptr<TypeMap>;
using UPTypeFunc    = std::unique_ptr<TypeFunc>;
using UPObject      = std::unique_ptr<Object>;
using UPStatement   = std::unique_ptr<Statement>;

enum class DeclType {Void, Namespace, TypeObject, TypeArray, TypeMap, TypeFunc, Object, Statement};
using DeclValue = std::variant<UPVoid, UPNamespace, UPTypeObject, UPTypeArray, UPTypeMap, UPTypeFunc, UPObject, UPStatement>;

class Decl
{
    DeclType    type;
    DeclValue   value;
    public:
        // Temporary;
        DeclType const&   getType() const {return type;}
        DeclValue const&  getValue() const {return value;}

        Decl()
            : type(DeclType::Void)
            , value(std::make_unique<Void>())
        {}
        Decl(UPNamespace&& nameSpace)
            : type(DeclType::Namespace)
            , value(std::move(nameSpace))
        {}
        Decl(UPTypeObject&& typeObject)
            : type(DeclType::TypeObject)
            , value(std::move(typeObject))
        {}
        Decl(UPTypeArray&& typeArray)
            : type(DeclType::TypeArray)
            , value(std::move(typeArray))
        {}
        Decl(UPTypeMap&& typeMap)
            : type(DeclType::TypeMap)
            , value(std::move(typeMap))
        {}
        Decl(UPTypeFunc&& typeFund)
            : type(DeclType::TypeFunc)
            , value(std::move(typeFund))
        {}
        Decl(UPObject&& object)
            : type(DeclType::Object)
            , value(std::move(object))
        {}
        Decl(UPStatement&& statement)
            : type(DeclType::Statement)
            , value(std::move(statement))
        {}
};

class Scope: public Container
{
};

class StandardScope: public Scope
{
    public:
        StandardScope()
        {
            UPTypeFunc    consolePrintMethodType(new TypeFunc);
            UPObject      consolePrintMethodFunc(new Object(*consolePrintMethodType));

            UPTypeObject  consoleType(new TypeObject);
            consoleType->add("Print", std::move(consolePrintMethodType));
            consoleType->add("print", std::move(consolePrintMethodFunc));

            UPObject      consoleObject(new Object(*consoleType));

            UPNamespace   system(new Namespace);
            system->add("Console", std::move(consoleType));
            system->add("console", std::move(consoleObject));

            add("System", std::move(system));

            UPNamespace   standard(new Namespace);
            UPTypeObject  typeInt(new TypeObject);
            UPTypeObject  typeString(new TypeObject);

            standard->add("Int", std::move(typeInt));
            standard->add("String", std::move(typeString));
            add("Std",    std::move(standard));
        }
};

class Compiler: public Action
{
    std::ifstream           file;
    Parser                  parser;
    StandardScope           globalScope;
    std::vector<std::reference_wrapper<Scope const>>  currentScope;

    public:
        Compiler(std::string const& fileName);
        virtual ~Compiler() override;

        void go();
};

template<typename T>
void Container::add(std::string const& name, T&& nameSpace)
{
    names[name] = std::move(nameSpace);
}

}

#endif
