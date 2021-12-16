#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_TYPES_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_TYPES_H

#include "Decl.h"
#include "Container.h"

namespace ThorsAnvil::Anvil::Ice
{

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
        virtual bool storeFunctionsInContainer() const override {return true;}
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

}

#endif
