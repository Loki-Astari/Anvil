#include "Action.h"
#include "Lexer.h"
#include "Storage.h"
#include "ParserTypes.h"

using namespace ThorsAnvil::Anvil::Ice;

struct ScopePrinter
{
    char const* f;
    ScopePrinter(char const* f)
        : f(f)
    {
        //std::cout << "Entered Scope: " << f << "\n";
    }
    ~ScopePrinter()
    {
        //std::cout << "Left Scope: " << f << "\n";
    }
};

Action::Action(Lexer& lexer, Scope& globalScope, Storage& storage, std::ostream& output)
    : lexer(lexer)
    , storage(storage)
    , globalScope(globalScope)
    , output(output)
    , lineNo(0)
    , offset(0)
{
    currentScope.emplace_back(globalScope);
}

Action::~Action()
{}


// Lexer Stuff
// ========================
void Action::token()
{
#pragma vera-pushoff
    using namespace std::string_literals;
#pragma vera-pop
    log("Token: "s .append(lexer.lexem()).c_str());
    addToLine();
}

void Action::addToLine()
{
    std::string_view    token = lexer.lexem();

    currentLine += token;
    offset += std::size(token);
}

void Action::resetLine()
{
    currentLine.clear();
    ++lineNo;
    ++offset;
}


// Parser
// ========================
VoidId Action::anvilProgram(NamespaceListId id)
{
    ScopePrinter scope("anvilProgram");
    NamespaceListAccess     access(storage, id);
    return anvilProgramV(access, [&access](){return access.reuse();});
}
VoidId Action::anvilProgramV(NamespaceList& /*list*/, Reuse&& /*reuse*/)
{
    return 0;
}
// ------------------

NamespaceListId Action::listNamespaceCreate()
{
    ScopePrinter scope("listNamespaceCreate");
    return listNamespaceCreateV();
}
NamespaceListId Action::listNamespaceCreateV()
{
    return storage.add<NamespaceList>(NamespaceList{});
}
// ------------------

NamespaceListId Action::listNamespaceAppend(NamespaceListId listId, NamespaceId id)
{
    ScopePrinter scope("listNamespaceAppend");
    NamespaceListAccess    access(storage, listId);
    return listNamespaceAppendV(access, NamespaceAccess(storage, id), [&access](){return access.reuse();});
}
NamespaceListId Action::listNamespaceAppendV(NamespaceList& list, Namespace& ns, Reuse&& reuse)
{
    list.emplace_back(ns);
    return reuse();
}
// ------------------

NamespaceId Action::scopeNamespaceOpen(IdentifierId id)
{
    ScopePrinter scope("scopeNamespaceOpen");
    IdentifierAccess    access(storage, id);
    return scopeNamespaceOpenV(access, [&access](){return access.reuse();});
}
NamespaceId Action::scopeNamespaceOpenV(std::string& namespaceName, Reuse&& /*reuse*/)
{
    Scope&          topScope = currentScope.back();
    Namespace&      newNameSpace = getOrAddScope<Namespace>(topScope, namespaceName);
    currentScope.emplace_back(newNameSpace);

    return storage.add(NamespaceRef{newNameSpace});
}
// ------------------

NamespaceId Action::scopeNamespaceClose(NamespaceId id, DeclListId listId)
{
    ScopePrinter scope("scopeNamespaceClose");
    NamespaceAccess     access(storage, id);
    return scopeNamespaceCloseV(access, DeclListAccess(storage, listId), [&access](){return access.reuse();});
}
NamespaceId Action::scopeNamespaceCloseV(Namespace& ns, DeclList&, Reuse&& reuse)
{
    Scope&          topScope = currentScope.back();
    Namespace*      topNS = dynamic_cast<Namespace*>(&topScope);

    if (&ns != topNS)
    {
        error("Internal Error: Scope Note correctly aligned from Namespace");
    }
    currentScope.pop_back();
    return reuse();;
}
// ------------------

ClassId Action::scopeClassOpen(IdentifierId id)
{
    ScopePrinter scope("scopeClassOpen");
    IdentifierAccess     access(storage, id);
    return scopeClassOpenV(access, [&access](){return access.reuse();});
}
ClassId Action::scopeClassOpenV(std::string& className, Reuse&& /*reuse*/)
{
    Scope&          topScope = currentScope.back();
    Class&          newClass = getOrAddScope<Class>(topScope, className);
    currentScope.emplace_back(newClass);

    return storage.add(ClassRef{newClass});
}
// ------------------

ClassId Action::scopeClassClose(ClassId id, DeclListId listId)
{
    ScopePrinter scope("scopeClassClose");
    ClassAccess     access(storage, id);
    return scopeClassCloseV(access, DeclListAccess(storage, listId), [&access](){return access.reuse();});
}
ClassId Action::scopeClassCloseV(Class& cl, DeclList& /*list*/, Reuse&& reuse)
{
    Scope&          topScope = currentScope.back();
    Class*          topClass = dynamic_cast<Class*>(&topScope);

    if (&cl != topClass)
    {
        error("Internal Error: Scope Note correctly aligned from Class");
    }
    currentScope.pop_back();
    return reuse();
}
// ------------------

DeclListId Action::listDeclCreate()
{
    ScopePrinter scope("listDeclCreate");
    return listDeclCreateV();
}
DeclListId Action::listDeclCreateV()
{
    return storage.add<DeclList>(DeclList{});
}
// ------------------

DeclListId Action::listDeclAppend(DeclListId listId, DeclId id)
{
    ScopePrinter scope("listDeclAppend");
    DeclListAccess     access(storage, listId);

    return listDeclAppendV(access, DeclAccess(storage, id), [&access](){return access.reuse();});
}
DeclListId Action::listDeclAppendV(DeclList& list, Decl& decl, Reuse&& reuse)
{
    list.emplace_back(decl);
    return reuse();
}
// ------------------

IdentifierId Action::identifierCreate()
{
    ScopePrinter scope("identifierCreate");
    return identifierCreateV();
}
IdentifierId Action::identifierCreateV()
{
    return storage.add<std::string>(std::string(lexer.lexem()));
}
// ------------------


// Action Utility Functions
// ========================
template<typename T>
T& Action::getOrAddScope(Scope& topScope, std::string const& scopeName)
{
    auto find = topScope.get(scopeName);
    if (find.first)
    {
        Decl*       foundDeclWithSameName = find.second->second.get();
        return dynamic_cast<T&>(*foundDeclWithSameName);
    }
    T& addedScope = topScope.add<T>(scopeName);
    addedScope.setPath(getCurrentScopeFullName());
    return addedScope;
}

std::string Action::getCurrentScopeFullName() const
{
    std::string result;
    for (auto& scopeRefForward: currentScope)
    {
        auto& scopeForward = scopeRefForward.get();
        result += (scopeForward.declName() + "-");
    }

    return result;
}


// Storage Access
// ========================
template<typename T>
IdAccess<T>::~IdAccess()
{
    if (index != 0)
    {
        storage.release(index);
    }
}

template<typename T>
IdAccess<T>::operator T&() const
{
    Int location = (index != 0) ? index : IdTraits<T>::defaultStorageId;
    return storage.get<AccessType>(location);
}

namespace ThorsAnvil::Anvil::Ice
{

std::ostream& operator<<(std::ostream& str, Action const& action)
{
    return str
             << "Last Token: >" << action.lexer.lexem() << "<\n"
             << "Line:  " << action.lineNo << " : " << action.currentLine.size() << "     FileOffset: " << action.offset << "\n"
             << action.currentLine << "\n"
             << "\n";
}

template struct IdAccess<Namespace>;
template struct IdAccess<Identifier>;

}
