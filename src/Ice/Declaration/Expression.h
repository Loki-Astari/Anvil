#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_EXPRESSION_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_EXPRESSION_H


#include "Decl.h"
#include "Types.h"

#include <list>
#include <functional>

namespace ThorsAnvil::Anvil::Ice
{

class Expression: public Decl
{
    TypeCRef            runtimeObjectType;
    public:
        Expression(std::string const& name, Type const& type)
            : Decl(name)
            , runtimeObjectType(type)
        {}

        virtual DeclType declType() const override  {return DeclType::Expression;}
        Type const& getType() const                 {return runtimeObjectType;}
};


class ObjectId: public Expression
{
    ObjectRef           parentObject;
    IdentifierList      runtimeResolution;

    public:
        ObjectId(std::string const& name, Object& parent, IdentifierList resolution, Type const& type)
            : Expression(name, type)
            , parentObject(parent)
            , runtimeResolution(std::move(resolution))
        {}
    private:
};

template<typename T>
class Literal: public Expression
{
    T value;
    public:
        Literal(std::string const& name, Type const& objectType, T const& value)
            : Expression(name, objectType)
            , value(value)
        {}
};

using ExpressionRef     = std::reference_wrapper<Expression>;
using ExpressionList    = std::list<ExpressionRef>;

}

#endif
