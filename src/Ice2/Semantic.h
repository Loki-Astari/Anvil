#ifndef THORSANVIL_ANVIL_ICE_SEMANTIC_H
#define THORSANVIL_ANVIL_ICE_SEMANTIC_H

#include "Action.h"
#include "Lexer.h"
#include "Declaration.h"


namespace ThorsAnvil::Anvil::Ice
{

class Storage;
class Semantic: public Action
{
    NamespaceDecOrder&      namespaceDecOrder;

    public:
        Semantic(Lexer& lexer, NamespaceDecOrder& namespaceDecOrder, Scope& globalScope, Storage& storage, std::ostream& output = std::cerr);
        virtual ~Semantic() override;

        virtual NamespaceId scopeNamespaceOpenV(std::string namespaceName, Reuse&& reuse) override;
};

}

#endif
