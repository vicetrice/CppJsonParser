
#ifndef TOKEN_HPP
#define TOKEN_HPP

#if __cplusplus >= 201703L
#include <string_view> // std::string_view
namespace JsonParser
{
	using StringType = std::string_view;
}
#else
#include <string> // std::string
namespace JsonParser
{
	using StringType = std::string;
}
#endif

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
		NUMBER,		   // 123, -123, 1.23, etc.
		BOOLEAN,	   // true, false
		NUL,		   // null
		END,		   // End of input
		UNDEFINED,	   // Ini of var
	}; // enum class TokenType

	struct Token
	{
		TokenType Type;
		StringType Value;
	}; // struct Token
}
#endif