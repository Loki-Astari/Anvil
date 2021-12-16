#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_OBJECTS_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_OBJECTS_H

#include "Decl.h"
#include "Types.h"

#include <list>
#include <functional>

namespace ThorsAnvil::Anvil::Ice
{

class Object: public Decl
{
    Type const&   objectType;
    public:
        Object(std::string const& name, Type const& objectType)
            : Decl(name)
            , objectType(objectType)
        {}
        Type const& getType() const                                                     {return objectType;}
        virtual DeclType declType() const override                                      {return DeclType::Object;}
        virtual std::pair<bool, NameRef> find(std::string const& name) const override   {return objectType.find(name);}
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

using ObjectIdRef       = std::reference_wrapper<ObjectId>;
using ObjectIdList      = std::list<ObjectIdRef>;

}

#endif
