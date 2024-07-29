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
        using VariantType = std::variant<std::string, int64_t, bool, double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;

        inline JSONobject() = default;
        inline ~JSONobject() = default;

        inline VariantType &operator[](const std::string &key)
        {
            return BasicPair[key];
        }

        inline void add(const std::string &key, VariantType &&value)
        {
            BasicPair[key] = std::move(value);
        }

        inline const VariantType &get(const std::string &key) const
        {
            auto it = BasicPair.find(key);
            if (it != BasicPair.end())
            {
                return it->second;
            }
            throw std::runtime_error("Key not found: " + key);
        }

        // Public getter for BasicPair
        inline const std::unordered_map<std::string, VariantType> &getBasicPair() const
        {
            return BasicPair;
        }

    private:
        //--------------------- ATTRIBUTES
        std::unordered_map<std::string, VariantType> BasicPair; // unordered_map of basic types

    }; // class JSONobject

} // namespace JsonParserVicetrice

#endif // JSONOBJECT_HPP
