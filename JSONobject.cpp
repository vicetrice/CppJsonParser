
#include <unordered_map> // std::unordered_map
#include <string>        // std::string
#include <variant>       // std::variant
#include <memory>        // std::unique_ptr
#include <iostream>      // std::ostream
#include <stdexcept>     //std::runtime_error
#include "JSONarray.hpp" //JSONarray
#include "JSONobject.hpp" //JSONobject

namespace JsonParserVicetrice
{
    /// Variant type used to store different types of elements in the object.
    using VariantType = std::variant<std::string, int64_t, bool, long double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;

    /// Variant type used to return different types of elements from the object.
    using VariantTypeI = std::variant<std::string, int64_t, bool, long double, char, JSONobject *, JSONarray *>;

    /**
     * @brief Adds a value to the object with a given key, excluding strings.
     *
     * Adds a value of any type except std::string to the object. If the value is a std::string,
     * a message is displayed instructing to use the add_string method instead.
     *
     * @param key The key associated with the value to add.
     * @param value The value to add to the object.
     */
    void JSONobject::add_any_except_string(const std::string &key, VariantType value) noexcept
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

    /**
     * @brief Retrieves an element from the object by key.
     *
     * Returns the value associated with the specified key. If the key is not found, an exception is thrown.
     *
     * @param key The key of the element to retrieve.
     * @return The value associated with the specified key.
     * @throws std::runtime_error If the key is not found in the object.
     */
    const VariantTypeI JSONobject::consult(const std::string &key) const
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
            return ptr->get();
        }
        else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&BasicPair.at(key)))
        {

            return ptr->get();
        }
        else
            return 0;
    }

    /**
     * @brief Checks if a key exists in the object.
     *
     * Throws an exception if the key already exists in the object, indicating that duplicate keys are not allowed.
     *
     * @param key The key to check for existence.
     * @throws std::runtime_error If the key is found in the object.
     */
    const void JSONobject::exists(const std::string &key) const
    {
        auto it = BasicPair.find(key);
        if (it != BasicPair.end())
        {
            throw std::runtime_error("Duplicated keys are not allowed.");
        }
    }

} // namespace JsonParserVicetrice
