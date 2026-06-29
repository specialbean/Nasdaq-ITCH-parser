#include <cstdint>
#include <cstring>
#include <functional>
#include <string>
#include <unordered_map>
#include <variant>

#include "includes/messages.hpp"
#include "reader.cpp"

template <typename T>
auto msgToFormat(const char* buf, size_t& offset) -> T {
    T res;
    std::memcpy(&res, buf + offset, sizeof(T));
    offset += sizeof(T);

    if constexpr (std::is_integral_v<T> && sizeof(T) > 1) {
        return std::byteswap(res);
    } else {
        return res;
    }
}

auto msgStringToFormat(const char* buf, size_t& offset, char* res, uint16_t string_length) -> void {
    std::memcpy(res, buf + offset, string_length);
    offset += string_length;
}

auto msgTimestamp(const char* buf, size_t& offset) -> uint64_t {
    uint16_t upper {};
    uint32_t lower {};
    std::memcpy(&upper, buf + offset, sizeof(upper));
    offset += sizeof(upper);
    std::memcpy(&lower, buf + offset, sizeof(lower));
    offset += sizeof(lower);
    upper = std::byteswap(upper);
    lower = std::byteswap(lower);
    return static_cast<uint64_t>(upper) << 32 | lower;
}

auto formatMessage(systemEventMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);
    msg.event_code            = msgToFormat<char>(buf, offset);
}

auto formatMessage(stockDirectoryMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.market_category            = msgToFormat<char>(buf, offset);
    msg.financial_status_indicator = msgToFormat<char>(buf, offset);
    msg.round_lot_size             = msgToFormat<uint32_t>(buf, offset);
    msg.round_lots_only            = msgToFormat<char>(buf, offset);
    msg.issue_classification       = msgToFormat<char>(buf, offset);

    msgStringToFormat(buf, offset, msg.issue_sub_type, 2);
    msg.authenticity                   = msgToFormat<char>(buf, offset);
    msg.short_sale_threshold_indicator = msgToFormat<char>(buf, offset);
    msg.ipo_flag                       = msgToFormat<char>(buf, offset);
    msg.luld_reference_price_tier      = msgToFormat<char>(buf, offset);
    msg.etp_flag                       = msgToFormat<char>(buf, offset);
    msg.etp_leverage_factor            = msgToFormat<uint32_t>(buf, offset);
    msg.inverse_indicator              = msgToFormat<char>(buf, offset);
}

auto formatMessage(stockTradingActionMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.trading_state = msgToFormat<char>(buf, offset);
    msg.reserved      = msgToFormat<char>(buf, offset);

    msgStringToFormat(buf, offset, msg.reason, 4);
}

auto formatMessage(regSHOMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.reg_sho_action = msgToFormat<char>(buf, offset);
}

auto formatMessage(marketParticipationPositionMessage& msg, const char* buf, size_t& offset)
    -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);

    msgStringToFormat(buf, offset, msg.MPID, 4);
    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.primary_market_maker     = msgToFormat<char>(buf, offset);
    msg.market_maker_mode        = msgToFormat<char>(buf, offset);
    msg.market_participant_state = msgToFormat<char>(buf, offset);
}

auto formatMessage(mwcbDeclineLevelMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);
    msg.level_1               = msgToFormat<uint64_t>(buf, offset);
    msg.level_2               = msgToFormat<uint64_t>(buf, offset);
    msg.level_3               = msgToFormat<uint64_t>(buf, offset);
}

auto formatMessage(mwcbStatusMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);
    msg.breached_level        = msgToFormat<char>(buf, offset);
}

auto formatMessage(ipoQuotingPeriodUpdate& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.ipo_quotation_release_time      = msgToFormat<uint32_t>(buf, offset);
    msg.ipo_qoutation_release_qualifier = msgToFormat<char>(buf, offset);
    msg.ipo_price                       = msgToFormat<uint32_t>(buf, offset);
}

auto formatMessage(luldAuctionCollarMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.auction_collar_reference_price = msgToFormat<uint32_t>(buf, offset);
    msg.upper_auction_collar_price     = msgToFormat<uint32_t>(buf, offset);
    msg.lower_auction_collar_price     = msgToFormat<uint32_t>(buf, offset);
    msg.auction_collar_extension       = msgToFormat<uint32_t>(buf, offset);
}

auto formatMessage(operationalHaltMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.market_code           = msgToFormat<char>(buf, offset);
    msg.operation_halt_action = msgToFormat<char>(buf, offset);
}
auto formatMessage(addOrderMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate           = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number        = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds  = msgToFormat<uint64_t>(buf, offset);
    msg.order_reference_number = msgToFormat<uint64_t>(buf, offset);
    msg.buy_sell_indicator     = msgToFormat<char>(buf, offset);
    msg.shares                 = msgToFormat<uint32_t>(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.price = msgToFormat<uint32_t>(buf, offset);
}

auto formatMessage(addOrderWithMPIDMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate           = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number        = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds  = msgToFormat<uint64_t>(buf, offset);
    msg.order_reference_number = msgToFormat<uint64_t>(buf, offset);
    msg.buy_sell_indicator     = msgToFormat<char>(buf, offset);
    msg.shares                 = msgToFormat<uint32_t>(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.price = msgToFormat<uint32_t>(buf, offset);
    msgStringToFormat(buf, offset, msg.attribution, 4);
}

auto formatMessage(orderExecutedMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate           = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number        = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds  = msgToFormat<uint64_t>(buf, offset);
    msg.order_reference_number = msgToFormat<uint64_t>(buf, offset);
    msg.executed_shares        = msgToFormat<uint32_t>(buf, offset);
    msg.match_number           = msgToFormat<uint64_t>(buf, offset);
}

auto formatMessage(orderExecutedWithPriceMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate           = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number        = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds  = msgToFormat<uint64_t>(buf, offset);
    msg.order_reference_number = msgToFormat<uint64_t>(buf, offset);
    msg.executed_shares        = msgToFormat<uint32_t>(buf, offset);
    msg.match_number           = msgToFormat<uint64_t>(buf, offset);
    msg.printable              = msgToFormat<char>(buf, offset);
    msg.execution_price        = msgToFormat<uint32_t>(buf, offset);
}

auto formatMessage(orderCancleMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate           = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number        = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds  = msgToFormat<uint64_t>(buf, offset);
    msg.order_reference_number = msgToFormat<uint64_t>(buf, offset);
    msg.cancelled_shares       = msgToFormat<uint32_t>(buf, offset);
}

auto formatMessage(orderDeleteMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate           = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number        = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds  = msgToFormat<uint64_t>(buf, offset);
    msg.order_reference_number = msgToFormat<uint64_t>(buf, offset);
}

auto formatMessage(orderReplaceMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate                    = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number                 = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds           = msgToFormat<uint64_t>(buf, offset);
    msg.original_order_reference_number = msgToFormat<uint64_t>(buf, offset);
    msg.new_order_reference_number      = msgToFormat<uint64_t>(buf, offset);
    msg.shares                          = msgToFormat<uint32_t>(buf, offset);
    msg.price                           = msgToFormat<uint32_t>(buf, offset);
}

auto formatMessage(tradeMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate           = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number        = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds  = msgToFormat<uint64_t>(buf, offset);
    msg.order_reference_number = msgToFormat<uint64_t>(buf, offset);
    msg.buy_sell_indicator     = msgToFormat<char>(buf, offset);
    msg.shares                 = msgToFormat<uint32_t>(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.price        = msgToFormat<uint32_t>(buf, offset);
    msg.match_number = msgToFormat<uint64_t>(buf, offset);
}

auto formatMessage(crossTradeMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);
    msg.shares                = msgToFormat<uint64_t>(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.cross_price  = msgToFormat<uint32_t>(buf, offset);
    msg.match_number = msgToFormat<uint64_t>(buf, offset);
    msg.cross_type   = msgToFormat<char>(buf, offset);
}

auto formatMessage(brokenTradeMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);
    msg.match_number          = msgToFormat<uint64_t>(buf, offset);
}

auto formatMessage(NOIIMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);
    msg.paired_shares         = msgToFormat<uint64_t>(buf, offset);
    msg.imbalance_shares      = msgToFormat<uint64_t>(buf, offset);
    msg.imbalance_direction   = msgToFormat<char>(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.far_price                 = msgToFormat<uint32_t>(buf, offset);
    msg.near_price                = msgToFormat<uint32_t>(buf, offset);
    msg.current_reference_price   = msgToFormat<uint32_t>(buf, offset);
    msg.cross_type                = msgToFormat<char>(buf, offset);
    msg.price_variation_indicator = msgToFormat<char>(buf, offset);
}

auto formatMessage(RPIIMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.interesting_flag = msgToFormat<char>(buf, offset);
}

auto formatMessage(DLCRPDMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgTimestamp(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, STOCK_LENGTH);
    msg.open_eligibility_status  = msgToFormat<char>(buf, offset);
    msg.minimum_allowable_price  = msgToFormat<uint32_t>(buf, offset);
    msg.maximum_allowable_price  = msgToFormat<uint32_t>(buf, offset);
    msg.near_execution_price     = msgToFormat<uint32_t>(buf, offset);
    msg.near_execution_time      = msgToFormat<uint64_t>(buf, offset);
    msg.lower_price_range_collar = msgToFormat<uint32_t>(buf, offset);
    msg.upper_price_range_collar = msgToFormat<uint32_t>(buf, offset);
}
// Parsing

using Message = std::variant<
    systemEventMessage,
    stockDirectoryMessage,
    stockTradingActionMessage,
    regSHOMessage,
    marketParticipationPositionMessage,
    mwcbDeclineLevelMessage,
    mwcbStatusMessage,
    ipoQuotingPeriodUpdate,
    luldAuctionCollarMessage,
    operationalHaltMessage,
    addOrderMessage,
    addOrderWithMPIDMessage,
    orderExecutedMessage,
    orderExecutedWithPriceMessage,
    orderCancleMessage,
    orderDeleteMessage,
    orderReplaceMessage,
    tradeMessage,
    crossTradeMessage,
    brokenTradeMessage,
    NOIIMessage,
    RPIIMessage,
    DLCRPDMessage>;

using msgHandler = std::function<Message(const char*)>;

std::unordered_map<char, msgHandler> msgTypeId;

template <typename T>
auto typeResolutionHandler(char type) -> void {
    msgTypeId[type] = [](const char* buf) -> Message {
        T      msg;
        size_t offset {};
        formatMessage(msg, buf, offset);
        return msg;
    };
}

auto populateTypeId() -> void {
    typeResolutionHandler<systemEventMessage>('S');
    typeResolutionHandler<stockDirectoryMessage>('R');
    typeResolutionHandler<stockTradingActionMessage>('H');
    typeResolutionHandler<regSHOMessage>('Y');
    typeResolutionHandler<marketParticipationPositionMessage>('L');
    typeResolutionHandler<mwcbDeclineLevelMessage>('V');
    typeResolutionHandler<mwcbStatusMessage>('W');
    typeResolutionHandler<ipoQuotingPeriodUpdate>('K');
    typeResolutionHandler<luldAuctionCollarMessage>('J');
    typeResolutionHandler<operationalHaltMessage>('h');
    typeResolutionHandler<addOrderMessage>('A');
    typeResolutionHandler<addOrderWithMPIDMessage>('F');
    typeResolutionHandler<orderExecutedMessage>('E');
    typeResolutionHandler<orderExecutedWithPriceMessage>('C');
    typeResolutionHandler<orderCancleMessage>('X');
    typeResolutionHandler<orderDeleteMessage>('D');
    typeResolutionHandler<orderReplaceMessage>('U');
    typeResolutionHandler<tradeMessage>('p');
    typeResolutionHandler<crossTradeMessage>('Q');
    typeResolutionHandler<brokenTradeMessage>('B');
    typeResolutionHandler<NOIIMessage>('I');
    typeResolutionHandler<RPIIMessage>('N');
    typeResolutionHandler<DLCRPDMessage>('O');
}

auto parse(const char* buf, std::size_t& offset) -> void {
    uint16_t msg_size {};
    memcpy(&msg_size, buf + offset, sizeof(uint16_t));
    msg_size = std::byteswap(msg_size);
    offset += sizeof(uint16_t);
    std::cout << "Message length :" << msg_size << '\n';

    char message_type {};
    memcpy(&message_type, buf + offset, sizeof(char));
    message_type = std::byteswap(message_type);
    offset += sizeof(char);
    std::cout << "Message type: " << message_type << '\n';

    auto handOff = msgTypeId.find(message_type);
    std::cout << "Executed successfully" << '\n' << '\n';
    offset += msg_size - 1;
}

int main() {
    auto* testingReader = new Reader("~/Downloads/sample.bin");

    auto        buf {testingReader->bufferMessage()};
    std::size_t off {0};

    while (off < buf.size()) {
        parse(buf.data(), off);
        std::cout << "Current offset is: " << off << '\n' << '\n';
    }
    // std::cout << parse(buf.data(), off) << '\n';

    // systemEventMessage temp{};

    // formatMessage(temp, buf.data(), off);

    // std::cout << temp.message_type << '\n';
}
