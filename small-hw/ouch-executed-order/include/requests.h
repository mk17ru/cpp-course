#pragma once

#include <string>
#include <vector>

enum class LiquidityIndicator {
    None,
    Added,
    Removed
};

struct ExecutionDetails
{
    std::string cl_ord_id;
    unsigned match_number;
    unsigned filled_volume;
    double price;
    std::string counterpart;
    LiquidityIndicator liquidity_indicator;
    bool internalized;
    bool self_trade;
};

ExecutionDetails decode_executed_order(const std::vector<unsigned char> & message);
