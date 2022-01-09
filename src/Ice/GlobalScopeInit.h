#ifndef THORSANVIL_ANVIL_ICE_GLOBAL_SCOPE_INIT_H
#define THORSANVIL_ANVIL_ICE_GLOBAL_SCOPE_INIT_H


#include "Common.h"
#include "Declaration.h"

namespace ThorsAnvil::Anvil::Ice
{

class Action;
class Scope;
struct GlobalScopeInit
{
    GlobalScopeInit(Action& action, Scope& scope);
};

}

#endif
