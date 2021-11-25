#include "Semantic.h"
#include <string>
#include <list>
#include <cctype>

using namespace ThorsAnvil::Anvil::Ice;

struct ScopePrinter
{
    char const* f;
    ScopePrinter(char const* f)
        : f(f)
    {
        //std::cerr << "Entered Scope: " << f << "\n";
    }
    ~ScopePrinter()
    {
        //std::cerr << "Left Scope: " << f << "\n";
    }
};

Semantic::Semantic(Lexer& lexer, Scope& globalScope, Storage& storage, std::ostream& output)
    : Action(lexer, output)
    , anonNameCount(0)
    , globalScope(globalScope)
    , storage(storage)
{
    currentScope.emplace_back(globalScope);
}

Semantic::~Semantic()
{
}

// Action Override
void Semantic::assertNoStorage(Int value)
{
    if (value != 0)
    {
        error("Internal Compiler Error: Expecting Storage to have been released");
    }
}

void Semantic::releaseStorage(Int value)
{
    storage.release(value);
}

char hex(std::size_t halfByte)
{
    return halfByte < 10
                ? '0' + halfByte
                : 'a' + (halfByte - 10);
}

void generateHexName(std::string& name, std::size_t count)
{
    for (int loop = 0; loop < 16; ++loop)
    {
        name[loop+1] = hex((count >> (loop * 4)) & 0xF);
    }
}

std::string Semantic::generateAnonNameString()
{
    // 64 bit Size: Convert to hex => 16 bit
    // Prefix with dollar => 17bit
    std::string name(17, '$');
    generateHexName(name, anonNameCount);
    ++anonNameCount;
    return name;
}

Int Semantic::generateAnonName()
{
    ScopePrinter    printer("generateAnonName");
    std::string name = generateAnonNameString();
    return storage.add(std::move(name));
}

Int Semantic::identifierCreate(std::string_view view)
{
    ScopePrinter    printer("identifierCreate");
    return storage.add(std::string(view));
}

/*
 * Object names all start with a lowercase letter
 * An object is anything that can be passed as a parameter.
 *      Values
 *      Functions
 *      Closures.
 */
Int Semantic::identifierCheckObject(Int id)
{
    ScopePrinter    printer("identifierCheckObject");
    SAccessString   identifier(storage, id);
    if (std::islower(identifier.get()[0]))
    {
        return identifier.reUse();
    }
    error("Invalid Identifier for Object");
}

/*
 * Type names are greater than three characters long,
 * start with an uppercase letter and don't contain underscore.
 */
Int Semantic::identifierCheckType(Int id)
{
    ScopePrinter    printer("identifierCheckType");
    SAccessString   identifier(storage, id);

    if (   (std::isupper(identifier.get()[0]))
        && (identifier.get().size() > 3)
        && (identifier.get().find('_') == std::string::npos)
    )
    {
        return identifier.reUse();
    }

    error("Invalid Identifier for Type");
}

/*
 * Namespace names start with an upper case letter, and each upper case letter is prefixed by underscore.
 * Either the name is shorter than 4 characters or if 4 or more contains at least two capitols.
 */
Int Semantic::identifierCheckNS(Int id)
{
    ScopePrinter    printer("identifierCheckNS");
    SAccessString   identifier(storage, id);

    bool lastUnderscore = true;
    auto findUpperNotPrefixedByUnderScore = [&lastUnderscore](char x)
    {
        if (std::isupper(x) && !lastUnderscore)
        {
            return true;
        }
        lastUnderscore = x == '_';
        return false;
    };

    if ((std::isupper(identifier.get()[0]))
     && (
           (identifier.get().size() <= 3)
        || (std::find_if(std::begin(identifier.get()) + 1, std::end(identifier.get()), [](char x){return std::isupper(x);}) != identifier.get().end())
        )
    )
    {
        auto find = std::find_if(std::begin(identifier.get()), std::end(identifier.get()), findUpperNotPrefixedByUnderScore);
        if (find == std::end(identifier.get()))
        {
            return identifier.reUse();
        }
    }
    error("Invalid Identifier for Namespace");
}

Int Semantic::fullIdentiferCreate()
{
    ScopePrinter    printer("fullIdentiferCreate");
    return storage.add(FullIdent{});
}

Int Semantic::fullIdentiferAddPath(Int fp, Int id)
{
    ScopePrinter        printer("fullIdentiferAddPath");
    SAccessFullIdent    fullIdent(storage, fp);
    SAccessString       identifier(storage, id);

    fullIdent.get().emplace_front(std::move(identifier.get()));

    return fullIdent.reUse();
}

Int Semantic::paramListCreate()
{
    ScopePrinter    printer("paramListCreate");
    return storage.add(ParamList{});
}

Int Semantic::paramListAdd(Int pl, Int type)
{
    ScopePrinter        printer("paramListAdd");
    SAccessParamList    paramList(storage, pl);
    SAccessType         typeRef(storage, type);

    paramList.get().emplace_back(std::move(typeRef));

    return paramList.reUse();
}

Int Semantic::objectListCreate()
{
    ScopePrinter    printer("objectListCreate");
    return storage.add(ObjectList{});
}

Int Semantic::objectListAdd(Int ol, Int object)
{
    ScopePrinter        printer("objectListAdd");
    SAccessObjectList   objectList(storage, ol);
    SAccessObject       objectRef(storage, object);

    objectList.get().emplace_back(std::move(objectRef));

    return objectList.reUse();
}

template<typename T>
struct ReversView
{
    T& range;
    ReversView(T& range)
        : range(range)
    {}

    auto begin() {return range.rbegin();}
    auto end()   {return range.rend();}
};

Decl* Semantic::searchScopeForIdentifier(std::string const& path, std::string& partialMatch)
{
    Decl* result = nullptr;

    // search the scopes from the current one back to the global scope to see
    // if we can can find the type identifier root element.
    for (auto& scopeRef: ReversView(currentScope))
    {
        // For ease of use.
        auto& scope = scopeRef.get();

        // Search the current scope of the first path segment.
        auto find = scope.get(path);
        if (find.first)
        {
            // We found it in this scope.
            // For better error message loop from the global scope out to this scope
            // To build the "absolute" path of the type identifier we have found.
            for (auto& scopeRefForward: currentScope)
            {
                auto& scopeForward = scopeRefForward.get();
                partialMatch += (scope.contName() + "::");

                if (&scopeForward == &scope)
                {
                    break;
                }
            }

            // Add the current path segment
            partialMatch += (path + "::");

            // So far so we good we have found the first part decl that matches the fullIdent.
            result = &(*(find.second->second));
            break;
        }
    }

    return result;
}

Decl&  Semantic::searchScopeForPath(FullIdent& fullIdent)
{
    // Strings used to build error message if needed.
    std::string     fullPathString;
    std::string     partialMatch;

    // The object we are trying to find.
    Decl*   decl = nullptr;

    // Special case element in the path.
    bool    rootOfPath = true;

    // Loop over all members of the fullIdent.
    for (auto& path: fullIdent)
    {
        // Each time through the loop build the fullPAthString.
        // To get a good error message we must loop over all the path members
        // and not break out of the loop early.
        fullPathString += (path + "::");

        // The first time through the loop (for the root element of the type identifier only)
        if (rootOfPath)
        {
            decl = searchScopeForIdentifier(path, partialMatch);
            rootOfPath = false;
            continue;
        }

        // If the decl is null then we have failed to find one part of the path.
        // We will continue to loop to build up the correct string for the error message.
        if (decl == nullptr)
        {
            continue;
        }

        // We are doing well we have the latest decl now we must find the next
        // decl inside this that matches the next part of the fullIdent
        auto find = decl->find(path);
        decl = nullptr;
        if (find.first)
        {
            decl = &(*(find.second->second));
        }
    }


    if (decl == nullptr)
    {
        // We failed to find the correct decl.
#pragma vera-pushoff
        using namespace std::string_literals;
#pragma vera-pop
        error("Could Not Find Type: "s + fullPathString + "   Found partial Match: " + partialMatch);
    }
    // All is good we found a decl.
    // This is owned by a scope so return it but we don't need to destroy it.
    return *decl;
}

Int Semantic::findTypeInScope(Int fp)
{
    ScopePrinter        printer("findTypeInScope");
    SAccessFullIdent    fullIdent(storage, fp);
    Decl&               decl = searchScopeForPath(fullIdent);
    return storage.add(TypeRef{dynamic_cast<Type&>(decl)});
}

Int Semantic::findObjectInScope(Int fp)
{
    ScopePrinter        printer("findObjectInScope");
    SAccessFullIdent    fullIdent(storage, fp);
    Decl&               decl = searchScopeForPath(fullIdent);
    return storage.add(ObjectRef{dynamic_cast<Object&>(decl)});
}

template<typename T>
T& Semantic::getOrAddScope(Scope& topScope, std::string const& scopeName)
{
    auto find = topScope.get(scopeName);
    if (find.first)
    {
        Decl*       foundDeclWithSameName = find.second->second.get();
        return dynamic_cast<T&>(*foundDeclWithSameName);
    }
    return topScope.add<T>(std::move(scopeName));
}
void Semantic::checkUniqueDeclName(Scope& topScope, std::string const& declName)
{
    auto find = topScope.get(declName);
    if (find.first)
    {
#pragma vera-pushoff
        using namespace std::string_literals;
#pragma vera-pop

        error("There already exists a declaration of '" + declName + "' in the current scope");
    }
}

Int Semantic::scopeAddNamespace(Int name)
{
    ScopePrinter    printer("scopeAddNamespace");
    SAccessString   namespaceName(storage, name);
    Scope&          topScope = currentScope.back().get();
    Namespace&      newNameSpace = getOrAddScope<Namespace>(topScope, namespaceName);
    currentScope.emplace_back(newNameSpace);

    return storage.add(ScopeRef{dynamic_cast<Scope&>(newNameSpace)});
}

Int Semantic::scopeAddClass(Int name)
{
    ScopePrinter    printer("scopeAddClass");
    SAccessString   className(storage, name);
    Scope&          topScope = currentScope.back().get();
    Class&          newClass = getOrAddScope<Class>(topScope, className);

    currentScope.emplace_back(newClass);

    return storage.add(ScopeRef{dynamic_cast<Scope&>(newClass)});
}

Int Semantic::scopeAddArray(Int name, Int type)
{
    ScopePrinter    printer("scopeAddArray");
    SAccessString   arrayName(storage, name);
    SAccessType     typeInfo(storage, type);

    Scope&          topScope = currentScope.back().get();
    checkUniqueDeclName(topScope, arrayName);
    Array&          newArray = topScope.add<Array>(std::move(arrayName), typeInfo);

    return storage.add(TypeRef{dynamic_cast<Type&>(newArray)});
}

Int Semantic::scopeAddMap(Int name, Int key, Int value)
{
    ScopePrinter    printer("scopeAddMap");
    SAccessString   mapName(storage, name);
    SAccessType     keyInfo(storage, key);
    SAccessType     valueInfo(storage, value);

    Scope&          topScope = currentScope.back().get();
    checkUniqueDeclName(topScope, mapName);
    Map&            newMap = topScope.add<Map>(std::move(mapName), keyInfo, valueInfo);

    return storage.add(TypeRef{dynamic_cast<Type&>(newMap)});
}

Int Semantic::scopeAddFunc(Int name, Int pl, Int ret)
{
    ScopePrinter        printer("scopeAddFunc");
    SAccessString       funcName(storage, name);
    SAccessParamList    paramList(storage, pl);
    SAccessType         retInfo(storage, ret);

    Scope&          topScope = currentScope.back().get();
    checkUniqueDeclName(topScope, funcName);
    Func&           newFunc = topScope.add<Func>(std::move(funcName), std::move(paramList), retInfo);

    return storage.add(TypeRef{dynamic_cast<Type&>(newFunc)});
}

Int Semantic::scopeAddObject(Int name, Int type, Int init)
{
    ScopePrinter        printer("scopeAddObject");
    SAccessString       objectName(storage, name);
    SAccessType         typeInfo(storage, type);

    Scope&          topScope = currentScope.back().get();
    checkUniqueDeclName(topScope, objectName);
    Object& object = topScope.add<Object>(std::move(objectName), typeInfo);

    if (typeInfo.get().declType() == DeclType::Class)
    {
        ObjectList empty;
        std::reference_wrapper<ObjectList> initList(empty);

        if (init != 0) // Default
        {
            SAccessObjectList   initInfo(storage, init);
            initList = initInfo.get();
        }
        // codeAddObjectInit(object, std::move(initList.get()));
        ((void)object);
    }
    // TODO Init other types.

    return 0;
}

Int Semantic::scopeAddStatement(Int s)
{
    ScopePrinter    printer("scopeAddStatement");
    assertNoStorage(s);
    return 0;
}

Int Semantic::scopeAddCodeBlock()
{
    ScopePrinter    printer("scopeAddCodeBlock");
    std::string     codeBlockName = generateAnonNameString();
    Scope&          topScope = currentScope.back().get();
    checkUniqueDeclName(topScope, codeBlockName);
    CodeBlock&      codeBlock = topScope.add<CodeBlock>(std::move(codeBlockName));

    currentScope.emplace_back(codeBlock);

    return storage.add(ScopeRef{dynamic_cast<Scope&>(codeBlock)});
}

Int Semantic::scopeClose(Int oldScopeId)
{
    ScopePrinter    printer("scopeClose");
    Scope&          topScope = currentScope.back().get();
    SAccessScope    oldScope(storage, oldScopeId);

    currentScope.pop_back();
    if (&topScope != &oldScope.get())
    {
#pragma vera-pushoff
        using namespace std::string_literals;
#pragma vera-pop

        error("Bad Scope Closure: Expecting: >"s + oldScope.get().contName() + "<  Current: >" + topScope.contName() + "< ");
    }

    Class*          scopeToClass = dynamic_cast<Class*>(&oldScope.get()); // Could by nullptr as this by Namespace
    if (scopeToClass == nullptr)
    {
        return 0;
    }
    Type&           classToType = dynamic_cast<Type&>(*scopeToClass);
    return storage.add(TypeRef{classToType});
}

Int Semantic::addLiteralString()
{
    ScopePrinter    printer("addLiteralString");
    std::string objectName = generateAnonNameString();
    Type& stringType = getScopeSymbol<Type>(globalScope, "Std", "String");

    // TODO: Literal needs to be a const
    Scope& literalScope = getScopeSymbol<Namespace>(globalScope, "$Literal");
    Object& literalObject = literalScope.add<Literal<std::string>>(std::move(objectName), stringType, std::string(lexer.lexem()));

    return storage.add(ObjectRef{literalObject});
}

// Code Generation
void Semantic::codeAddObjectInit(Object& object, ObjectList&& param)
{
    Scope&      topScope = currentScope.back().get();
    CodeBlock*  codeBlock = dynamic_cast<CodeBlock*>(&topScope);

    if (codeBlock == nullptr)
    {
        auto find = topScope.get("$constructor");
        if (find.first != true)
        {
            error("Could not find constructor for non code block");
        }
        Decl* blockDecl = find.second->second.get();
        codeBlock       = dynamic_cast<CodeBlock*>(blockDecl);
        if (codeBlock == nullptr)
        {
            error("Failed to find Code Block");
        }
    }
    codeBlock->addCode<StatExprInitObject>(object, std::move(param));
}

Int Semantic::codeAddFunctionCall(Int obj, Int pl)
{
    ScopePrinter        printer("codeAddFunctionCall");
    SAccessObject       object(storage, obj);
    SAccessObjectList   param(storage, pl);
    Scope&              topScope = currentScope.back().get();

    CodeBlock*  codeBlock = dynamic_cast<CodeBlock*>(&topScope);
    if (codeBlock == nullptr)
    {
        error("Adding code but not inside a code block. Statements can only be placed in functions and methods");
    }

    codeBlock->addCode<StatExprFunctionCall>(object.get(), std::move(param.get()));

    return 0;
}
