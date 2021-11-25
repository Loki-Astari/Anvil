#ifndef THORSANVIL_ANVIL_ICE_COMPILER_DECLARATION_H
#define THORSANVIL_ANVIL_ICE_COMPILER_DECLARATION_H

#include "Action.h"
#include "Lexer.h"
#include "Parser.h"

#include <fstream>
#include <map>
#include <list>
#include <variant>
#include <vector>

namespace ThorsAnvil::Anvil::Ice
{

// Console::print("Hello World");
class Type;
class Object;
class Decl;
class Container;

using Scope = Container;

using TypeRef   = std::reference_wrapper<Type>;
using ObjectRef = std::reference_wrapper<Object>;
using DeclRef   = std::reference_wrapper<Decl>;
using ScopeRef  = std::reference_wrapper<Scope>;
using ParamList = std::list<TypeRef>;
using ObjectList= std::list<ObjectRef>;
using FullIdent = std::list<std::string>;


class Decl;
class Container
{
    public:
        using NameMap = std::map<std::string, std::unique_ptr<Decl>>;
        using NameRef = NameMap::const_iterator;

        template<typename T, typename... Args>
        T& add(Args&&... args);
        virtual std::string const& contName() const = 0;
        std::pair<bool, NameRef> get(std::string const& name) const;

    private:
        NameMap     names;

        friend std::ostream& operator<<(std::ostream& stream, Container const& cont)
        {
            stream << "Container: " << cont.contName() << "{\n";
            for (auto const& value: cont.names)
            {
                stream << "\t" << value.first << ":\n";
            }
            stream << "}\n";
            return stream;
        }
};

enum class DeclType {Void, Namespace, Class, Array, Map, Func, Object, CodeBlock, Statement};

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

inline Decl::~Decl()
{}

template<typename D>
class DeclContainer: public D, public Container
{
    public:
        DeclContainer(std::string const& name)
            : D(name)
        {}
        virtual std::string const& contName() const override                                {return D::declName();}
        virtual std::pair<bool, Container::NameRef> find(std::string const& name) const override {return get(name);}
};

class Namespace: public DeclContainer<Decl>
{
    public:
        Namespace(std::string const& name);
        virtual DeclType declType() const override                                          {return DeclType::Namespace;}
};

class Statement;
class CodeBlock: public DeclContainer<Decl>
{
    std::vector<std::unique_ptr<Statement>>     code;
    public:
        using DeclContainer<Decl>::DeclContainer;
        virtual DeclType declType() const override                                          {return DeclType::CodeBlock;}

        template<typename T, typename... Args>
        Statement& addCode(Args&&... args)
        {
            code.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
            //code.emplace_back(new T(std::forward<Args>(args)...));
            return *code.back();
        }
};

class Type: public Decl
{
    public:
        Type(std::string const& name)
            : Decl(name)
        {}
        virtual ~Type() = 0;
};

inline Type::~Type()
{}

class Void: public Type
{
    public:
        Void()
            : Type("Void")
        {}
        virtual DeclType declType() const override                                          {return DeclType::Void;}
};

class Class: public DeclContainer<Type>
{
    public:
        Class(std::string const& name);
        virtual DeclType declType() const override                                          {return DeclType::Class;}
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

class Statement
{
    public:
        virtual ~Statement() = 0;
};

inline Statement::~Statement() {}

class StatExprInitObject: public Statement
{
    Object&     object;
    ObjectList  objectList;
    public:
        StatExprInitObject(Object& object, ObjectList&& objectList)
            : object(object)
            , objectList(std::move(objectList))
        {}
};

class StatExprFunctionCall: public Statement
{
    Object&     object;
    ObjectList  objectList;
    public:
        StatExprFunctionCall(Object& object, ObjectList&& objectList)
            : object(object)
            , objectList(std::move(objectList))
        {}
};


using UPVoid        = std::unique_ptr<Void>;
using UPNamespace   = std::unique_ptr<Namespace>;
using UPClass       = std::unique_ptr<Class>;
using UPCodeBlock   = std::unique_ptr<CodeBlock>;
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
            Void&         typeVoid = add<Void>();

            // Add "Std" namespace to the global scope.
            // Here we have the standard basic types.
            Namespace&    standard = add<Namespace>("Std");
            Class&        typeInt = standard.add<Class>("Integer");
            Class&        typeString = standard.add<Class>("String");
            ((void)typeInt);

            // Add "Sys" namespace to the global scope.
            // This has object to interact with the computer.
            //  console
            ParamList     consoleParamList;
            consoleParamList.emplace_back(typeString);

            Namespace&  system                  = add<Namespace>("Sys");

            Class&      consoleType             = system.add<Class>("Console");
            /*Object&     consoleObject           = */ system.add<Object>("console", consoleType);
            /*CodeBlock&  consoleConstructor      = */ system.add<CodeBlock>("$constructor");

            Func&       consolePrintMethodType  = consoleType.add<Func>("Print", consoleParamList, typeVoid);
            /*Object&     consolePrintMethodFunc  = */ consoleType.add<Object>("print", consolePrintMethodType);


            // Special scope to hold all literals.
            /*Namespace&  literal                 = */ add<Namespace>("$Literal");

            // Special scope to hold all Code.
            /*Namespace&  code                    = */ add<Namespace>("$Code");
        }
};

template<typename T, typename... Args>
inline T& Container::add(Args&&... args)
{
    std::unique_ptr<Decl> decl(new T(args...));
    auto& location = names[decl->declName()];
    location = std::move(decl);
    return dynamic_cast<T&>(*location);
}
inline std::pair<bool, Container::NameRef> Container::get(std::string const& name) const
{
    auto find = names.find(name);
    return {find != names.end(), find};
}

inline Namespace::Namespace(std::string const& name)
    : DeclContainer<Decl>(name)
{
    add<CodeBlock>("$container");
    add<CodeBlock>("$destructor");
}

inline Class::Class(std::string const& name)
    : DeclContainer<Type>(name)
{
    add<CodeBlock>("$container");
    add<CodeBlock>("$destructor");
}

}

#endif
