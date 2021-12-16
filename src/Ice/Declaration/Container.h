#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_CONTAINER_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_CONTAINER_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_H

#include "Common.h"

#include <map>
#include <utility>
#include <memory>
#include <string>
#include <ostream>


namespace ThorsAnvil::Anvil::Ice
{

class Decl;
class Container
{
    public:
        using NameMap = std::map<std::string, std::unique_ptr<Decl>>;
        using ObjectMap = std::map<std::string, std::size_t>;

        template<typename T, typename... Args>
        T& add(Args&&... args);
        virtual std::string const& contName() const = 0;
        std::pair<bool, NameRef> get(std::string const& name) const;

        std::size_t size() const {return nextObjectId;}

        virtual bool storeFunctionsInContainer() const {return false;}
    private:
        NameMap     names;
        ObjectMap   objectId;
        std::size_t nextObjectId = 0;


        friend std::ostream& operator<<(std::ostream& stream, Container const& cont)
        {
            stream << "Container: " << cont.contName() << "{\n";
            for (auto const& value: cont.names)
            {
                stream << "\t" << value.first << ":\n";
            }
            stream << "}\n";
            return stream;
        }
};

template<typename D>
class DeclContainer: public D, public Container
{
    public:
        DeclContainer(std::string const& name)
            : D(name)
        {}
        virtual std::string const& contName() const override                            {return D::declName();}
        virtual std::pair<bool, NameRef> find(std::string const& name) const override   {return get(name);}
};

using Scope     = Container;
using ScopeRef  = std::reference_wrapper<Scope>;

}

#endif
