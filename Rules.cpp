#include "Rules.hpp"
#include "Token.hpp" //TokenType
#include <stdexcept> //std::runtime_error

namespace JsonParser
{
	//--------------------- PRIVATE ---------------------

	// extract LSB of an array of bool
	void Rules::extractFirst()
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
	void Rules::SetBool(size_t booli)
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

	// See if the LSB is 0 or 1
	bool Rules::ImInBracket() const
	{
		if (OrderOfEntry.empty())
		{
			throw std::runtime_error("Vector is empty");
		}
		return (OrderOfEntry[pos] & 1);
	}

	//--------------------- GENERAL RULES

	// There can´t be a type 6/4/5 next to a type 6/4/5
	bool Rules::TwoSuccession(const TokenType &current_type) const
	{
		return (previous_type == current_type);
	}

	bool Rules::RightIsCommaOrRbr(const TokenType &current_type)
	{
		return (current_type == TokenType::COMMA || current_type == TokenType::RIGHT_BRACE || current_type == TokenType::RIGHT_BRACKET);
	}

	//--------------------- KEY RULES

	bool Rules::AfterKeyComesValue(const TokenType &current_type)
	{
		return (current_type == TokenType::BOOLEAN || current_type == TokenType::NUL || current_type == TokenType::LEFT_BRACE || current_type == TokenType::LEFT_BRACKET || current_type == TokenType::NUMBER || current_type == TokenType::STRING);
	}

	//--------------------- COMMA RULES

	bool Rules::RightIsKey(const TokenType &current_type)
	{
		return (current_type == TokenType::KEY);
	}

	bool Rules::RightIsNotValue(const TokenType &current_type)
	{
		return (current_type == TokenType::KEY || current_type == TokenType::RIGHT_BRACE || current_type == TokenType::RIGHT_BRACKET);
	}

	//--------------------- LBRACE RULES

	bool Rules::RightIsKeyOrRBrace(const TokenType &current_type)
	{
		return (current_type == TokenType::RIGHT_BRACE || current_type == TokenType::KEY);
	}

	//--------------------- PUBLIC ---------------------

	// Default constructor
	Rules::Rules() : previous_type{TokenType::UNDEFINED}, shifts{}, pos{}
	{
		OrderOfEntry.push_back(0);
	}

	// Destructor
	Rules::~Rules()
	{
		OrderOfEntry.clear();
		OrderOfEntry.shrink_to_fit();
	}

	/**
	 * @brief Inspect if the stream follow the rules
	 * @param current_type Type of the token
	 * @return bool indicating if it follows the rules, true if it follow the rules
	 */
	bool Rules::inspect(const TokenType &current_type)
	{
		// std::cout << " (Prev_Type: " << static_cast<int>(previous_type) << ")\n";
		// std::cout << " (Curr_Type: " << static_cast<int>(current_type) << ")\n";

		if (OrderOfEntry.empty())
		{
			throw std::runtime_error("End Expected");
		}
		if (previous_type != TokenType::UNDEFINED)
		{
			switch (previous_type)
			{
			case TokenType::STRING:
				if (TwoSuccession(current_type))
				{
					throw std::runtime_error("+2 string succession detected");
				}
				if (!RightIsCommaOrRbr(current_type))
				{
					throw std::runtime_error("Expected comma or RBrace/RBracket");
				}

				break;
			case TokenType::COMMA:
				if (TwoSuccession(current_type))
				{
					throw std::runtime_error("+2 comma succession detected");
				}

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
			case TokenType::NUMBER:

				if (!RightIsCommaOrRbr(current_type))
				{
					throw std::runtime_error("Expected comma or RBrace/RBracket");
				}

				break;
			case TokenType::BOOLEAN:

				if (!RightIsCommaOrRbr(current_type))
				{
					throw std::runtime_error("Expected comma or RBrace/RBracket");
				}

				break;
			case TokenType::NUL:

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
	 * @brief verify if the vectr OrderOfEntry of the brackets/braces is empty
	 * @return true if is empty
	 */
	bool Rules::empty() const
	{
		return OrderOfEntry.empty();
	}

} // namespace JsonParser