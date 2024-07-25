#ifndef RULES_HPP
#define RULES_HPP

#include <vector>	 //std::vector
#include <cctype>	 //size_t
#include "Token.hpp" //Token

namespace JsonParser
{
	class Rules
	{
	public:
		Rules();
		~Rules();

		/**
		 * @brief Inspect if the stream follow the rules
		 * @param current_type Type of the token
		 * @return bool indicating if it follows the rules, true if it follow the rules
		 */
		bool inspect(const TokenType &type);

	private:
		// std::vector<size_t> brace_parity;	// bitmask of braces, needs to be 0 before returning token vector
		// std::vector<size_t> bracket_parity; // bitmask of braces, needs to be 0 before returning token vector
		TokenType previous_type;

		//--------------------- GENERAL RULES

		// There can´t be a type 6/4 next to a type 6/4
		bool TwoSuccession(const TokenType &current_type) const;

		//--------------------- STRING RULES

		//--------------------- COLON RULES

	}; // class Rules
} // namespace JsonParser
#endif