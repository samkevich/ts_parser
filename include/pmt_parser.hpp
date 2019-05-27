#pragma once

#include <functional>
#include "parser.hpp"

typedef std::function<void(uint16_t pes_pid, uint8_t stream_type)> PMTCallback;

class PMTParser: public PayloadParser {
public:
    PMTParser(const PMTCallback& callback);
    bool parse(const Packet& packet, const char* data, size_t offset) const override;
    
private:
    PMTCallback m_callback;
};
