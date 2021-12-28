#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_TPP
#define THORSANVIL_ANVIL_ICE_DECLARATION_TPP

namespace ThorsAnvil::Anvil::Ice
{

template<typename T, typename... Args>
inline T& Scope::add(Action& action, Args&&... args)
{
    std::unique_ptr<Decl> decl(new T(&action, std::forward<Args>(args)...));
    Decl& object = saveMember(action, std::move(decl));
    return dynamic_cast<T&>(object);
}


}

#endif
