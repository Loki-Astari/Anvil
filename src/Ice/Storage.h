#ifndef THORSANVIL_ANVIL_ICE_STORAGE_H
#define THORSANVIL_ANVIL_ICE_STORAGE_H

#include "Common.h"

#include <vector>
#include <variant>

namespace ThorsAnvil::Anvil::Ice
{

using Data = std::variant<Int, DeclList, NamespaceList, TypeCList, ExpressionList, StatementList, MemberInitList, DeclRef, ScopeRef, CodeBlockRef, MemberInitRef, NamespaceRef, TypeRef, VoidRef, ClassRef, FunctionRef, ObjectRef, StatementRef, StatementCodeBlockRef, ExpressionRef, Identifier>;

class Storage
{
    std::vector<Data>   data;
    Int                 nextFree;
    public:
        Storage();
        template<typename T>
        Int add(T&& value);
        void release(Int index);
        template<typename T>
        T& get(Int index);
};

}

#include "Storage.tpp"

#endif
