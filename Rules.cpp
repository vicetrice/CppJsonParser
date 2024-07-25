#include "Rules.hpp"
#include "Token.hpp"
#include <stdexcept>

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

	//--------------------- COLON RULES

	//--------------------- PUBLIC ---------------------
	Rules::Rules() : previous_type{TokenType::UNDEFINED} {}
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
			switch (current_type)
			{
			case TokenType::STRING:
				if (TwoSuccession(current_type))
				{
					throw std::runtime_error("2 string succession detected");
				}
				break;
			case TokenType::COLON:
				if (TwoSuccession(current_type))
				{
					throw std::runtime_error("2 colon succession detected");
				}
				break;
			case TokenType::COMMA:
				if (TwoSuccession(current_type))
				{
					throw std::runtime_error("2 comma succession detected");
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