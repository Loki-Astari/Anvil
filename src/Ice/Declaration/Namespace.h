#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_NAMESPACE_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_NAMESPACE_H

#include "Decl.h"
#include "Container.h"

#include <string>

namespace ThorsAnvil::Anvil::Ice
{

class Namespace: public DeclContainer<Decl>
{
    std::string path;
    public:
        Namespace(std::string const& name);
        virtual DeclType declType() const override                                          {return DeclType::Namespace;}

        void setPath(std::string p)
        {
            path = std::move(p);
            path += contName();
        }
        std::string const& fullPathName() const
        {
            return path;
        }
};

using NamespaceRef      = std::reference_wrapper<Namespace>;
using NamespaceDecOrder = std::vector<NamespaceRef>;

}

#endif
