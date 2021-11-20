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
    public:
        using NameMap = std::map<std::string, std::unique_ptr<Decl>>;
        using NameRef = NameMap::iterator;

        template<typename T>
        void add(T&& decl);
        virtual std::string const& contName() const = 0;
        std::pair<bool, NameRef> get(std::string const& name);

    private:
        NameMap     names;
};

enum class DeclType {Void, Namespace, TypeObject, TypeArray, TypeMap, TypeFunc, Object, Statement};

class Decl
{
    std::string name;
    public:
        Decl(std::string const& name)
            : name(name)
        {}
        virtual ~Decl() = 0;
        virtual DeclType declType() const = 0;
        std::string const& declName() const                                                 {return name;}
        virtual std::pair<bool, Container::NameRef> find(std::string const& /*name*/)       {return {false, {}};}
};

class Namespace: public Decl, public Container
{
    public:
        Namespace(std::string const& name)
            : Decl(name)
        {}
        virtual std::string const& contName() const override                                {return declName();}
        virtual DeclType declType() const override                                          {return DeclType::Namespace;}
        virtual std::pair<bool, Container::NameRef> find(std::string const& name) override  {return get(name);}
};

class Type: public Decl
{
    public:
        Type(std::string const& name)
            : Decl(name)
        {}
        virtual ~Type() = 0;
};

class Void: public Type
{
    public:
        Void()
            : Type("Void")
        {}
        virtual DeclType declType() const override                                          {return DeclType::Void;}
};

class TypeObject: public Type, public Container
{
    public:
        TypeObject(std::string const& name)
            : Type(name)
        {}
        virtual std::string const& contName() const override                                {return declName();}
        virtual DeclType declType() const override                                          {return DeclType::TypeObject;}
        virtual std::pair<bool, Container::NameRef> find(std::string const& name) override  {return get(name);}
};

class TypeArray: public Type
{
    public:
        TypeArray(std::string const& name)
            : Type(name)
        {}
        virtual DeclType declType() const override                                          {return DeclType::TypeArray;}
};

class TypeMap: public Type
{
    public:
        TypeMap(std::string const& name)
            : Type(name)
        {}
        virtual DeclType declType() const override                                          {return DeclType::TypeMap;}
};

class TypeFunc: public Type
{
    public:
        TypeFunc(std::string const& name)
            : Type(name)
        {}
        virtual DeclType declType() const override                                          {return DeclType::TypeFunc;}
};

class Object: public Decl
{
    Type const&   objectType;
    public:
        Object(std::string const& name, Type const& objectType)
            : Decl(name)
            , objectType(objectType)
        {}
        virtual DeclType declType() const override                                          {return DeclType::Object;}
        // TODO This will need to be overridden
        virtual std::pair<bool, Container::NameRef> find(std::string const&) override       {return {false, {}};}
};

class Statement: public Decl
{
    public:
        Statement()
            : Decl("Statement")
        {}
        virtual DeclType declType() const override                                          {return DeclType::Statement;}
};

using UPVoid        = std::unique_ptr<Void>;
using UPNamespace   = std::unique_ptr<Namespace>;
using UPTypeObject  = std::unique_ptr<TypeObject>;
using UPTypeArray   = std::unique_ptr<TypeArray>;
using UPTypeMap     = std::unique_ptr<TypeMap>;
using UPTypeFunc    = std::unique_ptr<TypeFunc>;
using UPObject      = std::unique_ptr<Object>;
using UPStatement   = std::unique_ptr<Statement>;

using Scope = Container;

class StandardScope: public Namespace
{
    public:
        StandardScope()
            : Namespace("") // Its the global scope so has no name
        {
            UPTypeFunc    consolePrintMethodType(new TypeFunc("Print"));
            UPObject      consolePrintMethodFunc(new Object("print", *consolePrintMethodType));

            UPTypeObject  consoleType(new TypeObject("Console"));
            consoleType->add(std::move(consolePrintMethodType));
            consoleType->add(std::move(consolePrintMethodFunc));

            UPObject      consoleObject(new Object("console", *consoleType));

            UPNamespace   system(new Namespace("Sys"));
            system->add(std::move(consoleType));
            system->add(std::move(consoleObject));

            add(std::move(system));

            UPNamespace   standard(new Namespace("Std"));
            UPTypeObject  typeInt(new TypeObject("Integer"));
            UPTypeObject  typeString(new TypeObject("String"));

            standard->add(std::move(typeInt));
            standard->add(std::move(typeString));
            add(std::move(standard));
        }
};

class Semantic: public Action
{
    Parser                  parser;
    StandardScope           globalScope;
    std::vector<std::reference_wrapper<Scope>>  currentScope;

    public:
        Semantic(std::istream& input = std::cin, std::ostream& output = std::cout);
        virtual ~Semantic() override;

        bool go();
        void display(std::ostream& stream);

    // Action Virtual Functions override
        virtual Int identifierCreate(Lexer& lexer)                          override;
        virtual Int identifierCheckObject(Lexer& lexer, Int id)             override;
        virtual Int identifierCheckType(Lexer& lexer, Int id)               override;
        virtual Int identifierCheckNS(Lexer& lexer, Int id)                 override;

        virtual Int fullIdentiferCreate(Lexer& lexer)                       override;
        virtual Int fullIdentiferAddPath(Lexer& lexer, Int fp, Int id)      override;
        virtual Int findTypeInScope(Lexer& lexer, Int fp)                   override;
};

template<typename T>
inline void Container::add(T&& decl)
{
    names[decl->declName()] = std::move(decl);
}
inline std::pair<bool, Container::NameRef> Container::get(std::string const& name)
{
    auto find = names.find(name);
    return {find != names.end(), find};
}

}

#endif
