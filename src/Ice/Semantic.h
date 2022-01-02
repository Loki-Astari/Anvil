#ifndef THORSANVIL_ANVIL_ICE_SEMANTIC_H
#define THORSANVIL_ANVIL_ICE_SEMANTIC_H

#include "Action.h"
#include "Lexer.h"
#include "Declaration.h"
#include "Common.h"


namespace ThorsAnvil::Anvil::Ice
{

class Storage;
class Semantic: public Action
{
    NamespaceDecOrder&      namespaceDecOrder;

    public:
        Semantic(Lexer& lexer, NamespaceDecOrder& namespaceDecOrder, Scope& globalScope, Storage& storage, std::ostream& output = std::cerr);
        virtual ~Semantic() override;

        virtual NamespaceId scopeNamespaceOpenV(Scope& top, std::string namespaceName, Reuse&& reuse) override;
        virtual NamespaceId scopeNamespaceCloseV(Scope& top, Namespace& ns, DeclList decl, Reuse&& reuse) override;
        virtual ClassId     scopeClassOpenV(Scope& top, Identifier className, Reuse&& reuse) override;
        virtual ClassId     scopeClassCloseV(Scope& top, Class& cl, DeclList decl, Reuse&& reuse) override;
        virtual ObjectId    scopeObjectAddVariableV(Scope& top, Identifier name, Type const& type, ExpressionList init) override;
        virtual ObjectId    scopeObjectAddFunctionV(Scope& top, Identifier name, Type const& type, StatementCodeBlock& code, MemberInitList init) override;
    private:
        void addDefaultMethodsToScope(Scope& scope, DeclList declList);
        ObjectFunction& addFunctionToScope(Scope& scope, std::string name, Type const& type, StatementCodeBlock& code, MemberInitList init);

};

}

#endif
