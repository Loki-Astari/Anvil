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

Int Semantic::objectIDCreate(Int il_ns, Int il_ob)
{
    SAccessIdentifierList   namespaceList(storage, il_ns);
    SAccessIdentifierList   objectList(storage, il_ob);

    // Take the first member of the objectList and add to the namespace.
    // This will give you an identifiable object.
    // Note: if the namespace is empty then it is just the variable name in the current context.
    //          eg.         "My_NS1::My_NS2::My_NS3::variable.member.member"
    //              =>   namespaceList:  My_NS1::My_NS2::My_NS3
    //              =>   objectList:     variable.member.member
    //          After the next statement we get:
    //                  namespaceList:  My_NS1::My_NS2::My_NS3::variable
    //                  objectList:     member.member
    //          Now we search for "My_NS1::My_NS2::My_NS3::variable" from the current context to find an object.
    //
    //      Alternatively:  "variable.member.member"
    //              =>   namespaceList:
    //              =>   objectList:     variable.member.member
    //          After the next statement we get:
    //                  namespaceList:  variable
    //                  objectList:     member.member
    //          Now we search for "variable" from the current context to find an object.
    namespaceList.get().emplace_front(std::move(objectList.get().back()));
    objectList.get().pop_back();

    Decl&       findDecl    = searchScopeForPath(namespaceList.get());
    Object&     object      = dynamic_cast<Object&>(findDecl);
    Type const& memberType  = getMemberType(object, objectList.get());

    return storage.add(ObjectId(object, std::move(objectList.get()), memberType));
}

Int Semantic::identifierListCreate()
{
    return storage.add(IdentifierList{});
}

Int Semantic::identifierListAdd(Int il, Int id)
{
    SAccessIdentifierList   list(storage, il);
    SAccessString           identifier(storage, id);

    list.get().emplace_back(std::move(identifier.get()));
    return list.reUse();
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
    return storage.add(ObjectIdList{});
}

Int Semantic::objectListAdd(Int ol, Int object)
{
    ScopePrinter        printer("objectListAdd");
    SAccessObjectIdList objectList(storage, ol);
    SAccessObjectId     objectRef(storage, object);

    objectList.get().emplace_back(std::move(objectRef));

    return objectList.reUse();
}

Type const& Semantic::getMemberType(Object& from, IdentifierList const& memberName) const
{
    Object*  result = &from;

    for (auto const& path: memberName)
    {
        Type      const& type     = result->getType();
        Container const& contType = dynamic_cast<Container const&>(type);

        auto find = contType.get(path);
        if (!find.first)
        {
            std::stringstream errorMsg;
            std::string sep = "";
            errorMsg << "Invalid Member of Scope: '" << path << "'. Searching: '";
            for (auto const& info: memberName)
            {
                errorMsg << sep << info;
                sep = ".";
            }
            errorMsg << "'";
            error(errorMsg.str());
        }
        Decl*   decl    = find.second->second.get();

        result  = dynamic_cast<Object*>(decl);
        if (result == nullptr)
        {
            std::stringstream errorMsg;
            std::string sep = "";
            errorMsg << "Member of Scope was not an object '" << path << "'. Searching: '";
            for (auto const& info: memberName)
            {
                errorMsg << sep << info;
                sep = ".";
            }
            errorMsg << "'";
            error(errorMsg.str());
        }
    }
    return result->getType();
}

Decl* Semantic::searchScopeForIdentifier(std::string const& path, std::string& partialMatch) const
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

Decl&  Semantic::searchScopeForPath(IdentifierList const& fullIdent) const
{
    // Strings used to build error message if needed.
    std::string     fullPathString;
    std::string     partialMatch;

    // The object we are trying to find.
    Decl*   decl = nullptr;

    // Special case element in the path.
    bool    rootOfPath = true;

    // Loop over all members of the fullIdent.
    for (auto& path: ReversView{fullIdent})
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
    ScopePrinter            printer("findTypeInScope");
    SAccessIdentifierList   identList(storage, fp);
    Decl&                   decl = searchScopeForPath(identList);
    return storage.add(TypeRef{dynamic_cast<Type&>(decl)});
}

/*
Int Semantic::findObjectInScope(Int fp)
{
    ScopePrinter            printer("findObjectInScope");
    SAccessObjectId         objectId(storage, fp);

    if (objectType.declType() != DeclType::Func)
    {
        // TODO
        // Currently only works with Class
        // Should work with Array and Map
        //      Should be:
        //          Scope& workingScope = dynamic_cast<Scope&>(objectType);
        Scope const& workingScope = dynamic_cast<Scope const&>(dynamic_cast<Class const&>(objectType));
        // TODO FINISH
        ((void)workingScope);

    }
    else
    {
        if (objectId.get().resolutionPath().size() != 0)
        {
            std::stringstream errorMessage;
            errorMessage << "Invalid Object Identifier. Main object is a function '"
                         << objectId.get().objectName()
                         << "'. Resolution of sub path can't happen '"
                         << objectId.get().resolutionName()
                         << "'";
            error(errorMessage.str());
        }
    }
    //return storage.add(ObjectRef{dynamic_cast<Object&>(decl)});
    return 0;
}
*/

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
        ObjectIdList empty;
        std::reference_wrapper<ObjectIdList> initList(empty);

        if (init != 0) // Default
        {
            SAccessObjectIdList   initInfo(storage, init);
            initList = initInfo.get();
        }
        ObjectId objectInit(object, IdentifierList{"$constructor"}, getMemberType(object, IdentifierList{"$constructor"}));
        ObjectId objectDest(object, IdentifierList{"$destructor"},  getMemberType(object, IdentifierList{"$destructor"}));
        addCodeToCurrentScope<true, StatExprFunctionCall>(objectInit, std::move(initList.get()));
        addCodeToCurrentScope<false,StatExprFunctionCall>(objectDest, ObjectIdList{});
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
    std::string     objectName = generateAnonNameString();
    Type&           stringType = getScopeSymbol<Type>(globalScope, {"Std", "String"});

    Scope&          literalScope = getScopeSymbol<Namespace>(globalScope, {"$Literal"});
    Object&         literalObject = literalScope.add<Literal<std::string>>(std::move(objectName), stringType, std::string(lexer.lexem()));

    return storage.add(ObjectId{literalObject, {}, stringType});
}

void Semantic::codeAddFunctionCallError(char const* base, ObjectIdList const& parameters, ParamList const& paramTypeList)
{
    std::stringstream errorMsg;

    errorMsg << base << "  Got(";
    for (auto const& p: parameters)
    {
        errorMsg << ", " << p.get().getType().declName();
    }
    errorMsg << ")  Expecting: (";
    for (auto const& p: paramTypeList)
    {
        errorMsg << ", " << p.get().declName();
    }
    errorMsg << ")";

    error(errorMsg.str());
}

Int Semantic::codeAddFunctionCall(Int obj, Int pl)
{
    ScopePrinter        printer("codeAddFunctionCall");
    SAccessObjectId     object(storage, obj);
    SAccessObjectIdList param(storage, pl);

    // Compare the types in the parameter list to the expected types for the function call.
    Type const& funcCallType = object.get().getType();
    if (funcCallType.declType() != DeclType::Func)
    {
        error("Trying to make a function call with a non function object");
    }
    Func const& funcCall = dynamic_cast<Func const&>(funcCallType);
    ParamList const& functionParameter = funcCall.getParamList();

    auto paramTypeLoop = param.get().begin();
    auto paramTypeEnd  = param.get().end();
    auto funcParamLoop = functionParameter.begin();
    auto funcParamEnd  = functionParameter.end();

    for (; paramTypeLoop != paramTypeEnd && funcParamLoop != funcParamEnd; ++paramTypeLoop, ++funcParamLoop)
    {
        Type const&   paramType = paramTypeLoop->get().getType();
        Type const&   funcType = *funcParamLoop;

        if (paramType != funcType)
        {
            codeAddFunctionCallError("Parameter type mismatch", param, functionParameter);
        }
    }
    if (paramTypeLoop != paramTypeEnd)
    {
        codeAddFunctionCallError("To many parameters were passed to the function.", param, functionParameter);
    }
    if (funcParamLoop != funcParamEnd)
    {
        codeAddFunctionCallError("Not enough parameters were passed to the function.", param, functionParameter);
    }

    addCodeToCurrentScope<true, StatExprFunctionCall>(object.get(), std::move(param.get()));
    return 0;
}
