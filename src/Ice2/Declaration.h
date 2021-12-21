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


enum class DeclType {Void, Namespace, Class};
class Decl
{
    public:
        virtual ~Decl() {}
        virtual DeclType declType() const = 0;
        virtual std::string const& declName() const = 0;

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 3;
};


class Scope: public Decl
{
    MemberStorage   members;
    MemberIndex     objectId;
    std::size_t     nextObjectId;
    public:
        Scope()
            : nextObjectId(0)
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
        static constexpr Int defaultStorageId = 4;
};

class Void: public Scope
{
    public:
        virtual DeclType declType() const override {return DeclType::Void;}

        static constexpr bool valid = false;
};

class Class: public NamedScope
{
    public:
        Class(std::string name)
            : NamedScope(std::move(name))
        {}
        virtual DeclType declType() const override {return DeclType::Class;}
        virtual bool storeFunctionsInContainer() const override {return true;}

        static constexpr bool valid = true;
        static constexpr Int defaultStorageId = 5;
};

using DeclRef           = std::reference_wrapper<Decl>;
using ScopeRef          = std::reference_wrapper<Scope>;
using NamespaceRef      = std::reference_wrapper<Namespace>;
using ClassRef          = std::reference_wrapper<Class>;

using NamespaceDecOrder = std::vector<NamespaceRef>;

#include "Declaration.inl"

}

#endif
