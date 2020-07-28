#include "calc.h"

#include <cctype> // for std::isspace
#include <cmath> // various math functions
#include <iostream> // for error reporting via std::cerr
namespace {

const std::size_t max_decimal_digits = 10;

enum class Op {
      ERR
    , SET
    , ADD
    , SUB
    , MUL
    , DIV
    , REM
    , NEG
    , POW
    , SQRT
};

std::size_t arity(const Op op)
{
    switch (op) {
        // error
        case Op::ERR: return 0;
        // unary
        case Op::NEG: return 1;
        case Op::SQRT: return 1;
        // binary
        case Op::SET: return 2;
        case Op::ADD: return 2;
        case Op::SUB: return 2;
        case Op::MUL: return 2;
        case Op::DIV: return 2;
        case Op::REM: return 2;
        case Op::POW: return 2;
    }
    return 0;
}

Op parse_op(const std::string & line, std::size_t & i)
{
    const auto rollback = [&i, &line] (const std::size_t n) {
        i -= n;
        std::cerr << "Unknown operation " << line << std::endl;
        return Op::ERR;
    };
    switch (line[i++]) {
        case '0': [[fallthrough]];
        case '1': [[fallthrough]];
        case '2': [[fallthrough]];
        case '3': [[fallthrough]];
        case '4': [[fallthrough]];
        case '5': [[fallthrough]];
        case '6': [[fallthrough]];
        case '7': [[fallthrough]];
        case '8': [[fallthrough]];
        case '9':
            --i; // a first digit is a part of op's argument
            return Op::SET;
        case '+':
            return Op::ADD;
        case '-':
            return Op::SUB;
        case '*':
            return Op::MUL;
        case '/':
            return Op::DIV;
        case '%':
            return Op::REM;
        case '_':
            return Op::NEG;
        case '^':
            return Op::POW;
        case 'S':
                switch (line[i++]) {
                    case 'Q':
                        switch (line[i++]) {
                            case 'R':
                                switch (line[i++]) {
                                    case 'T':
                                        return Op::SQRT;
                                    default:
                                        return rollback(4);
                                }
                            default:
                                return rollback(3);
                        }
                    default:
                        return rollback(2);
                }
        default:
                return rollback(1);
    }
}

std::size_t skip_ws(const std::string & line, std::size_t i)
{
    while (i < line.size() && std::isspace(line[i])) {
        ++i;
    }
    return i;
}

double parse_arg(const std::string & line, std::size_t & i)
{

    if (line[i] < '0' || line[i] > '9') {
        std::cerr << "Invalid start of argument" << line << std::endl;
    }
    double res = 0;
    std::size_t count = 0;
    bool good = true;
    bool integer = true;
    int base = 10;
    bool have_base_zero = false;
    double fraction = 1;
    size_t start = i;
    while (good && i < line.size() && count < max_decimal_digits) {
        char cur_char = (char)toupper(line[i]);
        switch (cur_char) {
            case 'B':
                if (have_base_zero) {
                    base = 2;
                    have_base_zero = false;
                    ++i;
                    break;
                } else {
                    [[fallthrough]];
                }
            case '0':
                if (i == start) {
                    have_base_zero = true;
                    ++i;
                    break;
                } else {
                    [[fallthrough]];
                }
            case '1': [[fallthrough]];
            case '2': [[fallthrough]];
            case '3': [[fallthrough]];
            case '4': [[fallthrough]];
            case '5': [[fallthrough]];
            case '6': [[fallthrough]];
            case '7':
                if (have_base_zero) {
                    base = 8;
                    have_base_zero = false;
                }
                [[fallthrough]];
            case '8': [[fallthrough]];
            case '9': [[fallthrough]];
            case 'A': [[fallthrough]];
            case 'C': [[fallthrough]];
            case 'D': [[fallthrough]];
            case 'E': [[fallthrough]];
            case 'F':
                if (!((cur_char >= '0' && cur_char <= '0' + base - 1)
                      || (base == 16 && cur_char >= 'A' && cur_char <= 'F'))) {
                    good = false;
                    break;
                }
                if (integer) {
                    res *= base;
                    if (cur_char >= '0' && cur_char <= '9') {
                        res += cur_char - '0';
                    } else {
                        res += cur_char - 'A' + 10;
                    }
                }
                else {
                    fraction /= base;
                    if (cur_char >= '0' && cur_char <= '9') {
                        res += (cur_char - '0') * fraction;
                    } else {
                        res += (cur_char - 'A' + 10) * fraction;
                    }
                }
                ++i;
                ++count;
                break;
            case '.':
                integer = false;
                ++i;
                have_base_zero = false;
                break;
            case 'X':
                if (i != start + 1) {
                    good = false;
                    break;
                }
                base = 16;
                ++i;
                have_base_zero = false;
                break;
            default:
                good = false;
                break;
        }
    }
    if (i < line.size()) {
        std::cerr << "Argument isn't fully parsed, suffix left: '" << line.substr(i) << "'" << std::endl;
    }

    return res;
}

double unary(const double current, const Op op)
{
    switch (op) {
        case Op::NEG:
            return -current;
        case Op::SQRT:
            if (current > 0) {
                return std::sqrt(current);
            }
            else {
                std::cerr << "Bad argument for SQRT: " << current << std::endl;
                [[fallthrough]];
            }
        default:
            return current;
    }
}

double binary(const Op op, const double left, const double right)
{
    switch (op) {
        case Op::SET:
            return right;
        case Op::ADD:
            return left + right;
        case Op::SUB:
            return left - right;
        case Op::MUL:
            return left * right;
        case Op::DIV:
            if (right != 0) {
                return left / right;
            }
            else {
                std::cerr << "Bad right argument for division: " << right << std::endl;
                return left;
            }
        case Op::REM:
            if (right != 0) {
                return std::remainder(left, right);
            }
            else {
                std::cerr << "Bad right argument for remainder: " << right << std::endl;
                return left;
            }
        case Op::POW:
            return std::pow(left, right);
        default:
            return left;
    }
}

} // anonymous namespace

double process_line(const double current, const std::string & line)
{
    std::size_t i = 0;
    const auto op = parse_op(line, i);
    switch (arity(op)) {
        case 2: {
                    i = skip_ws(line, i);
                    const auto arg = parse_arg(line, i);
                    return binary(op, current, arg);
                }
        case 1: return unary(current, op);
        default: break;
    }
    return current;
}
