
#include <variant>        //std::variant
#include "JSONstruct.hpp" //JSONstruct
#include "JSONarray.hpp"  //JSONarray
#include "Parser.hpp"     //Parser
#include <cstddef>        //size_t
#include <stdexcept>      //std::runtime_error
#include <fstream>        //std::ifstream
#include "JSON.hpp"       //JSON
#include <string>         //std::string

namespace JsonParserVicetrice
{
    /**
     * @brief Default constructor.
     *
     * Constructs an empty JSON object.
     */
    JSON::JSON() = default;

    /**
     * @brief Destructor.
     *
     * Cleans up any allocated resources.
     */
    JSON::~JSON() = default;

    /**
     * @brief Parses a JSON string.
     *
     * Parses the given JSON string and constructs the internal representation.
     *
     * @param input The JSON string to parse.
     */
    void JSON::parse(const std::string &input)
    {
        try
        {
            Parser parser(input);
            start = std::move(parser.tokenize());
            start.consultIni(head);
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }

    /**
     * @brief Parses a JSON input stream.
     *
     * Parses the JSON content from the given input stream and constructs the internal representation.
     *
     * @param input The input stream to parse.
     */
    void JSON::parse(std::ifstream &input)
    {
        try
        {
            Parser parser(input);
            start = std::move(parser.tokenize());
            start.consultIni(head);
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }

    /**
     * @brief Accesses a JSON object element by key.
     *
     * Overloads the subscript operator to access elements of a JSON object by key.
     *
     * @param index The key of the element to access.
     * @return A reference to the JSON element.
     * @throws std::runtime_error If the element does not exist or is not an object.
     */
    JSON &JSON::operator[](const std::string &index)
    {
        try
        {
            if (auto ptr = std::get_if<JSONobject *>(&(this->head)))
            {
                JSONobject *stru = *ptr;
                this->element = stru->consult(index);
                if (auto ptr = std::get_if<JSONobject *>(&(this->element)))
                {
                    this->head = *ptr;
                }
                else if (auto ptr = std::get_if<JSONarray *>(&(this->element)))
                {
                    this->head = *ptr;
                }
            }
            else
                throw std::runtime_error("Only for searchs if inside a map.");
        }
        catch (const std::exception &e)
        {
            throw;
        }
        return *this;
    }

    /**
     * @brief Accesses a JSON array element by index.
     *
     * Overloads the subscript operator to access elements of a JSON array by index.
     *
     * @param index The index of the element to access.
     * @return A reference to the JSON element.
     * @throws std::runtime_error If the element does not exist or is not an array.
     */
    JSON &JSON::operator[](size_t index)
    {
        try
        {
            if (auto ptr = std::get_if<JSONarray *>(&(this->head)))
            {
                JSONarray *stru = *ptr;
                this->element = stru->consult(index);
                if (auto ptr = std::get_if<JSONobject *>(&(this->element)))
                {
                    this->head = *ptr;
                }
                else if (auto ptr = std::get_if<JSONarray *>(&(this->element)))
                {
                    this->head = *ptr;
                }
            }
            else
                throw std::runtime_error("Only for searchs if inside an array.");
        }
        catch (const std::exception &e)
        {
            throw;
        }
        return *this;
    }
    /**
     * @brief Consult the size of the JSONstruct
     * @return size of JSONstruct head is pointing to
     */
    size_t JSON::size() 
    {
        if (auto ptr = std::get_if<JSONobject *>(&head))
        {
            JSONobject *aux = *ptr;
            start.consultIni(head);
            return aux->size();
        }
        else if (auto ptr = std::get_if<JSONarray *>(&head))
        {
            JSONarray *aux = *ptr;
            start.consultIni(head);
            return aux->size();
        }
        start.consultIni(head);
        return 0;
    }
} // namespace JsonParserVicetrice
