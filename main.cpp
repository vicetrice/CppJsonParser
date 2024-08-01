#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include "single_include/JSON.hpp"
// #include <JSONarray.hpp>

int main()
{
	using namespace std::chrono; // Para simplificar el uso de chrono

	std::ifstream file("prueba.json");

	if (!file.is_open())
	{
		std::cerr << "Error opening file!" << std::endl;
		return 1; // Indicar que ocurrió un error
	}

	// Medir el tiempo para la sección de lectura de archivo y tokenización
	auto start = high_resolution_clock::now();

	JsonParserVicetrice::JSON json;
	json.parse(file);

	std::string loca = json["statuses"][0]["id_str"].get<std::string>();
	std::string loca2 = json["statuses"][1]["text"].get<std::string>();

	std::cout << loca << std::endl;
	std::cout << loca2 << std::endl;

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start).count();

	std::cout << "Tiempo de ejecución: " << duration << " ms" << std::endl;

	return 0;
}
