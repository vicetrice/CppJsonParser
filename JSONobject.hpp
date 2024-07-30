#ifndef __________JSONOBJECT_HPP__________
#define __________JSONOBJECT_HPP__________

#include <unordered_map> // std::unordered_map
#include <string>        // std::string
#include <variant>       // std::variant
#include <memory>        // std::unique_ptr
#include <iostream>      // std::ostream
#include <stdexcept>

namespace JsonParserVicetrice
{

    class JSONarray; // Forward declaration

    class JSONobject
    {
    public:
        using VariantType = std::variant<std::string, int64_t, bool, long double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;

        inline JSONobject() = default;
        inline ~JSONobject() = default;

        inline void add_any_except_string(const std::string &key, VariantType value)
        {
            if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&value))
            {
                BasicPair[key] = std::move(*ptr);
            }
            else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&value))
            {
                BasicPair[key] = std::move(*ptr);
            }
            else if (std::holds_alternative<std::string>(value))
            {
                std::cout << "Use add_string method" << std::endl;
            }
            else if (auto ptr = std::get_if<char>(&value))
            {
                BasicPair[key] = *ptr;
            }
            else if (auto ptr = std::get_if<long double>(&value))
            {
                BasicPair[key] = *ptr;
            }
            else if (auto ptr = std::get_if<bool>(&value))
            {
                BasicPair[key] = *ptr;
            }
            else if (auto ptr = std::get_if<int64_t>(&value))
            {
                BasicPair[key] = *ptr;
            }
        }

        inline void add_string(const std::string &key, const std::string &str)
        {
            BasicPair[key] = str;
        }

    private:
        //--------------------- ATTRIBUTES
        std::unordered_map<std::string, VariantType> BasicPair; // unordered_map of basic types

    }; // class JSONobject

} // namespace JsonParserVicetrice

#endif // JSONOBJECT_HPP
