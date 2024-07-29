#ifndef __________JSONARRAY_HPP__________
#define __________JSONARRAY_HPP__________

#include <string>  //std::string
#include <variant> //std::variant
#include <vector>  //std::vector

namespace JsonParserVicetrice
{
#ifndef __________VARIANTTYPE__________
#define __________VARIANTTYPE__________
    using VariantType = std::variant<std::string, int64_t, bool, double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;
#endif
    class JSONobject;

    class JSONarray
    {
    public:
        inline JSONarray() = default;

        inline ~JSONarray() = default;

        inline VariantType &operator[](size_t index)
        {
            return BasicElement[index];
        }

    private:
        //--------------------- ATTRIBUTES
        std::vector<VariantType> BasicElement; // Array of basic elements
    };

} // namespace JsonParse
#endif
