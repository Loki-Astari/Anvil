#ifndef THORSANVIL_ANVIL_ICE_COMPILER_H
#define THORSANVIL_ANVIL_ICE_COMPILER_H

#include "Action.h"
#include "Lexer.h"
#include "Parser.h"
#include "Storage.h"

#include <fstream>
#include <map>
#include <list>
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
        using NameRef = NameMap::const_iterator;

        template<typename T>
        T& add(std::unique_ptr<T>&& decl);
        virtual std::string const& contName() const = 0;
        std::pair<bool, NameRef> get(std::string const& name) const;

    private:
        NameMap     names;

        friend std::ostream& operator<<(std::ostream& stream, Container const& cont)
        {
            stream << "Container: " << cont.contName() << "\n";
            for (auto const& value: cont.names)
            {
                stream << "\t" << value.first << ":\n";
            }
            return stream;
        }
};

enum class DeclType {Void, Namespace, Class, Array, Map, Func, Object, Statement};

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
        virtual std::pair<bool, Container::NameRef> find(std::string const& /*name*/) const {return {false, {}};}
};

class Namespace: public Decl, public Container
{
    public:
        Namespace(std::string const& name)
            : Decl(name)
        {}
        virtual std::string const& contName() const override                                {return declName();}
        virtual DeclType declType() const override                                          {return DeclType::Namespace;}
        virtual std::pair<bool, Container::NameRef> find(std::string const& name) const override {return get(name);}
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

class Class: public Type, public Container
{
    public:
        Class(std::string const& name)
            : Type(name)
        {}
        virtual std::string const& contName() const override                                {return declName();}
        virtual DeclType declType() const override                                          {return DeclType::Class;}
        virtual std::pair<bool, Container::NameRef> find(std::string const& name) const override {return get(name);}
};

class Array: public Type
{
    Type&   memberType;
    public:
        Array(std::string const& name, Type& memberType)
            : Type(name)
            , memberType(memberType)
        {}
        virtual DeclType declType() const override                                          {return DeclType::Array;}
};

class Map: public Type
{
    Type&   keyType;
    Type&   valueType;
    public:
        Map(std::string const& name, Type& keyType, Type& valueType)
            : Type(name)
            , keyType(keyType)
            , valueType(valueType)
        {}
        virtual DeclType declType() const override                                          {return DeclType::Map;}
};

class Func: public Type
{
    ParamList   paramList;
    Type&       returnType;
    public:
        Func(std::string const& name, ParamList paramList, Type& returnType)
            : Type(name)
            , paramList(paramList)
            , returnType(returnType)
        {}
        virtual DeclType declType() const override                                          {return DeclType::Func;}
};

class Object: public Decl
{
    Type const&   objectType;
    public:
        Object(std::string const& name, Type const& objectType)
            : Decl(name)
            , objectType(objectType)
        {}
        Type const& getType() const                                                         {return objectType;}
        virtual DeclType declType() const override                                          {return DeclType::Object;}
        virtual std::pair<bool, Container::NameRef> find(std::string const& name) const override {return objectType.find(name);}
};

template<typename T>
class Literal: public Object
{
    T value;
    public:
        Literal(std::string const& name, Type const& objectType, T const& value)
            : Object(name, objectType)
            , value(value)
        {}
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
using UPClass       = std::unique_ptr<Class>;
using UPArray       = std::unique_ptr<Array>;
using UPMap         = std::unique_ptr<Map>;
using UPFunc        = std::unique_ptr<Func>;
using UPObject      = std::unique_ptr<Object>;
using UPStatement   = std::unique_ptr<Statement>;

using Scope = Container;

class StandardScope: public Namespace
{
    public:
        StandardScope()
            : Namespace("") // Its the global scope so has no name
        {
            // Add void to the global scope
            UPVoid        typeVoidPtr(new Void);
            Void&         typeVoid = add(std::move(typeVoidPtr));

            // Add "Std" namespace to the global scope.
            // Here we have the standard basic types.
            UPClass       typeIntPtr(new Class("Integer"));
            UPClass       typeStringPtr(new Class("String"));

            UPNamespace   standard(new Namespace("Std"));
            Class&        typeInt = standard->add(std::move(typeIntPtr));
            Class&        typeString = standard->add(std::move(typeStringPtr));
            add(std::move(standard));
            ((void)typeInt);

            // Add "Sys" namespace to the global scope.
            // This has object to interact with the computer.
            //  console
            ParamList     consoleParamList;
            consoleParamList.emplace_back(typeString);
            UPFunc        consolePrintMethodType(new Func("Print", consoleParamList, typeVoid));
            UPObject      consolePrintMethodFunc(new Object("print", *consolePrintMethodType));

            UPClass  consoleType(new Class("Console"));
            consoleType->add(std::move(consolePrintMethodType));
            consoleType->add(std::move(consolePrintMethodFunc));

            UPObject      consoleObject(new Object("console", *consoleType));

            UPNamespace   system(new Namespace("Sys"));
            system->add(std::move(consoleType));
            system->add(std::move(consoleObject));

            add(std::move(system));

            // Special scope to hold all literals.
            UPNamespace   literal(new Namespace("$Literal"));
            add(std::move(literal));

            // Special scope to hold all Code.
            UPNamespace   code(new Namespace("$Code"));
            add(std::move(code));
        }
};

class Semantic: public Action
{
    Parser      parser;
    Scope&      globalScope;
    std::vector<std::reference_wrapper<Scope>>  currentScope;
    Storage     storage;

    public:
        Semantic(Scope& globalScope, std::istream& input = std::cin, std::ostream& output = std::cout);
        virtual ~Semantic() override;

        bool go();
        void display(std::ostream& stream);

    // Action Virtual Functions override
        virtual void assertNoStorage(Lexer&, Int)                           override;
        virtual void releaseStorage(Lexer&, Int)                            override;
        virtual Int  generateAnonName(Lexer&)                               override;

        virtual Int identifierCreate(Lexer& lexer)                          override;
        virtual Int identifierCheckObject(Lexer& lexer, Int id)             override;
        virtual Int identifierCheckType(Lexer& lexer, Int id)               override;
        virtual Int identifierCheckNS(Lexer& lexer, Int id)                 override;

        virtual Int fullIdentiferCreate(Lexer& lexer)                       override;
        virtual Int fullIdentiferAddPath(Lexer& lexer, Int fp, Int id)      override;

        virtual Int paramListCreate(Lexer& lexer)                           override;
        virtual Int paramListAdd(Lexer& lexer, Int pl, Int type)            override;

        virtual Int objectListCreate(Lexer& lexer)                          override;
        virtual Int objectListAdd(Lexer& lexer, Int ol, Int object)         override;

        virtual Int findTypeInScope(Lexer& lexer, Int fp)                   override;
        virtual Int findObjectInScope(Lexer& lexer, Int fp)                 override;

        virtual Int scopeAddNamespace(Lexer& lexer, Int name)               override;
        virtual Int scopeAddClass(Lexer& lexer, Int name)                   override;
        virtual Int scopeAddArray(Lexer& lexer, Int name, Int type)         override;
        virtual Int scopeAddMap(Lexer& lexer, Int name, Int key, Int value) override;
        virtual Int scopeAddFunc(Lexer& lexer, Int name, Int param, Int ret)override;
        virtual Int scopeAddObject(Lexer& lexer, Int name, Int)             override;
        //virtual Int scopeAddStatement(Lexer& lexer, Int s)                      override;
        virtual Int scopeClose(Lexer& lexer, Int oldSCope)                  override;

        virtual Int addLiteralString(Lexer& lexer)                          override;

        virtual Int createFuncCall(Lexer& lexer, Int obj, Int param)        override;
    private:
        Decl& searchScopeForPath(Lexer& lexer, Int fp);
        Decl* searchScopeForIdentifier(std::string const& path, std::string& partialMatch);
        std::string  generateAnonNameString();

        template<typename T>
        T& getScopeSymbol(Scope& scope, std::string const& name);

        template<typename T, typename... Args>
        T& getScopeSymbol(Scope& scope, std::string const& name, Args&... path);
};

template<typename T>
inline T& Container::add(std::unique_ptr<T>&& decl)
{
    auto& location = names[decl->declName()];
    location = std::move(decl);
    return dynamic_cast<T&>(*location);
}
inline std::pair<bool, Container::NameRef> Container::get(std::string const& name) const
{
    auto find = names.find(name);
    return {find != names.end(), find};
}

template<typename T>
T& Semantic::getScopeSymbol(Scope& scope, std::string const& name)
{
    auto find = scope.get(name);
    //ASSERT_TRUE(find.first);
    if (!find.first)
    {
        throw std::runtime_error("Not Found");
    }

    Decl*  decl         = find.second->second.get();
    return *(dynamic_cast<T*>(decl));
}

template<typename T, typename... Args>
T& Semantic::getScopeSymbol(Scope& scope, std::string const& name, Args&... path)
{
    auto find = scope.get(name);
    //ASSERT_TRUE(find.first);
    if (!find.first)
    {
        throw std::runtime_error("Not Found");
    }

    Decl*  decl         = find.second->second.get();

    Class* classDecl    = dynamic_cast<Class*>(decl);
    if (classDecl)
    {
        return getScopeSymbol<T>(dynamic_cast<Scope&>(*classDecl), path...);
    }
    Namespace* namespaceDecl    = dynamic_cast<Namespace*>(decl);
    if (namespaceDecl)
    {
        return getScopeSymbol<T>(dynamic_cast<Scope&>(*namespaceDecl), path...);
    }
    throw std::runtime_error("Not a valid Scope Path");
}

}

#endif
