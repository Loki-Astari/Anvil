#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_OBJECTS_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_OBJECTS_H

#include "Decl.h"
#include "Types.h"

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

}

#endif
