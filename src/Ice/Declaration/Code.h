#ifndef THORSANVIL_ANVIL_ICE_DECLARATION_CODE_H
#define THORSANVIL_ANVIL_ICE_DECLARATION_CODE_H

#include "../Utility/View.h"
#include "Decl.h"
#include "Container.h"
#include "Expression.h"

#include <vector>
#include <memory>
#include <ostream>

namespace ThorsAnvil::Anvil::Ice
{

class Statement
{
    public:
        virtual ~Statement() = 0;
        virtual void generateAssembley(std::ostream& output) = 0;
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

        virtual void generateAssembley(std::ostream& output) override
        {
            for (auto const& stat: make_View<Forward>(code))
            {
                stat->generateAssembley(output);
            }
            for (auto const& stat: make_View<Reverse>(codeBack))
            {
                stat->generateAssembley(output);
            }
        }
};


class StatExprFunctionCall: public Statement
{
    Expression const&   object;
    ExpressionList      objectList;
    public:
        StatExprFunctionCall(Expression const& object, ExpressionList&& objectList)
            : object(object)
            , objectList(std::move(objectList))
        {}
        virtual void generateAssembley(std::ostream& output) override
        {
            output << "// TODO Generate function call\n";
        }
};

}

#endif
