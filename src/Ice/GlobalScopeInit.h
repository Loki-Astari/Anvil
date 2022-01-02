#ifndef THORSANVIL_ANVIL_ICE_GLOBAL_SCOPE_INIT_H
#define THORSANVIL_ANVIL_ICE_GLOBAL_SCOPE_INIT_H


#include "Common.h"
#include "Action.h"
#include "Declaration.h"

namespace ThorsAnvil::Anvil::Ice
{

struct GlobalScopeInit
{
    GlobalScopeInit(Action& action, Scope& /*scope*/)
    {
        IdentifierId         voidName   = action.identifierCreate("Void");
        ClassId              voidId     = action.scopeClassOpen(voidName);

        TypeCListId          typeListId = action.listCreate<Type const>();
        MemberInitListId     initListId = action.listCreate<MemberInit>();
        CodeBlockId          conBlock   = action.scopeCodeBlockOpen();
        StatementListId      conCode    = action.listCreate<Statement>();
        StatementCodeBlockId conStat    = action.scopeCodeBlockClose(conBlock, conCode);
        ObjectId             con        = action.scopeConstructorAdd(typeListId, initListId, conStat);

        CodeBlockId          desBlock   = action.scopeCodeBlockOpen();
        StatementListId      desCode    = action.listCreate<Statement>();
        StatementCodeBlockId desStat    = action.scopeCodeBlockClose(desBlock, desCode);
        ObjectId             des        = action.scopeDestructorAdd(desStat);

        DeclListId      voidDeclId  = action.listCreate<Decl>();
        voidDeclId = action.listAppend<Decl>(voidDeclId, con);
        voidDeclId = action.listAppend<Decl>(voidDeclId, des);
        action.scopeClassClose(voidId, voidDeclId);
    }
};

}

#endif
