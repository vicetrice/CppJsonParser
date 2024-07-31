#ifndef __________JSONARRAY_HPP__________
#define __________JSONARRAY_HPP__________

#include <string>  //std::string
#include <variant> //std::variant
#include <vector>  //std::vector
#include <memory>  //std::unique_ptr
#include <iostream>

namespace JsonParserVicetrice
{

    class JSONobject;

    class JSONarray
    {
    public:
        using VariantType = std::variant<std::string, int64_t, bool, long double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;
        using VariantTypeI = std::variant<std::string, int64_t, bool, long double, char, JSONobject *, JSONarray *>;

        inline JSONarray() = default;

        inline ~JSONarray() = default;

        inline void add_any_except_string(VariantType value)
        {
            if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&value))
            {
                BasicElement.push_back(std::move(*ptr));
            }
            else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&value))
            {
                BasicElement.push_back(std::move(*ptr));
            }
            else if (std::holds_alternative<std::string>(value))
            {
                std::cout << "Use add_string method" << std::endl;
            }
            else if (auto ptr = std::get_if<char>(&value))
            {
                BasicElement.push_back(*ptr);
            }
            else if (auto ptr = std::get_if<long double>(&value))
            {
                BasicElement.push_back(*ptr);
            }
            else if (auto ptr = std::get_if<bool>(&value))
            {
                BasicElement.push_back(*ptr);
            }
            else if (auto ptr = std::get_if<int64_t>(&value))
            {
                BasicElement.push_back(*ptr);
            }
        }

        inline void add_string(const std::string &str)
        {
            BasicElement.push_back(str);
        }

        const VariantTypeI consult(size_t index) const
        {

            if (index > BasicElement.size() || index < 0)
            {
                throw std::runtime_error("Index not available.");
            }

            if (auto ptr = std::get_if<std::string>(&BasicElement[index]))
            {
                return *ptr;
            }
            else if (auto ptr = std::get_if<bool>(&BasicElement[index]))
            {
                return *ptr;
            }
            else if (auto ptr = std::get_if<long double>(&BasicElement[index]))
            {
                return *ptr;
            }
            else if (auto ptr = std::get_if<char>(&BasicElement[index]))
            {
                return *ptr;
            }
            else if (auto ptr = std::get_if<int64_t>(&BasicElement[index]))
            {
                return *ptr;
            }
            if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&BasicElement[index]))
            {
                /* std::cout << "{\n";
                 JSONobject *aux = ptr->get();
                 aux->iterate(aux);
                 std::cout << "}" << std::endl;*/
                return ptr->get();
            }
            else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&BasicElement[index]))
            {
                /*std::cout << "[\n";
                JSONarray *aux = ptr->get();
                iterate(aux);
                std::cout << "]" << std::endl;*/
                return ptr->get();
            }
            else
                return 0;
        }
        /*
        const std::vector<VariantType> &
        GetVector() const
        {
            return BasicElement;
        }

        JSONobject *iterate(JSONarray *array) const
        {
            bool comma = false;
            for (const auto &element : array->GetVector())
            {
                if (!comma)
                {
                    comma = true;
                }
                else
                    std::cout << ", ";
                if (auto ptr = std::get_if<std::string>(&element))
                {
                    std::cout << *ptr;
                }
                else if (auto ptr = std::get_if<int64_t>(&element))
                {
                    std::cout << *ptr;
                }
                else if (auto ptr = std::get_if<bool>(&element))
                {
                    std::cout << std::boolalpha << *ptr;
                }
                else if (auto ptr = std::get_if<long double>(&element))
                {
                    std::cout << *ptr;
                }
                else if (auto ptr = std::get_if<char>(&element))
                {
                    std::cout << *ptr;
                }
                else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&element))
                {
                    std::cout << "[\n";
                    JSONarray *aux = ptr->get();
                    iterate(aux);
                    std::cout << "]" << std::endl;
                }
                else if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&element))
                {
                    return ptr->get();
                }
            }
            std::cout << std::endl;
            return nullptr;
        }
        */

    private:
        //--------------------- ATTRIBUTES
        std::vector<VariantType> BasicElement; // Array of basic elements
    };

} // namespace JsonParse
#endif
