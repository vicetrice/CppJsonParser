#ifndef __________JSON_HPP__________
#define __________JSON_HPP__________

#include <variant>
#include "JSONstruct.hpp"
#include "JSONarray.hpp"
#include "Parser.hpp"
#include <cstddef>
#include <stdexcept>
#include <fstream>

namespace JsonParserVicetrice
{
    class JSON
    {
    public:
        using VariantTypeI = std::variant<std::string, int64_t, bool, long double, char, JSONobject *, JSONarray *>;

        JSON() = default;
        ~JSON() = default;

        void parse(const std::string &input)
        {
            try
            {
                Parser parser(input);
                start = std::move(parser.tokenize());
                head = start.consultIni();
            }
            catch (const std::exception &e)
            {
                throw;
            }
        }

        void parse(std::ifstream &input)
        {
            try
            {
                Parser parser(input);
                start = std::move(parser.tokenize());
                head = start.consultIni();
            }
            catch (const std::exception &e)
            {
                throw;
            }
        }

        template <typename T>
        T get()
        {
            T resultado;
            try
            {
                if (auto ptr = std::get_if<std::string>(&element))
                {
                    if constexpr (std::is_same_v<T, std::string>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }
                else if (auto ptr = std::get_if<int64_t>(&element))
                {
                    if constexpr (std::is_same_v<T, int64_t>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }
                else if (auto ptr = std::get_if<long double>(&element))
                {
                    if constexpr (std::is_same_v<T, long double>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }
                else if (auto ptr = std::get_if<char>(&element))
                {
                    if constexpr (std::is_same_v<T, char>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }
                else if (auto ptr = std::get_if<bool>(&element))
                {
                    if constexpr (std::is_same_v<T, bool>)
                    {
                        resultado = *ptr;
                    }
                    else
                        throw std::runtime_error("Type to be storaged must match.");
                }else
                {
                    throw std::runtime_error("Type to be storaged must be primitive.");
                }
            }
            catch (const std::exception &e)
            {
                throw;
            }

            return resultado;
        }

        JSON &operator[](const std::string &index)
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
                    throw std::runtime_error("Only for searchs inside a map.");
            }
            catch (const std::exception &e)
            {
                throw;
            }
            return *this;
        }

        JSON &operator[](size_t index)
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
                    throw std::runtime_error("Only for searchs inside an array.");
            }
            catch (const std::exception &e)
            {
                throw;
            }
            return *this;
        }

    private:
        using VariantPtr = std::variant<JSONobject *, JSONarray *>;

        //--------------------- ATTRIBUTES
        JSONstruct start;
        VariantPtr head;
        VariantTypeI element;
    }; // class JSON
} // namespace JsonParserVicetrice

#endif