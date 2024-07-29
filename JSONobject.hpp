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
#ifndef __________VARIANTTYPE__________
#define __________VARIANTTYPE__________
    using VariantType = std::variant<std::string, int64_t, bool, double, char, std::unique_ptr<JsonParserVicetrice::JSONarray>, std::unique_ptr<JsonParserVicetrice::JSONobject>>;
#endif
    class JSONarray; // Forward declaration

    class JSONobject
    {
    public:
        inline JSONobject() = default;
        inline ~JSONobject() = default;

        inline VariantType &operator[](const std::string &key)
        {
            return BasicPair[key];
        }

    private:
        //--------------------- ATTRIBUTES
        std::unordered_map<std::string, VariantType> BasicPair; // unordered_map of basic types
    }; // class JSONobject

} // namespace JsonParserVicetrice

#endif // JSONOBJECT_HPP
