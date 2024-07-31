
```markdown
# JSON Parser

## Overview

This project provides a JSON parsing library implemented in C++. The library includes a `JSON` class designed for parsing and accessing JSON data efficiently. This README provides an overview of how to use the `JSON` class and run the example provided.

## Features

- **Parsing JSON from files and strings**: Easily parse JSON data from file streams and string inputs.
- **Access JSON elements**: Retrieve and manipulate JSON elements using the provided API.
- **Performance**: Includes basic performance measurement for parsing operations.

## Getting Started

### Prerequisites

- C++11 or later
- A JSON file to parse (e.g., `prueba.json`)

### Building

To build the project, ensure you have a C++ compiler and make utility available. Use the following command to compile the project:

```bash
g++ -std=c++11 -o parser main.cpp JSON.cpp JSONarray.cpp JSONobject.cpp
```

### Example Usage

The example below demonstrates how to use the `JSON` class to parse a JSON file and access its contents. 

Create a file named `main.cpp` and paste the following code:

```cpp
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include "JSON.hpp"

int main()
{
    using namespace std::chrono; // Simplify the use of chrono

    std::ifstream file("prueba.json");

    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1; // Indicate an error occurred
    }

    // Measure the time for file reading and tokenization
    auto start = high_resolution_clock::now();

    JsonParserVicetrice::JSON json;
    json.parse(file);

    std::string loca = json["statuses"][0]["id_str"].get<std::string>();
    std::string loca2 = json["statuses"][1]["text"].get<std::string>();

    std::cout << loca << std::endl;
    std::cout << loca2 << std::endl;

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    std::cout << "Execution time: " << duration << " ms" << std::endl;

    return 0;
}
```

### Running the Example

1. Ensure you have a valid JSON file named `prueba.json` in the same directory as your executable. The file should contain data structured similarly to this:

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

You should see the output of the JSON values from the `prueba.json` file along with the execution time.

## API Documentation

- **`void parse(const std::string &input)`**: Parses a JSON string.
- **`void parse(std::ifstream &input)`**: Parses JSON from an input stream.
- **`template <typename T> T get()`**: Retrieves the JSON element as the specified type.
- **`JSON &operator[](const std::string &index)`**: Accesses a JSON object element by key.
- **`JSON &operator[](size_t index)`**: Accesses a JSON array element by index.

## Contributing

If you would like to contribute to the project, please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
```

Just copy and paste this content into a file named `README.md` in your GitHub repository. Make sure to adjust any paths or details specific to your project setup as needed.
