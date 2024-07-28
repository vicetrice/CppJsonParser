#include "Rules.hpp"
#include "Token.hpp" //TokenType
#include <stdexcept> //std::runtime_error

#include <iostream>
namespace JsonParser
{
	//--------------------- PRIVATE METHODS ---------------------

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

	//--------------------- PUBLIC ---------------------

	// Default constructor
	Rules::Rules() noexcept : previous_type{TokenType::UNDEFINED}, shifts{}, pos{}
	{
		OrderOfEntry.push_back(0);
	}

	// Destructor
	Rules::~Rules() noexcept
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
	bool Rules::inspect(const TokenType &current_type)
	{
		// std::cout << " (Prev_Type: " << static_cast<int>(previous_type) << ")\n";
		// std::cout << " (Curr_Type: " << static_cast<int>(current_type) << ")\n";

		if (OrderOfEntry.empty())
			throw std::runtime_error("End/Start Expected");

		if (previous_type != TokenType::UNDEFINED)
		{
			switch (previous_type)
			{
			case TokenType::STRING:

				if (!RightIsCommaOrRbr(current_type))
				{
					throw std::runtime_error("Expected comma or RBrace/RBracket");
				}

				break;
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
				if (!RightIsCommaOrRbr(current_type))
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

} // namespace JsonParser