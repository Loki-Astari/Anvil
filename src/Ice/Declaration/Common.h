#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_COMMON_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_COMMON_H

#include <map>
#include <memory>
#include <string>
#include <list>

namespace ThorsAnvil::Anvil::Ice
{

enum class DeclType {Void, Namespace, Class, Array, Map, Func, Object, CodeBlock, Statement};

class Decl;
class Type;
class Object;

using NameMap       = std::map<std::string, std::unique_ptr<Decl>>;
using NameRef       = NameMap::const_iterator;
using TypeRef       = std::reference_wrapper<Type>;
using TypeCRef      = std::reference_wrapper<Type const>;
using ObjectRef     = std::reference_wrapper<Object>;
using IdentifierList= std::list<std::string>;
using ParamList     = std::list<TypeRef>;

}

#endif
