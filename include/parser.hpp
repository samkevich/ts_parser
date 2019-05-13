#pragma once

#include <unordered_map>
#include <memory>
#include "format.hpp"

struct Packet {
    bool payload_start_indicator;
    uint16_t pid;
    bool has_adaptation;
    bool has_payload;
};

class PayloadParser {
public:
    virtual ~PayloadParser() = default;
    PayloadParser() = default;
    PayloadParser(const PayloadParser&) = default;
    PayloadParser&operator=(const PayloadParser&) = default;
    PayloadParser(PayloadParser&&) = default;
    PayloadParser&operator=(PayloadParser&&) = default;
    virtual bool parse(const Packet& packet, const char* data, size_t offset) const = 0;
};

class TSParser {
public:
    bool parse(const char* data) const;
    void add_payload_parser(uint16_t pid, const std::shared_ptr<PayloadParser>& parser);
    
private:
    std::unordered_map<uint16_t, std::shared_ptr<PayloadParser>> m_parsers;
};
