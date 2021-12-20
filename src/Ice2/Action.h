#ifndef THORSANVIL_ANVIL_ICE_ACTION_H
#define THORSANVIL_ANVIL_ICE_ACTION_H

#include "ParserTypes.h"
#include "Declaration.h"
#include "Storage.h"

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

        // Parsing
        VoidId              anvilProgram(NamespaceListId id);
        NamespaceListId     listNamespaceCreate();
        NamespaceListId     listNamespaceAppend(NamespaceListId listId, NamespaceId id);
        NamespaceId         scopeNamespaceOpen(IdentifierId id);
        NamespaceId         scopeNamespaceClose(NamespaceId id, DeclListId listId);
        ClassId             scopeClassOpen(IdentifierId id);
        ClassId             scopeClassClose(ClassId id, DeclListId listId);
        DeclListId          listDeclCreate();
        DeclListId          listDeclAppend(DeclListId listId, DeclId id);
        IdentifierId        identifierCreate();

        template<ParserType From, ParserType To>
        Id<To> convert(Id<From> id)
        {
            using FromType = typename IdTraits<From>::ExportType;
            using ToType = typename IdTraits<To>::ExportType;
            using ToStoreType = typename IdTraits<To>::AccessType;
            IdAccess<From>  access(storage, id);
            FromType&  fromValue = access;
            ToType&    toType = dynamic_cast<ToType&>(fromValue);
            return storage.add<ToStoreType>(ToStoreType{toType});
        }


        // Parsing virtual methods
        using Reuse = std::function<Int()>;
        virtual VoidId              anvilProgramV(NamespaceList&, Reuse&& reuse);
        virtual NamespaceListId     listNamespaceCreateV();
        virtual NamespaceListId     listNamespaceAppendV(NamespaceList& listId, Namespace& id, Reuse&& reuse);
        virtual NamespaceId         scopeNamespaceOpenV(std::string&, Reuse&& reuse);
        virtual NamespaceId         scopeNamespaceCloseV(Namespace&, DeclList& list, Reuse&& reuse);
        virtual ClassId             scopeClassOpenV(std::string&, Reuse&& reuse);
        virtual ClassId             scopeClassCloseV(Class&, DeclList& list, Reuse&& reuse);
        virtual DeclListId          listDeclCreateV();
        virtual DeclListId          listDeclAppendV(DeclList& list, Decl& decl, Reuse&& reuse);
        virtual IdentifierId        identifierCreateV();
    private:
        void addToLine();
        void resetLine();

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
