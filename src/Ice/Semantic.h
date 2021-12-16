#ifndef THORSANVIL_ANVIL_ICE_SEMANTIC_H
#define THORSANVIL_ANVIL_ICE_SEMANTIC_H

#include "Action.h"
#include "Lexer.h"
#include "Parser.h"
#include "Declaration/Namespace.h"
#include "Declaration/Code.h"
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

    std::size_t             anonNameCount;
    Scope&                  globalScope;
    Storage&                storage;
    NamespaceDecOrder&      namespaceDecOrder;
    std::vector<ScopeRef>   currentScope;

    public:
        Semantic(Lexer& lexer, NamespaceDecOrder& namespaceDecOrder, Scope& globalScope, Storage& storage, std::ostream& output = std::cout);
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

        virtual Int objectIDCreate(Int il_ns, Int il_ob)            override;
        virtual Int identifierListCreate()                          override;
        virtual Int identifierListAdd(Int il, Int id)               override;

        virtual Int paramListCreate()                               override;
        virtual Int paramListAdd(Int pl, Int type)                  override;

        virtual Int objectListCreate()                              override;
        virtual Int objectListAdd(Int ol, Int object)               override;

        virtual Int findTypeInScope(Int fp)                         override;
        //virtual Int findObjectInScope(Int fp)                       override;

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
        void codeAddFunctionCallError(char const* base, ObjectIdList const& parameters, ParamList const& paramTypeList);
        Type const& getMemberType(Object& from, IdentifierList const& memberName) const;
        Decl& searchScopeForPath(IdentifierList const& objectId) const;
        Decl* searchScopeForIdentifier(std::string const& path, std::string& partialMatch) const;
        std::string  generateAnonNameString();
        std::string  getCurrentScopeFullName() const;

        template<typename T>
        T& getScopeSymbol(Scope& scope, IdentifierList const& identifier);

        template<bool B>
        auto getInitMethod(Scope& topScope)
        {
            if constexpr (B)
            {
                return topScope.get("$constructor$Code");
            }
            else
            {
                return topScope.get("$destructor$Code");
            }
        }
        template<bool B, typename T, typename... Args>
        void addCodeToCurrentScope(ObjectId const& objectId, Args&&... args)
        {
            Scope&      topScope = currentScope.back().get();
            CodeBlock*  codeBlock = dynamic_cast<CodeBlock*>(&topScope);

            if (codeBlock == nullptr)
            {
                auto find = getInitMethod<B>(topScope);
                if (find.first != true)
                {
                    error("Could not find constructor for non code block");
                }
                Decl*   blockDecl = find.second->second.get();
                codeBlock       = dynamic_cast<CodeBlock*>(blockDecl);
                if (codeBlock == nullptr)
                {
                    error("$constructor was not a code block!!!!");
                }
            }
            codeBlock->addCode<B, T>(objectId, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getOrAddScope(Scope& topScope, std::string const& scopeName);
        void checkUniqueDeclName(Scope& topScope, std::string const& declName);
};

template<typename T>
T& Semantic::getScopeSymbol(Scope& scopeStart, IdentifierList const& identifierList)
{
#pragma vera-pushoff
    using namespace std::string_literals;
#pragma vera-pop

    Scope*  scope = &scopeStart;
    for (auto const& name: identifierList)
    {
        auto find = scope->get(name);
        //ASSERT_TRUE(find.first);
        if (!find.first)
        {
            error("Symbol '"s + name + "' not found");
        }

        Decl*  decl         = find.second->second.get();

        Class* classDecl    = dynamic_cast<Class*>(decl);
        if (classDecl)
        {
            scope = dynamic_cast<Scope*>(classDecl);
            continue;
        }
        // TODO Add Map/Array
        Namespace* namespaceDecl    = dynamic_cast<Namespace*>(decl);
        if (namespaceDecl)
        {
            scope = dynamic_cast<Scope*>(namespaceDecl);
            continue;
        }
        CodeBlock* codeBlockDecl    = dynamic_cast<CodeBlock*>(decl);
        if (codeBlockDecl)
        {
            scope = dynamic_cast<Scope*>(codeBlockDecl);
            continue;
        }
        error("Not a valid Scope Path '"s + name + "'");
    }

    return dynamic_cast<T&>(dynamic_cast<Decl&>(*scope));
}

}

#endif
