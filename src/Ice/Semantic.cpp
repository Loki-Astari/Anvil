#include "Semantic.h"
#include <string>
#include <list>
#include <cctype>

using namespace ThorsAnvil::Anvil::Ice;

Decl::~Decl()
{}

Type::~Type()
{}

Semantic::Semantic(Scope& globalScope, std::istream& input, std::ostream& output)
    : Action(output)
    , parser(*this, input, output)
    , globalScope(globalScope)
{
    currentScope.emplace_back(globalScope);
}

Semantic::~Semantic()
{
}

bool Semantic::go()
{
    return parser.parse();
}

// Action Override
void Semantic::assertNoStorage(Lexer& lexer, Int value)
{
    if (value != 0)
    {
        error(lexer, "Internal Compiler Error: Expecting Storage to have been released");
    }
}

void Semantic::releaseStorage(Lexer&, Int value)
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
    static std::size_t count = 0;

    // 64 bit Size: Convert to hex => 16 bit
    // Prefix with dollar => 17bit
    std::string name(17, '$');
    generateHexName(name, count);
    return name;
}

Int Semantic::generateAnonName(Lexer&)
{
    std::string name = generateAnonNameString();
    return storage.add(std::move(name));
}

Int Semantic::identifierCreate(Lexer& lexer)
{
    return storage.add(std::string(lexer.lexem()));
}

/*
 * Object names all start with a lowercase letter
 * An object is anything that can be passed as a parameter.
 *      Values
 *      Functions
 *      Closures.
 */
Int Semantic::identifierCheckObject(Lexer& lexer, Int id)
{
    std::string&    identifier(storage.get<std::string>(id));
    if (std::islower(identifier[0]))
    {
        return id;
    }
    error(lexer, "Invalid Identifier for Object");
}

/*
 * Type names are greater than three characters long,
 * start with an uppercase letter and don't contain underscore.
 */
Int Semantic::identifierCheckType(Lexer& lexer, Int id)
{
    std::string&    identifier(storage.get<std::string>(id));
    if ((std::isupper(identifier[0])) && (identifier.size() > 3) && (identifier.find('_') == std::string::npos))
    {
        return id;
    }
    error(lexer, "Invalid Identifier for Type");
}

/*
 * Namespace names start with an upper case letter, and each upper case letter is prefixed by underscore.
 * Either the name is shorter than 4 characters or if 4 or more contains at least two capitols.
 */
Int Semantic::identifierCheckNS(Lexer& lexer, Int id)
{
    std::string&    identifier(storage.get<std::string>(id));

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

    if ((std::isupper(identifier[0])) && ((identifier.size() <= 3) || (std::find_if(std::begin(identifier) + 1, std::end(identifier), [](char x){return std::isupper(x);}) != identifier.end())))
    {
        auto find = std::find_if(std::begin(identifier), std::end(identifier), findUpperNotPrefixedByUnderScore);
        if (find == std::end(identifier))
        {
            return id;
        }
    }
    error(lexer, "Invalid Identifier for Namespace");
}

Int Semantic::fullIdentiferCreate(Lexer& /*lexer*/)
{
    return storage.add(FullIdent{});
}

Int Semantic::fullIdentiferAddPath(Lexer& /*lexer*/, Int fp, Int id)
{
    FullIdent&      fullIdent(storage.get<FullIdent>(fp));
    std::string&    identifier(storage.get<std::string>(id));

    fullIdent.emplace_front(std::move(identifier));
    storage.release(id);

    return fp;
}

Int Semantic::paramListCreate(Lexer& /*lexer*/)
{
    return storage.add(ParamList{});
}

Int Semantic::paramListAdd(Lexer& /*lexer*/, Int pl, Int type)
{
    ParamList&      paramList(storage.get<ParamList>(pl));
    TypeRef&        typeRef(storage.get<TypeRef>(type));

    paramList.emplace_back(std::move(typeRef));
    storage.release(type);

    return pl;
}

Int Semantic::objectListCreate(Lexer& /*lexer*/)
{
    return storage.add(ObjectList{});
}

Int Semantic::objectListAdd(Lexer& /*lexer*/, Int ol, Int object)
{
    ObjectList&     objectList(storage.get<ObjectList>(ol));
    ObjectRef&      objectRef(storage.get<ObjectRef>(object));

    objectList.emplace_back(std::move(objectRef));
    storage.release(object);

    return ol;
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

Decl&  Semantic::searchScopeForPath(Lexer& lexer, Int fp)
{
    FullIdent&  fullIdent(storage.get<FullIdent>(fp));

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
        error(lexer, "Could Not Find Type: "s + fullPathString + "   Found partial Match: " + partialMatch);
    }
    // All is good we found a decl.
    // This is owned by a scope so return it but we don't need to destroy it.
    return *decl;
}

Int Semantic::findTypeInScope(Lexer& lexer, Int fp)
{
    Decl&  decl = searchScopeForPath(lexer, fp);
    storage.release(fp);
    return storage.add(TypeRef{dynamic_cast<Type&>(decl)});
}

Int Semantic::findObjectInScope(Lexer& lexer, Int fp)
{
    Decl&  decl = searchScopeForPath(lexer, fp);
    storage.release(fp);
    return storage.add(ObjectRef{dynamic_cast<Object&>(decl)});
}

Int Semantic::scopeAddNamespace(Lexer& /*lexer*/, Int name)
{
    std::string&    namespaceName(storage.get<std::string>(name));
    Scope&          topScope = currentScope.back().get();
    Namespace&      newNameSpace = topScope.add(std::make_unique<Namespace>(std::move(namespaceName)));

    currentScope.emplace_back(newNameSpace);

    storage.release(name);
    return storage.add(ScopeRef{dynamic_cast<Scope&>(newNameSpace)});
}

Int Semantic::scopeAddClass(Lexer& /*lexer*/, Int name)
{
    std::string&    className(storage.get<std::string>(name));
    Scope&          topScope = currentScope.back().get();
    Class&          newClass = topScope.add(std::make_unique<Class>(std::move(className)));

    currentScope.emplace_back(newClass);

    storage.release(name);
    return storage.add(ScopeRef{dynamic_cast<Scope&>(newClass)});
}

Int Semantic::scopeAddArray(Lexer& /*lexer*/, Int name, Int type)
{
    std::string&    arrayName(storage.get<std::string>(name));
    Type&           typeInfo(storage.get<TypeRef>(type).get());

    Scope&          topScope = currentScope.back().get();
    Array&          newArray = topScope.add(std::make_unique<Array>(std::move(arrayName), typeInfo));

    storage.release(name);
    storage.release(type);
    return storage.add(TypeRef{dynamic_cast<Type&>(newArray)});
}

Int Semantic::scopeAddMap(Lexer& /*lexer*/, Int name, Int key, Int value)
{
    std::string&    mapName(storage.get<std::string>(name));
    Type&           keyInfo(storage.get<TypeRef>(key).get());
    Type&           valueInfo(storage.get<TypeRef>(value).get());

    Scope&          topScope = currentScope.back().get();
    Map&            newMap = topScope.add(std::make_unique<Map>(std::move(mapName), keyInfo, valueInfo));

    storage.release(name);
    storage.release(key);
    storage.release(value);
    return storage.add(TypeRef{dynamic_cast<Type&>(newMap)});
}

Int Semantic::scopeAddFunc(Lexer& /*lexer*/, Int name, Int pl, Int ret)
{
    std::string&    funcName(storage.get<std::string>(name));
    ParamList&      paramList(storage.get<ParamList>(pl));
    Type&           retInfo(storage.get<TypeRef>(ret).get());

    Scope&          topScope = currentScope.back().get();
    Func&           newFunc = topScope.add(std::make_unique<Func>(std::move(funcName), std::move(paramList), retInfo));

    storage.release(name);
    storage.release(pl);
    storage.release(ret);
    return storage.add(TypeRef{dynamic_cast<Type&>(newFunc)});
}

Int Semantic::scopeAddObject(Lexer& /*lexer*/, Int name, Int type)
{
    std::string&    objectName(storage.get<std::string>(name));
    Type&           typeInfo(storage.get<TypeRef>(type).get());

    Scope&          topScope = currentScope.back().get();
    topScope.add(std::make_unique<Object>(std::move(objectName), typeInfo));

    storage.release(name);
    storage.release(type);
    return 0;
}

Int Semantic::scopeClose(Lexer& lexer, Int oldScopeId)
{
    Scope&          topScope = currentScope.back().get();
    Scope&          oldScope(storage.get<ScopeRef>(oldScopeId).get());

    currentScope.pop_back();
    if (&topScope != &oldScope)
    {
#pragma vera-pushoff
        using namespace std::string_literals;
#pragma vera-pop

        error(lexer, "Bad Scope Closure: Expecting: >"s + oldScope.contName() + "<  Current: >" + topScope.contName() + "< ");
    }

    storage.release(oldScopeId);
    Class*          scopeToClass = dynamic_cast<Class*>(&oldScope); // Could by nullptr as this by Namespace
    if (scopeToClass == nullptr)
    {
        return 0;
    }
    Type&           classToType = dynamic_cast<Type&>(*scopeToClass);
    return storage.add(TypeRef{classToType});
}

Int Semantic::addLiteralString(Lexer& lexer)
{
    std::string objectName = generateAnonNameString();
    Type& stringType = getScopeSymbol<Type>(globalScope, "Std", "String");

    // TODO: Literal needs to be a const
    Scope& literalScope = getScopeSymbol<Namespace>(globalScope, "$Literal");
    Object& literalObject = literalScope.add(std::make_unique<Literal<std::string>>(std::move(objectName), stringType, std::string(lexer.lexem())));

    return storage.add(ObjectRef{literalObject});
}

Int Semantic::createFuncCall(Lexer& /*lexer*/, Int /*obj*/, Int /*param*/)
{
/*
    std::string objectName = generateAnonNameString();
    Type& stringType = getScopeSymbol<Type>(globalScope, "Std", "String");

    // TODO: Literal needs to be a const
    Scope& literalScope = getScopeSymbol<Namespace>(globalScope, "$Literal");
    Object& literalObject = literalScope.add(std::make_unique<Literal<std::string>>(std::move(objectName), stringType, std::string(lexer.lexem())));

    return storage.add(ObjectRef{literalObject});
*/
    return 0;
}
