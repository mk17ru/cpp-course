# Декодирование сообщения ExecutedOrder
## Новый протокол!
В примере на лекции мы познакомились с протоколом BOE, используемом преимущественно на торговых площадках, принадлежащих бирже CBOE.
В этом задании вам предлагается познакомиться с ещё одним протоколом - OUCH, используемом более широко (в разных вариациях), однако
мы сосредоточимся на его варианте, используемом на площадках Nasdaq Nordic (INET), объединяюищх скандинавские биржи.

Существенное отличие: числа кодируются в big endian формате.

## Идея
Когда какой-то заказ исполняется на бирже, то есть обнаруживается встречный заказ и биржа регистрирует сделку, участник, отправивший
заказ, получает уведомление о свершившейся сделке - исполнение заказа, order execution.
Заказ может быть исполнен полностью или частично, по указанной в нём цене или по более лучшей.

## Задание
Требуется реализовать декодер для сообщения ExecutedOrder (см. [protocol specifications](doc/OUCH_for_Nasdaq_Nordic_4.03.2.pdf)) имеющий следующий интерфейс:
```cpp
enum class LiquidityIndicator {
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

ExecutionDetails decode_executed_order(const std::vector<unsigned char> & message)
{
    ExecutionDetails exec_details;
    // fill exec_details fields
    //   exec_details.cl_ord_id.assign(char_ptr, length);
    // or
    //   exec_details.cl_ord_id = std::string{char_ptr, length};
    // ...
    //   exec_details.filled_volume = x;
    // ...
    return exec_details;
}
```

Чтобы получить указатель на `char`, требуемый для присваивания в строки, из вектора `unsigned char`, можно:
```cpp
const char * str = reinterpret_cast<const char *>(&message[n]);
```
где `n` - индекс первого символа строки внутри сообщения.

Требуемые поля:
* Order Token -> cl_ord_id
* Executed Quantity -> filled_volume
* Execution Price -> price
* Match Number -> match_number
* Contra Firm -> counterpart
* Liquidity Attributes -> liquidity_indicator, internalized, self_trade
