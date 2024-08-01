#ifndef __________JSON_HPP__________
#define __________JSON_HPP__________

#include <variant>        // std::variant
#include "JSONstruct.hpp" // JSONstruct
#include "JSONarray.hpp"  // JSONarray
#include "Parser.hpp"     // Parser
#include <cstddef>        // size_t
#include <stdexcept>      // std::runtime_error
#include <fstream>        // std::ifstream
#include <string>         // std::string

namespace JsonParserVicetrice
{
    class JSON
    {
    public:
        /// Variant type used to store different types of JSON elements.
        using VariantTypeI = std::variant<std::string, int64_t, bool, long double, char, JSONobject *, JSONarray *>;

        /**
         * @brief Default constructor.
         *
         * Constructs an empty JSON object.
         */
        JSON();

        /**
         * @brief Destructor.
         *
         * Cleans up any allocated resources.
         */
        ~JSON();

        /**
         * @brief Parses a JSON string.
         *
         * Parses the given JSON string and constructs the internal representation.
         *
         * @param input The JSON string to parse.
         * @throws runtime_error if syntax error.
         */
        void parse(const std::string &input);

        /**
         * @brief Parses a JSON input stream.
         *
         * Parses the JSON content from the given input stream and constructs the internal representation.
         *
         * @param input The input stream to parse.
         * @throws runtime_error if syntax error.
         */
        void parse(std::ifstream &input);

        /**
         * @brief Retrieves the JSON element as a specific type.
         *
         * Template method to retrieve the stored JSON element as the specified type.
         *
         * @tparam T The type to retrieve.
         * @return The JSON element as the specified type.
         * @throws std::runtime_error If the stored element is not of the requested type.
         */
        template <typename T>
        T get()
        {
            T resultado;
            try
            {
                if (auto ptr = std::get_if<std::string>(&element))
                {
                    if constexpr (std::is_same_v<T, std::string>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }
                else if (auto ptr = std::get_if<int64_t>(&element))
                {
                    if constexpr (std::is_same_v<T, int64_t>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }
                else if (auto ptr = std::get_if<long double>(&element))
                {
                    if constexpr (std::is_same_v<T, long double>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }
                else if (auto ptr = std::get_if<char>(&element))
                {
                    if constexpr (std::is_same_v<T, char>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }
                else if (auto ptr = std::get_if<bool>(&element))
                {
                    if constexpr (std::is_same_v<T, bool>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }
                else
                {
                    throw std::runtime_error("Type to be storaged must be primitive.");
                }
            }
            catch (const std::exception &e)
            {
                throw;
            }
            start.consultIni(head);
            return resultado;
        }

        /**
         * @brief Accesses a JSON object element by key.
         *
         * Overloads the subscript operator to access elements of a JSON object by key.
         *
         * @param index The key of the element to access.
         * @return A reference to the JSON element.
         * @throws std::runtime_error If the element does not exist or is not an object.
         */
        JSON &operator[](const std::string &index);

        /**
         * @brief Accesses a JSON array element by index.
         *
         * Overloads the subscript operator to access elements of a JSON array by index.
         *
         * @param index The index of the element to access.
         * @return A reference to the JSON element.
         * @throws std::runtime_error If the element does not exist or is not an array.
         */
        JSON &operator[](size_t index);

    private:
        //--------------------- ATTRIBUTES
        /// Variant type used to store pointers to JSON objects or arrays.
        using VariantPtr = std::variant<JSONobject *, JSONarray *>;

        /// The starting structure of the JSON document.
        JSONstruct start;

        /// Pointer to the head of the JSON document (either an object or an array).
        VariantPtr head;

        /// The current JSON element.
        VariantTypeI element;

    }; // class JSON
} // namespace JsonParserVicetrice

#endif
