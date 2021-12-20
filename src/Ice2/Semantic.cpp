#include "Semantic.h"
#include "Storage.h"
#include "Declaration.h"

using namespace ThorsAnvil::Anvil::Ice;

Semantic::Semantic(Lexer& lexer, NamespaceDecOrder& namespaceDecOrder, Scope& globalScope, Storage& storage, std::ostream& output)
    : Action(lexer, globalScope, storage, output)
    , namespaceDecOrder(namespaceDecOrder)
{}

Semantic::~Semantic()
{
}

// Action Override
NamespaceId Semantic::scopeNamespaceOpenV(std::string& namespaceName, Reuse&& reuse)
{
    NamespaceId newNamespace = Action::scopeNamespaceOpenV(namespaceName, std::move(reuse));
    namespaceDecOrder.emplace_back(storage.get<NamespaceRef>(newNamespace.value));

    return newNamespace;
}
