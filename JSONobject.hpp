#ifndef __________JSONOBJECT_HPP__________
#define __________JSONOBJECT_HPP__________

#include <unordered_map> // std::unordered_map
#include <string>        // std::string
#include <variant>       // std::variant
#include <memory>        // std::unique_ptr
#include <iostream>      // std::ostream
#include <stdexcept>
#include "JSONarray.hpp"

namespace JsonParserVicetrice
{

    class JSONarray; // Forward declaration

    class JSONobject
    {
    public:
        using VariantType = std::variant<std::string, int64_t, bool, long double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;
        using VariantTypeI = std::variant<std::string, int64_t, bool, long double, char, JSONobject *, JSONarray *>;

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

        const VariantTypeI consult(const std::string &key) const
        {
            auto it = BasicPair.find(key);
            if (it == BasicPair.end())
            {
                throw std::runtime_error("Key not found.");
                        }

            if (auto ptr = std::get_if<std::string>(&BasicPair.at(key)))
            {
                return *ptr;
            }
            else if (auto ptr = std::get_if<bool>(&BasicPair.at(key)))
            {
                return *ptr;
            }
            else if (auto ptr = std::get_if<long double>(&BasicPair.at(key)))
            {
                return *ptr;
            }
            else if (auto ptr = std::get_if<char>(&BasicPair.at(key)))
            {
                return *ptr;
            }
            else if (auto ptr = std::get_if<int64_t>(&BasicPair.at(key)))
            {
                return *ptr;
            }
            if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&BasicPair.at(key)))
            {
                /* std::cout << "{\n";
                 JSONobject *aux = ptr->get();
                 aux->iterate(aux);
                 std::cout << "}" << std::endl;*/
                return ptr->get();
            }
            else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&BasicPair.at(key)))
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
                        else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&pair.second))
                        {
                            std::cout << "[\n";
                            JSONarray *aux = ptr->get();
                            aux->iterate(aux);
                            std::cout << "]" << std::endl;
                        }
                    }
                    std::cout << std::endl;
                }
                */

    private:
        //--------------------- ATTRIBUTES
        std::unordered_map<std::string, VariantType>
            BasicPair; // unordered_map of basic types

    }; // class JSONobject
} // namespace JsonParserVicetrice

#endif // JSONOBJECT_HPP
