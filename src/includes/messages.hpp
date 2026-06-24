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

struct stockDirectoryMessage {
    char message_type = 'R';
    uint16_t stock_locate = 0;
    uint16_t tracking_number;
    uint64_t timestamp_nanoseconds;
    char stock[8];
    char market_category;
    char financial_status_indicator;
    uint32_t round_lot_size;
    char round_lots_only;
    char issue_classification;
    char issue_sub_type[2];
    char authenticity;
    char short_sale_threshold_indicator;
    char ipo_flag;
    char luld_reference_price_tier;
    char etp_flag;
    char etp_leverage_factor;
    char inverse_indicator;
};

#pragma pack(pop)
