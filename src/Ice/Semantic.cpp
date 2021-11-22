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
Int Semantic::identifierCreate(Lexer& lexer)
{
    return reinterpret_cast<Int>(new std::string(lexer.lexem()));
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
    std::unique_ptr<std::string>    identifier(reinterpret_cast<std::string*>(id));
    if (std::islower((*identifier)[0]))
    {
        identifier.release();
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
    std::unique_ptr<std::string>    identifier(reinterpret_cast<std::string*>(id));
    if ((std::isupper((*identifier)[0])) && (identifier->size() > 3) && (identifier->find('_') == std::string::npos))
    {
        identifier.release();
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
    std::unique_ptr<std::string>    identifier(reinterpret_cast<std::string*>(id));

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

    if ((std::isupper((*identifier)[0])) && ((identifier->size() <= 3) || (std::find_if(std::begin(*identifier) + 1, std::end(*identifier), [](char x){return std::isupper(x);}) != identifier->end())))
    {
        auto find = std::find_if(std::begin(*identifier), std::end(*identifier), findUpperNotPrefixedByUnderScore);
        if (find == std::end(*identifier))
        {
            identifier.release();
            return id;
        }
    }
    error(lexer, "Invalid Identifier for Namespace");
}

Int Semantic::fullIdentiferCreate(Lexer& /*lexer*/)
{
    return reinterpret_cast<Int>(new std::list<std::unique_ptr<std::string>>{});
}

Int Semantic::fullIdentiferAddPath(Lexer& /*lexer*/, Int fp, Int id)
{
    std::unique_ptr<std::list<std::unique_ptr<std::string>>>    fullPath(reinterpret_cast<std::list<std::unique_ptr<std::string>>*>(fp));
    std::unique_ptr<std::string>                                identifier(reinterpret_cast<std::string*>(id));
    fullPath->emplace_front(std::move(identifier));
    fullPath.release();
    return fp;
}

Int Semantic::paramListCreate(Lexer& /*lexer*/)
{
    return reinterpret_cast<Int>(new ParamList);
}

Int Semantic::paramListAdd(Lexer& /*lexer*/, Int pl, Int type)
{
    std::unique_ptr<ParamList>  paramList(reinterpret_cast<ParamList*>(pl));
    Type&                       typeInfo(*reinterpret_cast<Type*>(type));
    paramList->emplace_back(typeInfo);
    paramList.release();
    return pl;
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

Int Semantic::findTypeInScope(Lexer& lexer, Int fp)
{
    std::unique_ptr<std::list<std::unique_ptr<std::string>>>    fullPath(reinterpret_cast<std::list<std::unique_ptr<std::string>>*>(fp));

    // Strings used to build error message if needed.
    std::string  fullPathString;
    std::string  partialMatch;

    // The object we are trying to find.
    Decl*   decl = nullptr;

    // Special case element in the path.
    bool    rootOfPath = true;

    // Loop over all members of the fullPath.
    for (auto& path: *fullPath)
    {
        // For ease of use get a reference to the string rather than using the std::unique_ptr
        std::string const& pathSeg = *path;

        // Each time through the loop build the fullPAthString.
        // To get a good error message we must loop over all the path members
        // and not break out of the loop early.
        fullPathString += (pathSeg + "::");

        // The first time through the loop (for the root element of the type identifier only)
        if (rootOfPath)
        {
            // search the scopes from the current one back to the global scope to see
            // if we can can find the type identifier root element.
            for (auto& scopeRef: ReversView(currentScope))
            {
                // For ease of use.
                auto& scope = scopeRef.get();

                // Search the current scope of the first path segment.
                auto find = scope.get(pathSeg);
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
                    partialMatch += (pathSeg + "::");

                    // So far so we good we have found the first part decl that matches the fullPath.
                    decl = &(*(find.second->second));
                    break;
                }
            }
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
        // decl inside this that matches the next part of the fullPath
        auto find = decl->find(pathSeg);
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
    return reinterpret_cast<Int>(dynamic_cast<Type*>(decl));
}

Int Semantic::scopeAddNamespace(Lexer& /*lexer*/, Int name)
{
    std::unique_ptr<std::string> namespaceName(reinterpret_cast<std::string*>(name));
    Scope& topScope = currentScope.back().get();
    Namespace& newNameSpace = topScope.add(std::make_unique<Namespace>(*namespaceName));
    currentScope.emplace_back(newNameSpace);

    return reinterpret_cast<Int>(&dynamic_cast<Scope&>(newNameSpace));
}

Int Semantic::scopeAddClass(Lexer& /*lexer*/, Int name)
{
    std::unique_ptr<std::string> className(reinterpret_cast<std::string*>(name));
    Scope& topScope = currentScope.back().get();
    Class& newClass = topScope.add(std::make_unique<Class>(*className));
    currentScope.emplace_back(newClass);

    return reinterpret_cast<Int>(&dynamic_cast<Scope&>(newClass));
}

Int Semantic::scopeAddArray(Lexer& /*lexer*/, Int name, Int type)
{
    std::unique_ptr<std::string>    arrayName(reinterpret_cast<std::string*>(name));
    Type&                           typeInfo(*reinterpret_cast<Type*>(type));

    Scope& topScope = currentScope.back().get();
    Array& newArray = topScope.add(std::make_unique<Array>(*arrayName, typeInfo));

    return reinterpret_cast<Int>(&dynamic_cast<Decl&>(newArray));
}

Int Semantic::scopeAddMap(Lexer& /*lexer*/, Int name, Int key, Int value)
{
    std::unique_ptr<std::string>    mapName(reinterpret_cast<std::string*>(name));
    Type&                           keyInfo(*reinterpret_cast<Type*>(key));
    Type&                           valueInfo(*reinterpret_cast<Type*>(value));

    Scope& topScope = currentScope.back().get();
    Map& newMap = topScope.add(std::make_unique<Map>(*mapName, keyInfo, valueInfo));

    return reinterpret_cast<Int>(&dynamic_cast<Decl&>(newMap));
}

Int Semantic::scopeAddFunc(Lexer& /*lexer*/, Int name, Int pl, Int ret)
{
    std::unique_ptr<std::string>    funcName(reinterpret_cast<std::string*>(name));
    std::unique_ptr<ParamList>      paramList(reinterpret_cast<ParamList*>(pl));
    Type&                           retInfo(*reinterpret_cast<Type*>(ret));

    Scope& topScope = currentScope.back().get();
    Func& newFunc = topScope.add(std::make_unique<Func>(*funcName, *paramList, retInfo));

    return reinterpret_cast<Int>(&dynamic_cast<Decl&>(newFunc));
}

Int Semantic::scopeClose(Lexer& lexer, Int oldScopeId)
{
    Scope& topScope = currentScope.back().get();
    currentScope.pop_back();
    Int topScopeId = reinterpret_cast<Int>(&topScope);
    if (topScopeId != oldScopeId)
    {
#pragma vera-pushoff
        using namespace std::string_literals;
#pragma vera-pop
        Scope& oldScope = *reinterpret_cast<Scope*>(oldScopeId);
        error(lexer, "Bad Scope Closure: Expecting: >"s + oldScope.contName() + "<  Current: >" + topScope.contName() + "< ");
    }
    return 0;
}
