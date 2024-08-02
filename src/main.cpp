#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include "JSON.hpp"
#include <unordered_map>
// #include <JSONarray.hpp>

int main()
{
	using namespace std::chrono; // Para simplificar el uso de chrono

	std::unordered_map<int, int> loco;

	std::ifstream file("../prueba.json");

	if (!file.is_open())
	{
		std::cerr << "Error opening file!" << std::endl;
		return 1; // Indicar que ocurrió un error
	}

	// Medir el tiempo para la sección de lectura de archivo y tokenización
	auto start = high_resolution_clock::now();

	JsonParserVicetrice::JSON json;
	json.parse(file);
	size_t cont = json["statuses"].size();

	for (size_t i = 0; i < cont; i++)
	{
		std::string loca = json["statuses"][i]["source"].get<std::string>();
		std::cout << loca << std::endl;
	}

	std::cout << json["statuses"][1]["retweeted_status"].size()<<std::endl;

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - start).count();

	std::cout << "Tiempo de ejecución: " << duration << " ms" << std::endl;

	return 0;
}
