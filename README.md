
# JSON Parser

## Overview

This project provides a JSON parsing library implemented in C++. The library includes a `JSON` class designed for parsing and accessing JSON data efficiently. This README provides an overview of how to use the `JSON` class and run the example provided.

## Features

- **Parsing JSON from files and strings**: Easily parse JSON data from file streams and string inputs.
- **Access JSON elements**: Retrieve and manipulate JSON elements using the provided API.
- **Performance**: Includes basic performance measurement for parsing operations.

## Getting Started

### Prerequisites

- C++17 or later
- A JSON file to parse (e.g., `prueba.json`)

### Building

To build the project, ensure you have a C++ compiler and make utility available. Use the following command to compile the project:

```bash
g++ -std=c++17 -o parser main.cpp JSON.cpp JSONarray.cpp JSONobject.cpp JSONstruct.cpp
```
Or change `main.cpp` in `Makefile` to use `make`. 

it's not strict to use g++, can change for your favorite compiler.

You can also use the header `JSON.hpp` in `single_include` folder to compile your project without compiling directly the other .cpp of the project.

### Example Usage

The example below demonstrates how to use the `JSON` class to parse a JSON file and access its contents. 

```cpp
#include <string>
#include <iostream>
#include <fstream>
#include "single_include/JSON.hpp"

int main()
{
    std::ifstream file("prueba.json");

    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    JsonParserVicetrice::JSON json;
    json.parse(file);

    std::string str = json["statuses"][0]["id_str"].get<std::string>();
    std::string str2 = json["statuses"][1]["text"].get<std::string>();
    int64_t i = json["statuses"][0]["id"].get<int64_t>();
    JSONprimitiveType j = json["key"].get<JSONprimitiveType>();

    std::cout << str << std::endl;
    std::cout << str2 << std::endl;
    return 0;
}
```

### Running the Example

1. The JSON file should contain data structured similarly to this:

   ```json
   {
       "statuses": [
           { "id_str": "12345", "text": "Hello, world!" },
           { "id_str": "67890", "text": "Goodbye, world!" }
       ]
   }
   ```

2. Compile the `main.cpp` file as described in the Building section.

3. Run the executable:

   ```bash
   ./parser
   ```

You should see the output of the JSON values from the file.

## JSON Basic Types Conversion

The `JSON` class supports the following basic JSON types and their conversion:

- **`std::string`**: Represents a string value in JSON.
- **`bool`**: Represents a boolean value (`true` or `false`) in JSON.
- **`long double`**: Represents a floating-point number with extended precision in JSON.
- **`int64_t`**: Represents a 64-bit integer in JSON.
- **`char`**: Represents null type in JSON. returns in case of null(`\0`)

### Conversion Example

To convert a JSON element to a specific type, use the `get<T>()` method:

```cpp
std::string str_value = json["key"/index].get<std::string>();
bool bool_value = json["key"/index].get<bool>();
long double ld_value = json["key"/index].get<long double>();
int64_t int_value = json["key"/index].get<int64_t>();
char char_value = json["key"/index].get<char>();
```

If the stored element is not of the requested type, a `std::runtime_error` will be thrown.


## API Documentation

- **`void parse(const std::string &input)`**: Parses a JSON string.
- **`void parse(std::ifstream &input)`**: Parses JSON from an input stream.
- **`template <typename T> T get()`**: Retrieves the JSON element as the specified type.
- **`JSON &operator[](const std::string &index)`**: Accesses a JSON object element by key.
- **`JSON &operator[](size_t index)`**: Accesses a JSON array element by index.

## Contributing

If you would like to contribute to the project, please fork the repository and submit a pull request with your changes.
