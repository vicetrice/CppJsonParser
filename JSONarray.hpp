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

    private:
        //--------------------- ATTRIBUTES
        std::vector<VariantType> BasicElement; // Array of basic elements
    };

} // namespace JsonParse
#endif
