#ifndef RULES_HPP
#define RULES_HPP

#include <vector>	 //std::vector
#include <cstddef>	 //size_t
#include "Token.hpp" //Token
#include <stdexcept> //std::runtime_error

namespace JsonParser
{
	class Rules
	{
	public:
		// Default constructor
		Rules() noexcept;

		// Destructor
		~Rules() noexcept;

		/**
		 * @brief Inspect if the stream follow the rules.
		 * @param current_type Type of the token.
		 * @return bool indicating if it follows the rules, true if it follow the rules.
		 */
		bool inspect(const TokenType &current_type);

		/**
		 * @brief verify if the vectr OrderOfEntry of the brackets/braces is empty.
		 * @return true if is empty.
		 */
		inline bool empty() const noexcept
		{
			return OrderOfEntry.empty();
		}

		/**
		 * @brief verify if you are inside a bracket or a brace.
		 * @return true you are in a bracket, false you are in a brace.
		 * @throws std::runtime_error if the vector is empty.
		 */
		inline bool ImInBracket() const
		{
			if (OrderOfEntry.empty())
				throw std::runtime_error("Vector is empty");

			return (OrderOfEntry[pos] & 1);
		}

	private:
		//--------------------- ATTRIBUTES
		TokenType previous_type;
		std::vector<size_t> OrderOfEntry; // Storage of the order of entries in brackets/braces
		size_t shifts;					  // Number of shifts done
		size_t pos;						  // position in vector

		//--------------------- PRIVATE METHODS ---------------------

		// extract LSB of an array of bool
		void extractFirst();

		// set true/false the values of an array of bool
		void SetBool(size_t booli);

		//--------------------- GENERAL RULES

		static inline bool RightIsCommaOrRbr(const TokenType &current_type) noexcept
		{
			return (current_type == TokenType::COMMA || current_type == TokenType::RIGHT_BRACE || current_type == TokenType::RIGHT_BRACKET);
		}

		//--------------------- KEY RULES

		static inline bool AfterKeyComesValue(const TokenType &current_type) noexcept
		{
			return (current_type == TokenType::BOOLEAN || current_type == TokenType::NUL || current_type == TokenType::LEFT_BRACE || current_type == TokenType::LEFT_BRACKET || current_type == TokenType::NUMBER || current_type == TokenType::STRING || current_type == TokenType::DOUBLE);
		}

		//--------------------- COMMA RULES

		static inline bool RightIsKey(const TokenType &current_type) noexcept
		{
			return (current_type == TokenType::KEY);
		}

		static inline bool RightIsNotValue(const TokenType &current_type) noexcept
		{
			return (current_type == TokenType::KEY || current_type == TokenType::RIGHT_BRACE || current_type == TokenType::RIGHT_BRACKET);
		}

		//--------------------- LBRACE RULES

		static inline bool RightIsKeyOrRBrace(const TokenType &current_type) noexcept
		{
			return (current_type == TokenType::RIGHT_BRACE || current_type == TokenType::KEY);
		}

	}; // class Rules
} // namespace JsonParser
#endif