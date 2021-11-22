#ifndef THORSANVIL_ANVIL_ICE_STORAGE_H
#define THORSANVIL_ANVIL_ICE_STORAGE_H

#include "Action.h"     // Just to get Int

#include <list>
#include <vector>
#include <variant>

namespace ThorsAnvil::Anvil::Ice
{

/*
Anvil:              NamespaceList


NamespaceList:      Namespace
                |   NamespaceList Namespace

Namespace:          NAMESPACE NameSpaceIdentifer '{' DeclListOpt '}'

DeclListOpt:
                |   DeclList
DeclList:           Decl
                |   DeclList Decl

Decl:               => Decl*

ParamListOpt:       new std::list<std::reference_wrapper<Type>>
ParamList:          new std::list<std::reference_wrapper<Type>>

ObjectDecl:         => Type*

AnonDecl:           => Type*

Statement:          Expression ';'
Expression:         ExprFuncCall
ExprFuncCall:       ObjectName '(' ValueListOpt ')'

ValueListOpt:
                |   ValueList
ValueList:          Value
                |   ValueList ',' Value
Value:              ObjectName
                |   Literal

InitObject:         ';'

ObjectName:         ObjectIdentifer
                |   NameSpaceIdentifer SCOPE ObjectName
                |   ObjectIdentifer '.' ObjectName

TypeName:           => Type*
TypeNameFull:       => new std::list<std::unique_ptr<std::string>>

Literal:            => ???

NameSpaceIdentifer: => new std::string
TypeIdentifer:      => new std::string
ObjectIdentifer:    => new std::string

Identifer:          => new std::string
*/

class Type;
class Decl;
class Container;

using Scope = Container;

using TypeRef   = std::reference_wrapper<Type>;
using DeclRef   = std::reference_wrapper<Decl>;
using ScopeRef  = std::reference_wrapper<Scope>;
using ParamList = std::list<TypeRef>;
using FullIdent = std::list<std::string>;

using Data = std::variant<Int, TypeRef, DeclRef, ScopeRef, ParamList, FullIdent, std::string>;

class Storage
{
    std::vector<Data>   data;
    Int                 nextFree;

    public:
        Storage()
            : nextFree(-1)
        {}

        template<typename T>
        Int add(T&& value)
        {
            if (nextFree == -1)
            {
                data.emplace_back(std::forward<T>(value));
                return data.size() - 1;
            }
            Int result = nextFree;
            nextFree = std::get<Int>(data[nextFree]);
            data[result] = std::forward<T>(value);
            return result;
        }

        template<typename T>
        T& get(Int index)
        {
            return std::get<T>(data[index]);
        }

        void release(Int index)
        {
            data[index] = nextFree;
            nextFree = index;
        }
};

}

#endif
