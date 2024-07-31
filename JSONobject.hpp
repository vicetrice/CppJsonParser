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

        inline void consult(const std::string &key)
        {
            auto it = BasicPair.find(key);
            if (it == BasicPair.end())
            {
                std::cerr << "Key not found: " << key << std::endl;
                return;
            }

            if (auto ptr = std::get_if<std::string>(&BasicPair.at(key)))
            {
                std::cout << key << ": " << *ptr << std::endl;
            }
            else if (auto ptr = std::get_if<bool>(&BasicPair.at(key)))
            {
                std::cout << key << ": " << std::boolalpha << *ptr << std::endl;
            }
            else if (auto ptr = std::get_if<long double>(&BasicPair.at(key)))
            {
                std::cout << key << ": " << *ptr << std::endl;
            }
            else if (auto ptr = std::get_if<char>(&BasicPair.at(key)))
            {
                std::cout << key << ": " << *ptr << std::endl;
            }
            else if (auto ptr = std::get_if<int64_t>(&BasicPair.at(key)))
            {
                std::cout << key << ": " << *ptr << std::endl;
            }
            else if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&BasicPair.at(key)))
            {
                std::cout << key << ": " << "{\n";
                JSONobject *aux = ptr->get();
                iterate(aux);
                std::cout << "}" << std::endl;
            }
        }

        const std::unordered_map<std::string, VariantType> &GetMap() const
        {
            return BasicPair;
        }

    private:
        //--------------------- ATTRIBUTES
        std::unordered_map<std::string, VariantType> BasicPair; // unordered_map of basic types

        //--------------------- PRIVATE METHODS

        void iterate(JSONobject *object) const
        {
            bool comma = false;
            for (const auto &pair : object->GetMap())
            {
                if (!comma)
                {
                    comma = true;
                }
                else
                    std::cout << ", ";
                std::cout << pair.first << ": ";
                if (auto ptr = std::get_if<std::string>(&pair.second))
                {
                    std::cout << *ptr;
                }
                else if (auto ptr = std::get_if<int64_t>(&pair.second))
                {
                    std::cout << *ptr;
                }
                else if (auto ptr = std::get_if<bool>(&pair.second))
                {
                    std::cout << std::boolalpha << *ptr;
                }
                else if (auto ptr = std::get_if<long double>(&pair.second))
                {
                    std::cout << *ptr;
                }
                else if (auto ptr = std::get_if<char>(&pair.second))
                {
                    std::cout << *ptr;
                }
                else if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&pair.second))
                {
                    std::cout << "{\n";
                    JSONobject *aux = ptr->get();
                    iterate(aux);
                    std::cout << "}" << std::endl;
                }
            }
            std::cout << std::endl;
        }
    }; // class JSONobject

} // namespace JsonParserVicetrice

#endif // JSONOBJECT_HPP
