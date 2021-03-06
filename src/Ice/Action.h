#ifndef THORSANVIL_ANVIL_ICE_ACTION_H
#define THORSANVIL_ANVIL_ICE_ACTION_H


#include "Common.h"

#include <vector>
#include <string>
#include <iostream>

namespace ThorsAnvil::Anvil::Ice
{

class Lexer;
class Storage;
class Action
{
    protected:
    Lexer&                  lexer;
    Storage&                storage;
    private:
    Scope&                  globalScope;
    std::vector<ScopeRef>   currentScope;
    std::ostream&           output;
    std::string             currentLine;
    std::size_t             lineNo;
    std::size_t             offset;
    public:
        Action(Lexer& lexer, Scope& globalScope, Storage& storage, std::ostream& output = std::cerr);
        virtual ~Action();

        friend std::ostream& operator<<(std::ostream& str, Action const& action);

        template<typename... Args>
        void log(Args const&... args) const;
        template<typename... Args>
        [[ noreturn ]] void error(Args const&... args) const;

        // Lexing
        virtual void invalidCharacter();
        virtual void ignoreSpace();
        virtual void newLine();
        virtual void token();

        // Public Utility
        IdentifierId anonName();

        template<typename T>
        ListId<T> listCreate();
        template<typename T, typename S = T>
        ListId<T> listAppend(ListId<T> listId, Id<S> id);

        // Parsing
        VoidId              anvilProgram(NamespaceListId id);
        NamespaceId         scopeNamespaceOpen(IdentifierId id);
        NamespaceId         scopeNamespaceClose(NamespaceId id, DeclListId listId);
        ClassId             scopeClassOpen(IdentifierId id);
        ClassId             scopeClassClose(ClassId id, DeclListId listId);
        ClassId             scopeClassAnon();
        FunctionId          scopeFunctionAdd(IdentifierId id, TypeCListId listId, TypeId returnType);
        CodeBlockId         scopeCodeBlockOpen();
        StatementCodeBlockId scopeCodeBlockClose(CodeBlockId, StatementListId);
        ObjectId            scopeObjectAddVariable(IdentifierId name, TypeId id, ExpressionListId init);
        ObjectFunctionId    scopeFunctionOpen(IdentifierId name, TypeId id);
        ObjectFunctionId    scopeFunctionClose(ObjectFunctionId id, StatementCodeBlockId code, MemberInitListId init);
        ObjectFunctionId    scopeConstructorOpen(TypeCListId listId);
        ObjectFunctionId    scopeDestructorOpen();
        MemberInitId        memberInit(IdentifierId, ExpressionListId);
        IdentifierId        identifierCreate();
        IdentifierId        identifierCreate(std::string identifier);

        StatementId         statmentExpression(ExpressionId id);
        StatementId         statmentReturn(ExpressionId id);
        StatementId         statmentAssembley(Id<std::string> id);

        template<typename T>
        Id<T> getNameFromScopeStack(IdentifierId id);
        template<typename T>
        Id<T> getNameFromScope(ScopeId scopeId, IdentifierId id);
        // -------

        template<typename T>
        T& getRefFromScopeStack(Identifier const& id);
        template<typename T>
        T& getRefFromScope(ScopeRef const& scope, Identifier const& id);
        // --------

        template<typename From, typename To>
        Id<To> convert(Id<From> id);

        // Expression:
        ExpressionId        expressionAssign(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignMul(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignDiv(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignMod(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignAdd(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignSub(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignLSh(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignRSh(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignAnd(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignXOR(ExpressionId, ExpressionId);
        ExpressionId        expressionAssignOR(ExpressionId, ExpressionId);
        ExpressionId        expressionConditional(ExpressionId, ExpressionId, ExpressionId);
        ExpressionId        expressionLogicalOr(ExpressionId, ExpressionId);
        ExpressionId        expressionLogicalAnd(ExpressionId, ExpressionId);
        ExpressionId        expressionInclusiveOr(ExpressionId, ExpressionId);
        ExpressionId        expressionExclusiveOr(ExpressionId, ExpressionId);
        ExpressionId        expressionAnd(ExpressionId, ExpressionId);
        ExpressionId        expressionEqual(ExpressionId, ExpressionId);
        ExpressionId        expressionNotEqual(ExpressionId, ExpressionId);
        ExpressionId        expressionLess(ExpressionId, ExpressionId);
        ExpressionId        expressionGreat(ExpressionId, ExpressionId);
        ExpressionId        expressionLessEqual(ExpressionId, ExpressionId);
        ExpressionId        expressionGreatEqual(ExpressionId, ExpressionId);
        ExpressionId        expressionShiftLeft(ExpressionId, ExpressionId);
        ExpressionId        expressionShiftRight(ExpressionId, ExpressionId);
        ExpressionId        expressionAdd(ExpressionId, ExpressionId);
        ExpressionId        expressionSub(ExpressionId, ExpressionId);
        ExpressionId        expressionMul(ExpressionId, ExpressionId);
        ExpressionId        expressionDiv(ExpressionId, ExpressionId);
        ExpressionId        expressionMod(ExpressionId, ExpressionId);
        ExpressionId        expressionPreInc(ExpressionId);
        ExpressionId        expressionPreDec(ExpressionId);
        ExpressionId        expressionPlus(ExpressionId);
        ExpressionId        expressionNeg(ExpressionId);
        ExpressionId        expressionOneCompliment(ExpressionId);
        ExpressionId        expressionNot(ExpressionId);
        ExpressionId        expressionArrayAccess(ExpressionId, ExpressionId);
        ExpressionId        expressionFuncCall(ExpressionId id, ExpressionListId list);
        ExpressionId        expressionMemberAccess(ExpressionId id, IdentifierId mem);
        ExpressionId        expressionPostInc(ExpressionId);
        ExpressionId        expressionPostDec(ExpressionId);
        ExpressionId        expressionObject(ObjectId id);
        ExpressionId        expressionLiteralString();
        ExpressionId        expressionLiteralInt();

        // Assembley String Builder
        Id<std::string>     assemblyInit();
        void                assembleyAppend(Id<std::string> id);
        // Parsing virtual methods
        virtual VoidId              anvilProgramV(Scope& top, NamespaceList);
        virtual Namespace&          scopeNamespaceOpenV(Scope& top, Identifier);
        virtual Namespace&          scopeNamespaceCloseV(Scope& top, Namespace&, DeclList list);
        virtual Class&              scopeClassOpenV(Scope& top, Identifier);
        virtual Class&              scopeClassCloseV(Scope& top, Class&, DeclList list);
        virtual Function&           scopeFunctionAddV(Scope& top, Identifier id, TypeCList listId, Type const& returnType);
        virtual CodeBlock&          scopeCodeBlockOpenV(Scope& top);
        virtual StatementCodeBlock& scopeCodeBlockCloseV(Scope& top, CodeBlock&, StatementList);
        virtual ObjectVariable&     scopeObjectAddVariableV(Scope& top, Identifier name, Type const& id, ExpressionList init);
        virtual ObjectFunction&     scopeFunctionOpenV(Scope& top, Identifier name, Type const& id);
        virtual ObjectFunction&     scopeFunctionCloseV(Scope& top, ObjectFunction& id, StatementCodeBlock& code, MemberInitList init);
        virtual MemberInit&         memberInitV(Scope& top, Identifier name, ExpressionList init);
        virtual Identifier          identifierCreateV(Scope& top, std::string identifier);
    protected:
        template<typename Dst, typename... Param>
        Dst& getOrAddDeclToScope(Scope& scope, std::string declName, Param&&... param);
        Scope& getGlobalScope() const {return currentScope.front();}
        std::string getCurrentScopeFullName() const;
    private:
        void addToLine();
        void resetLine();

        template<typename Dst, typename... Param>
        Id<Base<Dst>> addDeclToScope(Id<Param>... id);
        template<typename T, typename V, typename E>
        T& getRefFromView(Identifier const& id, V view, E&& genError);
};

}

#include "Action.tpp"

#endif
