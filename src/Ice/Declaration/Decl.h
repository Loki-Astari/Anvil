#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_DECL_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_DECL_H

#include "Common.h"

#include <utility>

namespace ThorsAnvil::Anvil::Ice
{

class Decl
{
    std::string name;
    public:
        Decl(std::string const& name)
            : name(name)
        {}
        virtual ~Decl() = 0;
        virtual DeclType declType() const = 0;
        std::string const& declName() const                                         {return name;}
        virtual std::pair<bool, NameRef> find(std::string const& /*name*/) const    {return {false, {}};}
};

inline Decl::~Decl()
{}

using DeclRef           = std::reference_wrapper<Decl>;

}

#endif
