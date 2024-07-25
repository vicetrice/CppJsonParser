
#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>

namespace JsonParser
{
	enum class TokenType
	{
		LEFT_BRACE,	   // {
		RIGHT_BRACE,   // }
		LEFT_BRACKET,  // [
		RIGHT_BRACKET, // ]
		COLON,		   // :
		COMMA,		   // ,
		STRING,		   // "..."
		NUMBER,		   // 123, -123, 1.23, etc.
		BOOLEAN,	   // true, false
		NUL,		   // null
		END,		   // End of input
		UNDEFINED,	   // Ini of var
		KEY			   // "str":
	}; // enum class TokenType

	struct Token
	{
		TokenType Type;
		std::string Value;
	}; // struct Token
}
#endif