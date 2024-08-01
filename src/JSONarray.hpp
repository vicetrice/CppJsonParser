#ifndef __________JSONARRAY_HPP__________
#define __________JSONARRAY_HPP__________

#include <string>   // std::string
#include <variant>  // std::variant
#include <vector>   // std::vector
#include <memory>   // std::unique_ptr
#include <iostream> //std::cout

namespace JsonParserVicetrice
{

    class JSONobject;

    class JSONarray
    {
    public:
        /// Variant type used to return different types of elements from the array.
        using VariantTypeI = std::variant<std::string, int64_t, bool, long double, char, JSONobject *, JSONarray *>;
        /// Variant type used to store different types of elements in the array.
        using VariantType = std::variant<std::string, int64_t, bool, long double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;

        /**
         * @brief Default constructor.
         *
         * Constructs an empty JSON array.
         */
        inline JSONarray() = default;

        /**
         * @brief Default destructor.
         *
         * Cleans up any allocated resources.
         */
        inline ~JSONarray() = default;

        /**
         * @brief Adds a non-string value to the array.
         *
         * Adds a value of any type except std::string to the array.
         *
         * @param value The value to add to the array.
         */
        void add_any_except_string(VariantType value) noexcept;

        /**
         * @brief Adds a string to the array.
         *
         * Adds a std::string to the array.
         *
         * @param str The string to add to the array.
         */
        inline void add_string(const std::string &str) noexcept
        {
            BasicElement.push_back(str);
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
        const VariantTypeI consult(size_t index) const;

        /**
         * @brief Consult the size of the JSONarray
         * @return Size of JSONarray
         */
        inline size_t JSONarray::size() const
        {
            return BasicElement.size();
        }

    private:
        //--------------------- ATTRIBUTES

        /// Vector to store the elements of the JSON array.
        std::vector<VariantType> BasicElement;
    };

} // namespace JsonParserVicetrice

#endif
