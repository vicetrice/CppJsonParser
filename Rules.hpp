#ifndef RULES_HPP
#define RULES_HPP

#include <vector>	 //std::vector
#include <cstddef>	 //size_t
#include "Token.hpp" //Token

namespace JsonParser
{
	class Rules
	{
	public:
		// Default constructor
		Rules();

		// Destructor
		~Rules();

		/**
		 * @brief Inspect if the stream follow the rules
		 * @param current_type Type of the token
		 * @return bool indicating if it follows the rules, true if it follow the rules
		 */
		bool inspect(const TokenType &type);

	private:
		TokenType previous_type;
		std::vector<size_t> OrderOfEntry; // Storage of the order of entries in brackets/braces
		size_t shifts;					  // Number of shifts done
		size_t pos;						  // position in vector

		bool ImInBracket() const;

		void extractFirst();

		// set true the values of an array of bool
		void SetBool(size_t booli);

		//--------------------- GENERAL RULES

		// There can´t be a type 6/4 next to a type 6/4
		bool TwoSuccession(const TokenType &current_type) const;

		//--------------------- STRING RULES

		//--------------------- KEY RULES
		bool AfterKeyComesValue(const TokenType &current_type) const;

		//--------------------- LBRACE RULES
		bool RightIsKeyOrRBrace(const TokenType &current_type) const;

		//--------------------- COMMA RULES
		bool RightIsKeyOrLBraceOrBracket(const TokenType &current_type) const;
		bool RightIsNotValue(const TokenType &current_type) const;

	}; // class Rules
} // namespace JsonParser
#endif