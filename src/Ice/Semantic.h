#ifndef THORSANVIL_ANVIL_ICE_COMPILER_H
#define THORSANVIL_ANVIL_ICE_COMPILER_H

#include "Action.h"
#include "Lexer.h"
#include "Parser.h"
#include "Declaration.h"
#include "Storage.h"

#include <fstream>
#include <map>
#include <list>
#include <variant>
#include <vector>

namespace ThorsAnvil::Anvil::Ice
{

class Semantic: public Action
{
    std::size_t anonNameCount;
    Scope&      globalScope;
    Storage&    storage;
    std::vector<std::reference_wrapper<Scope>>  currentScope;

    public:
        Semantic(Lexer& lexer, Scope& globalScope, Storage& storage, std::ostream& output = std::cout);
        virtual ~Semantic() override;

        void display(std::ostream& stream);

    // Action Virtual Functions override
        virtual void assertNoStorage(Int)                           override;
        virtual void releaseStorage(Int)                            override;
        virtual Int  generateAnonName()                             override;

        virtual Int identifierCreate(std::string_view view)         override;
        virtual Int identifierCheckObject(Int id)                   override;
        virtual Int identifierCheckType(Int id)                     override;
        virtual Int identifierCheckNS(Int id)                       override;

        virtual Int fullIdentiferCreate()                           override;
        virtual Int fullIdentiferAddPath(Int fp, Int id)            override;

        virtual Int paramListCreate()                               override;
        virtual Int paramListAdd(Int pl, Int type)                  override;

        virtual Int objectListCreate()                              override;
        virtual Int objectListAdd(Int ol, Int object)               override;

        virtual Int findTypeInScope(Int fp)                         override;
        virtual Int findObjectInScope(Int fp)                       override;

        virtual Int scopeAddNamespace(Int name)                     override;
        virtual Int scopeAddClass(Int name)                         override;
        virtual Int scopeAddArray(Int name, Int type)               override;
        virtual Int scopeAddMap(Int name, Int key, Int value)       override;
        virtual Int scopeAddFunc(Int name, Int param, Int ret)      override;
        virtual Int scopeAddObject(Int name, Int type, Int init)    override;
        virtual Int scopeAddStatement(Int s)                        override;
        virtual Int scopeAddCodeBlock()                             override;
        virtual Int scopeClose(Int oldSCope)                        override;

        virtual Int addLiteralString()                              override;

        virtual Int codeAddFunctionCall(Int obj, Int param)         override;
    private:
        Decl& searchScopeForPath(FullIdent& fullIdent);
        Decl* searchScopeForIdentifier(std::string const& path, std::string& partialMatch);
        std::string  generateAnonNameString();

        template<typename T>
        T& getScopeSymbol(Scope& scope, std::string const& name);

        template<typename T, typename... Args>
        T& getScopeSymbol(Scope& scope, std::string const& name, Args&... path);

        void codeAddObjectInit(Object& obj, ObjectList&& param);

        template<typename T>
        T& getOrAddScope(Scope& topScope, std::string const& scopeName);
        void checkUniqueDeclName(Scope& topScope, std::string const& declName);
};

template<typename T>
T& Semantic::getScopeSymbol(Scope& scope, std::string const& name)
{
    auto find = scope.get(name);
    //ASSERT_TRUE(find.first);
    if (!find.first)
    {
#pragma vera-pushoff
        using namespace std::string_literals;
#pragma vera-pop

        error("Symbol '"s + name + "' not found");
    }

    Decl*  decl         = find.second->second.get();
    return dynamic_cast<T&>(*decl);
}

template<typename T, typename... Args>
T& Semantic::getScopeSymbol(Scope& scope, std::string const& name, Args&... path)
{
#pragma vera-pushoff
    using namespace std::string_literals;
#pragma vera-pop

    auto find = scope.get(name);
    //ASSERT_TRUE(find.first);
    if (!find.first)
    {
        error("Symbol '"s + name + "' not found");
    }

    Decl*  decl         = find.second->second.get();

    Class* classDecl    = dynamic_cast<Class*>(decl);
    if (classDecl)
    {
        return getScopeSymbol<T>(dynamic_cast<Scope&>(*classDecl), path...);
    }
    Namespace* namespaceDecl    = dynamic_cast<Namespace*>(decl);
    if (namespaceDecl)
    {
        return getScopeSymbol<T>(dynamic_cast<Scope&>(*namespaceDecl), path...);
    }
    CodeBlock* codeBlockDecl    = dynamic_cast<CodeBlock*>(decl);
    if (codeBlockDecl)
    {
        return getScopeSymbol<T>(dynamic_cast<Scope&>(*codeBlockDecl), path...);
    }
    error("Not a valid Scope Path '"s + name + "'");
}

}

#endif
