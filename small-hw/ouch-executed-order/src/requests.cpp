#include <algorithm>
#include "requests.h"
#define cur_char reinterpret_cast<const char *>(&message[i])
inline unsigned encode(size_t & ind, size_t length, const std::vector<unsigned char> & message, size_t shift)
{
    unsigned result = 0;
    for (size_t i = ind; i < ind + length; ++i) {
        const auto * str = reinterpret_cast<const unsigned char *>(&message[i]);
        result = result * 256 + *str;
    }
    ind += shift;
    return result;
}

inline double encode_double(size_t & ind, size_t length, const std::vector<unsigned char> & message, size_t shift)
{
    return static_cast<double>(encode(ind, length,message, shift)) / 10000;
}

enum Fields {
    SelfTrade = 0b10000000,
    Internalized =  0b00100000,
    LiquidityIndicator = 0b00011000,
};

ExecutionDetails decode_executed_order(const std::vector<unsigned char> & message)
{
    ExecutionDetails exec_details;
    std::string s;
    size_t i = 9;
    const char * str = cur_char;
    //case 9:
    s.assign(str, 14);
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    exec_details.cl_ord_id = s;
    i += 14;
    //case 23:
    exec_details.filled_volume = encode(i, static_cast<size_t>(4), message, 4);
    //case 27:
    exec_details.price = encode_double(i, static_cast<size_t>(4), message, 5);
    //case 32:
    exec_details.match_number = encode(i, static_cast<size_t>(4), message, 4);
    //case 36:
    str = cur_char;
    exec_details.counterpart.assign(str, 4);
    i += 7;

    //case 43:
    str = cur_char;
    unsigned temp = *str;
    exec_details.self_trade = temp & Fields::SelfTrade;
    exec_details.internalized = temp & Fields::Internalized;
    unsigned temp_ind = temp & Fields::LiquidityIndicator;
    if (temp_ind == 0) {
        exec_details.liquidity_indicator = LiquidityIndicator::Added;
    } else if (temp_ind == 8) {
            exec_details.liquidity_indicator = LiquidityIndicator::Removed;
    } else {
        exec_details.liquidity_indicator = LiquidityIndicator::None;
    }
    // fill exec_details fields
    static_cast<void>(message); // to suppress a warning about unused variable
    return exec_details;
}

