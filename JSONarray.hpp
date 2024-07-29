#ifndef __________JSONARRAY_HPP__________
#define __________JSONARRAY_HPP__________

#include <string>  //std::string
#include <variant> //std::variant
#include <vector>  //std::vector
#include <memory>  //std::unique_ptr

namespace JsonParserVicetrice
{

    class JSONobject;

    class JSONarray
    {
    public:
        using VariantType = std::variant<std::string, int64_t, bool, double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;

        inline JSONarray() = default;

        inline ~JSONarray() = default;

        inline VariantType &operator[](size_t index)
        {
            return BasicElement[index];
        }

        inline void add(VariantType &&value)
        {
            BasicElement.push_back(std::move(value));
        }

    private:
        //--------------------- ATTRIBUTES
        std::vector<VariantType> BasicElement; // Array of basic elements
    };

} // namespace JsonParse
#endif
