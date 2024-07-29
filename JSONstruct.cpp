#include "JSONstruct.hpp"

namespace JsonParserVicetrice
{

    JSONstruct::JSONstruct() = default;

    JSONstruct::~JSONstruct() = default;

    /**
     * @brief add elements to the final structure
     * @param token token that wil be stored in the final structure
     */
    void JSONstruct::add(const Token &token)
    {
        JSONobject *ptrObj;
        JSONarray *ptrArr;
        int64_t i;
        double d;
        bool out;
        std::string str;
        char car;

        switch (token.Type)
        {
        case TokenType::LEFT_BRACE:

            if (noFirst)
            {
                finalStruct = std::make_unique<JSONobject>();
                auto *ptr = std::get_if<std::unique_ptr<JSONobject>>(&finalStruct);
                position.push_back(ptr->get());
                noFirst = true;
            }
            else
            {
                std::unique_ptr<JSONobject> ptr = std::make_unique<JSONobject>();
                position.push_back(ptr.get());
                ptrObj = std::get<JSONobject *>(position.back());
                ptrObj->add(key, std::move(ptr));
            }

            break;
        case TokenType::LEFT_BRACKET:
            if (noFirst)
            {
                finalStruct = std::make_unique<JSONarray>();
                auto *ptr = std::get_if<std::unique_ptr<JSONarray>>(&finalStruct);
                position.push_back(ptr->get());
                noFirst = true;
            }
            else
            {
                std::unique_ptr<JSONarray> ptr = std::make_unique<JSONarray>();
                position.push_back(ptr.get());
                ptrArr = std::get<JSONarray *>(position.back());
                ptrArr->add(std::move(ptr));
            }
            break;

        case TokenType::RIGHT_BRACE:
        case TokenType::RIGHT_BRACKET:
            position.pop_back();
            break;
        case TokenType::DOUBLE:

            d = std::stod(token.Value);
            if (noFirst)
            {
                std::visit([&](auto &ptr)
                           {
                    using T = std::decay_t<decltype(ptr)>;
                    if constexpr (std::is_same_v<T, JSONobject *>)
                    {
                        ptr->add(key, d);
                    }
                    else if constexpr (std::is_same_v<T, JSONarray *>)
                    {
                        ptr->add(d);
                    } }, position.back());
            }

            break;
        case TokenType::NUMBER:

            i = std::stoll(token.Value);

            if (noFirst)
            {
                std::visit([&](auto &ptr)
                           {
                    using T = std::decay_t<decltype(ptr)>;
                    if constexpr (std::is_same_v<T, JSONobject *>)
                    {
                        ptr->add(key, i);
                    }
                    else if constexpr (std::is_same_v<T, JSONarray *>)
                    {
                        ptr->add(i);
                    } }, position.back());
            }

            break;
        case TokenType::BOOLEAN:

            out = token.Value == "true" ? true : false;
            if (noFirst)
            {
                std::visit([&](auto &ptr)
                           {
                    using T = std::decay_t<decltype(ptr)>;
                    if constexpr (std::is_same_v<T, JSONobject *>)
                    {
                        ptr->add(key, out);
                    }
                    else if constexpr (std::is_same_v<T, JSONarray *>)
                    {
                        ptr->add(out);
                    } }, position.back());
            }

            break;
        case TokenType::STRING:

            str = token.Value;
            if (noFirst)
            {
                std::visit([&](auto &ptr)
                           {
                    using T = std::decay_t<decltype(ptr)>;
                    if constexpr (std::is_same_v<T, JSONobject *>)
                    {
                        ptr->add(key, std::move(str));
                    }
                    else if constexpr (std::is_same_v<T, JSONarray *>)
                    {
                        ptr->add(std::move(str));
                    } }, position.back());
            }

            break;
        case TokenType::NUL:
            car = '\0';
            if (noFirst)
            {
                std::visit([&](auto &ptr)
                           {
                    using T = std::decay_t<decltype(ptr)>;
                    if constexpr (std::is_same_v<T, JSONobject *>)
                    {
                        ptr->add(key, car);
                    }
                    else if constexpr (std::is_same_v<T, JSONarray *>)
                    {
                        ptr->add(car);
                    } }, position.back());
            }

            break;

        case TokenType::KEY:
            key = token.Value;

            break;
        default:
            break;
        }
    }

}; // namespace JsonParserVicetrice
