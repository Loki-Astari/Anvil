#include "GlobalScopeInit.h"
#include "Action.h"

using namespace ThorsAnvil::Anvil::Ice;

GlobalScopeInit::GlobalScopeInit(Action& action, Scope& /*scope*/)
{
    IdentifierId         voidName   = action.identifierCreate("Void");
    ClassId              voidId     = action.scopeClassOpen(voidName);

    TypeCListId          typeListId = action.listCreate<Type const>();
    ObjectFunctionId     con        = action.scopeConstructorOpen(typeListId);
    CodeBlockId          conBlock   = action.scopeCodeBlockOpen();
    StatementListId      conCode    = action.listCreate<Statement>();
    StatementCodeBlockId conStat    = action.scopeCodeBlockClose(conBlock, conCode);
    action.scopeFunctionClose(con, conStat, action.listCreate<MemberInit>());

    ObjectFunctionId     des        = action.scopeDestructorOpen();
    CodeBlockId          desBlock   = action.scopeCodeBlockOpen();
    StatementListId      desCode    = action.listCreate<Statement>();
    StatementCodeBlockId desStat    = action.scopeCodeBlockClose(desBlock, desCode);
    action.scopeFunctionClose(des, desStat, action.listCreate<MemberInit>());

    DeclListId      voidDeclId  = action.listCreate<Decl>();
    voidDeclId = action.listAppend<Decl>(voidDeclId, con);
    voidDeclId = action.listAppend<Decl>(voidDeclId, des);
    action.scopeClassClose(voidId, voidDeclId);
}
