#ifndef THORSANVIL_ANVIL_ICE_ACTION_H
#define THORSANVIL_ANVIL_ICE_ACTION_H

#include "ParserTypes.h"
#include "Declaration.h"
#include "Storage.h"
#include "Utility/View.h"

#include <iostream>
#include <sstream>


namespace ThorsAnvil::Anvil::Ice
{

class Lexer;
class Store;

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
        // Creating an maintaining lists.
        template<typename T> using Ref              = std::reference_wrapper<T>;
        template<typename T> using List             = std::list<Ref<T>>;
        template<typename T> using ListId           = Id<List<T>>;
        template<typename T> using ListAccess       = IdAccess<List<T>>;
        template<typename T> using Base             = typename T::Base;
        /*
        struct ParserInfo
        {
            using Ref           = std::reference_wrapper<T>;
            using ID            = Id<T>;
            using IDAccess      = IdAccess<T>;
            using List          = std::list<Ref>;
            using ListId        = Id<List>;
            using ListIdAccess  = IdAccess<List>;
        };*/
        Action(Lexer& lexer, Scope& globalScope, Storage& storage, std::ostream& output = std::cerr);
        virtual ~Action();

        friend std::ostream& operator<<(std::ostream& str, Action const& action);

        template<typename... Args>
        void log(Args&&... args) const;
        template<typename... Args>
        [[ noreturn ]] void error(Args&&... args) const;

        // Lexing
        virtual void invalidCharacter()                                 {addToLine();error("Invalid Character");}
        virtual void ignoreSpace()                                      {addToLine();}
        virtual void newLine()                                          {resetLine();}
        virtual void token();

        // Public Utility
        std::string         anonName();

        template<typename T>
        ListId<T> listCreate()
        {
            return storage.add<List<T>>(List<T>{});
        }
        template<typename T>
        ListId<T> listAppend(ListId<T> listId, Id<T> id)
        {
            ListAccess<T>   listAccess(storage, listId);
            IdAccess<T>     objectAccess(storage, id);

            List<T>&        list = listAccess;
            T&              object = objectAccess;

            list.emplace_back(object);
            return listAccess.reuse();
        }

        // Parsing
        VoidId              anvilProgram(NamespaceListId id);
        NamespaceId         scopeNamespaceOpen(IdentifierId id);
        NamespaceId         scopeNamespaceClose(NamespaceId id, DeclListId listId);
        ClassId             scopeClassOpen(IdentifierId id);
        ClassId             scopeClassClose(ClassId id, DeclListId listId);
        ClassId             scopeClassAnon(DeclListId listId);
        FunctionId          scopeFunctionOpen(IdentifierId id);
        FunctionId          scopeFunctionClose(FunctionId id, TypeListId listId, TypeId returnType);
        FunctionId          scopeFunctionAnon(TypeListId listId, TypeId returnType);
        CodeBlockId         scopeCodeBlockOpen();
        StatementId         scopeCodeBlockClose(CodeBlockId, StatementListId);
        ObjectId            scopeObjectAddVariable(IdentifierId name, TypeId id, ExpressionListId init);
        ObjectId            scopeObjectAddFunction(IdentifierId name, TypeId id, StatementId init);
        FunctionId          scopeConstructorInit();
        ObjectId            scopeConstructorAdd(FunctionId id, TypeListId listId, StatementId code);
        FunctionId          scopeDestructorInit();
        ObjectId            scopeDestructorAdd(FunctionId id, TypeListId listId, StatementId code);
        IdentifierId        identifierCreate();

        StatementId         statmentExpression(ExpressionId id)                             {return addObjectToScope1<StatementExpression, Expression>(id);}
        StatementId         statmentReturn(ExpressionId id)                                 {return addObjectToScope1<StatementReturn, Expression>(id);}
        StatementId         statmentAssembley(Id<std::string> id)                           {return addObjectToScope1<StatementAssembley, std::string>(id);}

        template<typename T, typename V>
        Id<T> getNameFromView(IdentifierId id, V view)
        {
            using Ref = std::reference_wrapper<T>;
            //ScopePrinter scope("getTypeFromName");
            IdentifierAccess    access(storage, id);

            for (auto const& scope: view)
            {
                auto find = scope.get().get(access);
                if (find.first)
                {
                    Decl& decl = *find.second->second;
                    T& value = dynamic_cast<T&>(decl);
                    return storage.add<Ref>(Ref{value});
                }
            }
            error("Invalid Type Name: ", static_cast<std::string>(access));
        }

        template<typename T>
        Id<T> getNameFromScopeStack(IdentifierId id)
        {
            if (storage.get<Identifier>(id.value) == "::")
            {
                Scope&  global = currentScope.front();
                T& result = dynamic_cast<T&>(global);
                return storage.add<Ref<T>>(result);
            }
            return getNameFromView<T>(id, make_View<Reverse>(currentScope));
        }
        template<typename T>
        Id<T> getNameFromScope(ScopeId scopeId, IdentifierId id)
        {
            ScopeAccess     scopeAccess(storage, scopeId);
            ScopeRef        scope = ScopeRef(scopeAccess);
            return getNameFromView<T>(id, make_View<Reverse>(&scope, &scope+1));
        }
        // -------

        template<typename T, typename V>
        T& getRefFromView(Identifier const& id, V view)
        {
            for (auto const& scope: view)
            {
                auto find = scope.get().get(id);
                if (find.first)
                {
                    Decl& decl = *find.second->second;
                    T& value = dynamic_cast<T&>(decl);
                    return value;
                }
            }
            error("Invalid Type Name: ", id);
        }
        template<typename T>
        T& getRefFromScopeStack(Identifier const& id)
        {
            if (id == "::")
            {
                Scope&  global = currentScope.front();
                return dynamic_cast<T&>(global);
            }
            return getRefFromView<T>(id, make_View<Reverse>(currentScope));
        }
        template<typename T>
        T& getRefFromScope(ScopeRef const& scope, Identifier const& id)
        {
            return getRefFromView<T>(id, make_View<Reverse>(&scope, &scope+1));
        }
        // --------

        template<typename From, typename To>
        Id<To> convert(Id<From> id)
        {
            using ToStoreType = typename IdTraits<To>::AccessType;
            IdAccess<From>  access(storage, id);
            From&  fromValue = access;
            To&    toType = dynamic_cast<To&>(fromValue);
            return storage.add<ToStoreType>(ToStoreType{toType});
        }

        // Expression:
        ExpressionId        expressionAssign(ExpressionId, ExpressionId)                        {error("Not Implemented: expressionAssign");}
        ExpressionId        expressionAssignMul(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignMul");}
        ExpressionId        expressionAssignDiv(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignDiv");}
        ExpressionId        expressionAssignMod(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignMod");}
        ExpressionId        expressionAssignAdd(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignAdd");}
        ExpressionId        expressionAssignSub(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignSub");}
        ExpressionId        expressionAssignLSh(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignLSh");}
        ExpressionId        expressionAssignRSh(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignRSh");}
        ExpressionId        expressionAssignAnd(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignAnd");}
        ExpressionId        expressionAssignXOR(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionAssignXOR");}
        ExpressionId        expressionAssignOR(ExpressionId, ExpressionId)                      {error("Not Implemented: expressionAssignOR");}
        ExpressionId        expressionConditional(ExpressionId, ExpressionId, ExpressionId)     {error("Not Implemented: expressionConditional");}
        ExpressionId        expressionLogicalOr(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionLogicalOr");}
        ExpressionId        expressionLogicalAnd(ExpressionId, ExpressionId)                    {error("Not Implemented: expressionLogicalAnd");}
        ExpressionId        expressionInclusiveOr(ExpressionId, ExpressionId)                   {error("Not Implemented: expressionInclusiveOr");}
        ExpressionId        expressionExclusiveOr(ExpressionId, ExpressionId)                   {error("Not Implemented: expressionExclusiveOr");}
        ExpressionId        expressionAnd(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionAnd");}
        ExpressionId        expressionEqual(ExpressionId, ExpressionId)                         {error("Not Implemented: expressionEqual");}
        ExpressionId        expressionNotEqual(ExpressionId, ExpressionId)                      {error("Not Implemented: expressionNotEqual");}
        ExpressionId        expressionLess(ExpressionId, ExpressionId)                          {error("Not Implemented: expressionLess");}
        ExpressionId        expressionGreat(ExpressionId, ExpressionId)                         {error("Not Implemented: expressionGreat");}
        ExpressionId        expressionLessEqual(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionLessEqual");}
        ExpressionId        expressionGreatEqual(ExpressionId, ExpressionId)                    {error("Not Implemented: expressionGreatEqual");}
        ExpressionId        expressionShiftLeft(ExpressionId, ExpressionId)                     {error("Not Implemented: expressionShiftLeft");}
        ExpressionId        expressionShiftRight(ExpressionId, ExpressionId)                    {error("Not Implemented: expressionShiftRight");}
        ExpressionId        expressionAdd(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionAdd");}
        ExpressionId        expressionSub(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionSub");}
        ExpressionId        expressionMul(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionMul");}
        ExpressionId        expressionDiv(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionDiv");}
        ExpressionId        expressionMod(ExpressionId, ExpressionId)                           {error("Not Implemented: expressionMod");}
        ExpressionId        expressionPreInc(ExpressionId)                                      {error("Not Implemented: expressionPreInc");}
        ExpressionId        expressionPreDec(ExpressionId)                                      {error("Not Implemented: expressionPreDec");}
        ExpressionId        expressionPlus(ExpressionId)                                        {error("Not Implemented: expressionPlus");}
        ExpressionId        expressionNeg(ExpressionId)                                         {error("Not Implemented: expressionNeg");}
        ExpressionId        expressionOneCompliment(ExpressionId)                               {error("Not Implemented: expressionOneCompliment");}
        ExpressionId        expressionNot(ExpressionId)                                         {error("Not Implemented: expressionNot");}
        ExpressionId        expressionArrayAccess(ExpressionId, ExpressionId)                   {error("Not Implemented: expressionArrayAccess");}
        ExpressionId        expressionFuncCall(ExpressionId id, ExpressionListId list)      {return addObjectToScope2<ExpressionFuncCall, Expression, ExpressionList>(id, list);}
        ExpressionId        expressionMemberAccess(ExpressionId id, IdentifierId mem)       {return addObjectToScope2<ExpressionMemberAccess, Expression, Identifier>(id, mem);}
        ExpressionId        expressionPostInc(ExpressionId)                                     {error("Not Implemented: expressionPostInc");}
        ExpressionId        expressionPostDec(ExpressionId)                                     {error("Not Implemented: expressionPostDec");}
        ExpressionId        expressionObject(ObjectId id)                                   {return addObjectToScope1<ExpressionObject, Object>(id);}
        ExpressionId        expressionLiteralString();
        ExpressionId        expressionLiteralInt();

        // Assembley String Builder
        Id<std::string>     assemblyInit();
        void                assembleyAppend(Id<std::string> id);
        // Parsing virtual methods
        using Reuse = std::function<Int()>;
        virtual VoidId              anvilProgramV(NamespaceList&, Reuse&& reuse);
        virtual NamespaceId         scopeNamespaceOpenV(std::string&, Reuse&& reuse);
        virtual NamespaceId         scopeNamespaceCloseV(Namespace&, DeclList& list, Reuse&& reuse);
        virtual ClassId             scopeClassOpenV(std::string&, Reuse&& reuse);
        virtual ClassId             scopeClassCloseV(Class&, DeclList& list, Reuse&& reuse);
        virtual FunctionId          scopeFunctionOpenV(std::string& id, Reuse&& reuse);
        virtual FunctionId          scopeFunctionCloseV(Function& id, TypeList& listId, Type& returnType, Reuse&& reuse);
        virtual CodeBlockId         scopeCodeBlockOpenV();
        virtual StatementId         scopeCodeBlockCloseV(CodeBlock&, StatementList&);
        virtual ObjectId            scopeObjectAddVariableV(Identifier& name, Type& id, ExpressionList& init);
        virtual ObjectId            scopeObjectAddFunctionV(Identifier& name, Type& id, Statement& code);
        virtual IdentifierId        identifierCreateV();
    private:
        void addToLine();
        void resetLine();

        template<typename Dst, typename Param1>
        Id<Base<Dst>> addObjectToScope1(Id<Param1> id)
        {
            IdAccess<Param1>    param1Access(storage, id);
            Param1&             param1 = param1Access;

            Scope&     topScope = currentScope.back();
            Base<Dst>& result   = topScope.add<Dst>(*this, param1);
            return storage.add(Ref<Base<Dst>>{result});
        }
        template<typename Dst, typename Param1, typename Param2>
        Id<Base<Dst>> addObjectToScope2(Id<Param1> id1, Id<Param2> id2)
        {
            IdAccess<Param1>    param1Access(storage, id1);
            IdAccess<Param2>    param2Access(storage, id2);
            Param1&             param1 = param1Access;
            Param2&             param2 = param2Access;

            Scope&     topScope = currentScope.back();
            Base<Dst>& result   = topScope.add<Dst>(*this, param1, param2);
            return storage.add(Ref<Base<Dst>>{result});
        }
        template<typename T>
        T& getOrAddScope(Scope& topScope, std::string const& scopeName);
        std::string getCurrentScopeFullName() const;
};

template<typename... Args>
void Action::log(Args&&... args) const
{
    (output << ... << args);
}

template<typename... Args>
void Action::error(Args&&... args) const
{
    std::stringstream extended;
    extended << "Error: '";
    (extended << ... << args);
    extended << "'\n" << *this;
    throw std::runtime_error(extended.str());
}


}

#endif
