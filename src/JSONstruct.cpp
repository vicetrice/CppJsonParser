#include "JSONstruct.hpp"
#include <iostream>

namespace JsonParserVicetrice
{

    JSONstruct::JSONstruct() : noFirst{false} {}

    JSONstruct::~JSONstruct() = default;

    /**
     * @brief add elements to the final structure
     * @param token token that wil be stored in the final structure
     */
    void JSONstruct::add(const Token &token)
    {
        JSONobject *ptrObj;
        JSONarray *ptrArr;

        switch (token.Type)
        {
        case TokenType::LEFT_BRACE:

            if (!noFirst)
            {
                finalStruct = std::make_unique<JSONobject>();
                if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&finalStruct))
                {
                    position.push_back(ptr->get());
                    noFirst = true;
                }
            }
            else
            {

                std::unique_ptr<JSONobject> ptr_to_struct = std::make_unique<JSONobject>();

                if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                {
                    ptrObj = *ptr;
                    position.push_back(ptr_to_struct.get());
                    ptrObj->add_any_except_string(key, std::move(ptr_to_struct));
                }
                else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                {
                    ptrArr = *ptr;
                    position.push_back(ptr_to_struct.get());
                    ptrArr->add_any_except_string(std::move(ptr_to_struct));
                }
            }

            break;
        case TokenType::LEFT_BRACKET:
            if (!noFirst)
            {
                finalStruct = std::make_unique<JSONarray>();
                if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&finalStruct))
                {
                    position.push_back(ptr->get());
                    noFirst = true;
                }
                // std::cout << "pase por aqui 2" << '\n';
            }
            else
            {
                std::unique_ptr<JSONarray> ptr_to_struct = std::make_unique<JSONarray>();

                if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                {
                    ptrObj = *ptr;
                    position.push_back(ptr_to_struct.get());
                    ptrObj->add_any_except_string(key, std::move(ptr_to_struct));
                }
                else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                {
                    ptrArr = *ptr;
                    position.push_back(ptr_to_struct.get());
                    ptrArr->add_any_except_string(std::move(ptr_to_struct));
                }
            }
            break;

        case TokenType::RIGHT_BRACE:
        case TokenType::RIGHT_BRACKET:
            position.pop_back();
            break;
        case TokenType::DOUBLE:
            // std::cout << "pase por aqui DOUBLE con: " << token.Value << std::endl;

            if (noFirst)
            {

                if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                {
                    ptrObj = *ptr;
                    ptrObj->add_any_except_string(key, std::stold(token.Value));
                }
                else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                {
                    ptrArr = *ptr;
                    ptrArr->add_any_except_string(std::stold(token.Value));
                }
            }
            break;
        case TokenType::NUMBER:

            if (noFirst)
            {
                if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                {
                    ptrObj = *ptr;
                    ptrObj->add_any_except_string(key, std::stoll(token.Value));
                }
                else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                {
                    ptrArr = *ptr;
                    ptrArr->add_any_except_string(std::stoll(token.Value));
                }
            }

            break;
        case TokenType::BOOLEAN:

            if (noFirst)
            {
                bool booli = token.Value == "true" ? true : false;
                if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                {
                    ptrObj = *ptr;
                    ptrObj->add_any_except_string(key, booli);
                }
                else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                {
                    ptrArr = *ptr;
                    ptrArr->add_any_except_string(booli);
                }
            }

            break;
        case TokenType::STRING:

            if (noFirst)
            {
                if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                {
                    ptrObj = *ptr;
                    ptrObj->add_string(key, token.Value);
                }
                else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                {
                    ptrArr = *ptr;
                    ptrArr->add_string(token.Value);
                }
            }

            break;
        case TokenType::NUL:
            if (noFirst)
            {
                if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                {
                    ptrObj = *ptr;
                    ptrObj->add_any_except_string(key, '\0');
                }
                else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                {
                    ptrArr = *ptr;
                    ptrArr->add_any_except_string('\0');
                }
            }

            break;

        case TokenType::KEY:
            if (auto ptr = std::get_if<JSONobject *>(&position.back()))
            {
                ptrObj = *ptr;
                ptrObj->exists(token.Value);
            }
            key = token.Value;

            break;
        default:
            break;
        }
    }

}; // namespace JsonParserVicetrice
