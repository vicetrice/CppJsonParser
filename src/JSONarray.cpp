
#include <string>   //std::string
#include <variant>  //std::variant
#include <vector>   //std::vector
#include <memory>   //std::unique_ptr
#include <iostream> //std::cout
#include "JSONarray.hpp" //JSONarray
#include "JSONobject.hpp" //JSONobject

namespace JsonParserVicetrice
{
    /// Variant type used to return different types of elements from the array.
    using VariantTypeI = std::variant<std::string, int64_t, bool, long double, char, JSONobject *, JSONarray *>;

    /**
     * @brief Adds a non-string value to the array.
     *
     * Adds a value of any type except std::string to the array.
     *
     * @param value The value to add to the array.
     */
    void JSONarray::add_any_except_string(VariantType value) noexcept
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

    /**
     * @brief Retrieves an element from the array by index.
     *
     * Returns the element at the specified index.
     *
     * @param index The index of the element to retrieve.
     * @return The element at the specified index.
     * @throws std::runtime_error If the index is out of range.
     */
    const VariantTypeI JSONarray::consult(size_t index) const
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
            return ptr->get();
        }
        else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&BasicElement[index]))
        {
            return ptr->get();
        }
        else
            return 0;
    }

} // namespace JsonParse
