#ifndef THORSANVIL_ANVIL_ICE_SEMANTIC_H
#define THORSANVIL_ANVIL_ICE_SEMANTIC_H

#include "Action.h"
#include "Lexer.h"
#include "Declaration.h"


namespace ThorsAnvil::Anvil::Ice
{
class Namespace;
using NamespaceRef      = std::reference_wrapper<Namespace>;
using NamespaceDecOrder = std::vector<NamespaceRef>;

class Storage;
class Semantic: public Action
{
    NamespaceDecOrder&      namespaceDecOrder;

    public:
        Semantic(Lexer& lexer, NamespaceDecOrder& namespaceDecOrder, Scope& globalScope, Storage& storage, std::ostream& output = std::cerr);
        virtual ~Semantic() override;

        virtual NamespaceId scopeNamespaceOpenV(std::string namespaceName, Reuse&& reuse) override;
        virtual NamespaceId scopeNamespaceCloseV(Namespace& ns, DeclList decl, Reuse&& reuse) override;
        virtual ClassId     scopeClassOpenV(Identifier className, Reuse&& reuse) override;
        virtual ClassId     scopeClassCloseV(Class& cl, DeclList decl, Reuse&& reuse) override;
        virtual ObjectId    scopeObjectAddVariableV(Identifier name, Type const& type, ExpressionList init) override;
        virtual ObjectId    scopeObjectAddFunctionV(Identifier name, Type const& type, StatementCodeBlock& code) override;
    private:
        void addDefaultMethodsToScope(Scope& scope, DeclList declList);

};

}

#endif
