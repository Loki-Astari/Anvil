#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_H

#include "Action.h"
#include "Lexer.h"
#include "Parser.h"

#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <variant>
#include <vector>

namespace ThorsAnvil::Anvil::Ice
{

enum class DeclType {Void, Namespace, Class, Array, Map, Func, Object, CodeBlock, Statement};

// Console::print("Hello World");
class Type;
class Object;
class Decl;
class Container;
class ObjectId;

using Scope = Container;

using TypeRef       = std::reference_wrapper<Type>;
using TypeCRef      = std::reference_wrapper<Type const>;
using ObjectRef     = std::reference_wrapper<Object>;
using ObjectIdRef   = std::reference_wrapper<ObjectId>;
using DeclRef       = std::reference_wrapper<Decl>;
using ScopeRef      = std::reference_wrapper<Scope>;
using ParamList     = std::list<TypeRef>;
using ObjectIdList  = std::list<ObjectIdRef>;
using IdentifierList= std::list<std::string>;

template<typename T>
struct ReversView
{
    T& range;
    ReversView(T& range)
        : range(range)
    {}

    auto begin() {return range.rbegin();}
    auto end()   {return range.rend();}
};


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
    std::vector<std::unique_ptr<Statement>>     codeBack;
    public:
        using DeclContainer<Decl>::DeclContainer;
        virtual DeclType declType() const override                                          {return DeclType::CodeBlock;}

        template<bool B, typename T, typename... Args>
        Statement& addCode(Args&&... args)
        {
            if constexpr (B)
            {
                code.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
                return *code.back();
            }
            else
            {
                codeBack.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
                return *codeBack.back();
            }
        }
};

class Type: public Decl
{
    public:
        Type(std::string const& name)
            : Decl(name)
        {}
        virtual ~Type() = 0;
        friend bool operator==(Type const& lhs, Type const& rhs)
        {
            return &lhs == &rhs || (lhs.declType() == DeclType::Void && rhs .declType() == DeclType::Void);
        }
        friend bool operator!=(Type const& lhs, Type const& rhs)
        {
            return !(lhs == rhs);
        }
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
        ParamList const& getParamList() const                                               {return paramList;}
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

class ObjectId
{
    ObjectRef           parentObject;
    IdentifierList      runtimeResolution;
    TypeCRef            runtimeObjectType;

    public:
        ObjectId(Object& parent, IdentifierList&& resolution, Type const& type)
            : parentObject(parent)
            , runtimeResolution(std::move(resolution))
            , runtimeObjectType(type)
        {}
        Type const& getType() const
        {
            return runtimeObjectType;
        }
    private:
};

class Statement
{
    public:
        virtual ~Statement() = 0;
};

inline Statement::~Statement() {}

class StatExprFunctionCall: public Statement
{
    ObjectId        object;
    ObjectIdList    objectList;
    public:
        StatExprFunctionCall(ObjectId const& object, ObjectIdList&& objectList)
            : object(std::move(object))
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

            Func&       consolePrintMethodType  = consoleType.add<Func>("Print", consoleParamList, typeVoid);
            /*Object&     consolePrintMethodFunc  = */ consoleType.add<Object>("print", consolePrintMethodType);


            // Special scope to hold all literals.
            /*Namespace&  literal                 = */ add<Namespace>("$Literal");
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
    // Move to the globalscope.
    static Void  voidType;

    Func& constructType = add<Func>("$Constructor", ParamList{}, voidType);
    Func& destructType  = add<Func>("$Destructor",  ParamList{}, voidType);

    add<Object>("$constructor", constructType);
    add<Object>("$destructor",  destructType);

    add<CodeBlock>("$constructor$Code");
    add<CodeBlock>("$destructor$Code");
}

inline Class::Class(std::string const& name)
    : DeclContainer<Type>(name)
{
    // Move to the globalscope.
    static Void  voidType;

    Func& constructType = add<Func>("$Constructor", ParamList{}, voidType);
    Func& destructType  = add<Func>("$Destructor",  ParamList{}, voidType);

    add<Object>("$constructor", constructType);
    add<Object>("$destructor",  destructType);

    add<CodeBlock>("$constructor$Code");
    add<CodeBlock>("$destructor$Code");
}

}

#endif
