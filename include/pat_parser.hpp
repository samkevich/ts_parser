#pragma once

#include "parser.hpp"

typedef std::function<void(uint16_t pmt_pid) > PATCallback;

class PATParser: public PayloadParser {
public:
    PATParser(const PATCallback& callback);
    bool parse(const Packet& packet, const char* data, size_t offset) const override;
    
private:
    PATCallback m_callback;
};
