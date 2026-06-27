#pragma once

#include <cstdint>

#pragma pack(push, 1)

struct systemEventMessage {
    char     message_type = 'S';
    uint16_t stock_locate = 0;
    uint16_t tracking_number;
    uint64_t timestamp_nanoseconds;
    char     event_code;
};

struct stockDirectoryMessage {
    char     message_type = 'R';
    uint16_t stock_locate = 0;
    uint16_t tracking_number;
    uint64_t timestamp_nanoseconds;
    char     stock[8];
    char     market_category;
    char     financial_status_indicator;
    uint32_t round_lot_size;
    char     round_lots_only;
    char     issue_classification;
    char     issue_sub_type[2];
    char     authenticity;
    char     short_sale_threshold_indicator;
    char     ipo_flag;
    char     luld_reference_price_tier;
    char     etp_flag;
    char     etp_leverage_factor;
    char     inverse_indicator;
};

struct stockTradingActionMessage {
    char message_type = 'H';
};

struct regSHOMessage {
    char message_type = 'Y';
};

struct marketParticipationPositionMessage {
    char message_type = 'L';
};

struct mwcbDeclineLevelMessage {
    char message_type = 'V';
};

struct mwcbStatusMessage {
    char message_type = 'W';
};

struct quotingPeriodUpdate {
    char message_type = 'K';
};

struct luldAuctionCollarMessage {
    char message_type = 'J';
};

struct operationalHaltMessage {
    char message_type = 'h';
};

struct addOrderMessage {
    char message_type = 'A';
};

struct addOrderWithMPIDMessage {
    char message_type = 'F';
};

struct orderExecutedMessage {
    char message_type = 'E';
};

struct orderExecutedWithPriceMessage {
    char message_type = 'C';
};

struct orderCancleMessage {
    char message_type = 'X';
};

struct orderDeleteMessage {
    char message_type = 'D';
};

struct orderReplaceMessage {
    char message_type = 'U';
};

struct tradeMessage {
    char message_type = 'p';
};

struct crossTradeMessage {
    char message_type = 'Q';
};

struct brokenTradeMessage {
    char message_type = 'B';
};

struct NOIIMessage {
    char message_type = 'I';
};

struct RPIIMessage {
    char message_type = 'N';
};

struct DLCRPDMessage {
    char message_type = 'O';
};

#pragma pack(pop)
