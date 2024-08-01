#ifndef __________JSONOBJECT_HPP__________
#define __________JSONOBJECT_HPP__________

#include <unordered_map> // std::unordered_map
#include <string>        // std::string
#include <variant>       // std::variant
#include <memory>        // std::unique_ptr
#include <iostream>      // std::ostream
#include <stdexcept>     // std::runtime_error
#include "JSONarray.hpp"

namespace JsonParserVicetrice
{

    class JSONarray;

    class JSONobject
    {
    public:
        /// Variant type used to store different types of elements in the object.
        using VariantType = std::variant<std::string, int64_t, bool, long double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;

        /// Variant type used to return different types of elements from the object.
        using VariantTypeI = std::variant<std::string, int64_t, bool, long double, char, JSONobject *, JSONarray *>;

        /**
         * @brief Default constructor.
         *
         * Constructs an empty JSON object.
         */
        inline JSONobject() = default;

        /**
         * @brief Default destructor.
         *
         * Cleans up any allocated resources.
         */
        inline ~JSONobject() = default;

        /**
         * @brief Adds a value to the object with a given key, excluding strings.
         *
         * Adds a value of any type except std::string to the object. If the value is a std::string,
         * a message is displayed instructing to use the add_string method instead.
         *
         * @param key The key associated with the value to add.
         * @param value The value to add to the object.
         */
        void add_any_except_string(const std::string &key, VariantType value) noexcept;

        /**
         * @brief Adds a string to the object with a given key.
         *
         * Adds a std::string value to the object.
         *
         * @param key The key associated with the string to add.
         * @param str The string to add to the object.
         */
        inline void add_string(const std::string &key, const std::string &str) noexcept
        {
            BasicPair[key] = str;
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
        const VariantTypeI consult(const std::string &key) const;

        /**
         * @brief Checks if a key exists in the object.
         *
         * Throws an exception if the key already exists in the object, indicating that duplicate keys are not allowed.
         *
         * @param key The key to check for existence.
         * @throws std::runtime_error If the key is found in the object.
         */
         void exists(const std::string &key) const;

    private:
        //--------------------- ATTRIBUTES
        
        /// Unordered map to store key-value pairs of basic types.
        std::unordered_map<std::string, VariantType> BasicPair;
    }; // class JSONobject
} // namespace JsonParserVicetrice

#endif
