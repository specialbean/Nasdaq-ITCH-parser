#include "includes/messages.hpp"
#include "reader.cpp"
#include <cstring>
#include <iostream>
#include <vector>

namespace nasdaq {

namespace tooling {
template <typename T>

auto msgToFormat(const char *buf, size_t &offset) -> T {
    T res;
    std::memcpy(&res, buf + offset, sizeof(T));
    offset += sizeof(T);
    return res;
}
} // namespace tooling

auto parseMessage(systemEventMessage &msg, const char *buf, size_t &offset)
    -> void {
    msg.message_type = nasdaq::tooling::msgToFormat<char>(buf, offset);
}
} // namespace nasdaq

int main() {
    Reader *x = new Reader("../tests/testing_systeMessage.itch");

    auto buf{x->bufferMessage()};

    systemEventMessage temp{};
    std::size_t off{sizeof(uint16_t)};

    nasdaq::parseMessage(temp, buf.data(), off);

    std::cout << temp.message_type << '\n';
}
