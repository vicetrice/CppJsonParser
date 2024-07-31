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
        using VariantPtr = std::variant<JSONobject *, JSONarray *>;
        JSONstruct();

        ~JSONstruct();
        JSONstruct(JSONstruct &&) noexcept = default;
        JSONstruct &operator=(JSONstruct &&) noexcept = default;

        /**
         * @brief Add elements to the final structure.
         * @param token Token that wil be stored in the final structure.
         */
        void add(const Token &token);

        /*inline void consult(const std::string &key, size_t index)
        {
            if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&finalStruct))
            {
                JSONobject *stru = ptr->get();
                stru->consult(key);
            }
            else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&finalStruct))
            {
                JSONarray *stru = ptr->get();
                stru->consult(index);
            }
        }*/
        const VariantPtr consultIni() const
        {
            return position[0];
        }

    private:
        //--------------------- ATTRIBUTES
        using VariantStruct = std::variant<std::unique_ptr<JSONobject>, std::unique_ptr<JSONarray>>;

        VariantStruct finalStruct;        // Pointer to start of JsonStructure
        std::vector<VariantPtr> position; // Vector of position to the last Object/array access
        std::string key;
        bool noFirst;

    }; // class JSONstruct

} // namespace JsonParserVicetrice
#endif
