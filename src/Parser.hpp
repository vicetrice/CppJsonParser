#ifndef __________PARSER_HPP__________
#define __________PARSER_HPP__________

#include <string>	 //std::string
#include <vector>	 //std::vector
#include <sstream>	 //std::ifstream, std::istream
#include "Token.hpp" //Token
#include "JSONstruct.hpp" //JSONstruct

namespace JsonParserVicetrice
{
	class Parser
	{
	public:
		// Default constructor
		Parser() noexcept;

		/**
		 * @brief Generate a Parser object from a file
		 * @param file a ifstream object to read from, must be opened first
		 *
		 * Constructor from std::ifstream
		 */
		explicit Parser(std::ifstream &file);

		/**
		 * @brief Generate a Parser object from raw string
		 * @param str raw string to read from
		 *
		 * Constructor from std::string
		 */
		explicit Parser(const std::string &str) noexcept;

		// Destructor
		~Parser() noexcept;

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
		 * - @c KEY			("str":)				 Type 4,
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
		 * - @c END          (End of input):         Type 10,
		 *
		 * - @c UNDEFINED	(undefined):			 Type 11 ,
		 *
		 * - @c DOUBLE		(1.23):					 Type 12
		 */
		JSONstruct tokenize();

	private:
		//--------------------- ATTRIBUTES

		std::istream *input; // input stream object
		bool owns_stream;	 // bool for indicating who owns the stream, 1 for raw string
		char current_char;	 // current char taken

		//--------------------- PRIVATE METHODS

		// Gets the next token in the string or file
		Token nextToken();

		// Advance the position in the string or file
		Token advance(TokenType type) noexcept;

		// Skip whitespaces
		void skipWhitespace() noexcept;

		// Parse string
		Token parseString();

		// Parse number
		Token parseNumber();

		// Parse keywords(null, true, false)
		Token parseKeyword();

		char getNextChar() const noexcept;

	}; // class Parser
} // namespace JsonParserVicetrice

#endif