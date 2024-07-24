#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>   //std::string
#include <vector>   //std::vector
#include <sstream>  //std::ifstream, std::istream

namespace JsonParser
{
    enum class TokenType
    {
        LEFT_BRACE,    // {
        RIGHT_BRACE,   // }
        LEFT_BRACKET,  // [
        RIGHT_BRACKET, // ]
        COLON,         // :
        COMMA,         // ,
        STRING,        // "..."
        NUMBER,        // 123, -123, 1.23, etc.
        BOOLEAN,       // true, false
        NUL,           // null
        END            // End of input
    }; // enum class TokenType

    struct Token
    {
        TokenType Type;
        std::string Value;
    }; // struct Token

    class Lexer
    {
    public:
        // Default constructor
        Lexer() noexcept;

        /**
         * @brief Generate a Lexer object from a file
         * @param file a ifstream object to read from, must be opened first
         *
         * Constructor from std::ifstream
         */
        explicit Lexer(std::ifstream &file);

        /**
         * @brief Generate a Lexer object from raw string
         * @param str raw string to read from
         *
         * Constructor from std::string
         */
        explicit Lexer(const std::string &str) noexcept;

        // Destructor
        ~Lexer() noexcept;

        /**
         * @brief Tokenize a raw string or a file and save the tokens in a vector
         *
         * @c TYPES:
         *
         * - @c LEFT_BRACE   ({):                    Type 0,
         *
         * - @c RIGHT_BRACE  (}):                    Type 1,
         *
         * - @c LEFT_BRACKET ([):                    Type 2,
         *
         * - @c RIGHT_BRACKET(]):                    Type 3,
         *
         * - @c COLON        (:):                    Type 4,
         *
         * - @c COMMA        (,):                    Type 5,
         *
         * - @c STRING       ("..."):                Type 6,
         *
         * - @c NUMBER       (123, -123, 1.23, etc.):Type 7,
         *
         * - @c BOOLEAN      (true, false):          Type 8,
         *
         * - @c NUL          (null):                 Type 9,
         *
         * - @c END          (End of input):         Type 10
         */
        std::vector<Token> tokenize();

    private:
        std::istream *input; // input stream object
        bool owns_stream;    // bool for indicating who owns the stream, 1 for raw string
        char current_char;   // current char taken

        // Gets the next token in the string or file
        Token nextToken();

        // Advance the position in the string or file
        Token advance(TokenType type) noexcept;

        // Skip whitespaces
        void skipWhitespace() noexcept;

        // Parse string
        Token parseString();

        // Parse number
        Token parseNumber() noexcept;

        // Parse keywords(null, true, false)
        Token parseKeyword();

        // Get the next char of current_char
        char getNextChar() noexcept;
    }; // class Lexer
} // namespace JsonParser

#endif