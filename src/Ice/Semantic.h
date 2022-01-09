#ifndef THORSANVIL_ANVIL_ICE_SEMANTIC_H
#define THORSANVIL_ANVIL_ICE_SEMANTIC_H

#include "Action.h"
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

        virtual Namespace&      scopeNamespaceOpenV(Scope& top, std::string namespaceName) override;
        virtual Namespace&      scopeNamespaceCloseV(Scope& top, Namespace& ns, DeclList decl) override;
        virtual Class&          scopeClassOpenV(Scope& top, Identifier className) override;
        virtual Class&          scopeClassCloseV(Scope& top, Class& cl, DeclList decl) override;
        virtual ObjectVariable& scopeObjectAddVariableV(Scope& top, Identifier name, Type const& type, ExpressionList init) override;
        virtual ObjectFunction& scopeFunctionOpenV(Scope& top, Identifier name, Type const& id) override;
        virtual ObjectFunction& scopeFunctionCloseV(Scope& top, ObjectFunction& id, StatementCodeBlock& code, MemberInitList init) override;
    private:
        void addDefaultMethodsToScope(Scope& scope, DeclList declList);
        ObjectOverload& scopeAddFunctionOverload(Scope& top, Identifier name, Type const& type);

};

}

#endif
