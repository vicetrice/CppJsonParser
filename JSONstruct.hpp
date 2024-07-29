#ifndef __________JSONSTRUCT_HPP__________
#define __________JSONSTRUCT_HPP__________

#include <variant>        //std::variant
#include "Token.hpp"      //Token
#include "JSONarray.hpp"  //JSONarray
#include "JSONobject.hpp" //JSONobject

namespace JsonParserVicetrice
{

    class JSONstruct
    {
    public:
        JSONstruct();

        ~JSONstruct();
        JSONstruct(JSONstruct &&) noexcept = default;
        JSONstruct &operator=(JSONstruct &&) noexcept = default;

        /**
         * @brief Add elements to the final structure.
         * @param token Token that wil be stored in the final structure.
         */
        void add(const Token &token);

        void consult() const
        {
            if (const auto *ptr = std::get_if<std::unique_ptr<JSONobject>>(&finalStruct))
            {
                if (ptr->get())
                {
                    printJSONobject(*ptr->get());
                }
            }
        }

    private:
        //--------------------- ATTRIBUTES
        using VariantStruct = std::variant<std::unique_ptr<JSONobject>, std::unique_ptr<JSONarray>>;
        using VariantPtr = std::variant<JSONobject *, JSONarray *>;
        VariantStruct finalStruct;        // Pointer to start of JsonStructure
        std::vector<VariantPtr> position; // Vector of position to the last Object/array access
        std::string key;
        bool noFirst;

        void printJSONobject(const JSONobject &obj) const
        {
            for (const auto &[key, value] : obj.getBasicPair())
            {
                std::cout << key << ": ";
                std::visit([&](const auto &val)
                           { printVariant(val); }, value);
                std::cout << std::endl;
            }
        }

        // Helper function to print a VariantType
        void printVariant(const std::string &val) const
        {
            std::cout << val;
        }

        void printVariant(int64_t val) const
        {
            std::cout << val;
        }

        void printVariant(bool val) const
        {
            std::cout << (val ? "true" : "false");
        }

        void printVariant(double val) const
        {
            std::cout << val;
        }

        void printVariant(char val) const
        {
            if (val == '\0')
                std::cout << "null";
            else
                std::cout << val;
        }

        void printVariant(const std::unique_ptr<JSONarray> &val) const
        {
            std::cout << "[JSON array]"; // Placeholder, you could implement more detailed printing for arrays if needed
        }

        void printVariant(const std::unique_ptr<JSONobject> &val) const
        {
            std::cout << "{JSON object}" << std::endl;
            printJSONobject(*val);
        }

    }; // class JSONstruct

} // namespace JsonParserVicetrice
#endif
