#include "Lexer.hpp"
#include "Rules.hpp" //Rules
#include <string>	 //std::string
#include <stdexcept> //std::runtime_error
#include <fstream>	 //std::ifstream, std::istringstream,std::istreambuf_iterator
#include "Token.hpp" //Token, TokenType

#include <iostream>

namespace JsonParser
{
	//--------------------- PUBLIC
	// Default constructor
	Lexer::Lexer() noexcept : input{nullptr}, owns_stream{}, current_char{} {}

	/**
	 * @brief Generate a Lexer object from a file
	 * @param file a ifstream object to read from, must be opened first
	 *
	 * Constructor from std::ifstream
	 */
	Lexer::Lexer(std::ifstream &file) : input{&file}, owns_stream{false}
	{
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open file.");
		}
		current_char = getNextChar();
	}

	/**
	 * @brief Generate a Lexer object from raw string
	 * @param str raw string to read from
	 *
	 * Constructor from std::string
	 */
	Lexer::Lexer(const std::string &str) noexcept : input{new std::istringstream(str)}, owns_stream{true}
	{
		current_char = getNextChar();
	}

	// Destructor
	Lexer::~Lexer() noexcept
	{
		if (owns_stream)
		{
			delete input;
		}
	}

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
	 * - @c RIGHT_BRACKET(]):                  	 Type 3,
	 *
	 * - @c KEY			("str":)				Type 4,
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
	 * - @c UNDEFINED	(undefined):			 Type 11,
	 */
	std::vector<Token> Lexer::tokenize()
	{
		std::vector<Token> tokens; // storage of tokens
		Rules rules;

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
				// std::cout << "Token: " << token.Value << " (Type: " << static_cast<int>(token.Type) << ")\n";
				rules.inspect(token.Type);
				tokens.push_back(token);
			}
			catch (const std::exception &e)
			{
				tokens.clear();
				tokens.shrink_to_fit();
				if (owns_stream)
					delete input;
				throw;
			}
		} while (token.Type != TokenType::END);

		if (!rules.empty())
		{
			tokens.clear();
			tokens.shrink_to_fit();
			if (owns_stream)
				delete input;
			throw std::runtime_error("Unbalanced brackets/Braces");
		}

		return tokens;
	}

	//--------------------- PRIVATE
	// Gets the next token in the string or file
	Token Lexer::nextToken()
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
	Token Lexer::advance(TokenType type) noexcept
	{
		char previous_char = current_char;
		current_char = getNextChar();
		return {type, std::string(1, previous_char)};
	}

	// Skip whitespaces
	void Lexer::skipWhitespace() noexcept
	{
		while (!input->eof() && isspace(current_char))
		{
			current_char = getNextChar();
		}
	}

	// Parse string
	Token Lexer::parseString()
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
	Token Lexer::parseNumber()
	{
		std::string result;

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
		return {TokenType::NUMBER, result};
	}

	// Parse keywords(null, true, false)
	Token Lexer::parseKeyword()
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

} // namespace JsonParser