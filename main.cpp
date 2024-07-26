#include <string>
#include <iostream>
#include <fstream>
#include "Lexer.hpp"

int main()
{
	std::string json = R"({
        "name": "John",
        "age": -30,
        "isStudent": false,
        "address": null
    })";

	// Lexer from string
	JsonParser::Lexer lexer(json);
	std::vector<JsonParser::Token> tokens = lexer.tokenize();

	std::cout << "FROM LIT STRING" << '\n';

	for (const JsonParser::Token &token : tokens)
	{
		std::cout << "Token: " << token.Value << " (Type: " << static_cast<int>(token.Type) << ")\n";
	}

	std::cout << "FROM FILE" << '\n';

	std::ifstream file("prueba2.json");

	if (!file.is_open())
	{
		std::cerr << "Error opening file!" << std::endl;
		return 1; // Indicate an error occurred
	}

	JsonParser::Lexer lexer2(file);
	std::vector<JsonParser::Token> tokens2 = lexer2.tokenize(); // Fixed: Use lexer2 here

	for (auto &token : tokens2)
	{
		std::cout << "Token: " << token.Value << " (Type: " << static_cast<int>(token.Type) << ")\n";
	}

	return 0;
}
