#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include "Parser.hpp"
// #include <JSONarray.hpp>

int main()
{
	using namespace std::chrono; // Para simplificar el uso de chrono

	std::ifstream file("exit.json");

	if (!file.is_open())
	{
		std::cerr << "Error opening file!" << std::endl;
		return 1; // Indicar que ocurrió un error
	}

	// Medir el tiempo para la sección de lectura de archivo y tokenización
	auto start = high_resolution_clock::now();
	JsonParserVicetrice::Parser Parser2(file);
	JsonParserVicetrice::JSONstruct strut;
	strut = std::move(Parser2.tokenize());

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start).count();

	std::cout << "Tiempo de ejecución: " << duration << " ms" << std::endl;

	/*
	for (auto &token : tokens2)
	{
		std::cout << "Token: " << token.Value << " (Type: " << static_cast<int>(token.Type) << ")\n";
	}
	*/

	return 0;
}
