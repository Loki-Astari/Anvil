#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_CODE_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_CODE_H

#include "../Utility/View.h"
#include "Decl.h"
#include "Objects.h"
#include "Container.h"

#include <vector>
#include <memory>
#include <ostream>

namespace ThorsAnvil::Anvil::Ice
{

class Statement
{
    public:
        virtual ~Statement() = 0;
};

inline Statement::~Statement() {}

class CodeBlock: public DeclContainer<Decl>, public Statement
{
    std::vector<std::unique_ptr<Statement>>     code;
    std::vector<std::unique_ptr<Statement>>     codeBack;
    public:
        using DeclContainer<Decl>::DeclContainer;
        virtual DeclType declType() const override                                          {return DeclType::CodeBlock;}

        template<bool B, typename T, typename... Args>
        Statement& addCode(Args&&... args)
        {
            if constexpr (B)
            {
                code.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
                return *code.back();
            }
            else
            {
                codeBack.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
                return *codeBack.back();
            }
        }
};


class StatExprFunctionCall: public Statement
{
    ObjectId        object;
    ObjectIdList    objectList;
    public:
        StatExprFunctionCall(ObjectId const& object, ObjectIdList&& objectList)
            : object(std::move(object))
            , objectList(std::move(objectList))
        {}
};

}

#endif
