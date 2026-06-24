#include "includes/messages.hpp"
#include "reader.cpp"
#include <cstring>
#include <iostream>
#include <vector>

template <typename T> auto msgToFormat(const char *buf, size_t &offset) -> T {
    T res;
    std::memcpy(&res, buf + offset, sizeof(T));
    offset += sizeof(T);
    return res;
}

auto msgStringToFormat(const char *buf, size_t &offset, char *res,
                       uint16_t string_length) -> void {
    std::memcpy(res, buf + offset, string_length);
}

auto formatMessage(systemEventMessage &msg, const char *buf, size_t &offset)
    -> void {
    msg.stock_locate = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgToFormat<uint64_t>(buf, offset);
    msg.event_code = msgToFormat<char>(buf, offset);
}

auto formatMessage(stockDirectoryMessage &msg, const char *buf, size_t &offset)
    -> void {
    msg.stock_locate = msgToFormat<uint16_t>(buf, offset);
    msg.tracking_number = msgToFormat<uint16_t>(buf, offset);
    msg.timestamp_nanoseconds = msgToFormat<uint64_t>(buf, offset);

    msgStringToFormat(buf, offset, msg.stock, 8);
    msg.market_category = msgToFormat<char>(buf, offset);
    msg.financial_status_indicator = msgToFormat<char>(buf, offset);
    msg.round_lot_size = msgToFormat<uint32_t>(buf, offset);
    msg.round_lots_only = msgToFormat<char>(buf, offset);
    msg.issue_classification = msgToFormat<char>(buf, offset);

    msgStringToFormat(buf, offset, msg.issue_sub_type, 2);
    msg.authenticity = msgToFormat<char>(buf, offset);
    msg.short_sale_threshold_indicator = msgToFormat<char>(buf, offset);
    msg.ipo_flag = msgToFormat<char>(buf, offset);
    msg.luld_reference_price_tier = msgToFormat<char>(buf, offset);
    msg.etp_flag = msgToFormat<char>(buf, offset);
    msg.etp_leverage_factor = msgToFormat<uint32_t>(buf, offset);
    msg.inverse_indicator = msgToFormat<char>(buf, offset);
}

/* impelement function to determine message_type
auto parse(std::vector<char> *buf) -> void {

}
*/

int main() {
    Reader *x = new Reader("../tests/t_SystemEventMessage.itch");

    auto buf{x->bufferMessage()};

    systemEventMessage temp{};
    std::size_t off{sizeof(uint16_t)};

    formatMessage(temp, buf.data(), off);

    std::cout << temp.message_type << '\n';
}
