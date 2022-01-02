#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_TPP
#define THORSANVIL_ANVIL_ICE_DECLARATION_TPP

#include "Action.h"

namespace ThorsAnvil::Anvil::Ice
{

template<typename T, typename... Args>
inline T& Scope::add(ActionRef action, Args&&... args)
{
    std::unique_ptr<Decl> decl(new T(action, std::forward<Args>(args)...));
    Decl& object = saveMember(action, std::move(decl));
    return dynamic_cast<T&>(object);
}

template<typename T>
inline void ExpressionLiteral<T>::print(std::ostream& stream, int s, bool showName) const
{
    if (showName)
    {
        stream << indent(s) << "Decl::Expression::ExpressionLiteral\n";
    }
    Expression::print(stream, s, false);
    stream << indent(s+1) << "literal: >" << literal << "<\n";
    stream << indent(s+1) << "type:\n";
    type.print(stream, s+2, true);
}

template<typename T>
inline Type const& ExpressionLiteral<T>::findType(ActionRef action)
{
    Namespace& global = action->getRefFromScopeStack<Namespace>("::");
    Namespace& stdns  = action->getRefFromScope<Namespace>(global, "Std");
    Type&      string = action->getRefFromScope<Type>(stdns, ExpressionLiteralTypeName<T>::standardName);
    return string;
}

template<>
inline Type const& ExpressionLiteral<void*>::findType(ActionRef /*action*/)
{
    return Void::getInstance();
}


}

#endif
