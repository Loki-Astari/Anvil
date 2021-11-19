#include "Compiler.h"
#include <string>
#include <list>

using namespace ThorsAnvil::Anvil::Ice;

Type::~Type()
{}

Compiler::Compiler(std::string const& fileName, bool debug)
    : file(fileName)
    , parser(*this, file)
    , debug(debug)
{
    currentScope.push_back(std::ref(static_cast<Scope&>(globalScope)));
}

Compiler::~Compiler()
{
}

void Compiler::go()
{
    parser.parse();
}

// Action Override
void Compiler::log(char const* msg)
{
    if (debug)
    {
        std::cerr << msg << "\n";
    }
}

Int Compiler::identifierCreate(Lexer& lexer)
{
    return reinterpret_cast<Int>(new std::string(lexer.lexem()));
}

Int Compiler::identifierCheckObject(Lexer& /*lexer*/, Int id)
{
    return id;
}

Int Compiler::identifierCheckType(Lexer& /*lexer*/, Int id)
{
    return id;
}

Int Compiler::identifierCheckNS(Lexer& /*lexer*/, Int id)
{
    return id;
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
    for(auto const& path: fullPath) {
        std::cerr << (*path) << " :: ";
    }
    std::cerr << "\n";
    for(auto const& path: fullPath)
    {
        std::string& pathSeg = *path;
        for(auto& scope: ReversView(currentScope))
        {
            Decl& ref = scope.get().get(pathSeg);
            ((void)ref);
        }
    }
    std::cerr << "Return\n";
    return 0;
}
