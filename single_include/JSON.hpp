#ifndef __________JSON_HPP__________
#define __________JSON_HPP__________

#include <string>        // std::string
#include <vector>        //std::vector
#include <cstddef>       //size_t
#include <stdexcept>     //std::runtime_error
#include <variant>       // std::variant
#include <memory>        // std::unique_ptr
#include <iostream>      //std::cout
#include <unordered_map> // std::unordered_map
#include <fstream>       // std::ifstream
#include <sstream>       //std::istringstream

namespace JsonParserVicetrice
{
    enum class TokenType
    {
        LEFT_BRACE,    // {
        RIGHT_BRACE,   // }
        LEFT_BRACKET,  // [
        RIGHT_BRACKET, // ]
        KEY,           // "str":
        COMMA,         // ,
        STRING,        // "..."
        NUMBER,        // 123, -123, etc.
        BOOLEAN,       // true, false
        NUL,           // null
        END,           // End of input
        UNDEFINED,     // Ini of var
        DOUBLE         // 1.23
    }; // enum class TokenType

    struct Token
    {
        //--------------------- ATTRIBUTES

        TokenType Type;
        std::string Value;
    }; // struct Token

    class Rules
    {
    public:
        // Default constructor
        Rules() noexcept : previous_type{TokenType::UNDEFINED}, shifts{}, pos{}
        {
            OrderOfEntry.push_back(0);
        }

        // Destructor
        ~Rules() noexcept
        {
            OrderOfEntry.clear();
            OrderOfEntry.shrink_to_fit();
        }

        /**
         * @brief Inspect if the stream follow the rules
         * @param current_type Type of the token
         * @return bool indicating if it follows the rules, true if it follow the rules
         *  @throws std::runtime_error if Rules not followed
         */
        bool inspect(const TokenType &current_type)
        {

            if (OrderOfEntry.empty())
                throw std::runtime_error("End/Start Expected");

            if (previous_type != TokenType::UNDEFINED)
            {
                switch (previous_type)
                {
                case TokenType::COMMA:

                    if (shifts)
                    {
                        if (!ImInBracket())
                        {
                            if (!RightIsKey(current_type))
                            {
                                throw std::runtime_error("Property expected or trailing comma in brace");
                            }
                        }
                        else
                        {
                            if (RightIsNotValue(current_type))
                            {
                                throw std::runtime_error("Property expected or trailing comma in bracket");
                            }
                        }
                    }

                    break;
                case TokenType::KEY:
                    if (!AfterKeyComesValue(current_type))
                    {
                        throw std::runtime_error("Value expected");
                    }

                    break;
                case TokenType::LEFT_BRACE:
                    if (!RightIsKeyOrRBrace(current_type))
                    {
                        throw std::runtime_error("Expected key or Rbrace");
                    }
                    SetBool(0);

                    break;
                case TokenType::LEFT_BRACKET:

                    SetBool(1);
                    break;
                case TokenType::RIGHT_BRACKET:
                    if (ImInBracket())
                    {
                        extractFirst();
                    }
                    else
                    {
                        throw std::runtime_error("Expected Bracket");
                    }
                    if (!RightIsCommaOrRbr(current_type) && current_type != TokenType::END)
                    {
                        throw std::runtime_error("Expected comma or RBrace/RBracket");
                    }

                    break;
                case TokenType::RIGHT_BRACE:

                    if (!ImInBracket())
                    {
                        extractFirst();
                    }
                    else
                    {
                        throw std::runtime_error("Expected Brace");
                    }
                    if (!RightIsCommaOrRbr(current_type) && current_type != TokenType::END)
                    {
                        throw std::runtime_error("Expected comma or RBrace/RBracket");
                    }

                    break;
                case TokenType::DOUBLE:
                case TokenType::NUMBER:
                case TokenType::BOOLEAN:
                case TokenType::NUL:
                case TokenType::STRING:

                    if (!RightIsCommaOrRbr(current_type))
                    {
                        throw std::runtime_error("Expected comma or RBrace/RBracket");
                    }

                    break;
                case TokenType::END:
                    break;
                default:
                    throw std::runtime_error("Unknown Type");
                    break;
                }
            }
            previous_type = current_type;
            return true;
        }

        /**
         * @brief verify if the vectr OrderOfEntry of the brackets/braces is empty.
         * @return true if is empty.
         */
        inline bool empty() const noexcept
        {
            return OrderOfEntry.empty();
        }

        /**
         * @brief verify if you are inside a bracket or a brace.
         * @return true you are in a bracket, false you are in a brace.
         * @throws std::runtime_error if the vector is empty.
         */
        inline bool ImInBracket() const
        {
            if (OrderOfEntry.empty())
                throw std::runtime_error("Vector is empty");

            return (OrderOfEntry[pos] & 1);
        }

    private:
        //--------------------- ATTRIBUTES
        TokenType previous_type;
        std::vector<size_t> OrderOfEntry; // Storage of the order of entries in brackets/braces
        size_t shifts;                    // Number of shifts done
        size_t pos;                       // position in vector

        //--------------------- PRIVATE METHODS ---------------------

        // extract LSB of an array of bool
        void extractFirst()
        {

            if (OrderOfEntry.empty())
            {
                throw std::runtime_error("Vector is empty");
            }
            // popback bit
            OrderOfEntry[pos] >>= 1;
            --shifts;
            if (shifts == 0)
            {
                if (pos)
                {
                    shifts = sizeof(size_t) * 8;
                    --pos;
                }
                OrderOfEntry.pop_back();
            }
        }

        // set true/false the values of an array of bool
        void SetBool(size_t booli)
        {
            if (OrderOfEntry.empty())
            {
                OrderOfEntry.push_back(0);
            }
            OrderOfEntry[pos] = (OrderOfEntry[pos] << 1) | booli;
            ++shifts;
            if (shifts == sizeof(size_t) * 8)
            {
                shifts = 0;
                OrderOfEntry.push_back(0);
                pos++;
            }
        }

        //--------------------- GENERAL RULES

        static inline bool RightIsCommaOrRbr(const TokenType &current_type) noexcept
        {
            return (current_type == TokenType::COMMA || current_type == TokenType::RIGHT_BRACE || current_type == TokenType::RIGHT_BRACKET);
        }

        //--------------------- KEY RULES

        static inline bool AfterKeyComesValue(const TokenType &current_type) noexcept
        {
            return (current_type == TokenType::BOOLEAN || current_type == TokenType::NUL || current_type == TokenType::LEFT_BRACE || current_type == TokenType::LEFT_BRACKET || current_type == TokenType::NUMBER || current_type == TokenType::STRING || current_type == TokenType::DOUBLE);
        }

        //--------------------- COMMA RULES

        static inline bool RightIsKey(const TokenType &current_type) noexcept
        {
            return (current_type == TokenType::KEY);
        }

        static inline bool RightIsNotValue(const TokenType &current_type) noexcept
        {
            return (current_type == TokenType::KEY || current_type == TokenType::RIGHT_BRACE || current_type == TokenType::RIGHT_BRACKET);
        }

        //--------------------- LBRACE RULES

        static inline bool RightIsKeyOrRBrace(const TokenType &current_type) noexcept
        {
            return (current_type == TokenType::RIGHT_BRACE || current_type == TokenType::KEY);
        }

    }; // class Rules

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
        void add_any_except_string(VariantType value) noexcept
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
        const VariantTypeI consult(size_t index) const
        {

            if (index > BasicElement.size())
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
         * @brief Consult the size of the JSONarray
         * @return Size of JSONarray
         */
        inline size_t size() const
        {
            return BasicElement.size();
        }

    private:
        //--------------------- ATTRIBUTES

        /// Vector to store the elements of the JSON array.
        std::vector<VariantType> BasicElement;
    };

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
         * @brief Adds a value to the object with a given key, excluding strings.
         *
         * Adds a value of any type except std::string to the object. If the value is a std::string,
         * a message is displayed instructing to use the add_string method instead.
         *
         * @param key The key associated with the value to add.
         * @param value The value to add to the object.
         */
        void add_any_except_string(const std::string &key, VariantType value) noexcept
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
        const VariantTypeI consult(const std::string &key) const
        {
            auto it = BasicPair.find(key);
            if (it == BasicPair.end())
            {
                throw std::runtime_error(key + " not found.");
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
        void exists(const std::string &key) const
        {
            auto it = BasicPair.find(key);
            if (it != BasicPair.end())
            {
                throw std::runtime_error("Duplicated keys are not allowed.");
            }
        }

        /**
         * @brief Consult the size of the JSONobject
         * @return Size of JSONobject
         */
        inline size_t size() const
        {
            return BasicPair.size();
        }

    private:
        //--------------------- ATTRIBUTES

        /// Unordered map to store key-value pairs of basic types.
        std::unordered_map<std::string, VariantType> BasicPair;
    }; // class JSONobject

    class JSONstruct
    {
    public:
        using VariantPtr = std::variant<JSONobject *, JSONarray *>;

        JSONstruct(JSONstruct &&) noexcept = default;
        JSONstruct &operator=(JSONstruct &&) noexcept = default;

        /// @brief Default constructor
        JSONstruct() : noFirst{false} {}

        /// @brief Default destructor
        ~JSONstruct() = default;

        /**
         * @brief add elements to the final structure
         * @param token token that wil be stored in the final structure
         */
        void add(const Token &token)
        {
            JSONobject *ptrObj;
            JSONarray *ptrArr;

            switch (token.Type)
            {
            case TokenType::LEFT_BRACE:

                if (!noFirst)
                {
                    finalStruct = std::make_unique<JSONobject>();
                    if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&finalStruct))
                    {
                        position.push_back(ptr->get());
                        noFirst = true;
                    }
                }
                else
                {

                    std::unique_ptr<JSONobject> ptr_to_struct = std::make_unique<JSONobject>();

                    if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                    {
                        ptrObj = *ptr;
                        position.push_back(ptr_to_struct.get());
                        ptrObj->add_any_except_string(key, std::move(ptr_to_struct));
                    }
                    else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                    {
                        ptrArr = *ptr;
                        position.push_back(ptr_to_struct.get());
                        ptrArr->add_any_except_string(std::move(ptr_to_struct));
                    }
                }

                break;
            case TokenType::LEFT_BRACKET:
                if (!noFirst)
                {
                    finalStruct = std::make_unique<JSONarray>();
                    if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&finalStruct))
                    {
                        position.push_back(ptr->get());
                        noFirst = true;
                    }
                    // std::cout << "pase por aqui 2" << '\n';
                }
                else
                {
                    std::unique_ptr<JSONarray> ptr_to_struct = std::make_unique<JSONarray>();

                    if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                    {
                        ptrObj = *ptr;
                        position.push_back(ptr_to_struct.get());
                        ptrObj->add_any_except_string(key, std::move(ptr_to_struct));
                    }
                    else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                    {
                        ptrArr = *ptr;
                        position.push_back(ptr_to_struct.get());
                        ptrArr->add_any_except_string(std::move(ptr_to_struct));
                    }
                }
                break;

            case TokenType::RIGHT_BRACE:
            case TokenType::RIGHT_BRACKET:
                position.pop_back();
                break;
            case TokenType::DOUBLE:
                // std::cout << "pase por aqui DOUBLE con: " << token.Value << std::endl;

                if (noFirst)
                {

                    if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                    {
                        ptrObj = *ptr;
                        ptrObj->add_any_except_string(key, std::stold(token.Value));
                    }
                    else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                    {
                        ptrArr = *ptr;
                        ptrArr->add_any_except_string(std::stold(token.Value));
                    }
                }
                break;
            case TokenType::NUMBER:

                if (noFirst)
                {
                    if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                    {
                        ptrObj = *ptr;
                        ptrObj->add_any_except_string(key, std::stoll(token.Value));
                    }
                    else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                    {
                        ptrArr = *ptr;
                        ptrArr->add_any_except_string(std::stoll(token.Value));
                    }
                }

                break;
            case TokenType::BOOLEAN:

                if (noFirst)
                {
                    bool booli = token.Value == "true" ? true : false;
                    if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                    {
                        ptrObj = *ptr;
                        ptrObj->add_any_except_string(key, booli);
                    }
                    else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                    {
                        ptrArr = *ptr;
                        ptrArr->add_any_except_string(booli);
                    }
                }

                break;
            case TokenType::STRING:

                if (noFirst)
                {
                    if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                    {
                        ptrObj = *ptr;
                        ptrObj->add_string(key, token.Value);
                    }
                    else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                    {
                        ptrArr = *ptr;
                        ptrArr->add_string(token.Value);
                    }
                }

                break;
            case TokenType::NUL:
                if (noFirst)
                {
                    if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                    {
                        ptrObj = *ptr;
                        ptrObj->add_any_except_string(key, '\0');
                    }
                    else if (auto ptr = std::get_if<JSONarray *>(&position.back()))
                    {
                        ptrArr = *ptr;
                        ptrArr->add_any_except_string('\0');
                    }
                }

                break;

            case TokenType::KEY:
                if (auto ptr = std::get_if<JSONobject *>(&position.back()))
                {
                    ptrObj = *ptr;
                    ptrObj->exists(token.Value);
                }
                key = token.Value;

                break;
            default:
                break;
            }
        }

        /**
         * @brief Consult the start of the struct
         * @return pointer of type VariantPtr to start of struct
         */
        inline void consultIni(VariantPtr &ret) const noexcept
        {
            if (auto ptr = std::get_if<std::unique_ptr<JSONobject>>(&finalStruct))
                ret = ptr->get();
            else if (auto ptr = std::get_if<std::unique_ptr<JSONarray>>(&finalStruct))
                ret = ptr->get();
        }

    private:
        //--------------------- ATTRIBUTES

        using VariantStruct = std::variant<std::unique_ptr<JSONobject>, std::unique_ptr<JSONarray>>;

        VariantStruct finalStruct;        // Pointer to start of JsonStructure
        std::vector<VariantPtr> position; // Vector of position to the last Object/array access
        std::string key;
        bool noFirst;

    }; // class JSONstruct

    class Parser
    {
    public:
        //--------------------- PUBLIC
        // Default constructor
        Parser() noexcept : input{nullptr}, owns_stream{}, current_char{} {}

        /**
         * @brief Generate a Parser object from a file.
         * @param file a ifstream object to read from, must be opened first.
         *
         * Constructor from std::ifstream
         */
        Parser(std::ifstream &file) : input{&file}, owns_stream{false}
        {
            if (!file.is_open())
            {
                throw std::runtime_error("Could not open file.");
            }
            current_char = getNextChar();
        }

        /**
         * @brief Generate a Parser object from raw string.
         * @param str raw string to read from.
         *
         * Constructor from std::string.
         */
        Parser(const std::string &str) noexcept : input{new std::istringstream(str)}, owns_stream{true}
        {
            current_char = getNextChar();
        }

        // Destructor
        ~Parser() noexcept
        {
            if (owns_stream)
            {
                delete input;
            }
        }

        /**
         * @brief Tokenize a raw string or a file and save the tokens in a vector.
         *
         * @c TYPES:
         *
         * - @c LEFT_BRACE   ({):                    Type 0,
         *
         * - @c RIGHT_BRACE  (}):                    Type 1,
         *
         * - @c LEFT_BRACKET ([):                    Type 2,
         *
         * - @c RIGHT_BRACKET(]):                  	 Type 3,
         *
         * - @c KEY			("str":)				Type 4,
         *
         * - @c COMMA        (,):                    Type 5,
         *
         * - @c STRING       ("..."):                Type 6,
         *
         * - @c NUMBER       (123, -123, etc.):		Type 7,
         *
         * - @c BOOLEAN      (true, false):          Type 8,
         *
         * - @c NUL          (null):                 Type 9,
         *
         * - @c END          (End of input):         Type 10,
         *
         * - @c UNDEFINED	(undefined):			 Type 11,
         *
         *  - @c DOUBLE 	(1.23):					Type 12
         */
        JSONstruct tokenize()
        {
            Rules rules;
            JSONstruct finalStruct;

            Token token;
            do
            {
                try
                {
                    skipWhitespace();
                    token = nextToken();

                    if (token.Type == TokenType::STRING && current_char == ':')
                    {
                        token.Type = TokenType::KEY;
                        current_char = getNextChar();
                    }

                    rules.inspect(token.Type);
                    finalStruct.add(token);
                }
                catch (const std::exception &e)
                {
                    if (owns_stream)
                        delete input;

                    throw;
                }
            } while (token.Type != TokenType::END);

            if (!rules.empty())
            {
                if (owns_stream)
                    delete input;
                throw std::runtime_error("Unbalanced brackets/Braces");
            }

            return finalStruct;
        }

    private:
        //--------------------- ATTRIBUTES

        std::istream *input; // input stream object
        bool owns_stream;    // bool for indicating who owns the stream, 1 for raw string
        char current_char;   // current char taken

        //--------------------- PRIVATE METHODS

        // Gets the next token in the string or file
        Token nextToken()
        {

            if (input->eof())
                return {TokenType::END, ""};

            if (current_char == '{')
                return advance(TokenType::LEFT_BRACE);
            if (current_char == '}')
                return advance(TokenType::RIGHT_BRACE);
            if (current_char == '[')
                return advance(TokenType::LEFT_BRACKET);
            if (current_char == ']')
                return advance(TokenType::RIGHT_BRACKET);
            if (current_char == ',')
                return advance(TokenType::COMMA);
            if (current_char == '"')
                return parseString();
            if (isdigit(current_char) || current_char == '-')
                return parseNumber();
            if (isalpha(current_char))
                return parseKeyword();

            throw std::runtime_error("Unexpected character: " + std::string(1, current_char));
        }

        // Advance the position in the string or file
        Token advance(TokenType type) noexcept
        {
            char previous_char = current_char;
            current_char = getNextChar();
            return {type, std::string(1, previous_char)};
        }

        // Skip whitespaces
        void skipWhitespace() noexcept
        {
            while (!input->eof() && isspace(current_char))
            {
                current_char = getNextChar();
            }
        }

        // Parse string
        Token parseString()
        {
            std::string result;
            current_char = getNextChar();
            while (!input->eof() && current_char != '"')
            {
                if (current_char == '\\')
                {
                    current_char = getNextChar();
                    switch (current_char)
                    {
                    case '"':
                        result += '"';
                        break;
                    case '\\':
                        result += '\\';
                        break;
                    case '/':
                        result += '/';
                        break;
                    case 'b':
                        result += '\b';
                        break;
                    case 'f':
                        result += '\f';
                        break;
                    case 'n':
                        result += '\n';
                        break;
                    case 'r':
                        result += '\r';
                        break;
                    case 't':
                        result += '\t';
                        break;
                    case 'u':
                    {
                        std::string hex_code;
                        for (int i = 0; i < 4; ++i)
                        {
                            current_char = getNextChar();
                            if (!isxdigit(current_char))
                                throw std::runtime_error("Invalid Unicode escape sequence");
                            hex_code += current_char;
                        }
                        result += static_cast<char>(std::stoi(hex_code, nullptr, 16));
                        break;
                    }
                    default:
                        throw std::runtime_error("Invalid escape sequence");
                    }
                }
                else
                {
                    result += current_char;
                }
                current_char = getNextChar();
            }
            current_char = getNextChar();
            return {TokenType::STRING, result};
        }

        // Parse number
        Token parseNumber()
        {
            std::string result;
            TokenType type = TokenType::NUMBER;

            if (current_char == '-')
            {
                result += current_char;
                current_char = getNextChar();
            }

            if (current_char == '0')
            {
                result += current_char;
                current_char = getNextChar();
                // If the number is not just zero
                if (isdigit(current_char))
                {
                    throw std::runtime_error("Invalid number sequence: leading zeros are not allowed");
                }
            }
            else if (isdigit(current_char))
            {
                while (!input->eof() && isdigit(current_char))
                {
                    result += current_char;
                    current_char = getNextChar();
                }
            }
            else
            {
                throw std::runtime_error("Invalid number sequence: expected digit after optional minus sign");
            }

            // Frac part
            if (!input->eof() && current_char == '.')
            {
                result += current_char;
                current_char = getNextChar();
                type = TokenType::DOUBLE;

                if (!isdigit(current_char))
                {
                    throw std::runtime_error("Invalid number sequence: expected digit after decimal point");
                }

                while (!input->eof() && isdigit(current_char))
                {
                    result += current_char;
                    current_char = getNextChar();
                }
            }

            // Exp part
            if (!input->eof() && (current_char == 'e' || current_char == 'E'))
            {
                result += current_char;
                current_char = getNextChar();
                type = TokenType::DOUBLE;

                if (current_char == '+' || current_char == '-')
                {
                    result += current_char;
                    current_char = getNextChar();
                }

                if (!isdigit(current_char))
                {
                    throw std::runtime_error("Invalid number sequence: expected digit after exponent indicator");
                }

                while (!input->eof() && isdigit(current_char))
                {
                    result += current_char;
                    current_char = getNextChar();
                }
            }
            return {type, result};
        }

        // Parse keywords(null, true, false)
        Token parseKeyword()
        {
            std::string result;
            while (!input->eof() && isalpha(current_char))
            {
                result += current_char;
                current_char = getNextChar();
            }
            if (result == "true" || result == "false")
                return {TokenType::BOOLEAN, result};
            if (result == "null")
                return {TokenType::NUL, result};
            throw std::runtime_error("Unknown keyword: " + result);
        }

        // Get the next char of current_char
        char getNextChar() const noexcept
        {
            int ch = input->get();
            if (ch == std::istream::traits_type::eof())
                return '\0';

            return static_cast<char>(ch);
        }

    }; // class Parser

    class JSON
    {
    public:
        /// Variant type used to store different types of JSON elements.
        using VariantTypeI = std::variant<std::string, int64_t, bool, long double, char, JSONobject *, JSONarray *>;

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
         * @brief Default constructor.
         *
         * Constructs an empty JSON object.
         */
        JSON() = default;

        /**
         * @brief Destructor.
         *
         * Cleans up any allocated resources.
         */
        ~JSON() = default;

        /**
         * @brief Parses a JSON string.
         *
         * Parses the given JSON string and constructs the internal representation.
         *
         * @param input The JSON string to parse.
         */
        void parse(const std::string &input)
        {
            try
            {

                Parser parser(input);
                start = std::move(parser.tokenize());
                start.consultIni(head);
            }
            catch (const std::exception &e)
            {
                throw;
            }
        }

        /**
         * @brief Parses a JSON input stream.
         *
         * Parses the JSON content from the given input stream and constructs the internal representation.
         *
         * @param input The input stream to parse.
         */
        void parse(std::ifstream &input)
        {
            try
            {
                Parser parser(input);
                start = std::move(parser.tokenize());
                start.consultIni(head);
            }
            catch (const std::exception &e)
            {
                throw;
            }
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
        JSON &operator[](const std::string &index)
        {

            try
            {
                if (auto ptr = std::get_if<JSONobject *>(&(head)))
                {
                    JSONobject *stru = *ptr;
                    element = stru->consult(index);
                    if (auto ptr2 = std::get_if<JSONobject *>(&(element)))
                    {
                        head = *ptr2;
                    }
                    else if (auto ptr2 = std::get_if<JSONarray *>(&(element)))
                    {
                        head = *ptr2;
                    }
                }
                else
                    throw std::runtime_error("key string only for searchs if inside a map.");
            }
            catch (const std::exception &e)
            {
                throw;
            }
            return *this;
        }

        /**
         * @brief Accesses a JSON array element by index.
         *
         * Overloads the subscript operator to access elements of a JSON array by index.
         *
         * @param index The index of the element to access.
         * @return A reference to the JSON element.
         * @throws std::runtime_error If the element does not exist or is not an array.
         */
        JSON &operator[](size_t index)
        {
            try
            {
                if (auto ptr = std::get_if<JSONarray *>(&(head)))
                {
                    JSONarray *stru = *ptr;
                    element = stru->consult(index);
                    if (auto ptr2 = std::get_if<JSONobject *>(&(element)))
                    {
                        head = *ptr2;
                    }
                    else if (auto ptr2 = std::get_if<JSONarray *>(&(element)))
                    {
                        head = *ptr2;
                    }
                }
                else
                    throw std::runtime_error("numerical index only for searchs if inside an array.");
            }
            catch (const std::exception &e)
            {
                throw;
            }

            return *this;
        }

        /**
         * @brief Consult the size of the JSONstruct
         * @return size of JSONstruct head is pointing to
         */
        size_t size()
        {
            if (auto ptr = std::get_if<JSONobject *>(&head))
            {
                JSONobject *aux = *ptr;
                start.consultIni(head);
                return aux->size();
            }
            else if (auto ptr = std::get_if<JSONarray *>(&head))
            {
                JSONarray *aux = *ptr;
                start.consultIni(head);
                return aux->size();
            }
            start.consultIni(head);
            return 0;
        }

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
#endif //__________JSON_HPP__________