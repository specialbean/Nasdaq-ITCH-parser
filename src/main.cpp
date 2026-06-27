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
    return res;
}

auto msgStringToFormat(const char* buf, size_t& offset, char* res, uint16_t string_length) -> void {
    std::memcpy(res, buf + offset, string_length);
}

auto formatMessage(systemEventMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgToFormat<uint64_t>(buf, offset);
    msg.event_code            = msgToFormat<char>(buf, offset);
}

auto formatMessage(stockDirectoryMessage& msg, const char* buf, size_t& offset) -> void {
    msg.stock_locate          = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number       = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgToFormat<uint64_t>(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, 8);
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

// Parsing

using Message = std::variant<systemEventMessage,
                             stockDirectoryMessage,
                             stockTradingActionMessage,
                             regSHOMessage,
                             marketParticipationPositionMessage,
                             mwcbDeclineLevelMessage,
                             mwcbStatusMessage,
                             quotingPeriodUpdate,
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
        size_t offset{};
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
    typeResolutionHandler<quotingPeriodUpdate>('K');
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
    uint16_t msg_size{};
    memcpy(&msg_size, buf + offset, sizeof(uint16_t));
    msg_size = std::byteswap(msg_size);
    offset += sizeof(uint16_t);

    char message_type{};
    memcpy(&message_type, buf + offset, sizeof(char));
    message_type = std::byteswap(message_type);
    offset += sizeof(char);

    auto handOff = msgTypeId.find(message_type);
    handOff->second(message_type);
}

int main() {
    Reader* x = new Reader("../tests/t_SystemEventMessage.itch");

    auto buf{x->bufferMessage()};

    std::size_t off{};
    // std::cout << parse(buf.data(), off) << '\n';

    // systemEventMessage temp{};

    // formatMessage(temp, buf.data(), off);

    // std::cout << temp.message_type << '\n';
}
