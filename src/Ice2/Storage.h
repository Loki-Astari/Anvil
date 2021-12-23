#ifndef THORSANVIL_ANVIL_ICE_STORAGE_H
#define THORSANVIL_ANVIL_ICE_STORAGE_H

#include "ParserTypes.h"
#include "Declaration.h"

#include <vector>
#include <variant>

namespace ThorsAnvil::Anvil::Ice
{

using Data = std::variant<Int, DeclList, NamespaceList, TypeList, ExpressionList, StatementList, DeclRef, ScopeRef, NamespaceRef, TypeRef, VoidRef, ClassRef, FunctionRef, ObjectRef, StatementRef, ExpressionRef, ObjectInitRef, Identifier>;

template<typename T>
class StorageAccess;

class Storage
{
    std::vector<Data>   data;
    Int                 nextFree;
    // TODO defaultDecl can we make it another type?
    Namespace           defaultDecl;
    Namespace           defaultNamespace;
    Void                defaultType;
    Void                defaultVoid;
    Class               defaultClass;
    Function            defaultFunction;
    Object              defaultObject;
    Identifier          defaultIdentifier;
    ObjectInit          defaultInit;
    Statement           defaultStatement;
    ExpressionObject    defaultExpression;

    public:
        Storage()
            : nextFree(0)
            , defaultDecl(ActionRef{}, "Invalid Decl")
            , defaultNamespace(ActionRef{}, "Invalid Namespace")
            , defaultType(ActionRef{}, "Invalid Type")
            , defaultVoid(ActionRef{}, "Invalid Void")
            , defaultClass(ActionRef{}, "Invalid Class")
            , defaultFunction(ActionRef{}, "Invalid Function")
            , defaultObject(ActionRef{}, "Invalid Object", defaultClass)
            , defaultInit(ActionRef{})
            , defaultStatement(ActionRef{})
            , defaultExpression(ActionRef{}, defaultObject)
        {
            // We don't use location 0.
            // This is because ice.y parser passes back zero for no object returned.
            // So we don't want to confuse a no-object with an object.
            data.emplace_back(0UL);
            // ---
            data.emplace_back(Int{0});
            // ---
            data.emplace_back(Identifier{"Invalid Identifier"});
            // ----
            data.emplace_back(DeclList{});
            data.emplace_back(NamespaceList{});
            data.emplace_back(TypeList{});
            data.emplace_back(StatementList{});
            data.emplace_back(ExpressionList{});
            // ----
            data.emplace_back(DeclRef{defaultDecl});
            data.emplace_back(NamespaceRef{defaultNamespace});
            data.emplace_back(TypeRef{defaultType});
            data.emplace_back(VoidRef{defaultVoid});
            data.emplace_back(ClassRef{defaultClass});
            data.emplace_back(FunctionRef{defaultFunction});
            data.emplace_back(ObjectRef{defaultObject});
            data.emplace_back(ObjectInitRef{defaultInit});
            data.emplace_back(StatementRef{defaultStatement});
            data.emplace_back(ExpressionRef{defaultExpression}); // 16
        }

        template<typename T>
        Int add(T&& value)
        {
            if (nextFree == 0)
            {
                data.emplace_back(std::forward<T>(value));
                return data.size() - 1;
            }
            Int result = nextFree;
            nextFree = std::get<Int>(data[nextFree]);
            data[result] = std::move(value);
            return result;
        }

        void release(Int index)
        {
            if (index == 0)
            {
                throw std::runtime_error("Internal Compiler Error: Releasing Element Zero");
            }
            if (static_cast<std::size_t>(index) >= data.size())
            {
                throw std::runtime_error("Internal Compiler Error: Releasing Element beyond Range");
            }
            data[index] = nextFree;
            nextFree = index;
        }
    //private:
    //    template<typename T> friend class StorageAccess;

        template<typename T>
        T& get(Int index)
        {
            if (index == 0)
            {
                throw std::runtime_error("Internal Compiler Error: Getting Element Zero");
            }
            if (static_cast<std::size_t>(index) >= data.size())
            {
                throw std::runtime_error("Internal Compiler Error: Getting Element beyond Range");
            }
            return std::get<T>(data[index]);
        }

};

}

#endif
