#pragma once

#include <cstdint>

#pragma pack(push, 1)

struct systemEventMessage {
    char message_type = 'S';
    uint16_t stock_locate = 0;
    uint16_t tracking_number;
    uint64_t timestamp_nanoseconds;
    char event_code;
};
