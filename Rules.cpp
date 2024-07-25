#include "Rules.hpp"
#include "Token.hpp"
#include <stdexcept>

#include <iostream>

namespace JsonParser
{
	//--------------------- PRIVATE ---------------------

	//--------------------- GENERAL RULES

	// There can´t be a type 6/4/5 next to a type 6/4/5
	bool Rules::TwoSuccession(const TokenType &current_type) const
	{
		return (previous_type == current_type);
	}

	//--------------------- STRING RULES

	//--------------------- KEY RULES
	bool Rules::AfterKeyComesValue(const TokenType &current_type) const
	{
		return (current_type == TokenType::BOOLEAN || current_type == TokenType::NUL || current_type == TokenType::LEFT_BRACE || current_type == TokenType::LEFT_BRACKET || current_type == TokenType::NUMBER || current_type == TokenType::STRING);
	}

	//--------------------- LBRACE RULES
	bool Rules::RightIsKeyOrRBrace(const TokenType &current_type) const
	{
		return (current_type == TokenType::RIGHT_BRACE || current_type == TokenType::KEY);
	}

	//--------------------- PUBLIC ---------------------

	// Default constructor
	Rules::Rules() : previous_type{TokenType::UNDEFINED}
	{
	}

	// Destructor
	Rules::~Rules() {}

	/**
	 * @brief Inspect if the stream follow the rules
	 * @param current_type Type of the token
	 * @return bool indicating if it follows the rules, true if it follow the rules
	 */
	bool Rules::inspect(const TokenType &current_type)
	{
		// std::cout << " (Prev_Type: " << static_cast<int>(previous_type) << ")\n";
		// std::cout << " (Curr_Type: " << static_cast<int>(current_type) << ")\n";
		if (previous_type != TokenType::UNDEFINED)
		{
			switch (previous_type)
			{
			case TokenType::STRING:
				if (TwoSuccession(current_type))
				{
					throw std::runtime_error("+2 string succession detected");
				}
				break;
			case TokenType::COMMA:
				if (TwoSuccession(current_type))
				{
					throw std::runtime_error("+2 comma succession detected");
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

				break;

			default:
				break;
			}
		}
		previous_type = current_type;

		return true;
	}

} // namespace JsonParser