
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string> // std::string

namespace JsonParser
{
	enum class TokenType
	{
		LEFT_BRACE,	   // {
		RIGHT_BRACE,   // }
		LEFT_BRACKET,  // [
		RIGHT_BRACKET, // ]
		KEY,		   // "str":
		COMMA,		   // ,
		STRING,		   // "..."
		NUMBER,		   // 123, -123, etc.
		BOOLEAN,	   // true, false
		NUL,		   // null
		END,		   // End of input
		UNDEFINED,	   // Ini of var
		DOUBLE		   // 1.23
	}; // enum class TokenType

	struct Token
	{
		//--------------------- ATTRIBUTES

		TokenType Type;
		std::string Value;
	}; // struct Token
}
#endif