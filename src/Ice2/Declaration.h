#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_H

#include <functional>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <iostream>

namespace ThorsAnvil::Anvil::Ice
{

class Decl;

using Int           = std::size_t;
using MemberStorage = std::map<std::string, std::unique_ptr<Decl>>;
using MemberIndex   = std::map<std::string, std::size_t>;
using NameRef       = MemberStorage::const_iterator;


enum class DeclType {Void, Namespace, Class, Function, Object, ObjectInit, Statement, Expression};
class Decl
{
    public:
        virtual ~Decl() {}
        virtual DeclType declType() const = 0;
        virtual std::string const& declName() const = 0;

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 7;
};


class Scope: public Decl
{
    MemberStorage   members;
    MemberIndex     objectId;
    std::size_t     nextObjectId;
    std::size_t     anonNameCount;
    public:
        Scope()
            : nextObjectId(0)
            , anonNameCount(0)
        {}
        std::pair<bool, NameRef> get(std::string const& name) const;
        template<typename T, typename... Args>
        T& add(Args&&... args);
        virtual bool storeFunctionsInContainer() const {return false;}

        friend std::ostream& operator<<(std::ostream& str, Scope& data)
        {
            str << "\n\n\n===================\n";
            for (auto const& member: data.members)
            {
                str << member.first << " : " << member.second->declName() << "\n";
            }
            str << "===================\n\n\n";
            return str;
        }
        std::string anonName()
        {
            // 64 bit Size: Convert to hex => 16 bit
            // Prefix with dollar => 17bit
            std::string name(17, '$');
            generateHexName(name, anonNameCount);
            ++anonNameCount;
            return name;
        }
    private:
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

};

class NamedScope: public Scope
{
    std::string name;
    std::string fullName;
    public:
        NamedScope(std::string name)
            : name(std::move(name))
        {}
        virtual std::string const& declName() const override {return name;}
        void setPath(std::string path) {fullName = std::move(path);}
};

class Namespace: public NamedScope
{
    public:
        Namespace(std::string name)
            : NamedScope(std::move(name))
        {}
        virtual DeclType declType() const override {return DeclType::Namespace;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 8;
};

class Type: public NamedScope
{
    public:
        using NamedScope::NamedScope;
        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 9;
};

class Void: public Type
{
    public:
        using Type::Type;
        virtual DeclType declType() const override {return DeclType::Void;}

        static constexpr bool valid = false;
        static constexpr Int defaultStorageId = 10;
};

class Class: public Type
{
    public:
        using Type::Type;
        virtual DeclType declType() const override {return DeclType::Class;}
        virtual bool storeFunctionsInContainer() const override {return true;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 11;
};

class Function: public Type
{
    public:
        using Type::Type;
        virtual DeclType declType() const override {return DeclType::Function;}
        virtual bool storeFunctionsInContainer() const override {return true;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 12;
};

class Object: public Decl
{
    std::string     name;
    Type const&     type;
    public:
        Object(std::string name, Type const& type)
            : name(std::move(name))
            , type(type)
        {}
        virtual DeclType declType() const override {return DeclType::Object;}
        virtual std::string const& declName() const override {return name;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 13;
};

class ObjectInit: public Decl
{
    std::string name = "ObjectInit";
    public:
        virtual DeclType declType() const override {return DeclType::ObjectInit;}
        virtual std::string const& declName() const override {return name;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 14;
};

class Statement: public Decl
{
    std::string name = "Statement";
    public:
        virtual DeclType declType() const override {return DeclType::Statement;}
        virtual std::string const& declName() const override {return name;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 15;
};

class Expression: public Decl
{
    std::string name = "Expression";
    public:
        virtual DeclType declType() const override {return DeclType::Expression;}
        virtual std::string const& declName() const override {return name;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 16;
};

using DeclRef           = std::reference_wrapper<Decl>;
using ScopeRef          = std::reference_wrapper<Scope>;
using NamespaceRef      = std::reference_wrapper<Namespace>;
using TypeRef           = std::reference_wrapper<Type>;
using VoidRef           = std::reference_wrapper<Void>;
using ClassRef          = std::reference_wrapper<Class>;
using FunctionRef       = std::reference_wrapper<Function>;
using ObjectRef         = std::reference_wrapper<Object>;
using ObjectInitRef     = std::reference_wrapper<ObjectInit>;
using StatementRef      = std::reference_wrapper<Statement>;
using ExpressionRef     = std::reference_wrapper<Expression>;

using NamespaceDecOrder = std::vector<NamespaceRef>;

#include "Declaration.inl"

}

#endif
