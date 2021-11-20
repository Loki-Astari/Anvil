#include "Compiler.h"
#include <string>
#include <list>
#include <cctype>

using namespace ThorsAnvil::Anvil::Ice;

Type::~Type()
{}

Compiler::Compiler(std::istream& input, std::ostream& output)
    : Action(output)
    , parser(*this, input, output)
{
    currentScope.push_back(std::ref(static_cast<Scope&>(globalScope)));
}

Compiler::~Compiler()
{
}

bool Compiler::go()
{
    return parser.parse();
}

// Action Override
Int Compiler::identifierCreate(Lexer& lexer)
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
Int Compiler::identifierCheckObject(Lexer& /*lexer*/, Int id)
{
    std::string&    identifier = *reinterpret_cast<std::string*>(id);
    if (std::islower(identifier[0]))
    {
        return id;
    }
    delete &identifier;
    return -1;
}

/*
 * Type names are greater than three characters long,
 * start with an uppercase letter and don't contain underscore.
 */
Int Compiler::identifierCheckType(Lexer& /*lexer*/, Int id)
{
    std::string&    identifier = *reinterpret_cast<std::string*>(id);
    if ((std::isupper(identifier[0])) && (identifier.size() > 3) && (identifier.find('_') == std::string::npos))
    {
        return id;
    }
    delete &identifier;
    return -1;
}

/*
 * Namespace names start with an upper case letter, and each upper case letter is prefixed by underscore.
 * Either the name is shorter than 4 characters or if 4 or more contains at least two capitols.
 */
Int Compiler::identifierCheckNS(Lexer& /*lexer*/, Int id)
{
    std::string&    identifier = *reinterpret_cast<std::string*>(id);

    bool lastUnderscore = true;
    auto findUpperNotPrefixedByUnderScore = [&lastUnderscore](char x)
    {
        if (std::isupper(x) && !lastUnderscore)
            return true;
        lastUnderscore = x == '_';
        return false;
    };

    if ((std::isupper(identifier[0])) && ((identifier.size() <= 3) || (std::find_if(std::begin(identifier) + 1, std::end(identifier), [](char x){return std::isupper(x);}) != identifier.end())))
    {
        auto find = std::find_if(std::begin(identifier), std::end(identifier), findUpperNotPrefixedByUnderScore);
        if (find == std::end(identifier))
            return id;
    }
    delete &identifier;
    return -1;
}

Int Compiler::fullIdentiferCreate(Lexer& /*lexer*/)
{
    return reinterpret_cast<Int>(new std::list<std::unique_ptr<std::string>>{});
}

Int Compiler::fullIdentiferAddPath(Lexer& /*lexer*/, Int fp, Int id)
{
    std::list<std::unique_ptr<std::string>>&    fullPath = *reinterpret_cast<std::list<std::unique_ptr<std::string>>*>(fp);
    std::string*                                identifier = reinterpret_cast<std::string*>(id);
    fullPath.emplace_front(identifier);
    return fp;
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

Int Compiler::findTypeInScope(Lexer& /*lexer*/, Int fp)
{
    std::list<std::unique_ptr<std::string>>&    fullPath = *reinterpret_cast<std::list<std::unique_ptr<std::string>>*>(fp);
    std::cerr << "findTypeInScope: ";
    for (auto const& path: fullPath)
    {
        std::cerr << (*path) << " :: ";
    }
    std::cerr << "\n";
    for (auto const& path: fullPath)
    {
        std::string& pathSeg = *path;
        for (auto& scope: ReversView(currentScope))
        {
            Decl& ref = scope.get().get(pathSeg);
            ((void)ref);
        }
    }
    std::cerr << "Return\n";
    return 0;
}
