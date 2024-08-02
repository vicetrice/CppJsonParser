#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include "../single_include/JSON.hpp"


int main()
{
	using namespace std::chrono; // Para simplificar el uso de chrono



	std::ifstream file("../simple.json");

	if (!file.is_open())
	{
		std::cerr << "Error opening file!" << std::endl;
		return 1; // Indicar que ocurrió un error
	}

	// Medir el tiempo para la sección de lectura de archivo y tokenización
	auto start = high_resolution_clock::now();

	JsonParserVicetrice::JSON json;
	json.parse(file);
	size_t cont = json.size();

	std::cout << cont << std::endl;

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start).count();

	std::cout << "Tiempo de ejecución: " << duration << " ms" << std::endl;

	return 0;
}
