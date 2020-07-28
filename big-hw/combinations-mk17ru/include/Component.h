#pragma once

#include <ctime>
#include <istream>
#include <string>

enum class InstrumentType : char
{
    C = 'C',
    F = 'F',
    O = 'O',
    P = 'P',
    U = 'U',
    Unknown = '\0'
};

struct Component
{
    static Component from_stream(std::istream &);
    static Component from_string(const std::string &);

    InstrumentType type{InstrumentType::Unknown};
    double ratio{0};
    double strike{0};
    std::tm expiration;
};

