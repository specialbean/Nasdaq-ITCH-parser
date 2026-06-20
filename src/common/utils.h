#pragma once
#include <cstdint>

inline uint8_t hexCharToByte(char c) {
    return (c >= '0' && c <= '9') ? (c - '0') : (c >= 'A' && c <= 'F') ? (c - 'A' + 10) : ( c-'a' + 10);
}


inline uint8_t hexPairToByte(char hexPair0, char hexPair1) {
    return ((hexCharToByte(hexPair0) << 4) | hexCharToByte(hexPair1));
}
